#pragma once

#include "mtc.hpp"
#include "mt_integral.hpp"

namespace rasmeta {

  /////////////////////////////////
  // Eq typeclass
  template<class T>
  struct mtc_eq {
    static_assert(____false_depend<T>::value, "Type T does not inhabit the typeclass Eq");

    // required function: eq :: a -> a -> Bool
    // implicit function: neq :: a -> a -> Bool
  };

  template<class T>
  struct implicits<mtc_eq, T> {
    template<class L, class R>
    // (implicit) neq :: a -> a -> Bool
    decltype(!mtc_eq<T>::eq(L(), R())) neq(L, R) {
      return !mtc_eq<T>::eq(L(), R());
    }
  };


  
  //// The following provides an instance of Eq for all integral constant types
  //// NOTE: This would normally occur in the mt_integral.hpp file, however this solves a chicken-and-egg issue
  template<class IntegralT>
  struct mtc_eq<mt_integral<IntegralT>> : implicits<mtc_eq, mt_integral<IntegralT>>{
    template<IntegralT N, IntegralT M>
    // eq :: (Num a) => a -> a -> Bool
    bool_c<(N == M)> eq(constant_c<IntegralT, N>, constant_c<IntegralT, M>) {
      return bool_c<(N == M)>();
    }
  };

}