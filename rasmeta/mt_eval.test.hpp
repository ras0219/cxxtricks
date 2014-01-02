#pragma once

#include "mt_eval.hpp"

namespace rasmeta {

  namespace _mt_eval_test1 {
    using ast1 = int_c<0>;
    using ast2 = lambda<_0>;
    using test = simplify<app<ast2, ast1>>;

    static_assert(std::is_same<test, ast1>::value, "");
  }

  namespace _mt_eval_test2 {
    using zero = int_c<0>;
    using one = int_c<1>;
    using ast2 = lambda<lambda<_1>>;
    using ast3 = app<ast2, one>;
    using ast4 = app<ast3, zero>;
    // (\y.\z.y) 1 0
    using test = simplify<ast4>;

    static_assert(std::is_same<test, one>::value, "");
  }

  namespace _mt_eval_test3 {
    using zero = int_c<0>;
    using one = int_c<1>;
    using two = int_c<2>;
    using ast2 = lambda<lambda<lambda<_1>>>;
    using ast3 = app<ast2, two>;
    using ast4 = app<ast3, one>;
    using ast5 = app<ast4, zero>;
    // (\x.\y.\z.y) 2 1 0
    using test = simplify<ast5>;

    static_assert(std::is_same<test, one>::value, "");
  }

  namespace _mt_eval_test4 {
    using zero = int_c<0>;
    using one = int_c<1>;
    using two = int_c<2>;
    using ast1 = lambda<_0>;
    using ast2 = lambda<ast1>;
    using ast3 = app<ast1, ast2>;
    using ast4 = app<ast3, one>;
    using ast5 = app<ast4, zero>;
    // (\x.x) (\y.\z.z) 1 0
    using test = simplify<ast5>;

    static_assert(std::is_same<test, zero>::value, "");
  }

  namespace _mt_eval_test5 {
    using zero = int_c<0>;
    using one = int_c<1>;
    using two = int_c<2>;
    // (\y. y)
    using ast1 = lambda<_0>;
    // (\y. y) ?
    using ast2 = app<ast1, _0>;
    // (\x. (\y. y) x)
    using ast3 = lambda<ast2>;
    using ast4 = app<ast3, zero>;
    // (\x. (\y. y) x) 0
    using test = simplify<ast4>;

    static_assert(std::is_same<test, zero>::value, "");
  }

  namespace _mt_eval_test6 {
    using zero = int_c<0>;
    using one = int_c<1>;
    using two = int_c<2>;
    // (\y.\z. y)
    using ast1 = lambda<_0>;
    // (\y.\z. y) ?
    using ast2 = app<ast1, _0>;
    // (\x. (\y.\z. y) x)
    using ast3 = lambda<ast2>;
    using ast4 = app<ast3, zero>;
    // (\x. (\y.\z. y) x) 0 1
    using test = simplify<ast4>;

    static_assert(std::is_same<test, zero>::value, "");
  }

  namespace _mt_eval_test7 {
    // u y = y y
    using fixpoint = lambda<app<_0, _0>>;
    // apply f x = f x
    using apply = lambda<lambda<app<_1, _0>>>;
    // constant k _ = k
    using constant = lambda<lambda<_1>>;
    // zero _ x = x
    using zero = lambda<lambda<_0>>;
    // inc n f x = f (n f x)
    using inc = lambda<lambda<lambda<app<_1, app<app<_2, _1>, _0>>>>>;
    // one = inc zero
    using one = app<inc, zero>;
    // two = inc one
    using two = app<inc, one>;
    // three = inc two
    using three = app<inc, two>;
    // four = inc three
    using four = app<inc, three>;
    // compose f g x = f (g x)
    using compose = lambda<lambda<lambda<app<_2, app<_1, _0>>>>>;

    // Assert that compose zero zero 1 0 = 0
    using test1 = simplify<app<app<app<app<compose, zero>, zero>, int_c<1>>, int_c<0>>>;
    static_assert(std::is_same<test1, int_c<0>>::value, "");

  }

}