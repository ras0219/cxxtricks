#pragma once

#include "mt.hpp"

namespace rasmeta {

  ////////////////////////////////////
  // Helpers for lambdas
  template<class Argument, class DIVIDER, class Required, class T>
  struct assert_type {
    static_assert(same_mt<Argument, Required>::value, "Type failure");

    using type = T;
  };
  template<class A, class R, class T>
  using assert_type_t = assert_type<A, struct XXXXXXXX, R, T>;

  //////////////////////////////////
  // Lambdas
  template<template<class>class Impl, class Arg1T, class RetT>
  struct lambda1 {
    template<class A>
    struct apply {
      //static_assert(is_mt<A>::value, "A must have a metatype");
      // check type & dispatch
      using type = typename assert_type_t<Metatype<A>, Arg1T, Impl<A>>::type;
    };
    using metatype = mt_arr<Arg1T, RetT>;
  };

  template<template<class, class>class Impl, class Arg1T, class Arg2T, class RetT>
  struct lambda2 {
    template<class A>
    struct apply {
      //static_assert(is_mt<A>::value, "A must have a metatype");
      struct _type {
        template<class B>
        struct apply {
          //static_assert(is_mt<B>::value, "B must have a metatype");
          // check type & dispatch
          using type = typename assert_type_t<Metatype<B>, Arg2T, Impl<A, B>>::type;
        };
        using metatype = mt_arr<Arg2T, RetT>;
      };
      // check type
      using type = assert_type_t<Metatype<A>, Arg1T, _type>;
    };
    using metatype = mt_arr<Arg1T, mt_arr<Arg2T, RetT>>;
  };

  template<template<class, class, class>class Impl,
  class Arg1T, class Arg2T, class Arg3T, class RetT>
    // Helper lambda to autoconstruct early-type detection functions (3)
  struct lambda3 {
    template<class A>
    struct apply {
      //static_assert(is_mt<A>::value, "A must have a metatype");
      struct _type {
        template<class B>
        struct apply {
          //static_assert(is_mt<B>::value, "B must have a metatype");
          struct _type {
            template<class C>
            struct apply {
              //static_assert(is_mt<C>::value, "C must have a metatype");
              // check type & dispatch
              using type = typename assert_type_t<Metatype<C>, Arg3T, Impl<A, B, C>>::type;
            };
            using metatype = mt_arr<Arg3T, RetT>;
          };
          // check type
          using type = assert_type_t<Metatype<B>, Arg2T, _type>;
        };
        using metatype = mt_arr<Arg2T, mt_arr<Arg3T, RetT>>;
      };
      // check type
      using type = assert_type_t<Metatype<A>, Arg1T, _type>;
    };
    using metatype = mt_arr<Arg1T, mt_arr<Arg2T, mt_arr<Arg3T, RetT>>>;
  };

}