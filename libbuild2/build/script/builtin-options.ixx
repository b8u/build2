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
  namespace build
  {
    namespace script
    {
      // depdb_dyndep_options
      //

      inline const path& depdb_dyndep_options::
      file () const
      {
        return this->file_;
      }

      inline path& depdb_dyndep_options::
      file ()
      {
        return this->file_;
      }

      inline void depdb_dyndep_options::
      file (const path& x)
      {
        this->file_ = x;
      }

      inline bool depdb_dyndep_options::
      file_specified () const
      {
        return this->file_specified_;
      }

      inline void depdb_dyndep_options::
      file_specified (bool x)
      {
        this->file_specified_ = x;
      }

      inline const string& depdb_dyndep_options::
      format () const
      {
        return this->format_;
      }

      inline string& depdb_dyndep_options::
      format ()
      {
        return this->format_;
      }

      inline void depdb_dyndep_options::
      format (const string& x)
      {
        this->format_ = x;
      }

      inline bool depdb_dyndep_options::
      format_specified () const
      {
        return this->format_specified_;
      }

      inline void depdb_dyndep_options::
      format_specified (bool x)
      {
        this->format_specified_ = x;
      }

      inline const string& depdb_dyndep_options::
      what () const
      {
        return this->what_;
      }

      inline string& depdb_dyndep_options::
      what ()
      {
        return this->what_;
      }

      inline void depdb_dyndep_options::
      what (const string& x)
      {
        this->what_ = x;
      }

      inline bool depdb_dyndep_options::
      what_specified () const
      {
        return this->what_specified_;
      }

      inline void depdb_dyndep_options::
      what_specified (bool x)
      {
        this->what_specified_ = x;
      }

      inline const dir_paths& depdb_dyndep_options::
      include_path () const
      {
        return this->include_path_;
      }

      inline dir_paths& depdb_dyndep_options::
      include_path ()
      {
        return this->include_path_;
      }

      inline void depdb_dyndep_options::
      include_path (const dir_paths& x)
      {
        this->include_path_ = x;
      }

      inline bool depdb_dyndep_options::
      include_path_specified () const
      {
        return this->include_path_specified_;
      }

      inline void depdb_dyndep_options::
      include_path_specified (bool x)
      {
        this->include_path_specified_ = x;
      }

      inline const string& depdb_dyndep_options::
      default_type () const
      {
        return this->default_type_;
      }

      inline string& depdb_dyndep_options::
      default_type ()
      {
        return this->default_type_;
      }

      inline void depdb_dyndep_options::
      default_type (const string& x)
      {
        this->default_type_ = x;
      }

      inline bool depdb_dyndep_options::
      default_type_specified () const
      {
        return this->default_type_specified_;
      }

      inline void depdb_dyndep_options::
      default_type_specified (bool x)
      {
        this->default_type_specified_ = x;
      }

      inline const bool& depdb_dyndep_options::
      adhoc () const
      {
        return this->adhoc_;
      }

      inline bool& depdb_dyndep_options::
      adhoc ()
      {
        return this->adhoc_;
      }

      inline void depdb_dyndep_options::
      adhoc (const bool& x)
      {
        this->adhoc_ = x;
      }

      inline const dir_path& depdb_dyndep_options::
      cwd () const
      {
        return this->cwd_;
      }

      inline dir_path& depdb_dyndep_options::
      cwd ()
      {
        return this->cwd_;
      }

      inline void depdb_dyndep_options::
      cwd (const dir_path& x)
      {
        this->cwd_ = x;
      }

      inline bool depdb_dyndep_options::
      cwd_specified () const
      {
        return this->cwd_specified_;
      }

      inline void depdb_dyndep_options::
      cwd_specified (bool x)
      {
        this->cwd_specified_ = x;
      }

      inline const bool& depdb_dyndep_options::
      drop_cycles () const
      {
        return this->drop_cycles_;
      }

      inline bool& depdb_dyndep_options::
      drop_cycles ()
      {
        return this->drop_cycles_;
      }

      inline void depdb_dyndep_options::
      drop_cycles (const bool& x)
      {
        this->drop_cycles_ = x;
      }
    }
  }
}

// Begin epilogue.
//
//
// End epilogue.
