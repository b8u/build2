// file      : build2/cc/parser.hxx -*- C++ -*-
// copyright : Copyright (c) 2014-2017 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#ifndef BUILD2_CC_PARSER_HXX
#define BUILD2_CC_PARSER_HXX

#include <build2/types.hxx>
#include <build2/utility.hxx>

#include <build2/diagnostics.hxx>

#include <build2/cc/types.hxx>

namespace build2
{
  namespace cc
  {
    // Extract translation unit information from a preprocessed C/C++ source.
    //
    struct token;
    class lexer;

    class parser
    {
    public:
      translation_unit
      parse (istream&, const path& name);

    private:
      void
      parse_import (token&, bool);

      void
      parse_module (token&, bool);

      string
      parse_module_name (token&);

    public:
      uint64_t export_pos; // Temporary hack, see parse_unit().

    private:
      lexer* l_;
      translation_unit* u_;
    };
  }
}

#endif // BUILD2_CC_PARSER_HXX
