// file      : libbuild2/install/rule.hxx -*- C++ -*-
// license   : MIT; see accompanying LICENSE file

#ifndef LIBBUILD2_INSTALL_RULE_HXX
#define LIBBUILD2_INSTALL_RULE_HXX

#include <libbuild2/types.hxx>
#include <libbuild2/utility.hxx>

#include <libbuild2/rule.hxx>
#include <libbuild2/action.hxx>
#include <libbuild2/target.hxx>
#include <libbuild2/filesystem.hxx>

#include <libbuild2/export.hxx>

namespace build2
{
  namespace install
  {
    class LIBBUILD2_SYMEXPORT alias_rule: public simple_rule
    {
    public:
      virtual bool
      match (action, target&) const override;

      // Return NULL if this prerequisite should be ignored and pointer to its
      // target otherwise.
      //
      // The default implementation ignores prerequsites that are outside of
      // the installation scope (see install_scope() for details).
      //
      // The prerequisite is passed as an iterator allowing the filter to
      // "see" inside groups.
      //
      using prerequisite_iterator =
        prerequisite_members_range<group_prerequisites>::iterator;

      virtual const target*
      filter (const scope*,
              action, const target&, prerequisite_iterator&) const;

      virtual const target*
      filter (const scope*, action, const target&, const prerequisite&) const;

      virtual recipe
      apply (action, target&) const override;

      alias_rule () {}
      static const alias_rule instance;
    };

    class fsdir_rule: public simple_rule
    {
    public:
      virtual bool
      match (action, target&) const override;

      virtual recipe
      apply (action, target&) const override;

      fsdir_rule () {}
      static const fsdir_rule instance;
    };

    // In addition to the alias rule's semantics, this rule sees through to
    // the group's members.
    //
    // The default group_rule::instance matches any target for which it was
    // registered. It is to be used for non-see-through groups that should
    // exhibit the see-through behavior for install (see lib{} in the bin
    // module for an example).
    //
    // We also register (for all targets) another instance of this rule that
    // only matches see-through groups.
    //
    class LIBBUILD2_SYMEXPORT group_rule: public alias_rule
    {
    public:
      virtual bool
      match (action, target&) const override;

      // Return NULL if this group member should be ignored and pointer to its
      // target otherwise.
      //
      // The default implementation accepts all members.
      //
      virtual const target*
      filter (action, const target&, const target& group_member) const;

      // Return NULL if this prerequisite should be ignored and pointer to its
      // target otherwise.
      //
      // The same semantics as in file_rule below.
      //
      using alias_rule::filter; // "Unhide" to make Clang happy.

      virtual const target*
      filter (const scope*,
              action, const target&,
              const prerequisite&) const override;

      virtual recipe
      apply (action, target&) const override;

      group_rule (bool sto): see_through_only (sto) {}
      static const group_rule instance;

      bool see_through_only;
    };

    struct install_dir;

    class LIBBUILD2_SYMEXPORT file_rule: public simple_rule
    {
    public:
      virtual bool
      match (action, target&) const override;

      // Return NULL if this prerequisite should be ignored and pointer to its
      // target otherwise.
      //
      // The default implementation ignores prerequsites that are outside of
      // the installation scope (see install_scope() for details). It also
      // ignores exe{} prerequisites assuming an exe{} listed for a file
      // target is there to execute (e.g., to generate that target) and
      // normally should not be installed (an exe{} would typically be
      // installed via the dir{./} alias). But this can be overridden with a
      // prerequisite-specific install=true, for example:
      //
      // exe{foo}: exe{bar}: install = true # foo runs bar
      //
      // The prerequisite is passed as an iterator allowing the filter to
      // "see" inside groups.
      //
      using prerequisite_iterator =
        prerequisite_members_range<group_prerequisites>::iterator;

      virtual const target*
      filter (const scope*,
              action, const target&, prerequisite_iterator&) const;

      virtual const target*
      filter (const scope*, action, const target&, const prerequisite&) const;

      virtual recipe
      apply (action, target&) const override;

      // Implementation of apply() that returns empty_recipe if the target is
      // not installable.
      //
      recipe
      apply_impl (action, target&) const;

      static target_state
      perform_update (action, const target&);

      // Extra un/installation hooks. Return true if anything was actually
      // un/installed.
      //
      using install_dir = install::install_dir; // For derived rules.

      virtual bool
      install_extra (const file&, const install_dir&) const;

      virtual bool
      uninstall_extra (const file&, const install_dir&) const;

      // Lower-level pre/post installation hooks that can be used to override
      // the source file path being installed (for example, to implement
      // post-processing, etc).
      //
      // Note that one cannot generally perform post-processing in-place
      // because of permissions.
      //
      virtual auto_rmfile
      install_pre (const file&, const install_dir&) const;

      virtual bool
      install_post (const file&, const install_dir&, auto_rmfile&&) const;

      // Installation/uninstallation "commands".
      //
      // The verbosity argument specified the level to start printing the
      // command at. Note that these functions respect the dry_run flag.

      // Install (create) a directory:
      //
      // install -d <dir>
      //
      // Note: <dir> is expected to be absolute.
      //
      // Note that the target argument only specifies which target caused
      // this directory to be created.
      //
      static void
      install_d (const scope& rs,
                 const install_dir& base,
                 const dir_path& dir,
                 const file& target,
                 uint16_t verbosity = 1);

      // Install a file:
      //
      // install <file> <base>/        # if <name> is empty
      // install <file> <base>/<name>  # if <name> is not empty
      //
      // Note that <name> should be a simple path.
      //
      static void
      install_f (const scope& rs,
                 const install_dir& base,
                 const path& name,
                 const file& target,
                 const path& file,
                 uint16_t verbosity = 1);

      // Install (make) a symlink:
      //
      // install -l <link_target> <base>/<link>
      //
      // Which is essentially:
      //
      // ln -s <link_target> <base>/<link>
      //
      // Note that <link> should be a simple path. Note that <link_target>
      // must not be absolute if relocatable installation is requested
      // (config.install.relocatable).
      //
      // Note that the target argument only specifies which target this
      // symlink "belongs" to.
      //
      static void
      install_l (const scope& rs,
                 const install_dir& base,
                 const path& link,
                 const file& target,
                 const path& link_target,
                 uint16_t verbosity = 1);

      // Uninstall (remove) a file or symlink:
      //
      // uninstall <target> <base>/  # rm <base>/<target>.leaf (); name empty
      // uninstall <target> <name>   # rm <base>/<name>; target can be NULL
      //
      // Return false if nothing has been removed (i.e., the file does not
      // exist).
      //
      static bool
      uninstall_f (const scope& rs,
                   const install_dir& base,
                   const file* target,
                   const path& name,
                   uint16_t verbosity = 1);

      // Uninstall (remove) a symlink.
      //
      // This is essentially unistall_f() but with better low-verbosity
      // diagnostics.
      //
      static bool
      uninstall_l (const scope& rs,
                   const install_dir& base,
                   const path& link,
                   const path& link_target,
                   uint16_t verbosity = 1);


      // Uninstall (remove) an empty directory.
      //
      // uninstall -d <dir>
      //
      // We try to remove all the directories between base and dir but not
      // base itself unless base == dir. Return false if nothing has been
      // removed (i.e., the directories do not exist or are not empty).
      //
      static bool
      uninstall_d (const scope& rs,
                   const install_dir& base,
                   const dir_path& dir,
                   uint16_t verbosity = 1);

      target_state
      perform_install (action, const target&) const;

      target_state
      perform_uninstall (action, const target&) const;

      static const file_rule instance;
      file_rule () {}
    };
  }
}

#endif // LIBBUILD2_INSTALL_RULE_HXX
