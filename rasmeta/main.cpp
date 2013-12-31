#include "mt_list.hpp"
#include "mt_integral.hpp"
#include "mt_util_uncurry.hpp"

using namespace rasmeta;

template<class A, class DIVIDER, class B>
struct assert_same_type {
  static_assert(std::is_same<A, B>::value, "Types do not match");
  static const int x = 0;
};

int main()
{
  //wrap_f<rasmeta::head> headfunc;

  //static_assert(same_mt<mt_list<mt_int>, mt_list<mt_any_>>::value, "Lists of ints must match lists of any");

  static_assert(std::is_same<decltype(mt_int(), mt_bool(), mt_int()), mt_tuple<mt_int, mt_bool, mt_int>>::value, "Failed operator, overload test1");

  auto f = nil;
  auto f1 = apply(cons, int_c<0>(), f);

  using L0 = nil_t;
  using L2 = apply_t<cons_t, int_c<0>, L0>;
  using L3 = apply_t<cons_t, int_c<2>, L2>;
  using L4 = apply_t<cons_t, int_c<1>, L3>;

  using L0b = make_list_t<>;
  using L2b = make_list_t<int_c<0>>;
  using L4b = make_list_t<int_c<1>, int_c<2>, int_c<0>>;

  auto L4c = make_list(int_c<1>(), int_c<2>(), int_c<0>());
  auto _42 = assert_same_type<decltype(L4()), struct XXXXXXXX, decltype(L4c)>::x;
  auto _42b = assert_same_type<L4b, struct XXXXXXXX, decltype(L4c)>::x;

  auto _0 = assert_same_type<Metatype<L2>, struct XXXXXXXX, Metatype<L2b>>::x;
  auto _4 = assert_same_type<L4, struct XXXXXXXX, L4b>::x;

  auto f2 = make_list(int_c<1>(), int_c<0>());

}