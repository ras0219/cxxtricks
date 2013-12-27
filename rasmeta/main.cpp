#include "mt_list.hpp"
#include "mt_integral.hpp"

using namespace rasmeta;

int main()
{
  auto f = nil();
  auto f1 = cons(int_c<1>(), f);
  auto f2 = cons(int_c<2>(), f1);
  auto f3 = cons(bool_c<true>(), f);
}