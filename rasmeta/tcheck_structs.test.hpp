#pragma once

#include "tcheck_structs.hpp"

namespace rasmeta {

  namespace tcheck {

    namespace {
      template<long long N> struct _;

      using list1 = variadic<pair<_<1>, _<1>>, pair<_<2>, _<4>>, pair<_<3>, _<9>>>;
      using list2 = insert<insert<variadic<>, _<1>, _<2>>, _<1>, _<1>>;
      using list3 = erase<list2, _<1>>;

      static_assert(std::is_same<list3, variadic<>>::value, "");
      static_assert(std::is_same<lookup<variadic<>, _<1>>, not_found>::value, "");
      static_assert(std::is_same<lookup<list1, _<2>>, _<4>>::value, "");
      static_assert(std::is_same<lookup<list2, _<2>>, not_found>::value, "");
      static_assert(std::is_same<lookup<list2, _<1>>, _<1>>::value, "");
    }

  }

}