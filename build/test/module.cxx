// file      : build/test/module.cxx -*- C++ -*-
// copyright : Copyright (c) 2014-2015 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#include <build/test/module>

#include <build/scope>
#include <build/target>
#include <build/rule>
#include <build/diagnostics>

#include <build/test/operation>
#include <build/test/rule>

using namespace std;
using namespace butl;

namespace build
{
  namespace test
  {
    static rule rule_;

    extern "C" void
    test_boot (scope& root, const location&, unique_ptr<module>&)
    {
      tracer trace ("test::boot");

      level5 ([&]{trace << "for " << root.out_path ();});

      // Register the test operation.
      //
      root.operations.insert (test_id, test);
    }

    extern "C" bool
    test_init (scope& root,
               scope&,
               const location& l,
               unique_ptr<module>&,
               bool first,
               bool)
    {
      tracer trace ("test::init");

      if (!first)
      {
        warn (l) << "multiple test module initializations";
        return true;
      }

      const dir_path& out_root (root.out_path ());
      level5 ([&]{trace << "for " << out_root;});

      // Enter module variables.
      //
      {
        auto& v (var_pool);

        v.find ("test", bool_type);
        v.find ("test.input", name_type);
        v.find ("test.output", name_type);
        v.find ("test.roundtrip", name_type);
        v.find ("test.options", strings_type);
        v.find ("test.arguments", strings_type);
      }

      // Register rules.
      //
      {
        auto& r (root.rules);

        // Register our test running rule.
        //
        r.insert<target> (perform_test_id, "test", rule_);

        // Register our rule for the dist meta-operation. We need
        // to do this because we have "ad-hoc prerequisites" (test
        // input/output files) that need to be entered into the
        // target list.
        //
        r.insert<target> (dist_id, test_id, "test", rule_);
      }

      return true;
    }
  }
}
