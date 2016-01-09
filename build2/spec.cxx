// file      : build2/spec.cxx -*- C++ -*-
// copyright : Copyright (c) 2014-2016 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#include <build2/spec>

#include <ostream>

#include <build2/context>
#include <build2/diagnostics>

using namespace std;

namespace build2
{
  ostream&
  operator<< (ostream& os, const targetspec& s)
  {
    if (!s.src_base.empty ())
    {
      string d (diag_relative (s.src_base, false));

      if (!d.empty ())
        os << d << '@';
    }

    os << s.name;
    return os;
  }

  ostream&
  operator<< (ostream& os, const opspec& s)
  {
    bool hn (!s.name.empty ());
    bool ht (!s.empty ());

    //os << s.name;
    os << (hn ? "\"" : "") << s.name << (hn ? "\"" : "");

    if (hn && ht)
      os << '(';

    for (auto b (s.begin ()), i (b); i != s.end (); ++i)
      os << (i != b ? " " : "") << *i;

    if (hn && ht)
      os << ')';

    return os;
  }

  ostream&
  operator<< (ostream& os, const metaopspec& s)
  {
    bool hn (!s.name.empty ());
    bool ho (!s.empty ());

    //os << s.name;
    os << (hn ? "\'" : "") << s.name << (hn ? "\'" : "");

    if (hn && ho)
      os << '(';

    for (auto b (s.begin ()), i (b); i != s.end (); ++i)
      os << (i != b ? " " : "") << *i;

    if (hn && ho)
      os << ')';

    return os;
  }

  ostream&
  operator<< (ostream& os, const buildspec& s)
  {
    for (auto b (s.begin ()), i (b); i != s.end (); ++i)
      os << (i != b ? " " : "") << *i;

    return os;
  }
}
