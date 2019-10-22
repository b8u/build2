// file      : tests/libbuild2/driver.cxx -*- C++ -*-
// copyright : Copyright (c) 2014-2019 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#include <libbuild2/types.hxx>
#include <libbuild2/utility.hxx>

#include <libbuild2/context.hxx>
#include <libbuild2/scheduler.hxx>

#include <libbuild2/in/init.hxx>
#include <libbuild2/bin/init.hxx>
#include <libbuild2/c/init.hxx>
#include <libbuild2/cc/init.hxx>
#include <libbuild2/cxx/init.hxx>
#include <libbuild2/bash/init.hxx>
#include <libbuild2/version/init.hxx>

using namespace build2;

int
main (int, char* argv[])
{
  // Fake build system driver, default verbosity.
  //
  init_diag (1);
  init (nullptr, argv[0]);

  bin::build2_bin_load ();
  cc::build2_cc_load ();
  c::build2_c_load ();
  cxx::build2_cxx_load ();
  version::build2_version_load ();
  in::build2_in_load ();
  bash::build2_bash_load ();

  // Serial execution.
  //
  scheduler sched (1);
  global_mutex_shards shards (1);
  context ctx (sched, shards);

  return 0;
}
