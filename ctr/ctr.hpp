#pragma once

#include <utility>
#include <tuple>

namespace exp_cxx14 {
  template< int ... > struct index_sequence{ using type = index_sequence; };

  template< typename T> using invoke = typename T::type;

  template< typename T, typename U > struct concate;
  template< int ...i, int ... j>
  struct concate< index_sequence<i...>, index_sequence<j...> >
    : index_sequence< i..., (j + sizeof ... (i))... >{};

  template< int n>
  struct make_index_sequence_help : concate<
    invoke< make_index_sequence_help<n / 2>>,
    invoke< make_index_sequence_help<n - n / 2>>
  > {};

  template<> struct make_index_sequence_help <0> : index_sequence<>{};
  template<> struct make_index_sequence_help <1> : index_sequence<0>{};

  template< int n> using make_index_sequence = invoke< make_index_sequence_help<n> >;
}

template<class...Args>
struct Storage : std::tuple<Args...> {
  template<class Tuple>
  Storage(Tuple&& t) : std::tuple<Args...>(std::forward<Tuple>(t)) {}

  template<class F>
  void visit(F& visitor);
};

template<class F, size_t N, class...Args>
struct _storage_visit_impl {
  static inline void visit(Storage<Args...>& s, F& f) {
    f(std::get<N - 1>(s));
    return _storage_visit_impl<F, N - 1, Args...>::visit(s, f);
  }
};
template<class F, class...Args>
struct _storage_visit_impl<F, 0, Args...> {
  static inline void visit(Storage<Args...>&, F&) {}
};

template<class...Args>
template<class F>
void Storage<Args...>::visit(F& f) {
  _storage_visit_impl<F, sizeof...(Args), Args...>::visit(*this, f);
}
