#pragma once

namespace rasmeta {

  ///////////////////////////////////
  // The typesystem

  // type variable alpha, where alpha is not in freevars(G)
  template<long long N> struct tvar {};
  // t1 -> t2
  template<class t1, class t2> struct arr {};
  // some concrete type defined by Uniques
  template<class...Uniques> struct concrete_type {};
  // forall <anonymous>. t
  template<class t> struct forall {};
  // A de bruijn reference to an enclosing forall
  template<long long N> struct tref {};

  ///////////////////////////////////
  // Core algs over the typesystem

  ///// map_type
  template<template<class>class F, class T>
  struct map_type_impl {
    using type = F<T>;
  };
  template<template<class>class F, class T1, class T2>
  struct map_type_impl<F, arr<T1, T2>> {
    using t1 = F<T1>;
    using t2 = F<T2>;
    using type = F<arr<t1, t2>>;
  };
  template<template<class>class F, class T>
  struct map_type_impl<F, forall<T>> {
    using t = F<T>;
    using type = F<forall<t>>;
  };
  template<template<class>class F, class T>
  // map_type :: (TypeF a -> a) -> Type -> a
  using map_type = typename map_type_impl<F, T>::type;

  ///// fold_type
  template<template<class, class>class F, class T, class I>
  struct fold_type_impl {
    using aux = F<T, I>;
    using first = typename aux::first;
    using second = typename aux::second;
  };
  template<template<class, class>class F, class T1, class T2, class I>
  struct fold_type_impl<F, arr<T1, T2>, I> {
    using t1 = F<T1, I>;
    using t2 = F<T2, typename t1::second>;
    using aux = F<arr<typename t1::first, typename t2::first>, typename t2::second>;

    using first = typename aux::first;
    using second = typename aux::second;
  };
  template<template<class, class>class F, class T, class I>
  struct fold_type_impl<F, forall<T>, I> {
    using t1 = F<T, I>;
    using aux = F<forall<typename t1::first>, typename t1::second>;

    using first = typename aux::first;
    using second = typename aux::second;
  };
  template<template<class,class>class F, class T, class I>
  // fold_type :: (TypeF a -> b -> (a, b)) -> Type -> b -> (a, b)
  using fold_type = typename fold_type_impl<F, T, I>::type;

}