#pragma once

#include "mt.hpp"

namespace rasmeta {
  template<class F, class A, class B>
  // swap :: (a -> b -> c) -> b -> a -> c
  struct _swap_impl {
    using type = decltype(apply(F(), B(), A()));
  };

  // swap :: (a -> b -> c) -> b -> a -> c
  static const struct swap_t {
    using FMT = decltype(mt_any_() >>= mt_any_() >>= mt_any_());
    using metatype = decltype(FMT() >>= mt_any_() >>= mt_any_() >>= mt_any_());

    template<class F>
    struct apply {
      static_assert(has_metatype<A>::value, "Argument F must have a metatype");
      static_assert(same_mt<Metatype<A>, FMT>::value, "Argument F must unify with type 'a -> b -> c'");
      struct type {
        template<class L>
        using apply = _cons_impl<A, L>;
        using metatype = decltype(mt_list<Metatype<A>>() >>= mt_list<Metatype<A>>());
      };
    };
  } swap;
}
