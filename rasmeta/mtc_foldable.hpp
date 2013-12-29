#pragma once

#include "mtc.hpp"

namespace rasmeta {

  /////////////////////////////////
  // Foldable typeclass
  template<class T>
  struct mtc_foldable {
    static_assert(____false_depend<T>::value, "Type T of kind '* -> *' does not inhabit the typeclass Foldable");

    // required: foldr :: (a -> b -> b) -> b -> T a -> b
    // TODO: implicit: foldl :: (a -> b -> a) -> a -> T b -> a
  };

  template<class T>
  struct implicits<mtc_foldable, T> {
  };

}