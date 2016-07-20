// file      : build2/variable.txx -*- C++ -*-
// copyright : Copyright (c) 2014-2016 Code Synthesis Ltd
// license   : MIT; see accompanying LICENSE file

#include <build2/diagnostics>

namespace build2
{
  // value
  //
  template <typename T>
  void
  default_dtor (value& v)
  {
    v.as<T> ().~T ();
  }

  template <typename T>
  void
  default_copy_ctor (value& l, const value& r, bool m)
  {
    if (m)
      new (&l.data_) T (move (const_cast<value&> (r).as<T> ()));
    else
      new (&l.data_) T (r.as<T> ());
  }

  template <typename T>
  void
  default_copy_assign (value& l, const value& r, bool m)
  {
    if (m)
      l.as<T> () = move (const_cast<value&> (r).as<T> ());
    else
      l.as<T> () = r.as<T> ();
  }

  template <typename T>
  bool
  default_empty (const value& v)
  {
    return value_traits<T>::empty (v.as<T> ());
  }

  template <typename T, bool empty>
  void
  simple_assign (value& v, names&& ns, const variable* var)
  {
    size_t n (ns.size ());

    if (empty ? n <= 1 : n == 1)
    {
      try
      {
        value_traits<T>::assign (
          v,
          (n == 0
           ? T ()
           : value_traits<T>::convert (move (ns.front ()), nullptr)));

        return;
      }
      catch (const invalid_argument&) {} // Fall through.
    }

    {
      diag_record dr (error);

      dr << "invalid " << value_traits<T>::value_type.name
         << " value '" << ns << "'";

      if (var != nullptr)
        dr << " in variable " << var->name;
    }

    throw failed ();
  }

  template <typename T, bool empty>
  void
  simple_append (value& v, names&& ns, const variable* var)
  {
    size_t n (ns.size ());

    if (empty ? n <= 1 : n == 1)
    {
      try
      {
        value_traits<T>::append (
          v,
          (n == 0
           ? T ()
           : value_traits<T>::convert (move (ns.front ()), nullptr)));

        return;
      }
      catch (const invalid_argument&) {} // Fall through.
    }

    {
      diag_record dr (error);

      dr << "invalid " << value_traits<T>::value_type.name
         << " value '" << ns << "'";

      if (var != nullptr)
        dr << " in variable " << var->name;
    }

    throw failed ();
  }

  template <typename T, bool empty>
  void
  simple_prepend (value& v, names&& ns, const variable* var)
  {
    size_t n (ns.size ());

    if (empty ? n <= 1 : n == 1)
    {
      try
      {
        value_traits<T>::prepend (
          v,
          (n == 0
           ? T ()
           : value_traits<T>::convert (move (ns.front ()), nullptr)));

        return;
      }
      catch (const invalid_argument&) {} // Fall through.
    }

    {
      diag_record dr (error);

      dr << "invalid " << value_traits<T>::value_type.name
         << " value '" << ns << "'";

      if (var != nullptr)
        dr << " in variable " << var->name;
    }

    throw failed ();
  }

  template <typename T>
  names_view
  simple_reverse (const value& v, names& s)
  {
    s.emplace_back (value_traits<T>::reverse (v.as<T> ()));
    return s;
  }

  template <typename T>
  int
  simple_compare (const value& l, const value& r)
  {
    return value_traits<T>::compare (l.as<T> (), r.as<T> ());
  }

  // vector<T> value
  //

  template <typename T>
  void
  vector_append (value& v, names&& ns, const variable* var)
  {
    vector<T>& p (v
                  ? v.as<vector<T>> ()
                  : *new (&v.data_) vector<T> ());

    // Convert each element to T while merging pairs.
    //
    for (auto i (ns.begin ()); i != ns.end (); ++i)
    {
      name& n (*i);
      name* r (nullptr);

      if (n.pair)
      {
        r = &*++i;

        if (n.pair != '@')
        {
          diag_record dr (fail);

          dr << "unexpected pair style for "
             << value_traits<T>::value_type.name << " value "
             << "'" << n << "'" << n.pair << "'" << *r << "'";

          if (var != nullptr)
            dr << " in variable " << var->name;
        }
      }

      try
      {
        p.push_back (value_traits<T>::convert (move (n), r));
      }
      catch (const invalid_argument&)
      {
        diag_record dr (fail);

        dr << "invalid " << value_traits<T>::value_type.name;

        if (n.pair)
          dr << " element pair '" << n << "'@'" << *r << "'";
        else
          dr << " element '" << n << "'";

        if (var != nullptr)
          dr << " in variable " << var->name;
      }
    }
  }

  template <typename T>
  void
  vector_assign (value& v, names&& ns, const variable* var)
  {
    if (v)
      v.as<vector<T>> ().clear ();

    vector_append<T> (v, move (ns), var);
  }

  template <typename T>
  void
  vector_prepend (value& v, names&& ns, const variable* var)
  {
    // Reduce to append.
    //
    vector<T> t;
    vector<T>* p;

    if (v)
    {
      p = &v.as<vector<T>> ();
      p->swap (t);
    }
    else
      p = new (&v.data_) vector<T> ();

    vector_append<T> (v, move (ns), var);

    p->insert (p->end (),
               make_move_iterator (t.begin ()),
               make_move_iterator (t.end ()));
  }

  template <typename T>
  static names_view
  vector_reverse (const value& v, names& s)
  {
    auto& vv (v.as<vector<T>> ());
    s.reserve (vv.size ());

    for (const T& x: vv)
      s.push_back (value_traits<T>::reverse (x));

    return s;
  }

  template <typename T>
  static int
  vector_compare (const value& l, const value& r)
  {
    auto& lv (l.as<vector<T>> ());
    auto& rv (r.as<vector<T>> ());

    auto li (lv.begin ()), le (lv.end ());
    auto ri (rv.begin ()), re (rv.end ());

    for (; li != le && ri != re; ++li, ++ri)
      if (int r = value_traits<T>::compare (*li, *ri))
        return r;

    if (li == le && ri != re) // l shorter than r.
      return -1;

    if (ri == re && li != le) // r shorter than l.
      return 1;

    return 0;
  }

  template <typename T>
  const string value_traits<vector<T>>::type_name = string (
    value_traits<T>::type_name) + 's';

  template <typename T>
  const value_type value_traits<vector<T>>::value_type = // VC 19 wants =.
  {
    value_traits<vector<T>>::type_name.c_str (),
    sizeof (vector<T>),
    nullptr,                          // No base.
    &default_dtor<vector<T>>,
    &default_copy_ctor<vector<T>>,
    &default_copy_assign<vector<T>>,
    &vector_assign<T>,
    &vector_append<T>,
    &vector_prepend<T>,
    &vector_reverse<T>,
    nullptr,                          // No cast (cast data_ directly).
    &vector_compare<T>,
    &default_empty<vector<T>>
  };

  // map<K, V> value
  //
  template <typename K, typename V>
  void
  map_append (value& v, names&& ns, const variable* var)
  {
    using std::map;

    map<K, V>& p (v
                  ? v.as<map<K, V>> ()
                  : *new (&v.data_) map<K, V> ());

    // Verify we have a sequence of pairs and convert each lhs/rhs to K/V.
    //
    for (auto i (ns.begin ()); i != ns.end (); ++i)
    {
      name& l (*i);

      if (!l.pair)
      {
        diag_record dr (fail);

        dr << value_traits<map<K, V>>::value_type.name << " key-value "
           << "pair expected instead of '" << l << "'";

        if (var != nullptr)
          dr << " in variable " << var->name;
      }

      name& r (*++i); // Got to have the second half of the pair.

      if (l.pair != '@')
      {
        diag_record dr (fail);

        dr << "unexpected pair style for "
           << value_traits<map<K, V>>::value_type.name << " key-value "
           << "'" << l << "'" << l.pair << "'" << r << "'";

        if (var != nullptr)
          dr << " in variable " << var->name;
      }

      try
      {
        K k (value_traits<K>::convert (move (l), nullptr));

        try
        {
          V v (value_traits<V>::convert (move (r), nullptr));

          p.emplace (move (k), move (v));
        }
        catch (const invalid_argument&)
        {
          diag_record dr (fail);

          dr << "invalid " << value_traits<V>::value_type.name
             << " element value '" << r << "'";

          if (var != nullptr)
            dr << " in variable " << var->name;
        }
      }
      catch (const invalid_argument&)
      {
        diag_record dr (fail);

        dr << "invalid " << value_traits<K>::value_type.name
           << " element key '" << l << "'";

        if (var != nullptr)
          dr << " in variable " << var->name;
      }
    }
  }

  template <typename K, typename V>
  void
  map_assign (value& v, names&& ns, const variable* var)
  {
    using std::map;

    if (v)
      v.as<map<K, V>> ().clear ();

    map_append<K, V> (v, move (ns), var);
  }

  template <typename K, typename V>
  static names_view
  map_reverse (const value& v, names& s)
  {
    using std::map;

    auto& vm (v.as<map<K, V>> ());
    s.reserve (2 * vm.size ());

    for (const auto& p: vm)
    {
      s.push_back (value_traits<K>::reverse (p.first));
      s.back ().pair = '@';
      s.push_back (value_traits<V>::reverse (p.second));
    }

    return s;
  }

  template <typename K, typename V>
  static int
  map_compare (const value& l, const value& r)
  {
    using std::map;

    auto& lm (l.as<map<K, V>> ());
    auto& rm (r.as<map<K, V>> ());

    auto li (lm.begin ()), le (lm.end ());
    auto ri (rm.begin ()), re (rm.end ());

    for (; li != le && ri != re; ++li, ++ri)
    {
      int r;
      if ((r = value_traits<K>::compare (li->first, ri->first)) != 0 ||
          (r = value_traits<V>::compare (li->second, ri->second)) != 0)
        return r;
    }

    if (li == le && ri != re) // l shorter than r.
      return -1;

    if (ri == re && li != le) // r shorter than l.
      return 1;

    return 0;
  }

  template <typename K, typename V>
  const string value_traits<std::map<K, V>>::type_name = string (
    value_traits<K>::type_name) + '_' +
    value_traits<V>::type_name + "_map";

  template <typename K, typename V>
  const value_type value_traits<std::map<K, V>>::value_type = // VC 19 wants =.
  {
    value_traits<map<K, V>>::type_name.c_str (),
    sizeof (map<K, V>),
    nullptr,             // No base.
    &default_dtor<map<K, V>>,
    &default_copy_ctor<map<K, V>>,
    &default_copy_assign<map<K, V>>,
    &map_assign<K, V>,
    &map_append<K, V>,
    &map_append<K, V>,   // Prepend is the same as append.
    &map_reverse<K, V>,
    nullptr,             // No cast (cast data_ directly).
    &map_compare<K, V>,
    &default_empty<map<K, V>>
  };
}
