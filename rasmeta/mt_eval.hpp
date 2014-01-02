#include "mt_tsystem.hpp"
#include "if_c.hpp"

namespace rasmeta {

  // Increment all de bruijn references
  template<class e>
  struct inc_refs_impl {
    using type = e;
  };
  template<long long N>
  struct inc_refs_impl<ref<N>> {
    using type = ref<N + 1>;
  };

  template<class e>
  using inc_refs_wrap = typename inc_refs_impl<e>::type;

  template<class e>
  using inc_refs = map_type<inc_refs_wrap, e>;

  // Perform a beta reduction
  template<class e, class e2, long long N>
  // Substitute e2 for ref<N> in e
  struct beta_reduce_impl {
    using type = e;
  };

  template<class e, class e2, long long N>
  struct beta_reduce_impl<lambda<e>, e2, N> {
    using type = lambda<typename beta_reduce_impl<e, e2, N + 1>::type>;
  };

  template<class ef, class ea, class e2, long long N>
  struct beta_reduce_impl<app<ef,ea>, e2, N> {
    using ef2 = typename beta_reduce_impl<ef, e2, N>::type;
    using ea2 = typename beta_reduce_impl<ea, e2, N>::type;
    using type = app<ef2, ea2>;
  };

  template<long long N2, class e2, long long N>
  struct beta_reduce_impl<ref<N2>, e2, N> {
    using type = if_c<(N > N2), ref<N2>, ref<(N2-1)>>;
  };

  template<class e2, long long N>
  struct beta_reduce_impl<ref<N>, e2, N> {
    using type = e2;
  };

  template<class e, class e2, long long N>
  using beta_reduce = typename beta_reduce_impl<e, e2, N>::type;

  // Simplify an expression
  template<class e>
  struct simplify1 {
    using type = e;
  };
  template<class e> using simplify = typename simplify1<e>::type;

  template<
    class ef,
    class ea,
      bool is_ma = std::is_base_of<functional_ast, ef>::value,
      bool is_mf = (!is_ma) && std::is_base_of<functional_metafunc, ef>::value
  >
  struct simplify_apply1 {
    // Function application failed -- this is probably due to some black box builtin
    // Since we can't simplify the function itself, let's at least try to simplify the arg
    using type = app<ef, simplify<ea>>;
  };
  template<class ef, class ea>
  struct simplify_apply1<ef, ea, false, true> {
    // ef is a metafunction! We can apply this at compile time!

    // We will switch to "Applicative order" evaluation here in order to make metafunctions easier to use
    using type = simplify<typename ef::func::template apply<simplify<ea>>>;
  };

  template<class ef, class ea>
  struct simplify_apply1<ef, ea, true, false> {
    // ef is a compiled ast! This means it provides a simplified ast as X::ast
    using type = typename simplify_apply1<typename ef::ast, ea>::type;
  };

  template<class ef, class ea>
  struct simplify_apply1<lambda<ef>, ea, false, false> {
    using e2 = beta_reduce<ef, ea, 0>;
    // Since we've successfully applied the function, simplify the result
    using type = simplify<e2>;
  };

  template<class ef, class ea>
  struct simplify1<app<ef, ea>> {
    using ef2 = simplify<ef>;
    // Only simplify the function, we're using "Normal order" evaluation
    using type = typename simplify_apply1<ef2, ea>::type;
  };

  template<class AST>
  struct compile : functional, functional_ast {
    using original_ast = AST;
    using type = typecheck<original_ast>;
    using ast = simplify<original_ast>;
  };

}