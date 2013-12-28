#pragma once

#include <type_traits>

#if (defined(__GNUG__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 8))) \
  || (defined(__clang__))
namespace std {
  // template<bool B, class T = void>
  // struct enable_if {};

  // template<class T>
  // struct enable_if<true, T> { typedef T type; };

  template<bool B, class T = void>
  using enable_if_t = typename enable_if<B, T>::type;
}
#endif

// So it goes like this:
//  * Metatypes are structs with the prefix mt_
//  * Metatypes of kind * are simple structs
//  * Metatypes of kind * -> * are template structs that are bound to the struct mt_meta

namespace rasmeta {
  // Base Metatype. Used for metatype detection.
  struct mt_base {};

  template<class T>
  using is_mt = std::is_base_of<mt_base, T>;

  // Wildcard Metatype. Used (for now) to make * -> * Metakinds concrete
  template<class...Uniquifiers>
  struct mt_any : mt_base {};

  using mt_any_ = mt_any<>;

  // Arrow Metatype. Used to mark functions.
  template<class mt_L, class mt_R>
  struct mt_arr : mt_base {};

  //// This might be awful. Or awesome. Probably awfully awesome...
  // Design decisions: both > and >> are left associative.
  //    -> requires an identifier on the right hand side
  // But >>= is actually right associative!
  // Thus the decltype(mt_any<>() >>= mt_num() >>= mt_list<mt_any<>>()) was born...
  template<class mt_L, class mt_R, class = std::enable_if_t<is_mt<mt_L>::value && is_mt<mt_R>::value>>
  mt_arr<mt_L, mt_R> operator >>=(mt_L, mt_R) { return mt_arr<mt_L, mt_R>(); }

  // Tuple Metatype. Use for multiple parameters
  template<class...mt_T>
  struct mt_tuple : mt_base {};

  //// More awfully awesomely terrible overloads!
  template<class mt_L, class mt_R, class = std::enable_if_t<is_mt<mt_L>::value && is_mt<mt_R>::value>>
  mt_tuple<mt_L, mt_R> operator ,(mt_L, mt_R) { return mt_tuple<mt_L, mt_R>(); }
  template<class...mt_L, class mt_R, class = std::enable_if_t<is_mt<mt_R>::value>>
  mt_tuple<mt_L..., mt_R> operator ,(mt_tuple<mt_L...>, mt_R) { return mt_tuple<mt_L..., mt_R>(); }

  ////////////////////////////////////
  // same_mt calculates if two metatypes are unifiable
  template <class L, class R>
  struct same_mt {
    enum { value = false };
  };
  template <class L, class...Us>
  struct same_mt<L, mt_any<Us...>> {
    enum { value = true };
  };
  template <class...Us, class R>
  struct same_mt<mt_any<Us...>, R> {
    enum { value = true };
  };
  template <class L>
  struct same_mt<L, L> {
    enum { value = true };
  };
  template <template<class>class C, class L, class R>
  struct same_mt<C<L>, C<R>> {
    enum { value = same_mt<L, R>::value };
  };

  /////////////////////////////////
  template<class T>
  // Function to extract the metatype
  using Metatype = typename T::metatype;

  ///////////////////////////////
  // Convenience alias for applications
  template<class...B>
  struct _Apply_impl;
  
  template<class F>
  struct _Apply_impl<F> {
    using type = F;
  };

  template<class F, class A, class...B>
  struct _Apply_impl<F, A, B...> {
    using F2 = typename F::template apply<A>::type;
    using type = typename _Apply_impl<F2, B...>::type;
  };

  template<class...F>
  using Apply = typename _Apply_impl<F...>::type;

  template<class F, class...Args>
  Apply<F, Args...> apply(Args...) { return Apply<F, Args...>(); }

  template<class F, class...Args>
  Apply<F, Args...> partial(F, Args...) { return Apply<F, Args...>(); }
}
