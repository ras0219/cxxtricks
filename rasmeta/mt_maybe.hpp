#pragma once

#include "mt.hpp"
#include "mt_lambda.hpp"

namespace rasmeta {

  template<class mt_E>
  struct mt_maybe : mt_base {};

  template<class mt_E>
  struct nothing_t {
    using metatype = mt_maybe<mt_E>;
  };

  template<class T>
  struct just_t {
    static_assert(is_mt<T>::value, "T does not have a Metatype");
    using metatype = mt_maybe<Metatype<T>>;
  };

  static const nothing_t<mt_any_> nothing;
}

#include "mtc_monoid.hpp"

namespace rasmeta {

  namespace _maybe_mappend {
    template<class A, class B> struct s {};

    template<class A, class B> struct s<just_t<A>, just_t<B>> {
      static_assert(same_mt<Metatype<A>, Metatype<B>>::value, "Type Failure");
      using base_monoid = mtc_monoid<Metatype2<A, B>>;
      using type = apply_t<typename base_monoid::mappend_t, A, B>;
    };
    template<class A, class B> struct s<nothing_t<A>, just_t<B>> {
      using type = just_t<B>;
    };
    template<class A, class B> struct s<just_t<A>, nothing_t<B>> {
      using type = just_t<A>;
    };
    template<class A, class B> struct s<nothing_t<A>, nothing_t<B>> {
      using type = nothing_t<Metatype2<A, B>>;
    };

    template<class A, class B> using u = s<A, B>;
    using l = lambda2<u, mt_maybe<mt_any_>, mt_maybe<mt_any_>, mt_maybe<mt_any_>>;
  }

  template<class T>
  // (Monoid a) => Monoid (Maybe a)
  struct mtc_monoid<mt_maybe<T>> : implicits<mtc_monoid, mt_maybe<T>> {
    // mempty :: Nothing
    using mempty_t = nothing_t<Metatype<T>>;
    // mempty :: Nothing
    static const mempty_t mempty;

    ////////////////////////////////////
    // mappend :: (Monoid a) => Maybe a -> Maybe a -> Maybe a
    using mappend_t = _maybe_mappend::l;
    // mappend :: (Monoid a) => Maybe a -> Maybe a -> Maybe a
    static const mappend_t mappend;
  };

}

#include "mtc_foldable.hpp"

namespace rasmeta {

  namespace _maybe_foldr {
    template<class A, class B, class C> struct s {};

    template<class A, class B, class C> struct s<A, B, just_t<C>> {
      using type = apply_t<A, C, B>;
    };
    template<class A, class B, class C> struct s<A, B, nothing_t<C>> {
      using type = B;
    };

    template<class A, class B, class C> using u = s<A, B, C>;
    using l = lambda3<u, mt_arr2<mt_any_, mt_any_, mt_any_>, mt_any_, mt_maybe<mt_any_>, mt_any_>;
  }

  template<class T>
  // Foldable Maybe
  struct mtc_foldable<mt_maybe<T>> : implicits<mtc_foldable, mt_maybe<T>>{
    // foldr :: (a -> b -> b) -> b -> Maybe a -> b
    using foldr_t = _maybe_foldr::l;
    // foldr :: (a -> b -> b) -> b -> Maybe a -> b
    static const foldr_t foldr;
  };

}