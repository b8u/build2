// file      : build2/cxx/msvc.cxx -*- C++ -*-
// copyright : Copyright (c) 2014-2016 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#include <build2/scope>
#include <build2/target>
#include <build2/context>
#include <build2/variable>
#include <build2/filesystem>
#include <build2/diagnostics>

#include <build2/cxx/common>

using namespace std;
using namespace butl;

namespace build2
{
  namespace cxx
  {
    using namespace bin;

    // Extract system library search paths from MSVC.
    //
    void
    msvc_library_search_paths (scope&, const string&, dir_paths&)
    {
      // The linker doesn't seem to have any built-in paths and all of them
      // come from the LIB environment variable.

      // @@ VC: how are we going to do this? E.g., cl-14 does this internally.
      //    cl.exe /Be prints LIB.
      //
      //    Should we actually bother? LIB is normally used for system
      //    libraries and its highly unlikely we will see an explicit import
      //    for a library from one of those directories.
      //
    }

    // Inspect the file and determine if it is static or import library.
    // Return otype::e if it is neither (which we quietly ignore).
    //
    static otype
    library_type (const path& ld, const path& l)
    {
      // The are several reasonably reliable methods to tell whether it is a
      // static or import library. One is lib.exe /LIST -- if there aren't any
      // .obj members, then it is most likely an import library (it can also
      // be an empty static library in which case there won't be any members).
      // For an import library /LIST will print a bunch of .dll members.
      //
      // Another approach is dumpbin.exe (link.exe /DUMP) with /ARCHIVEMEMBERS
      // (similar to /LIST) and /LINKERMEMBER (looking for __impl__ symbols or
      // _IMPORT_DESCRIPTOR_).
      //
      // Note also, that apparently it is possible to have a hybrid library.
      //
      // While the lib.exe approach is probably the simplest, the problem is
      // it will require us loading the bin.ar module even if we are not
      // building any static libraries. On the other hand, if we are searching
      // for libraries then we have bin.ld. So we will use the link.exe /DUMP
      // /ARCHIVEMEMBERS.
      //
      const char* args[] = {ld.string ().c_str (),
                            "/DUMP",               // Must come first.
                            "/NOLOGO",
                            "/ARCHIVEMEMBERS",
                            l.string ().c_str (),
                            nullptr};

      // Link.exe seem to always dump everything to stdout but just in case
      // redirect stderr to stdout.
      //
      process pr (start_run (args, false));
      ifdstream is (pr.in_ofd);

      bool obj (false), dll (false);

      string s;
      while (getline (is, s))
      {
        // Detect the one error we should let through.
        //
        if (s.compare (0, 18, "unable to execute ") == 0)
          break;

        // The lines we are interested in seem to have this form (though
        // presumably the "Archive member name at" part can be translated):
        //
        // Archive member name at 746: [...]hello.dll[/][ ]*
        // Archive member name at 8C70: [...]hello.lib.obj[/][ ]*
        //
        size_t n (s.size ());

        for (; n != 0 && s[n - 1] == ' '; --n) ; // Skip trailing spaces.

        if (n >= 7) // At least ": X.obj" or ": X.dll".
        {
          --n;

          if (s[n] == '/') // Skip trailing slash if one is there.
            --n;

          n -= 3; // Beginning of extension.

          if (s[n] == '.')
          {
            // Make sure there is ": ".
            //
            size_t p (s.rfind (':', n - 1));

            if (p != string::npos && s[p + 1] == ' ')
            {
              if (s.compare (n + 1, 3, "obj") == 0) // @@ CASE
                obj = true;

              if (s.compare (n + 1, 3, "dll") == 0) // @@ CASE
                dll = true;
            }
          }
        }
      }

      is.close (); // Don't block.

      if (!finish_run (args, false, pr, s))
        return otype::e;

      if (obj && dll)
      {
        warn << l << " looks like hybrid static/import library, ignoring";
        return otype::e;
      }

      if (!obj && !dll)
      {
        warn << l << " looks like empty static or import library, ignoring";
        return otype::e;
      }

      return obj ? otype::a : otype::s;
    }

    template <typename T>
    static T*
    search_library (const path& ld,
                    const dir_path& d,
                    prerequisite& p,
                    otype lt,
                    const char* pfx,
                    const char* sfx)
    {
      // Pretty similar logic to link::search_library().
      //
      tracer trace ("cxx::msvc_search_library");

      // Assemble the file path.
      //
      path f (d);

      if (*pfx != '\0')
      {
        f /= pfx;
        f += p.name;
      }
      else
        f /= p.name;

      if (*sfx != '\0')
        f += sfx;

      const string& e (
        p.ext == nullptr || p.is_a<lib> () // Only for liba/libs.
        ? extension_pool.find ("lib")
        : *p.ext);

      if (!e.empty ())
      {
        f += '.';
        f += e;
      }

      // Check if the file exists and is of the expected type.
      //
      timestamp mt (file_mtime (f));

      if (mt != timestamp_nonexistent && library_type (ld, f) == lt)
      {
        // Enter the target.
        //
        T& t (targets.insert<T> (d, dir_path (), p.name, &e, trace));

        if (t.path ().empty ())
          t.path (move (f));

        t.mtime (mt);
        return &t;
      }

      return nullptr;
    }

    liba*
    msvc_search_static (const path& ld, const dir_path& d, prerequisite& p)
    {
      liba* r (nullptr);

      auto search = [&r, &ld, &d, &p] (const char* pf, const char* sf) -> bool
      {
        r = search_library<liba> (ld, d, p, otype::a, pf, sf);
        return r != nullptr;
      };

      // Try:
      //      foo.lib
      //   libfoo.lib
      //      foolib.lib
      //      foo_static.lib
      //
      return
        search ("",    "")    ||
        search ("lib", "")    ||
        search ("",    "lib") ||
        search ("",    "_static") ? r : nullptr;
    }

    libs*
    msvc_search_shared (const path& ld, const dir_path& d, prerequisite& p)
    {
      libs* r (nullptr);

      auto search = [&r, &ld, &d, &p] (const char* pf, const char* sf) -> bool
      {
        r = search_library<libs> (ld, d, p, otype::s, pf, sf);
        return r != nullptr;
      };

      // Try:
      //      foo.lib
      //   libfoo.lib
      //      foodll.lib
      //
      return
        search ("",    "")    ||
        search ("lib", "")    ||
        search ("",    "dll") ? r : nullptr;
    }
  }
}
