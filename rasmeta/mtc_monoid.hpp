#pragma once

#include "mtc.hpp"

namespace rasmeta {

  /////////////////////////////////
  // Monoid typeclass
  template<class T>
  struct mtc_monoid {
    static_assert(____false_depend<T>::value, "Type T does not inhabit the typeclass Monoid");

    // required: mempty :: a
    // required: mappend :: a -> a -> a
    // implicit: mconcat :: [a] -> a
  };

  template<class T> struct mtc_foldable;

  template<class T>
  struct implicits<mtc_monoid, T> {
    using __base = mtc_monoid<T>;
    using __list = mtc_foldable<mt_list<mt_any_>>;

    // (implicit) mconcat :: [a] -> a
    using mconcat_t = decltype(apply(__list::foldr, __base::mappend, __base::mempty));
    // (implicit) mconcat :: [a] -> a
    static const mconcat_t mconcat;
  };

}