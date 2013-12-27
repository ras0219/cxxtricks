#pragma once

#include "mtc.hpp"
#include "mt_integral.hpp"

namespace rasmeta {

  /////////////////////////////////
  // Ord typeclass
  template<class T>
  struct mtc_ord {
    static_assert(____false_depend<T>::value, "Type T does not inhabit the typeclass Ord");

    // required instance: Eq a
    // required function: lt :: a -> a -> Bool
    // TODO: implicit functions
  };

  //// The following provides an instance of Ord for all integral constant types
  //// NOTE: This would normally occur in the mt_integral.hpp file, however this solves a chicken-and-egg issue
  template<class IntegralT>
  struct mtc_ord<mt_integral<IntegralT>> : implicits<mtc_ord, mt_integral<IntegralT>>{
    template<IntegralT N, IntegralT M>
    // lt :: (Num a) => a -> a -> Bool
    bool_c<(N < M)> lt(constant_c<IntegralT, N>, constant_c<IntegralT, M>) {
      return bool_c<(N < M)>();
    }
  };
}