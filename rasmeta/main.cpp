#include "tcheck.hpp"
#include "mt_tsystem.hpp"
//#include "tcheck.test.hpp"
#include "mt_eval.test.hpp"
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

struct f : functional {
  using ast = lambda<_0>;
  using type = typecheck<ast>;
};
struct f2 : functional {
  using ast = lambda<app<_0, bool_c<true>>>;
  using type = typecheck<ast>;
};
struct f3 : functional {
  using ast = lambda<f>;
  using type = typecheck<ast>;
};

int main() {
  cout << "f :: " << f::type() << endl;
  cout << "f2 :: " << f2::type() << endl;
  cout << "f3 :: " << f3::type() << endl;
  cin.get();
}
