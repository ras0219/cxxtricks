#pragma once

namespace rasmeta {

  template<class IntegralT>
  struct mt_integral {};

  using mt_int = mt_integral<long long>;
  using mt_bool = mt_integral<bool>;

  template<class Num, Num N>
  // This type covers integral constants
  struct integral_c;

  template<long long N>
  struct integral_c<long long, N> {
    using metatype = mt_int;
  };

  template<bool N>
  struct integral_c<bool, N> {
    using metatype = mt_bool;
  };

  //// Some very nice little shortcuts
  template<bool N>
  using bool_c = integral_c<bool, N>;
  template<long long N>
  using int_c = integral_c<long long, N>;

  //// This operator is needed for the implicit parts of the Eq Metatypeclass
  template<bool B>
  bool_c<!B> operator!(bool_c<B>) { return bool_c<!B>(); }

}