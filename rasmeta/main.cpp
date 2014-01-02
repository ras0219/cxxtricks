#include "tcheck.hpp"
#include "mt_tsystem.hpp"
#include "mt_eval.hpp"
#include "mt_prettyprinter.hpp"
//#include "tcheck.test.hpp"
//#include "mt_eval.test.hpp"
#include <iostream>

using namespace std;
using namespace rasmeta;

using f = compile<lambda<_0>>;

using f2 = compile<lambda<app<_0, bool_c<true>>>>;

using f3 = compile<lambda<f>>;

int main() {
  cout << "f :: " << f::type() << endl;
  cout << "f "; ultra_pp(cout, f::ast()); cout << endl;
  cout << "f2 :: " << f2::type() << endl;
  cout << "f2 "; ultra_pp(cout, f2::ast()); cout << endl;
  cout << "f3 :: " << f3::type() << endl;
  cout << "f3 "; ultra_pp(cout, f3::ast()); cout << endl;
  cin.get();
}
