#pragma once

// So it goes like this:
//  * Metatypes are structs with the prefix mt_
//  * Metatypes of kind * are simple structs
//  * Metatypes of kind * -> * are template structs that are bound to the struct mt_meta

namespace rasmeta {
  // Wildcard Metatype. Used (for now) to make * -> * Metakinds concrete
  template<class...Uniquifiers>
  struct mt_any {};

  // Arrow Metatype. Used to mark functions. NOTE: All functions are curried!
  template<class mt_L, class mt_R>
  struct mt_arr {};

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

}