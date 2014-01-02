#pragma once

#include "false_depend.hpp"
#include <type_traits>
#include "tcheck_structs.hpp"
#include "mt_type.hpp"
#include "mt_expr.hpp"

namespace rasmeta {

  ///////////////////////////////////
  // Sub-algorithms for alg W

  template<long long N, class T>
  // subst_tref :: (int, type) -> type
  struct subst_tref {
    using type = T;
  };
  template<long long N, long long N2>
  struct subst_tref<N, tref<N2>> {
    using type = tref<N2 - 1>;
  };
  template<long long N>
  struct subst_tref<N, tref<0>> {
    using type = tvar<N>;
  };
  template<long long N, class T1, class T2>
  struct subst_tref<N, arr<T1, T2>> {
    using t1 = typename subst_tref<N, T1>::type;
    using t2 = typename subst_tref<N, T2>::type;
    using type = arr<t1, t2>;
  };
 
  // instantiate :: (int, type) -> (int, type)
  // The general idea here is to substitute the forall's one at a time, inside out.
  template<long long N, class TypeExpr>
  struct instantiate_impl {
    using type = TypeExpr;
    static const long long N_ = N;
  };

  template<long long N, class TypeExpr>
  struct instantiate_impl<N, forall<TypeExpr>> {
    using aux = instantiate_impl<N, TypeExpr>;

    using type2 = typename aux::type;
    static const long long n2 = aux::N_;

    using type = typename subst_tref<n2, type2>::type;
    static const long long N_ = n2 + 1;
  };

  template<long long N, class TypeExpr>
  // instantiate :: (int, type) -> (int, type)
  using instantiate = instantiate_impl<N, TypeExpr>;

  ///// substitute
  template<class Submap, class TypeExpr>
  struct substitute_impl {
    using type = TypeExpr;
  };

  template<class Submap, class TypeExpr>
  using substitute = typename substitute_impl<Submap, TypeExpr>::type;

  template<class Submap, class T1, class T2>
  struct substitute_impl<Submap, arr<T1, T2>> {
    using t1 = substitute<Submap, T1>;
    using t2 = substitute<Submap, T2>;
    using type = arr<t1, t2>;
  };

  template<class Submap, long long N>
  struct substitute_impl<Submap, tvar<N>> {
    using type = tcheck::fix_lookup<Submap, tvar<N>>;
  };

  ///// unify
  template<class Submap, class T1, class T2>
  struct unify_impl {
    static_assert(____false_depend<T1>::value, "Compile-time typechecking failed.");
  };
  template<class Submap, class T1, class T2>
  using unify = typename unify_impl<Submap, T1, T2>::type;

  template<class Submap, class T>
  struct unify_impl<Submap, T, T> {
    using type = Submap;
  };
  template<class Submap, class T1, class T2, class T3, class T4>
  struct unify_impl<Submap, arr<T1, T2>, arr<T3, T4>> {
    using aux1 = unify<Submap, T1, T3>;
    using T2_ = substitute<aux1, T2>;
    using T4_ = substitute<aux1, T4>;

    using type = unify<aux1, T2_, T4_>;
  };
  template<class Submap, class T1, class T2>
  struct unify_impl<Submap, arr<T1, T2>, arr<T1, T2>> {
    using type = Submap;
  };
  template<class Submap, long long N, class T>
  struct unify_impl<Submap, tvar<N>, T> {
    using type = tcheck::insert<Submap, tvar<N>, T>;
  };
  template<class Submap, long long N, class T>
  struct unify_impl<Submap, T, tvar<N>> {
    using type = tcheck::insert<Submap, tvar<N>, T>;
  };
  template<class Submap, long long N, long long M>
  struct unify_impl<Submap, tvar<N>, tvar<M>> {
    using type = tcheck::insert<Submap, tvar<N>, tvar<M>>;
  };
  template<class Submap, long long N>
  struct unify_impl<Submap, tvar<N>, tvar<N>> {
    using type = Submap;
  };

  ///// Generalize
  template<long long N, class Submap, class T>
  struct generalize_impl {
    using type = T;
    using S = Submap;
    static const long long n = N;
  };
  template<long long n0, class S0, class T1, class T2>
  struct generalize_impl<n0, S0, arr<T1, T2>> {
    using g1 = generalize_impl<n0, S0, T1>;
    using S1 = typename g1::S;
    using type1 = typename g1::type;
    static const long long n1 = g1::n;

    using g2 = generalize_impl<n1, S1, T2>;
    using type2 = typename g2::type;
    // return vals
    using type = arr<type1, type2>;
    using S = typename g2::S;
    static const long long n = g2::n;
  };
  // <helper>
  template<class M, class Key, class Value, long long N>
  struct add_if_missing {
    using S = M;
    using type = Value;
    static const long long n = N;
  };
  template<class M, class Key, long long N>
  struct add_if_missing<M, Key, tcheck::not_found, N> {
    using S = tcheck::insert<M, Key, tref<N>>;
    using type = tref<N>;
    static const long long n = N + 1;
  };
  // </helper>
  template<long long n0, class S0, long long N>
  struct generalize_impl<n0, S0, tvar<N>> {
    using aux = add_if_missing<S0, tvar<N>, tcheck::lookup<S0, tvar<N>>, n0>;
    // return vals
    using type = typename aux::type;
    using S = typename aux::S;
    static const long long n = aux::n;
  };

  template<class Expr>
  struct generalize_impl2 {
    using g = generalize_impl<0, tcheck::variadic<>, Expr>;
    using type = tcheck::wrap<forall, typename g::type, g::n>;
  };
  template<class Expr>
  using generalize = typename generalize_impl2<Expr>::type;

  ///////////////////////////////////
  // Algorithm W
  template<long long N, class Submap, class LamStack, class Expr>
  struct W {
    using aux = instantiate<N, typename metatype<Expr>::type>;

    static const long long N_ = aux::N_;
    using Submap_ = Submap;
    using type = typename aux::type;
  };
  template<long long N, class Submap, class LamStack, class Expr>
  struct W<N, Submap, LamStack, lambda<Expr>>{
    using beta = tvar<N>;
    using LamStack2 = tcheck::push<LamStack, beta>;
    using aux = W<N + 1, Submap, LamStack2, Expr>;
    using tau = typename aux::type;
    using S = typename aux::Submap_;

    using type = substitute<S, arr<beta, tau>>;
    using Submap_ = S;
    static const long long N_ = aux::N_;
  };
  template<long long N, class Submap, class LamStack, class E1, class E2>
  struct W<N, Submap, LamStack, app<E1, E2>> {
    using aux1 = W<N, Submap, LamStack, E1>;
    using tau1 = typename aux1::type;
    using S1 = typename aux1::Submap_;

    using aux2 = W<aux1::N_, S1, LamStack, E2>;
    using tau2 = typename aux2::type;
    using S2 = typename aux2::Submap_;

    using beta = tvar<aux2::N_>;
    // return vals
    using Submap_ = unify<S2, substitute<S2, tau1>, arr<tau2, beta>>;
    using type = substitute<Submap, beta>;
    static const long long N_ = aux2::N_ + 1;
  };
  template<long long N, class Submap, class LamStack, long long N2>
  struct W<N, Submap, LamStack, ref<N2>>{
    using tau = tcheck::at<LamStack, N2>;

    static const long long N_ = N;
    using Submap_ = Submap;
    using type = substitute<Submap, tau>;
  };

  template<class Expr>
  struct typecheck_impl {
    using aux = W<0, tcheck::variadic<>, tcheck::variadic<>, Expr>;

    using type = generalize<typename aux::type>;
  };
  template<class Expr>
  using typecheck = typename typecheck_impl<Expr>::type;
}