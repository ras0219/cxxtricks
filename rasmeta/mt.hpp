#pragma once

#include <type_traits>
#include "false_depend.hpp"

#if (defined(__GNUG__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 8))) \
  || (defined(__clang__))
namespace std {
  // template<bool B, class T = void>
  // struct enable_if {};

  // template<class T>
  // struct enable_if<true, T> { typedef T type; };

  template<bool B, class T = void>
  using enable_if_t = typename enable_if<B, T>::type;
}
#endif

// So it goes like this:
//  * Metatypes are structs with the prefix mt_
//  * Metatypes of kind * are simple structs
//  * Metatypes of kind * -> * are template structs that are bound to the struct mt_meta

namespace rasmeta {
  // Base Metatype. Used for metatype detection.
  struct mt_base {};

  template<class T>
  using is_mt = std::is_base_of<mt_base, T>;

  // Wildcard Metatype. Used (for now) to make * -> * Metakinds concrete
  template<class...Uniquifiers>
  struct mt_any : mt_base {};

  using mt_any_ = mt_any<>;

  // Arrow Metatype. Used to mark functions.
  template<class mt_L, class mt_R>
  struct mt_arr : mt_base {};

  // Convenience function for retrieving function parts
  template<class T> struct _mt_arr_t_fetch_impl;
  template<class L, class R> struct _mt_arr_t_fetch_impl<mt_arr<L, R>> {
    using left = L;
    using right = R;
  };

  template<class ArrMT>
  using arg_mt = typename _mt_arr_t_fetch_impl<ArrMT>::left;
  template<class ArrMT>
  using ret_mt = typename _mt_arr_t_fetch_impl<ArrMT>::right;

  //// This might be awful. Or awesome. Probably awfully awesome...
  // Design decisions: both > and >> are left associative.
  //    -> requires an identifier on the right hand side
  // But >>= is actually right associative!
  // Thus the decltype(mt_any<>() >>= mt_num() >>= mt_list<mt_any<>>()) was born...
  template<class mt_L, class mt_R, class = std::enable_if_t<is_mt<mt_L>::value && is_mt<mt_R>::value>>
  mt_arr<mt_L, mt_R> operator >>=(mt_L, mt_R) { return mt_arr<mt_L, mt_R>(); }

  // Tuple Metatype. Use for multiple parameters
  template<class...mt_T>
  struct mt_tuple : mt_base {};

  //// More awfully awesomely terrible overloads!
  template<class mt_L, class mt_R, class = std::enable_if_t<is_mt<mt_L>::value && is_mt<mt_R>::value>>
  mt_tuple<mt_L, mt_R> operator ,(mt_L, mt_R) { return mt_tuple<mt_L, mt_R>(); }
  template<class...mt_L, class mt_R, class = std::enable_if_t<is_mt<mt_R>::value>>
  mt_tuple<mt_L..., mt_R> operator ,(mt_tuple<mt_L...>, mt_R) { return mt_tuple<mt_L..., mt_R>(); }

  ////////////////////////////////////
  // same_mt calculates if two metatypes are unifiable
  template <class L, class R>
  struct same_mt {
    enum { value = false };
  };
  template <class L>
  struct same_mt<L, mt_any_> {
    enum { value = true };
  };
  template <class R>
  struct same_mt<mt_any_, R> {
    enum { value = true };
  };
  template <class L>
  struct same_mt<L, L> {
    enum { value = true };
  };
  template <template<class>class C, class L, class R>
  struct same_mt<C<L>, C<R>> {
    enum { value = same_mt<L, R>::value };
  };
  template <template<class>class C, class L>
  struct same_mt<C<L>, C<L>> {
    enum { value = true };
  };
  template <template<class, class>class C, class L, class L2, class R, class R2>
  struct same_mt<C<L, L2>, C<R, R2>> {
    enum { value = same_mt<L, R>::value && same_mt<L2, R2>::value };
  };
  template <template<class, class>class C, class L, class L2>
  struct same_mt<C<L, L2>, C<L, L2>> {
    enum { value = true };
  };
  /////////////////////////////////
  // Perform unification on types
  template <class L, class R>
  struct unify_mt {
    static_assert(!same_mt<L, R>::value, "INTERNAL LIBRARY ERROR -- unify_mt<> and same_mt<> differ!");
    static_assert(____false_depend<R>::value, "Unification failed");
  };
  template<class L>
  struct unify_mt<L, mt_any_> {
    using type = L;
  };
  template<class R>
  struct unify_mt<mt_any_, R> {
    using type = R;
  };
  template<class L>
  struct unify_mt<L, L> {
    using type = L;
  };
  template<template<class>class C, class L, class R>
  struct unify_mt<C<L>, C<R>> {
    using type = C<typename unify_mt<L, R>::type>;
  };
  template<template<class>class C, class L>
  struct unify_mt<C<L>, C<L>> {
    using type = C<L>;
  };
  template<template<class, class>class C, class L, class L2, class R, class R2>
  struct unify_mt<C<L, L2>, C<R, R2>> {
    using type = C<typename unify_mt<L, R>::type, typename unify_mt<L2, R2>::type>;
  };
  template<template<class, class>class C, class L, class R>
  struct unify_mt<C<L, R>, C<L, R>> {
    using type = C<L, R>;
  };
  template<class L, class R>
  using unify_mt_t = typename unify_mt<L, R>::type;

  /////////////////////////////////
  // Function to extract the metatype
  template<class T>
  using Metatype = typename T::metatype;


  //////////////////////////////////
  // Function to detect the metatype
  template<class F>
  struct has_metatype {
    using yes = char[1];
    using no = char[2];

    template<class T>
    static yes& test(Metatype<T>*);
    template<class T>
    static no& test(...);

    static const bool value = sizeof(test<F>(0)) == sizeof(yes);
  };

  ///////////////////////////////
  // Convenience alias for applications
  template<class...B>
  struct _Apply_impl;
  
  template<class F>
  struct _Apply_impl<F> {
    using type = F;
  };

  template<class L, class R>
  struct static_assert_helper {
    static_assert(same_mt<L, R>::value, "Metatype mismatch");
  };

  template<class F, class A, class...B>
  struct _Apply_impl<F, A, B...> {
    using MTF = Metatype<F>;
    using MTA = mt_arr<Metatype<A>, mt_any_>;
    static static_assert_helper<Metatype<F>, mt_arr<Metatype<A>, mt_any_>> f;
    using F2 = typename F::template apply<A>::type;
    using type = typename _Apply_impl<F2, B...>::type;
  };

  template<class...F>
  using apply_t = typename _Apply_impl<F...>::type;

  template<class...Args>
  apply_t<Args...> apply(Args...) { return apply_t<Args...>(); }
}
