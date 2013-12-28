#include "mt_list.hpp"
#include "mt_integral.hpp"
#include "mt_util_uncurry.hpp"

using namespace rasmeta;

int main()
{
  //wrap_f<rasmeta::head> headfunc;

  static_assert(same_mt<mt_list<mt_int>, mt_list<mt_any_>>::value, "Lists of ints must match lists of any");

  static_assert(std::is_same<decltype(mt_int(), mt_bool(), mt_int()), mt_tuple<mt_int, mt_bool, mt_int>>::value, "Failed operator, overload test1");

  auto f = nil;
  auto f1 = apply(cons, int_c<0>(), f);

  auto f2 = make_list(int_c<1>(), int_c<0>());

}