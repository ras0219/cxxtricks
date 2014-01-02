#include "tcheck.hpp"
#include "mt_tsystem.hpp"
#include "mt_eval.hpp"
//#include "tcheck.test.hpp"
//#include "mt_eval.test.hpp"
#include <iostream>

using namespace std;
using namespace rasmeta;

template<long long N>
ostream& operator<<(ostream& os, tvar<N>) {
  return os << 't' << N;
}

template<class T1, class T2>
ostream& operator<<(ostream& os, arr<T1, T2>) {
  return os << T1() << " -> " << T2();
}

template<class T1a, class T1b, class T2>
ostream& operator<<(ostream& os, arr<arr<T1a, T1b>, T2>) {
  return os << '(' << arr<T1a, T1b>() << ") -> " << T2();
}

template<class T>
ostream& operator<<(ostream& os, forall<T>) {
  return os << "\\. " << T();
}

template<long long N>
ostream& operator<<(ostream& os, tref<N>) {
  if (N >= 0 && N < 26)
    return os << (char)('a' + N);
  return os << 'r' << N;
}

ostream& operator<<(ostream& os, concrete_type<int>) {
  return os << "int";
}

ostream& operator<<(ostream& os, concrete_type<bool>) {
  return os << "bool";
}

template<class T, T N>
ostream& operator<<(ostream& os, constant_c<T, N>) {
  return os << N;
}

template<class E>
ostream& operator<<(ostream& os, lambda<lambda<E>>) {
  return os << "\\." << lambda<E>();
}
template<class E>
ostream& operator<<(ostream& os, lambda<E>) {
  return os << "\\. " << E();
}
template<class E1, class E2>
ostream& operator<<(ostream& os, app<E1, E2>) {
  return os << E1() << " " << E2();
}
template<class E1, class E2>
ostream& operator<<(ostream& os, app<lambda<E1>, E2>) {
  return os << "(" << lambda<E1>() << ") " << E2();
}
template<class E1, class E2, class E3>
ostream& operator<<(ostream& os, app<E1, app<E2, E3>>) {
  return os << E1() << " (" << app<E2, E3>() << ")";
}
template<class E1, class E2, class E3>
ostream& operator<<(ostream& os, app<lambda<E1>, app<E2, E3>>) {
  return os << "(" << lambda<E1>() << ") " << "(" << app<E2, E3>() << ")";
}
template<long long N>
ostream& operator<<(ostream& os, rasmeta::ref<N>) {
  return os << 'r' << N;
}
template<class E>
ostream& operator<<(ostream& os, compile<E>) {
  return os << "<compiled>";
}

using f = compile<lambda<_0>>;

using f2 = compile<lambda<app<_0, bool_c<true>>>>;

using f3 = compile<lambda<f>>;

int main() {
  cout << "f :: " << f::type() << endl;
  cout << "f = " << f::original_ast() << endl;
  cout << "  = " << f::ast() << endl;
  cout << "f2 :: " << f2::type() << endl;
  cout << "f2 = " << f2::original_ast() << endl;
  cout << "   = " << f2::ast() << endl;
  cout << "f3 :: " << f3::type() << endl;
  cout << "f3 = " << f3::original_ast() << endl;
  cout << "   = " << f3::ast() << endl;
  cin.get();
}
