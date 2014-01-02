#pragma once

namespace rasmeta {

  ///////////////////////////////////
  // The language

  // \<anon>. e
  template<class e> struct lambda {};
  // A de bruijn reference to an enclosing lambda
  template<long long N> struct ref {}; using _0 = ref<0>; using _1 = ref<1>; using _2 = ref<2>;
  // e1(e2)
  template<class e1, class e2> struct app {};

  ///////////////////////////////////
  // Core Algorithms over ExprF
  ///// map_expr
  template<template<class>class F, class T>
  struct map_expr_impl {
    using type = F<T>;
  };
  template<template<class>class F, class T1, class T2>
  struct map_expr_impl<F, app<T1, T2>> {
    using t1 = F<T1>;
    using t2 = F<T2>;
    using type = F<app<t1, t2>>;
  };
  template<template<class>class F, class T>
  struct map_expr_impl<F, lambda<T>> {
    using t = F<T>;
    using type = F<lambda<t>>;
  };
  template<template<class>class F, class T>
  // map_expr :: (ExprF a -> a) -> Expr -> a
  using map_expr = typename map_expr_impl<F, T>::type;

  ///////////////////////////////////
  // Type Lookup Helpers
  struct functional {};
  struct functional_metaval {};
  struct functional_metafunc {};
  struct functional_ast {};

  template<class e>
  struct metatype {
    static_assert(std::is_base_of<functional, e>::value, "expression 'e' does not specialize metatype<e> or inherit functional");
    using type = typename e::type;
  };
  template<class e>
  struct metafunc {
    static_assert(std::is_base_of<functional_metafunc, e>::value, "expression 'e' does not specialize metafunc<e> or inherit functional_metafunc");
    using type = typename e::func;
  };
  template<class e>
  struct metaval {
    static_assert(std::is_base_of<functional_metaval, e>::value, "expression 'e' does not specialize metaval<e> or inherit functional_metaval");
    using type = typename e::value_type;
    static const type value = e::value;
  };

  ///////////////////////////////////
  // Builtins
  template<class T, T N> struct constant_c : functional, functional_metaval {
    using value_type = T;
    static const T value = N;

    using type = concrete_type<T>;
  };

  template<long long N>
  using int_c = constant_c<long long, N>;
  template<bool N>
  using bool_c = constant_c<bool, N>;

  template<class T> struct add_c : functional, functional_metafunc, functional_metaval {
    struct value_type {
      T operator()(T t1, T t2) const noexcept {
        return t1 + t2;
      }
    };
    static const value_type value;

    using type = arr<concrete_type<T>, arr<concrete_type<T>, concrete_type<T>>>;

    struct func {
      template<class T1>
      struct apply : functional, functional_metafunc, functional_metaval {
        using type = arr<concrete_type<T>, concrete_type<T>>;

        struct func {
          template<class T2>
          using apply = constant_c<T, T1::value + T2::value>;
        };

        struct value_type {
          T operator()(T t2) const noexcept {
            return T1::value + t2;
          }
        };
        static const value_type value;
      };
    };
  };

}
