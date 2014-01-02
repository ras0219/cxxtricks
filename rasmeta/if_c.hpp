#pragma once

namespace rasmeta {

  template<bool c, class e1, class e2>
  struct if_c_impl {
    using type = e1;
  };
  template<class e1, class e2>
  struct if_c_impl<false, e1, e2> {
    using type = e2;
  };

  template<bool c, class e1, class e2>
  // if_c<true,  e1, __> = e1
  // if_c<false, __, e2> = e2
  using if_c = typename if_c_impl<c, e1, e2>::type;

}