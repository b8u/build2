// -*- C++ -*-
//
// This file was generated by CLI, a command line interface
// compiler for C++.
//

// Begin prologue.
//
//
// End prologue.

#include <libbuild2/script/builtin-options.hxx>

#include <map>
#include <set>
#include <string>
#include <vector>
#include <ostream>
#include <sstream>

namespace build2
{
  namespace script
  {
    namespace cli
    {
      // unknown_option
      //
      unknown_option::
      ~unknown_option () throw ()
      {
      }

      void unknown_option::
      print (::std::ostream& os) const
      {
        os << "unknown option '" << option ().c_str () << "'";
      }

      const char* unknown_option::
      what () const throw ()
      {
        return "unknown option";
      }

      // unknown_argument
      //
      unknown_argument::
      ~unknown_argument () throw ()
      {
      }

      void unknown_argument::
      print (::std::ostream& os) const
      {
        os << "unknown argument '" << argument ().c_str () << "'";
      }

      const char* unknown_argument::
      what () const throw ()
      {
        return "unknown argument";
      }

      // missing_value
      //
      missing_value::
      ~missing_value () throw ()
      {
      }

      void missing_value::
      print (::std::ostream& os) const
      {
        os << "missing value for option '" << option ().c_str () << "'";
      }

      const char* missing_value::
      what () const throw ()
      {
        return "missing option value";
      }

      // invalid_value
      //
      invalid_value::
      ~invalid_value () throw ()
      {
      }

      void invalid_value::
      print (::std::ostream& os) const
      {
        os << "invalid value '" << value ().c_str () << "' for option '"
           << option ().c_str () << "'";

        if (!message ().empty ())
          os << ": " << message ().c_str ();
      }

      const char* invalid_value::
      what () const throw ()
      {
        return "invalid option value";
      }

      // eos_reached
      //
      void eos_reached::
      print (::std::ostream& os) const
      {
        os << what ();
      }

      const char* eos_reached::
      what () const throw ()
      {
        return "end of argument stream reached";
      }

      // scanner
      //
      scanner::
      ~scanner ()
      {
      }

      // argv_scanner
      //
      bool argv_scanner::
      more ()
      {
        return i_ < argc_;
      }

      const char* argv_scanner::
      peek ()
      {
        if (i_ < argc_)
          return argv_[i_];
        else
          throw eos_reached ();
      }

      const char* argv_scanner::
      next ()
      {
        if (i_ < argc_)
        {
          const char* r (argv_[i_]);

          if (erase_)
          {
            for (int i (i_ + 1); i < argc_; ++i)
              argv_[i - 1] = argv_[i];

            --argc_;
            argv_[argc_] = 0;
          }
          else
            ++i_;

          return r;
        }
        else
          throw eos_reached ();
      }

      void argv_scanner::
      skip ()
      {
        if (i_ < argc_)
          ++i_;
        else
          throw eos_reached ();
      }

      // vector_scanner
      //
      bool vector_scanner::
      more ()
      {
        return i_ < v_.size ();
      }

      const char* vector_scanner::
      peek ()
      {
        if (i_ < v_.size ())
          return v_[i_].c_str ();
        else
          throw eos_reached ();
      }

      const char* vector_scanner::
      next ()
      {
        if (i_ < v_.size ())
          return v_[i_++].c_str ();
        else
          throw eos_reached ();
      }

      void vector_scanner::
      skip ()
      {
        if (i_ < v_.size ())
          ++i_;
        else
          throw eos_reached ();
      }

      template <typename X>
      struct parser
      {
        static void
        parse (X& x, bool& xs, scanner& s)
        {
          using namespace std;

          const char* o (s.next ());
          if (s.more ())
          {
            string v (s.next ());
            istringstream is (v);
            if (!(is >> x && is.peek () == istringstream::traits_type::eof ()))
              throw invalid_value (o, v);
          }
          else
            throw missing_value (o);

          xs = true;
        }
      };

      template <>
      struct parser<bool>
      {
        static void
        parse (bool& x, scanner& s)
        {
          s.next ();
          x = true;
        }
      };

      template <>
      struct parser<std::string>
      {
        static void
        parse (std::string& x, bool& xs, scanner& s)
        {
          const char* o (s.next ());

          if (s.more ())
            x = s.next ();
          else
            throw missing_value (o);

          xs = true;
        }
      };

      template <typename X>
      struct parser<std::vector<X> >
      {
        static void
        parse (std::vector<X>& c, bool& xs, scanner& s)
        {
          X x;
          bool dummy;
          parser<X>::parse (x, dummy, s);
          c.push_back (x);
          xs = true;
        }
      };

      template <typename X, typename C>
      struct parser<std::set<X, C> >
      {
        static void
        parse (std::set<X, C>& c, bool& xs, scanner& s)
        {
          X x;
          bool dummy;
          parser<X>::parse (x, dummy, s);
          c.insert (x);
          xs = true;
        }
      };

      template <typename K, typename V, typename C>
      struct parser<std::map<K, V, C> >
      {
        static void
        parse (std::map<K, V, C>& m, bool& xs, scanner& s)
        {
          const char* o (s.next ());

          if (s.more ())
          {
            std::string ov (s.next ());
            std::string::size_type p = ov.find ('=');

            K k = K ();
            V v = V ();
            std::string kstr (ov, 0, p);
            std::string vstr (ov, (p != std::string::npos ? p + 1 : ov.size ()));

            int ac (2);
            char* av[] =
            {
              const_cast<char*> (o),
              0
            };

            bool dummy;
            if (!kstr.empty ())
            {
              av[1] = const_cast<char*> (kstr.c_str ());
              argv_scanner s (0, ac, av);
              parser<K>::parse (k, dummy, s);
            }

            if (!vstr.empty ())
            {
              av[1] = const_cast<char*> (vstr.c_str ());
              argv_scanner s (0, ac, av);
              parser<V>::parse (v, dummy, s);
            }

            m[k] = v;
          }
          else
            throw missing_value (o);

          xs = true;
        }
      };

      template <typename X, typename T, T X::*M>
      void
      thunk (X& x, scanner& s)
      {
        parser<T>::parse (x.*M, s);
      }

      template <typename X, typename T, T X::*M, bool X::*S>
      void
      thunk (X& x, scanner& s)
      {
        parser<T>::parse (x.*M, x.*S, s);
      }
    }
  }
}

#include <map>
#include <cstring>

namespace build2
{
  namespace script
  {
    // set_options
    //

    set_options::
    set_options ()
    : exact_ (),
      newline_ (),
      whitespace_ ()
    {
    }

    set_options::
    set_options (int& argc,
                 char** argv,
                 bool erase,
                 ::build2::script::cli::unknown_mode opt,
                 ::build2::script::cli::unknown_mode arg)
    : exact_ (),
      newline_ (),
      whitespace_ ()
    {
      ::build2::script::cli::argv_scanner s (argc, argv, erase);
      _parse (s, opt, arg);
    }

    set_options::
    set_options (int start,
                 int& argc,
                 char** argv,
                 bool erase,
                 ::build2::script::cli::unknown_mode opt,
                 ::build2::script::cli::unknown_mode arg)
    : exact_ (),
      newline_ (),
      whitespace_ ()
    {
      ::build2::script::cli::argv_scanner s (start, argc, argv, erase);
      _parse (s, opt, arg);
    }

    set_options::
    set_options (int& argc,
                 char** argv,
                 int& end,
                 bool erase,
                 ::build2::script::cli::unknown_mode opt,
                 ::build2::script::cli::unknown_mode arg)
    : exact_ (),
      newline_ (),
      whitespace_ ()
    {
      ::build2::script::cli::argv_scanner s (argc, argv, erase);
      _parse (s, opt, arg);
      end = s.end ();
    }

    set_options::
    set_options (int start,
                 int& argc,
                 char** argv,
                 int& end,
                 bool erase,
                 ::build2::script::cli::unknown_mode opt,
                 ::build2::script::cli::unknown_mode arg)
    : exact_ (),
      newline_ (),
      whitespace_ ()
    {
      ::build2::script::cli::argv_scanner s (start, argc, argv, erase);
      _parse (s, opt, arg);
      end = s.end ();
    }

    set_options::
    set_options (::build2::script::cli::scanner& s,
                 ::build2::script::cli::unknown_mode opt,
                 ::build2::script::cli::unknown_mode arg)
    : exact_ (),
      newline_ (),
      whitespace_ ()
    {
      _parse (s, opt, arg);
    }

    typedef
    std::map<std::string, void (*) (set_options&, ::build2::script::cli::scanner&)>
    _cli_set_options_map;

    static _cli_set_options_map _cli_set_options_map_;

    struct _cli_set_options_map_init
    {
      _cli_set_options_map_init ()
      {
        _cli_set_options_map_["--exact"] =
        &::build2::script::cli::thunk< set_options, bool, &set_options::exact_ >;
        _cli_set_options_map_["-e"] =
        &::build2::script::cli::thunk< set_options, bool, &set_options::exact_ >;
        _cli_set_options_map_["--newline"] =
        &::build2::script::cli::thunk< set_options, bool, &set_options::newline_ >;
        _cli_set_options_map_["-n"] =
        &::build2::script::cli::thunk< set_options, bool, &set_options::newline_ >;
        _cli_set_options_map_["--whitespace"] =
        &::build2::script::cli::thunk< set_options, bool, &set_options::whitespace_ >;
        _cli_set_options_map_["-w"] =
        &::build2::script::cli::thunk< set_options, bool, &set_options::whitespace_ >;
      }
    };

    static _cli_set_options_map_init _cli_set_options_map_init_;

    bool set_options::
    _parse (const char* o, ::build2::script::cli::scanner& s)
    {
      _cli_set_options_map::const_iterator i (_cli_set_options_map_.find (o));

      if (i != _cli_set_options_map_.end ())
      {
        (*(i->second)) (*this, s);
        return true;
      }

      return false;
    }

    bool set_options::
    _parse (::build2::script::cli::scanner& s,
            ::build2::script::cli::unknown_mode opt_mode,
            ::build2::script::cli::unknown_mode arg_mode)
    {
      // Can't skip combined flags (--no-combined-flags).
      //
      assert (opt_mode != ::build2::script::cli::unknown_mode::skip);

      bool r = false;
      bool opt = true;

      while (s.more ())
      {
        const char* o = s.peek ();

        if (std::strcmp (o, "--") == 0)
        {
          opt = false;
          s.skip ();
          r = true;
          continue;
        }

        if (opt)
        {
          if (_parse (o, s))
          {
            r = true;
            continue;
          }

          if (std::strncmp (o, "-", 1) == 0 && o[1] != '\0')
          {
            // Handle combined option values.
            //
            std::string co;
            if (const char* v = std::strchr (o, '='))
            {
              co.assign (o, 0, v - o);
              ++v;

              int ac (2);
              char* av[] =
              {
                const_cast<char*> (co.c_str ()),
                const_cast<char*> (v)
              };

              ::build2::script::cli::argv_scanner ns (0, ac, av);

              if (_parse (co.c_str (), ns))
              {
                // Parsed the option but not its value?
                //
                if (ns.end () != 2)
                  throw ::build2::script::cli::invalid_value (co, v);

                s.next ();
                r = true;
                continue;
              }
              else
              {
                // Set the unknown option and fall through.
                //
                o = co.c_str ();
              }
            }

            // Handle combined flags.
            //
            char cf[3];
            {
              const char* p = o + 1;
              for (; *p != '\0'; ++p)
              {
                if (!((*p >= 'a' && *p <= 'z') ||
                      (*p >= 'A' && *p <= 'Z') ||
                      (*p >= '0' && *p <= '9')))
                  break;
              }

              if (*p == '\0')
              {
                for (p = o + 1; *p != '\0'; ++p)
                {
                  std::strcpy (cf, "-");
                  cf[1] = *p;
                  cf[2] = '\0';

                  int ac (1);
                  char* av[] =
                  {
                    cf
                  };

                  ::build2::script::cli::argv_scanner ns (0, ac, av);

                  if (!_parse (cf, ns))
                    break;
                }

                if (*p == '\0')
                {
                  // All handled.
                  //
                  s.next ();
                  r = true;
                  continue;
                }
                else
                {
                  // Set the unknown option and fall through.
                  //
                  o = cf;
                }
              }
            }

            switch (opt_mode)
            {
              case ::build2::script::cli::unknown_mode::skip:
              {
                s.skip ();
                r = true;
                continue;
              }
              case ::build2::script::cli::unknown_mode::stop:
              {
                break;
              }
              case ::build2::script::cli::unknown_mode::fail:
              {
                throw ::build2::script::cli::unknown_option (o);
              }
            }

            break;
          }
        }

        switch (arg_mode)
        {
          case ::build2::script::cli::unknown_mode::skip:
          {
            s.skip ();
            r = true;
            continue;
          }
          case ::build2::script::cli::unknown_mode::stop:
          {
            break;
          }
          case ::build2::script::cli::unknown_mode::fail:
          {
            throw ::build2::script::cli::unknown_argument (o);
          }
        }

        break;
      }

      return r;
    }

    // timeout_options
    //

    timeout_options::
    timeout_options ()
    : success_ ()
    {
    }

    timeout_options::
    timeout_options (int& argc,
                     char** argv,
                     bool erase,
                     ::build2::script::cli::unknown_mode opt,
                     ::build2::script::cli::unknown_mode arg)
    : success_ ()
    {
      ::build2::script::cli::argv_scanner s (argc, argv, erase);
      _parse (s, opt, arg);
    }

    timeout_options::
    timeout_options (int start,
                     int& argc,
                     char** argv,
                     bool erase,
                     ::build2::script::cli::unknown_mode opt,
                     ::build2::script::cli::unknown_mode arg)
    : success_ ()
    {
      ::build2::script::cli::argv_scanner s (start, argc, argv, erase);
      _parse (s, opt, arg);
    }

    timeout_options::
    timeout_options (int& argc,
                     char** argv,
                     int& end,
                     bool erase,
                     ::build2::script::cli::unknown_mode opt,
                     ::build2::script::cli::unknown_mode arg)
    : success_ ()
    {
      ::build2::script::cli::argv_scanner s (argc, argv, erase);
      _parse (s, opt, arg);
      end = s.end ();
    }

    timeout_options::
    timeout_options (int start,
                     int& argc,
                     char** argv,
                     int& end,
                     bool erase,
                     ::build2::script::cli::unknown_mode opt,
                     ::build2::script::cli::unknown_mode arg)
    : success_ ()
    {
      ::build2::script::cli::argv_scanner s (start, argc, argv, erase);
      _parse (s, opt, arg);
      end = s.end ();
    }

    timeout_options::
    timeout_options (::build2::script::cli::scanner& s,
                     ::build2::script::cli::unknown_mode opt,
                     ::build2::script::cli::unknown_mode arg)
    : success_ ()
    {
      _parse (s, opt, arg);
    }

    typedef
    std::map<std::string, void (*) (timeout_options&, ::build2::script::cli::scanner&)>
    _cli_timeout_options_map;

    static _cli_timeout_options_map _cli_timeout_options_map_;

    struct _cli_timeout_options_map_init
    {
      _cli_timeout_options_map_init ()
      {
        _cli_timeout_options_map_["--success"] =
        &::build2::script::cli::thunk< timeout_options, bool, &timeout_options::success_ >;
        _cli_timeout_options_map_["-s"] =
        &::build2::script::cli::thunk< timeout_options, bool, &timeout_options::success_ >;
      }
    };

    static _cli_timeout_options_map_init _cli_timeout_options_map_init_;

    bool timeout_options::
    _parse (const char* o, ::build2::script::cli::scanner& s)
    {
      _cli_timeout_options_map::const_iterator i (_cli_timeout_options_map_.find (o));

      if (i != _cli_timeout_options_map_.end ())
      {
        (*(i->second)) (*this, s);
        return true;
      }

      return false;
    }

    bool timeout_options::
    _parse (::build2::script::cli::scanner& s,
            ::build2::script::cli::unknown_mode opt_mode,
            ::build2::script::cli::unknown_mode arg_mode)
    {
      // Can't skip combined flags (--no-combined-flags).
      //
      assert (opt_mode != ::build2::script::cli::unknown_mode::skip);

      bool r = false;
      bool opt = true;

      while (s.more ())
      {
        const char* o = s.peek ();

        if (std::strcmp (o, "--") == 0)
        {
          opt = false;
          s.skip ();
          r = true;
          continue;
        }

        if (opt)
        {
          if (_parse (o, s))
          {
            r = true;
            continue;
          }

          if (std::strncmp (o, "-", 1) == 0 && o[1] != '\0')
          {
            // Handle combined option values.
            //
            std::string co;
            if (const char* v = std::strchr (o, '='))
            {
              co.assign (o, 0, v - o);
              ++v;

              int ac (2);
              char* av[] =
              {
                const_cast<char*> (co.c_str ()),
                const_cast<char*> (v)
              };

              ::build2::script::cli::argv_scanner ns (0, ac, av);

              if (_parse (co.c_str (), ns))
              {
                // Parsed the option but not its value?
                //
                if (ns.end () != 2)
                  throw ::build2::script::cli::invalid_value (co, v);

                s.next ();
                r = true;
                continue;
              }
              else
              {
                // Set the unknown option and fall through.
                //
                o = co.c_str ();
              }
            }

            // Handle combined flags.
            //
            char cf[3];
            {
              const char* p = o + 1;
              for (; *p != '\0'; ++p)
              {
                if (!((*p >= 'a' && *p <= 'z') ||
                      (*p >= 'A' && *p <= 'Z') ||
                      (*p >= '0' && *p <= '9')))
                  break;
              }

              if (*p == '\0')
              {
                for (p = o + 1; *p != '\0'; ++p)
                {
                  std::strcpy (cf, "-");
                  cf[1] = *p;
                  cf[2] = '\0';

                  int ac (1);
                  char* av[] =
                  {
                    cf
                  };

                  ::build2::script::cli::argv_scanner ns (0, ac, av);

                  if (!_parse (cf, ns))
                    break;
                }

                if (*p == '\0')
                {
                  // All handled.
                  //
                  s.next ();
                  r = true;
                  continue;
                }
                else
                {
                  // Set the unknown option and fall through.
                  //
                  o = cf;
                }
              }
            }

            switch (opt_mode)
            {
              case ::build2::script::cli::unknown_mode::skip:
              {
                s.skip ();
                r = true;
                continue;
              }
              case ::build2::script::cli::unknown_mode::stop:
              {
                break;
              }
              case ::build2::script::cli::unknown_mode::fail:
              {
                throw ::build2::script::cli::unknown_option (o);
              }
            }

            break;
          }
        }

        switch (arg_mode)
        {
          case ::build2::script::cli::unknown_mode::skip:
          {
            s.skip ();
            r = true;
            continue;
          }
          case ::build2::script::cli::unknown_mode::stop:
          {
            break;
          }
          case ::build2::script::cli::unknown_mode::fail:
          {
            throw ::build2::script::cli::unknown_argument (o);
          }
        }

        break;
      }

      return r;
    }

    // export_options
    //

    export_options::
    export_options ()
    : unset_ (),
      unset_specified_ (false),
      clear_ (),
      clear_specified_ (false)
    {
    }

    export_options::
    export_options (int& argc,
                    char** argv,
                    bool erase,
                    ::build2::script::cli::unknown_mode opt,
                    ::build2::script::cli::unknown_mode arg)
    : unset_ (),
      unset_specified_ (false),
      clear_ (),
      clear_specified_ (false)
    {
      ::build2::script::cli::argv_scanner s (argc, argv, erase);
      _parse (s, opt, arg);
    }

    export_options::
    export_options (int start,
                    int& argc,
                    char** argv,
                    bool erase,
                    ::build2::script::cli::unknown_mode opt,
                    ::build2::script::cli::unknown_mode arg)
    : unset_ (),
      unset_specified_ (false),
      clear_ (),
      clear_specified_ (false)
    {
      ::build2::script::cli::argv_scanner s (start, argc, argv, erase);
      _parse (s, opt, arg);
    }

    export_options::
    export_options (int& argc,
                    char** argv,
                    int& end,
                    bool erase,
                    ::build2::script::cli::unknown_mode opt,
                    ::build2::script::cli::unknown_mode arg)
    : unset_ (),
      unset_specified_ (false),
      clear_ (),
      clear_specified_ (false)
    {
      ::build2::script::cli::argv_scanner s (argc, argv, erase);
      _parse (s, opt, arg);
      end = s.end ();
    }

    export_options::
    export_options (int start,
                    int& argc,
                    char** argv,
                    int& end,
                    bool erase,
                    ::build2::script::cli::unknown_mode opt,
                    ::build2::script::cli::unknown_mode arg)
    : unset_ (),
      unset_specified_ (false),
      clear_ (),
      clear_specified_ (false)
    {
      ::build2::script::cli::argv_scanner s (start, argc, argv, erase);
      _parse (s, opt, arg);
      end = s.end ();
    }

    export_options::
    export_options (::build2::script::cli::scanner& s,
                    ::build2::script::cli::unknown_mode opt,
                    ::build2::script::cli::unknown_mode arg)
    : unset_ (),
      unset_specified_ (false),
      clear_ (),
      clear_specified_ (false)
    {
      _parse (s, opt, arg);
    }

    typedef
    std::map<std::string, void (*) (export_options&, ::build2::script::cli::scanner&)>
    _cli_export_options_map;

    static _cli_export_options_map _cli_export_options_map_;

    struct _cli_export_options_map_init
    {
      _cli_export_options_map_init ()
      {
        _cli_export_options_map_["--unset"] =
        &::build2::script::cli::thunk< export_options, vector<string>, &export_options::unset_,
          &export_options::unset_specified_ >;
        _cli_export_options_map_["-u"] =
        &::build2::script::cli::thunk< export_options, vector<string>, &export_options::unset_,
          &export_options::unset_specified_ >;
        _cli_export_options_map_["--clear"] =
        &::build2::script::cli::thunk< export_options, vector<string>, &export_options::clear_,
          &export_options::clear_specified_ >;
        _cli_export_options_map_["-c"] =
        &::build2::script::cli::thunk< export_options, vector<string>, &export_options::clear_,
          &export_options::clear_specified_ >;
      }
    };

    static _cli_export_options_map_init _cli_export_options_map_init_;

    bool export_options::
    _parse (const char* o, ::build2::script::cli::scanner& s)
    {
      _cli_export_options_map::const_iterator i (_cli_export_options_map_.find (o));

      if (i != _cli_export_options_map_.end ())
      {
        (*(i->second)) (*this, s);
        return true;
      }

      return false;
    }

    bool export_options::
    _parse (::build2::script::cli::scanner& s,
            ::build2::script::cli::unknown_mode opt_mode,
            ::build2::script::cli::unknown_mode arg_mode)
    {
      // Can't skip combined flags (--no-combined-flags).
      //
      assert (opt_mode != ::build2::script::cli::unknown_mode::skip);

      bool r = false;
      bool opt = true;

      while (s.more ())
      {
        const char* o = s.peek ();

        if (std::strcmp (o, "--") == 0)
        {
          opt = false;
          s.skip ();
          r = true;
          continue;
        }

        if (opt)
        {
          if (_parse (o, s))
          {
            r = true;
            continue;
          }

          if (std::strncmp (o, "-", 1) == 0 && o[1] != '\0')
          {
            // Handle combined option values.
            //
            std::string co;
            if (const char* v = std::strchr (o, '='))
            {
              co.assign (o, 0, v - o);
              ++v;

              int ac (2);
              char* av[] =
              {
                const_cast<char*> (co.c_str ()),
                const_cast<char*> (v)
              };

              ::build2::script::cli::argv_scanner ns (0, ac, av);

              if (_parse (co.c_str (), ns))
              {
                // Parsed the option but not its value?
                //
                if (ns.end () != 2)
                  throw ::build2::script::cli::invalid_value (co, v);

                s.next ();
                r = true;
                continue;
              }
              else
              {
                // Set the unknown option and fall through.
                //
                o = co.c_str ();
              }
            }

            // Handle combined flags.
            //
            char cf[3];
            {
              const char* p = o + 1;
              for (; *p != '\0'; ++p)
              {
                if (!((*p >= 'a' && *p <= 'z') ||
                      (*p >= 'A' && *p <= 'Z') ||
                      (*p >= '0' && *p <= '9')))
                  break;
              }

              if (*p == '\0')
              {
                for (p = o + 1; *p != '\0'; ++p)
                {
                  std::strcpy (cf, "-");
                  cf[1] = *p;
                  cf[2] = '\0';

                  int ac (1);
                  char* av[] =
                  {
                    cf
                  };

                  ::build2::script::cli::argv_scanner ns (0, ac, av);

                  if (!_parse (cf, ns))
                    break;
                }

                if (*p == '\0')
                {
                  // All handled.
                  //
                  s.next ();
                  r = true;
                  continue;
                }
                else
                {
                  // Set the unknown option and fall through.
                  //
                  o = cf;
                }
              }
            }

            switch (opt_mode)
            {
              case ::build2::script::cli::unknown_mode::skip:
              {
                s.skip ();
                r = true;
                continue;
              }
              case ::build2::script::cli::unknown_mode::stop:
              {
                break;
              }
              case ::build2::script::cli::unknown_mode::fail:
              {
                throw ::build2::script::cli::unknown_option (o);
              }
            }

            break;
          }
        }

        switch (arg_mode)
        {
          case ::build2::script::cli::unknown_mode::skip:
          {
            s.skip ();
            r = true;
            continue;
          }
          case ::build2::script::cli::unknown_mode::stop:
          {
            break;
          }
          case ::build2::script::cli::unknown_mode::fail:
          {
            throw ::build2::script::cli::unknown_argument (o);
          }
        }

        break;
      }

      return r;
    }
  }
}

// Begin epilogue.
//
//
// End epilogue.

