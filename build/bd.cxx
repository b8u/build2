// file      : build/bd.cxx -*- C++ -*-
// copyright : Copyright (c) 2014-2015 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <time.h> // tzset()

#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <system_error>

#include <build/target>
#include <build/rule>
#include <build/process>
#include <build/diagnostics>

#include <build/lexer>
#include <build/parser>

using namespace std;

namespace build
{
  bool
  match (target& t)
  {
    // Because we match the target first and then prerequisites,
    // any additional dependency information injected by the rule
    // will be covered as well.
    //
    if (!t.recipe ())
    {
      for (auto ti (&t.type_id ());
           ti != nullptr && !t.recipe ();
           ti = ti->base)
      {
        for (auto rs (rules.equal_range (ti->id));
             rs.first != rs.second;
             ++rs.first)
        {
          const rule& ru (rs.first->second);

          recipe re;

          {
            auto g (
              make_exception_guard (
                [] (target& t)
                {
                  cerr << "info: while matching a rule for target " << t << endl;
                },
                t));

            re = ru.match (t);
          }

          if (re)
          {
            t.recipe (re);
            break;
          }
        }
      }

      if (!t.recipe ())
      {
        cerr << "error: no rule to update target " << t << endl;
        return false;
      }
    }

    for (target& p: t.prerequisites ())
    {
      if (!match (p))
      {
        cerr << "info: required by " << t << endl;
        return false;
      }
    }

    return true;
  }

  target_state
  update (target& t)
  {
    assert (t.state () == target_state::unknown);

    target_state ts;

    for (target& p: t.prerequisites ())
    {
      if (p.state () == target_state::unknown)
      {
        p.state ((ts = update (p)));

        if (ts == target_state::failed)
          return ts;
      }
    }

    const recipe& r (t.recipe ());

    {
      auto g (
        make_exception_guard (
          [] (target& t)
          {
            cerr << "info: while building target " << t << endl;
          },
          t));

      ts = r (t);
    }

    assert (ts != target_state::unknown);
    t.state (ts);
    return ts;
  }
}

#include <build/native>

#include <build/cxx/target>
#include <build/cxx/rule>


using namespace build;

int
main (int argc, char* argv[])
{
  // Initialize time conversion data that is used by localtime_r().
  //
  tzset ();

  // Parse buildfile.
  //
  path bf ("buildfile");

  ifstream ifs (bf.string ().c_str ());
  if (!ifs.is_open ())
  {
    cerr << "error: unable to open " << bf << " in read mode" << endl;
    return 1;
  }

  ifs.exceptions (ifstream::failbit | ifstream::badbit);
  parser p;

  try
  {
    p.parse (ifs, bf);
  }
  catch (const lexer_error&)
  {
    return 1; // Diagnostics has already been issued.
  }
  catch (const parser_error&)
  {
    return 1; // Diagnostics has already been issued.
  }
  catch (const std::ios_base::failure&)
  {
    cerr << "error: failed to read from " << bf << endl;
    return 1;
  }

  return 0;


  // Register rules.
  //
  cxx::link cxx_link;
  rules.emplace (typeid (exe), cxx_link);

  cxx::compile cxx_compile;
  rules.emplace (typeid (obj), cxx_compile);

  default_path_rule path_exists;
  rules.emplace (typeid (path_target), path_exists);

  //
  //
  using namespace build::cxx;

  exe bd ("bd");
  obj bd_o ("bd");
  bd.prerequisite (bd_o);

  cxx::cxx bd_cxx ("bd");
  bd_cxx.path (path ("bd.cxx"));

  bd_o.prerequisite (bd_cxx);

  //
  //
  try
  {
    if (!match (bd))
      return 1; // Diagnostics has already been issued.

    switch (update (bd))
    {
    case target_state::uptodate:
      {
        cerr << "info: target " << bd << " is up to date" << endl;
        break;
      }
    case target_state::updated:
      break;
    case target_state::failed:
      {
        cerr << "error: failed to update target " << bd << endl;
        return 1;
      }
    case target_state::unknown:
      assert (false);
    }
  }
  catch (const error&)
  {
    return 1; // Diagnostics has already been issued.
  }
  catch (const std::exception& e)
  {
    cerr << "error: " << e.what () << endl;
    return 1;
  }
}
