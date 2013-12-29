#pragma once

#include <type_traits>
#include "mt.hpp"
#include "false_depend.hpp"
#include "mt_util_uncurry.hpp"
#include "mt_swap.hpp"

namespace rasmeta {

  template<class mt_E>
  struct mt_list : mt_base {};

  ////////////////////////////////
  template<class mt_E>
  struct empty_list {
    using metatype = mt_list<mt_E>;
  };

  template<class E, class L>
  struct list {
    using metatypeE = mt_list<Metatype<E>>;
    using metatypeL = Metatype<L>;
    template<class metatypE, class metatypL>
    struct TYPECHECK {
      static_assert(same_mt<metatypE, metatypL>::value, "World 2: Cannot add an E into a list of type [A]");
    };
    static TYPECHECK<metatypeE, metatypeL> __typecheck;
    using metatype = typename unify_mt<metatypeE, metatypeL>::type;
  };
  ///////////////////////////////////

  template<class T>
  struct _head_impl {
    static_assert(____false_depend<T>::value, "World 2: cannot unify T with [a]");
  };

  template<class mt_E>
  // head :: [] -> *error*
  struct _head_impl<empty_list<mt_E>> {
    static_assert(____false_depend<mt_E>::value, "World 1: attempted to take head of empty list");
  };

  template<class E, class L>
  // head :: [a] -> a
  struct _head_impl<list<E, L>> {
    using type = E;
  };

  // head :: [a] -> a
  struct head_t {
    template<class List>
    using apply = _head_impl<List>;
  } head;

  ///////////////////////////////////

  template<class T>
  struct _list_impl {
    static_assert(____false_depend<T>::value, "World 2: cannot unify T with [a]");
  };

  template<class mt_E>
  // tail :: [] -> *error*
  struct _list_impl<empty_list<mt_E>> {
    static_assert(____false_depend<mt_E>::value, "World 1: attempted to take tail of empty list");
  };
  template<class E, class L>
  // tail :: [a] -> [a]
  struct _list_impl<list<E, L>> {
    using type = L;
  };

  // tail :: [a] -> [a]
  struct tail_t {
    template<class List>
    using apply = _list_impl<List>;
  } tail;

  ///////////////////////////////////

  template<class A, class L>
  struct _cons_impl {
    static_assert(____false_depend<L>::value, "World 2: cannot unify L with [A]");
  };

  template<class A, class H, class L>
  // cons :: a -> [a] -> [a]
  struct _cons_impl<A, list<H, L>> {
    static_assert(std::is_same<mt_list<Metatype<A>>, Metatype<L>>::value, "World 2: Cannot add an A into a list of type [H]");
    using type = list<A, list<H, L>>;
  };

  template<class A, class mt_>
  // cons :: a -> [] -> [a]
  struct _cons_impl<A, empty_list<mt_>> {
    static_assert(same_mt<Metatype<A>, mt_>::value, "World 2: Cannot add an A into an empty list of type [B]");
    using type = list<A, empty_list<Metatype<A>>>;
  };

  // cons :: a -> [a] -> [a]
  static const struct cons_t {
    template<class A>
    struct apply {
      struct type {
        template<class L>
        using apply = _cons_impl<A, L>;
        using metatype = decltype(mt_list<Metatype<A>>() >>= mt_list<Metatype<A>>());
      };
      static_assert(has_metatype<A>::value, "Argument A must have a metatype");
    };
    using metatype = decltype(mt_any_() >>= mt_list<mt_any_>() >>= mt_list<mt_any_>());
  } cons;

  ////////////////////////////////////
  // empty_list :: []
  using nil_t = empty_list<mt_any_>;
  static const nil_t nil;

  ////////////////////////////////////
  // make_list -- shortcut function
  template<class...Args>
  struct _make_list_impl;

  template<>
  struct _make_list_impl<> {
    using type = nil_t;
  };

  template<class A, class...Args>
  struct _make_list_impl<A, Args...> {
    using type = apply_t<cons_t, A, typename _make_list_impl<Args...>::type>;
  };

  template<class...Args>
  typename _make_list_impl<Args...>::type make_list(Args...) {
    return typename _make_list_impl<Args...>::type();
  }

  template<class...Args>
  using make_list_t = typename _make_list_impl<Args...>::type;

}

#include "mtc_foldable.hpp"

namespace rasmeta {

  template<class T>
  struct mtc_foldable<mt_list<T>> : implicits<mtc_foldable, mt_list<T>>{
    ////////////////////////////////////
    // foldr :: (a -> b -> b) -> b -> [a] -> b
    static const struct foldr_t foldr;

    template<class F, class I, class L>
    struct _foldr_impl {
      static_assert(same_mt<Metatype<L>, mt_list<mt_any_>>::value, "foldr's third argument must be a list");
      static_assert(____false_depend<L>::value, "INTERNAL LIBRARY ERROR: Dispatch failed on list!");
    };
    template<class F, class I, class L_MT>
    struct _foldr_impl<F, I, empty_list<L_MT>> {
      using type = I;
    };
    template<class F, class I, class H, class L>
    struct _foldr_impl<F, I, list<H, L>> {
      using type = decltype(apply(F(), H(), apply(foldr, F(), I(), L())));
    };

    struct foldr_t {
      using FMT = decltype(mt_any_() >>= mt_any_() >>= mt_any_());
      using metatype = decltype(FMT() >>= mt_any_() >>= mt_list<mt_any_>() >>= mt_any_());
      template<class F>
      struct apply {
        static_assert(same_mt<Metatype<F>, FMT>::value, "foldr must be applied to a function of type 'a -> b -> b'");
        using FMT2 = unify_mt_t<Metatype<F>, FMT>; // retrieve the unified type of the first argument
        using A_MT = arg_mt<FMT2>; // Fetch the 'a' type variable from the function 'a -> b -> b'
        using R_MT = ret_mt<FMT2>; // Fetch the 'b -> b' type from the function
        using B_MT = arg_mt<R_MT>; // Fetch the 'b' type variable from 'b -> b'
        struct type {
          using metatype = decltype(B_MT() >>= mt_list<A_MT>() >>= B_MT());
          template<class I>
          struct apply {
            static_assert(same_mt<Metatype<I>, B_MT>::value, "foldr's second argument must be of type 'b'");
            using B2_MT = unify_mt_t<Metatype<I>, B_MT>; // Utilize the new information about type variable b
            // TODO: recheck entire type up to here
            struct type {
              using metatype = decltype(mt_list<A_MT>() >>= B2_MT());
              template<class L>
              using apply = _foldr_impl<F, I, L>;
            };
          };
        };
      };
    };
  };

  ////////////////////////////////////////
  // concat :: [a] -> [a] -> [a]
  struct _concat_impl {
    using foldr_t = mtc_foldable<mt_list<mt_any<>>>::foldr_t;

    using concat_t = apply_t<swap_t, apply_t<foldr_t, cons_t>>;
  };
  // concat :: [a] -> [a] -> [a]
  using concat_t = _concat_impl::concat_t;
}

#include "mtc_monoid.hpp"

namespace rasmeta {

  template<class T>
  struct mtc_monoid<mt_list<T>> : implicits<mtc_monoid, mt_list<T>>{
    using mempty_t = nil_t;
    static const mempty_t mempty;

    ////////////////////////////////////
    // mappend :: [a] -> [a] -> [a]
    using mappend_t = concat_t;
    static const mappend_t mappend;
  };

}
