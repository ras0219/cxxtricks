#pragma once

#include <type_traits>
#include "mt.hpp"
#include "false_depend.hpp"
#include "mt_util_uncurry.hpp"

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
  struct cons_t {
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
  static nil_t nil;

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
    using type = list<A, typename _make_list_impl<Args...>::type>;
  };

  template<class...Args>
  typename _make_list_impl<Args...>::type make_list(Args...) {
    return typename _make_list_impl<Args...>::type();
  }

}

