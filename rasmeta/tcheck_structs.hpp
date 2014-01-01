#pragma once

#include <utility>
#include <type_traits>

namespace rasmeta {

  namespace tcheck {

    using std::pair;

    template<class...T> struct variadic {};

    template<template<class,class>class F, class I, class C>
    struct variadic_foldl_impl;

    template<template<class, class>class F, class I, class C>
    using variadic_foldl = typename variadic_foldl_impl<F, I, C>::type;

    template<template<class, class>class F, class I>
    struct variadic_foldl_impl<F, I, variadic<>> {
      using type = I;
    };

    template<template<class, class>class F, class I, class Arg, class...Args>
    struct variadic_foldl_impl<F, I, variadic<Arg, Args...>> {
      using type = variadic_foldl<F, F<I, Arg>, variadic<Args...>>;
    };


    ////////////////////////////////////////
    // Lookup
    struct not_found;

    template<class Key, class I, class Pairing> struct _lookup_impl2;

    template<class Key, class I, class Value>
    // _lookup_impl2 k i (k2, v) | k == k2 = i
    struct _lookup_impl2<Key, I, pair<Key, Value>> { using type = I; };

    template<class Key, class I, class Key2, class Value>
    //                           | otherwise = i
    struct _lookup_impl2<Key, I, pair<Key2, Value>> { using type = I; };

    template<class Key, class Value>
    // _lookup_impl2 k Nothing (k2, v) | k == k2 = Just v
    struct _lookup_impl2<Key, not_found, pair<Key, Value>> { using type = Value; };

    template<class Key>
    struct _lookup_impl {
      template<class I, class P>
      using type = typename _lookup_impl2<Key, I, P>::type;
    };

    template<class M, class Key>
    using lookup = variadic_foldl<_lookup_impl<Key>::template type, not_found, M>;

    ////////////////////////////////////////
    // Fix_lookup
    template<class M, class Key, class Result>
    struct fix_lookup_impl {
      using type = typename fix_lookup_impl<M, Result, lookup<M, Result>>::type;
    };
    template<class M, class Key>
    struct fix_lookup_impl<M, Key, not_found> {
      using type = Key;
    };

    template<class M, class Key>
    using fix_lookup = typename fix_lookup_impl<M, Key, lookup<M, Key>>::type;

    ////////////////////////////////////////
    // Insert
    template<class M, class Key, class Value>
    struct _insert_impl;

    template<class Key, class Value, class...Args>
    struct _insert_impl<variadic<Args...>, Key, Value> {
      using type = variadic<pair<Key, Value>, Args...>;
    };

    template<class M, class Key, class Value>
    using insert = typename _insert_impl<M, Key, Value>::type;

    /////////////////////////////////////////
    // Remove
    template<class Key, class M2, class P>
    struct _erase_impl2;

    template<class Key>
    struct _erase_impl {
      template<class M2, class P>
      using type = typename _erase_impl2<Key, M2, P>::type;
    };

    template<class Key, class P, class...Args>
    struct _erase_impl2<Key, variadic<Args...>, P> {
      using type = variadic<Args..., P>;
    };

    template<class Key, class Value, class...Args>
    struct _erase_impl2<Key, variadic<Args...>, pair<Key, Value>> {
      using type = variadic<Args...>;
    };

    template<class M, class Key>
    using erase = variadic_foldl<_erase_impl<Key>::template type, variadic<>, M>;

    /////////////////////////////////////////////
    // Union
    template<class V1, class V2>
    struct union_impl {};
    template<class...Args1, class...Args2>
    struct union_impl<variadic<Args1...>, variadic<Args2...>> {
      using type = variadic<Args1..., Args2...>;
    };
    template<class V1, class V2>
    using union_ = typename union_impl<V1, V2>::type;

    /////////////////////////////////////////////
    // At
    template<class C, long long I> struct at_impl;
    template<class Arg, class...Args, long long I>
    struct at_impl<variadic<Arg, Args...>, I> {
      using type = typename at_impl<variadic<Args...>, I - 1>::type;
    };
    template<class Arg, class...Args>
    struct at_impl<variadic<Arg, Args...>, 0> {
      using type = Arg;
    };

    template<class C, long long I>
    using at = typename at_impl<C, I>::type;

    /////////////////////////////////////////////
    // Push
    template<class C, class N> struct push_impl;
    template<class...Args, class N>
    struct push_impl<variadic<Args...>, N> {
      using type = variadic<N, Args...>;
    };
    template<class C, class N>
    using push = typename push_impl<C, N>::type;

    /////////////////////////////////////////////
    // Wrap
    template<template<class>class T, class U, long long N>
    struct wrap_impl {
      using type = typename wrap_impl<T, T<U>, N-1>::type;
    };
    template<template<class>class T, class U>
    struct wrap_impl<T, U, 0> {
      using type = U;
    };
    template<template<class>class T, class U, long long N>
    using wrap = typename wrap_impl<T, U, N>::type;
  }

}