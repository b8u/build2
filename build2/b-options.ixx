// -*- C++ -*-
//
// This file was generated by CLI, a command line interface
// compiler for C++.
//

// Begin prologue.
//
//
// End prologue.

namespace build2
{
  namespace cl
  {
    // usage_para
    //
    inline usage_para::
    usage_para (value v)
    : v_ (v)
    {
    }

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
                   const std::string& value)
    : option_ (option),  value_ (value)
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

    // file_io_failure
    //
    inline file_io_failure::
    file_io_failure (const std::string& file)
    : file_ (file)
    {
    }

    inline const std::string& file_io_failure::
    file () const
    {
      return file_;
    }

    // unmatched_quote
    //
    inline unmatched_quote::
    unmatched_quote (const std::string& argument)
    : argument_ (argument)
    {
    }

    inline const std::string& unmatched_quote::
    argument () const
    {
      return argument_;
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

    // argv_file_scanner
    //
    inline argv_file_scanner::
    argv_file_scanner (int& argc,
                       char** argv,
                       const std::string& option,
                       bool erase)
    : argv_scanner (argc, argv, erase),
      option_ (option),
      options_ (&option_info_),
      options_count_ (1),
      skip_ (false)
    {
      option_info_.option = option_.c_str ();
      option_info_.search_func = 0;
    }

    inline argv_file_scanner::
    argv_file_scanner (int start,
                       int& argc,
                       char** argv,
                       const std::string& option,
                       bool erase)
    : argv_scanner (start, argc, argv, erase),
      option_ (option),
      options_ (&option_info_),
      options_count_ (1),
      skip_ (false)
    {
      option_info_.option = option_.c_str ();
      option_info_.search_func = 0;
    }

    inline argv_file_scanner::
    argv_file_scanner (int& argc,
                       char** argv,
                       const option_info* options,
                       std::size_t options_count,
                       bool erase)
    : argv_scanner (argc, argv, erase),
      options_ (options),
      options_count_ (options_count),
      skip_ (false)
    {
    }

    inline argv_file_scanner::
    argv_file_scanner (int start,
                       int& argc,
                       char** argv,
                       const option_info* options,
                       std::size_t options_count,
                       bool erase)
    : argv_scanner (start, argc, argv, erase),
      options_ (options),
      options_count_ (options_count),
      skip_ (false)
    {
    }
  }
}

namespace build2
{
  // options
  //

  inline const bool& options::
  v () const
  {
    return this->v_;
  }

  inline const bool& options::
  V () const
  {
    return this->V_;
  }

  inline const bool& options::
  quiet () const
  {
    return this->quiet_;
  }

  inline const uint16_t& options::
  verbose () const
  {
    return this->verbose_;
  }

  inline bool options::
  verbose_specified () const
  {
    return this->verbose_specified_;
  }

  inline const size_t& options::
  jobs () const
  {
    return this->jobs_;
  }

  inline bool options::
  jobs_specified () const
  {
    return this->jobs_specified_;
  }

  inline const size_t& options::
  max_jobs () const
  {
    return this->max_jobs_;
  }

  inline bool options::
  max_jobs_specified () const
  {
    return this->max_jobs_specified_;
  }

  inline const size_t& options::
  queue_depth () const
  {
    return this->queue_depth_;
  }

  inline bool options::
  queue_depth_specified () const
  {
    return this->queue_depth_specified_;
  }

  inline const bool& options::
  serial_stop () const
  {
    return this->serial_stop_;
  }

  inline const bool& options::
  no_column () const
  {
    return this->no_column_;
  }

  inline const bool& options::
  no_line () const
  {
    return this->no_line_;
  }

  inline const path& options::
  buildfile () const
  {
    return this->buildfile_;
  }

  inline bool options::
  buildfile_specified () const
  {
    return this->buildfile_specified_;
  }

  inline const path& options::
  config_guess () const
  {
    return this->config_guess_;
  }

  inline bool options::
  config_guess_specified () const
  {
    return this->config_guess_specified_;
  }

  inline const path& options::
  config_sub () const
  {
    return this->config_sub_;
  }

  inline bool options::
  config_sub_specified () const
  {
    return this->config_sub_specified_;
  }

  inline const string& options::
  pager () const
  {
    return this->pager_;
  }

  inline bool options::
  pager_specified () const
  {
    return this->pager_specified_;
  }

  inline const strings& options::
  pager_option () const
  {
    return this->pager_option_;
  }

  inline bool options::
  pager_option_specified () const
  {
    return this->pager_option_specified_;
  }

  inline const bool& options::
  help () const
  {
    return this->help_;
  }

  inline const bool& options::
  version () const
  {
    return this->version_;
  }
}

// Begin epilogue.
//
//
// End epilogue.
