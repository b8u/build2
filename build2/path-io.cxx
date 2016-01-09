// file      : build2/path-io.cxx -*- C++ -*-
// copyright : Copyright (c) 2014-2016 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#include <build2/path-io>

#include <ostream>

#include <build2/context>
#include <build2/diagnostics>

using namespace std;

namespace build2
{
  ostream&
  operator<< (ostream& os, const path& p)
  {
    return os << (relative (os) ? diag_relative (p) : p.string ());
  }

  ostream&
  operator<< (ostream& os, const dir_path& d)
  {
    if (relative (os))
      os << diag_relative (d);
    else
    {
      const string& s (d.string ());

      // Print the directory with trailing '/'.
      //
      if (!s.empty ())
        os << s << (dir_path::traits::is_separator (s.back ()) ? "" : "/");
    }

    return os;
  }
}
