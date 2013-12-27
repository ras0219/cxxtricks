#pragma once

#include <type_traits>
#include "mt.hpp"

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
    using metatype = Metatype<L>;
    using head = E;
    using tail = L;
  };
  ///////////////////////////////////

  template<class mt_E>
  // head :: [] -> *error*
  void head(empty_list<mt_E>) {
    static_assert(false, "World 1: attempted to take head of empty list");
  }
  template<class E, class L>
  // head :: [a] -> a
  E head(list<E, L>) { return E(); }

  ///////////////////////////////////

  template<class mt_E>
  // tail :: [] -> *error*
  void tail(empty_list<mt_E>) {
    static_assert(false, "World 1: attempted to take tail of empty list");
  }
  template<class E, class L>
  // tail :: [a] -> [a]
  L tail(list<E, L>) { return L(); }

  ///////////////////////////////////

  template<class E, class A, class L>
  // cons :: a -> [a] -> [a]
  list<E, list<A, L>> cons(E, list<A, L>) {
    static_assert(std::is_same<mt_list<Metatype<E>>, Metatype<L>>::value, "World 2: Cannot add an E into a list of type [A]");
    return list<E, list<A, L>>();
  }

  template<class E, class mt_A>
  // cons :: a -> [] -> [a]
  list<E, empty_list<Metatype<E>>> cons(E, empty_list<mt_A>) {
    static_assert(same_mt<Metatype<E>, mt_A>::value, "World 2: Cannot add an E into a list of type [A]");
    return list<E, empty_list<Metatype<E>>>();
  }

  ////////////////////////////////////
  // empty_list :: []
  empty_list<mt_any<>> nil() {
    return empty_list<mt_any<>>();
  }

  ////////////////////////////////////

}