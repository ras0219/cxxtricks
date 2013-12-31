#pragma once

#include <type_traits>

namespace rasmeta {

  struct mtk_base {};

  template<class T>
  using is_mtk = std::is_base_of<mtk_base, T>;

  template<class L, class R>
  struct mtk_arr : mtk_base {};

  // All metatypes should have a "metakind" field
  template<class MT>
  using Metakind = typename MT::metakind;

  ////////
  // same_mtk for comparing metakinds
  template<class L, class R>
  struct same_mtk {
    enum { value = false };
  };
  template<class L>
  struct same_mtk<L, L> {
    enum { value = true };
  };
  template<class L, class L2, class R, class R2>
  struct same_mtk<mtk_arr<L, L2>, mtk_arr<R, R2>> {
    enum { value = same_mtk<L,R>::value && same_mtk<L2, R2>::value };
  };
  template<class L, class L2>
  struct same_mtk<mtk_arr<L, L2>, mtk_arr<L, L2>> {
    enum { value = true };
  };


}