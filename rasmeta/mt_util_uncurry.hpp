#pragma once

namespace rasmeta {

  template<class Metafunc, class Arg, class...Args>
  decltype(uncurry_apply(Metafunc::apply<Arg>::type(), Args()...))
    uncurry_apply(Metafunc, Arg, Args...) {
      return uncurry_apply(Metafunc::apply<Arg>::type(), Args()...);
    }

  template<class Metafunc>
  Metafunc uncurry_apply(Metafunc) { return Metafunc(); }
}

#define PRETTIFY(METAFUNC)                                                      \
namespace pretty {                                                              \
  template<class...Args>                                                        \
  decltype(uncurry_apply(rasmeta::METAFUNC(), Args()...)) METAFUNC(Args...) {   \
    return uncurry_apply(rasmeta::METAFUNC(), Args()...);                       \
  }                                                                             \
}
