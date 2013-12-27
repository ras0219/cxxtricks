#pragma once

#include <type_traits>
#include "mt.hpp"
#include "false_depend.hpp"
#include "mt_util_uncurry.hpp"

namespace rasmeta {

  template<class mt_E>
  struct mt_list {};

  ////////////////////////////////
  template<class mt_E>
  struct empty_list {
    using metatype = mt_list<mt_E>;
  };

  template<class E, class L>
  struct list {
    static_assert(std::is_same<mt_list<Metatype<E>>, Metatype<L>>::value, "World 2: Cannot add an E into a list of type [A]");
    using metatype = mt_list<Metatype<E>>;
  };
  ///////////////////////////////////

  // head :: [a] -> a
  struct head {
    template<class List>
    struct apply {
      static_assert(____false_depend<List>::value, "World 2: cannot unify List with [a]");
    };
  };

  template<class mt_E>
  // head :: [] -> *error*
  struct head::apply<empty_list<mt_E>> {
    static_assert(____false_depend<mt_E>::value, "World 1: attempted to take head of empty list");
  };

  template<class E, class L>
  // head :: [a] -> a
  struct head::apply<list<E, L>> {
    using type = E;
  };

  PRETTIFY(head);

  ///////////////////////////////////

  // tail :: [a] -> [a]
  struct tail {
    template<class List>
    struct apply {
      static_assert(____false_depend<List>::value, "World 2: cannot unify List with [a]");
    };
  };

  template<class mt_E>
  // tail :: [] -> *error*
  struct tail::apply<empty_list<mt_E>> {
    static_assert(____false_depend<mt_E>::value, "World 1: attempted to take tail of empty list");
  };
  template<class E, class L>
  // tail :: [a] -> [a]
  struct tail::apply<list<E, L>> {
    using type = L;
  };

  PRETTIFY(tail);

  ///////////////////////////////////

  // cons :: a -> [a] -> [a]
  struct cons {
    template<class E, class L>
    struct _impl {
      static_assert(____false_depend<L>::value, "World 2: cannot unify Element with List");
    };

    template<class Element>
    struct apply {
      struct type {
        template<class List>
        struct apply {
          using type = typename _impl<Element, List>::type;
        };
      };
    };
  };

  template<class E, class A, class L>
  // cons :: a -> [a] -> [a]
  struct cons::_impl<E, list<A, L>> {
    static_assert(std::is_same<mt_list<Metatype<E>>, Metatype<L>>::value, "World 2: Cannot add an E into a list of type [A]");
    using type = list<E, list<A, L>>;
  };

  template<class E, class mt_A>
  // cons :: a -> [] -> [a]
  struct cons::_impl<E, empty_list<mt_A>> {
    //static_assert(same_mt<Metatype<E>, mt_A>::value, "World 2: Cannot add an E into a list of type [A]");
    using type = list<E, empty_list<Metatype<E>>>();
  };

  PRETTIFY(cons);

  ////////////////////////////////////
  // empty_list :: []
  empty_list<mt_any<>> nil() {
    return empty_list<mt_any<>>();
  }

  ////////////////////////////////////

}