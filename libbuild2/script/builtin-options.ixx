// -*- C++ -*-
//
// This file was generated by CLI, a command line interface
// compiler for C++.
//

// Begin prologue.
//
//
// End prologue.

#include <cassert>

namespace build2
{
  namespace script
  {
    namespace cli
    {
      // unknown_mode
      //
      inline unknown_mode::
      unknown_mode (value v)
      : v_ (v)
      {
      }

      // exception
      //
      inline ::std::ostream&
      operator<< (::std::ostream& os, const exception& e)
      {
        e.print (os);
        return os;
      }

      // unknown_option
      //
      inline unknown_option::
      unknown_option (const std::string& option)
      : option_ (option)
      {
      }

      inline const std::string& unknown_option::
      option () const
      {
        return option_;
      }

      // unknown_argument
      //
      inline unknown_argument::
      unknown_argument (const std::string& argument)
      : argument_ (argument)
      {
      }

      inline const std::string& unknown_argument::
      argument () const
      {
        return argument_;
      }

      // missing_value
      //
      inline missing_value::
      missing_value (const std::string& option)
      : option_ (option)
      {
      }

      inline const std::string& missing_value::
      option () const
      {
        return option_;
      }

      // invalid_value
      //
      inline invalid_value::
      invalid_value (const std::string& option,
                     const std::string& value,
                     const std::string& message)
      : option_ (option),
        value_ (value),
        message_ (message)
      {
      }

      inline const std::string& invalid_value::
      option () const
      {
        return option_;
      }

      inline const std::string& invalid_value::
      value () const
      {
        return value_;
      }

      inline const std::string& invalid_value::
      message () const
      {
        return message_;
      }

      // argv_scanner
      //
      inline argv_scanner::
      argv_scanner (int& argc, char** argv, bool erase)
      : i_ (1), argc_ (argc), argv_ (argv), erase_ (erase)
      {
      }

      inline argv_scanner::
      argv_scanner (int start, int& argc, char** argv, bool erase)
      : i_ (start), argc_ (argc), argv_ (argv), erase_ (erase)
      {
      }

      inline int argv_scanner::
      end () const
      {
        return i_;
      }

      // vector_scanner
      //
      inline vector_scanner::
      vector_scanner (const std::vector<std::string>& v, std::size_t i)
      : v_ (v), i_ (i)
      {
      }

      inline std::size_t vector_scanner::
      end () const
      {
        return i_;
      }

      inline void vector_scanner::
      reset (std::size_t i)
      {
        i_ = i;
      }
    }
  }
}

namespace build2
{
  namespace script
  {
    // set_options
    //

    inline const bool& set_options::
    exact () const
    {
      return this->exact_;
    }

    inline bool& set_options::
    exact ()
    {
      return this->exact_;
    }

    inline void set_options::
    exact (const bool& x)
    {
      this->exact_ = x;
    }

    inline const bool& set_options::
    newline () const
    {
      return this->newline_;
    }

    inline bool& set_options::
    newline ()
    {
      return this->newline_;
    }

    inline void set_options::
    newline (const bool& x)
    {
      this->newline_ = x;
    }

    inline const bool& set_options::
    whitespace () const
    {
      return this->whitespace_;
    }

    inline bool& set_options::
    whitespace ()
    {
      return this->whitespace_;
    }

    inline void set_options::
    whitespace (const bool& x)
    {
      this->whitespace_ = x;
    }

    // timeout_options
    //

    inline const bool& timeout_options::
    success () const
    {
      return this->success_;
    }

    inline bool& timeout_options::
    success ()
    {
      return this->success_;
    }

    inline void timeout_options::
    success (const bool& x)
    {
      this->success_ = x;
    }

    // export_options
    //

    inline const vector<string>& export_options::
    unset () const
    {
      return this->unset_;
    }

    inline vector<string>& export_options::
    unset ()
    {
      return this->unset_;
    }

    inline void export_options::
    unset (const vector<string>& x)
    {
      this->unset_ = x;
    }

    inline bool export_options::
    unset_specified () const
    {
      return this->unset_specified_;
    }

    inline void export_options::
    unset_specified (bool x)
    {
      this->unset_specified_ = x;
    }

    inline const vector<string>& export_options::
    clear () const
    {
      return this->clear_;
    }

    inline vector<string>& export_options::
    clear ()
    {
      return this->clear_;
    }

    inline void export_options::
    clear (const vector<string>& x)
    {
      this->clear_ = x;
    }

    inline bool export_options::
    clear_specified () const
    {
      return this->clear_specified_;
    }

    inline void export_options::
    clear_specified (bool x)
    {
      this->clear_specified_ = x;
    }
  }
}

// Begin epilogue.
//
//
// End epilogue.
