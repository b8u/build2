// file      : build/variable.cxx -*- C++ -*-
// copyright : Copyright (c) 2014-2015 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#include <build/variable>

#include <iterator> // make_move_iterator()

#include <build/utility>
#include <build/diagnostics>

using namespace std;

namespace build
{
  // value
  //
  void
  assign (value& v, const value_type* t, const variable& var)
  {
    if (v.type == nullptr)
    {
      v.type = t;

      if (v && t->assign != nullptr)
        v.state_ = t->assign (v.data_, var)
          ? value::state_type::filled
          : value::state_type::empty;
    }
    else
      fail << "variable '" << var.name << "' type mismatch" <<
        info << "value '" << v.data_ << "' is " << v.type->name <<
        info << (t == var.type ? "variable" : "new type") << " is "
           << (var.type != nullptr ? var.type->name : "untyped");
  }

  value& value::
  operator= (value&& v)
  {
    assert (type == nullptr || type == v.type);

    // Since the types are the same, we don't need to call
    // the callbacks.
    //
    type = v.type;
    state_ = v.state_;
    data_ = move (v.data_);

    return *this;
  }

  value& value::
  append (value v, const variable& var)
  {
    assert (type == v.type);
    append (move (v.data_), var);
    return *this;
  }

  void value::
  append (names v, const variable& var)
  {
    // Treat append to NULL as assign.
    //
    if (!null () && type != nullptr && type->append != nullptr)
    {
      state_ = type->append (data_, move (v), var)
        ? state_type::filled
        : state_type::empty;
      return;
    }

    if (data_.empty ())
      data_ = move (v);
    else
      data_.insert (data_.end (),
                    make_move_iterator (v.begin ()),
                    make_move_iterator (v.end ()));

    state_ = (type != nullptr && type->assign != nullptr
              ? type->assign (data_, var)
              : !data_.empty ())
      ? state_type::filled
      : state_type::empty;
  }

  // bool value
  //
  bool value_traits<bool>::
  assign (name& n)
  {
    if (n.simple ())
    {
      const string& s (n.value);

      if (s == "true" || s == "false")
        return true;
    }

    return false;
  }

  static bool
  bool_assign (names& v, const variable& var)
  {
    // Verify the value is either "true" or "false".
    //
    if (v.size () == 1)
    {
      name& n (v.front ());

      if (value_traits<bool>::assign (n))
        return true;
    }

    fail << "invalid bool variable '" << var.name << "' value '" << v << "'";
    return false;
  }

  static bool
  bool_append (names& v, names a, const variable& var)
  {
    // Translate append to OR.
    //
    bool_assign (a, var); // Verify "true" or "false".

    if (a.front ().value[0] == 't' && v.front ().value[0] == 'f')
      v = move (a);

    return true;
  }

  const value_type value_traits<bool>::value_type
  {
    "bool",
    &bool_assign,
    &bool_append
  };

  const value_type* bool_type = &value_traits<bool>::value_type;

  // string value
  //
  bool value_traits<string>::
  assign (name& n)
  {
    // Convert directory to string.
    //
    if (n.directory ())
    {
      n.value = std::move (n.dir).string (); // Move string out of path.

      // Add / back to the end of the path unless it is already there.
      // Note that the string cannot be empty (n.directory () would
      // have been false).
      //
      if (!dir_path::traits::is_separator (n.value[n.value.size () - 1]))
        n.value += '/';
    }

    return n.simple ();
  }

  static bool
  string_assign (names& v, const variable& var)
  {
    // Verify/convert the value is/to a single simple name.
    //
    if (v.empty ())
    {
      v.emplace_back (name ()); // Canonical empty string representation.
      return false;
    }
    else if (v.size () == 1)
    {
      name& n (v.front ());

      if (value_traits<string>::assign (n))
        return !n.value.empty ();
    }

    fail << "invalid string variable '" << var.name << "' value '" << v << "'";
    return false;
  }

  static bool
  string_append (names& v, names a, const variable& var)
  {
    // Translate append to string concatenation.
    //
    string_assign (a, var); // Verify/convert value is/to string.

    if (v.front ().value.empty ())
      v = move (a);
    else
      v.front ().value += a.front ().value;

    return !v.front ().value.empty ();
  }

  const value_type value_traits<string>::value_type
  {
    "string",
    &string_assign,
    &string_append
  };

  const value_type* string_type = &value_traits<string>::value_type;

  // dir_path value
  //
  bool value_traits<dir_path>::
  assign (name& n)
  {
    if (n.directory ())
      return true;

    if (n.simple ())
    {
      try
      {
        n.dir = n.empty () ? dir_path () : dir_path (move (n.value));
        n.value.clear ();
        return true;
      }
      catch (const invalid_path&) {} // Fall through.
    }

    return false;
  }

  static bool
  dir_path_assign (names& v, const variable& var)
  {
    // Verify/convert the value is/to a single directory name.
    //
    if (v.empty ())
    {
      v.emplace_back (dir_path ()); // Canonical empty path representation.
      return false;
    }
    else if (v.size () == 1)
    {
      name& n (v.front ());

      if (value_traits<dir_path>::assign (n))
        return !n.dir.empty ();
    }

    fail << "invalid dir_path variable '" << var.name << "' "
         << "value '" << v << "'";
    return false;
  }

  static bool
  dir_path_append (names& v, names a, const variable& var)
  {
    // Translate append to path concatenation.
    //
    dir_path_assign (a, var); // Verify/convert value is/to dir_path.

    dir_path& d (a.front ().dir);
    if (d.relative ())
      return !(v.front ().dir /= d).empty ();
    else
      fail << "append of absolute path '" << d << "' to dir_path variable "
           << var.name;

    return false;
  }

  const value_type value_traits<dir_path>::value_type
  {
    "dir_path",
    &dir_path_assign,
    &dir_path_append
  };

  const value_type* dir_path_type = &value_traits<dir_path>::value_type;

  // name value
  //
  static bool
  name_assign (names& v, const variable& var)
  {
    // Verify the value is a single name.
    //
    if (v.size () == 1)
      return v.front ().empty ();

    fail << "invalid string variable '" << var.name << "' value '" << v << "'";
    return false;
  }

  static bool
  name_append (names&, names, const variable& var)
  {
    fail << "append to name variable '" << var.name << "'";
    return false;
  }

  const value_type value_traits<name>::value_type
  {
    "name",
    &name_assign,
    &name_append
  };

  const value_type* name_type = &value_traits<name>::value_type;

  // vector<T> value
  //
  const value_type* strings_type = &value_traits<strings>::value_type;
  const value_type* dir_paths_type = &value_traits<dir_paths>::value_type;
  const value_type* names_type = &value_traits<names>::value_type;

  // variable_set
  //
  variable_set variable_pool;
}
