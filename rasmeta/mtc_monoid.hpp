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
    // TODO: implicit: mconcat :: [a] -> a
  };

  template<class T>
  struct implicits<mtc_monoid, T> {
  };

}