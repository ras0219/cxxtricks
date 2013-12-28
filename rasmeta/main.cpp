#include "mt_list.hpp"
#include "mt_integral.hpp"
#include "mt_util_uncurry.hpp"

using namespace rasmeta;

int main()
{
  //wrap_f<rasmeta::head> headfunc;

  auto f = nil();
  auto f1 = apply<cons>(int_c<0>());
  list<int_c<0>, empty_list<Metatype<int_c<0>>>> f2 = partial(f1, f);

  //auto f1 = pretty::cons(int_c<1>(), f);
  //auto f2 = pretty::cons(int_c<2>(), f1);
  //auto f3 = pretty::cons(bool_c<true>(), f);

  //auto f2head = pretty::head(f2);
}