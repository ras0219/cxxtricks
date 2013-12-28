#pragma once

// The following code may or may not work according to the c++11 standard
// I read the standard and couldn't figure out whether or not it should be supported...
// Either way, EDG says it's good but MSVC says it's not. Thus, I can't use it.
#if 0

template<class F>
struct has_apply1 {
  using yes = char[1];
  using no = char[2];

  template<class T>
  static yes& test(typename T::template apply<void>*);
  template<class T>
  static no& test(...);

  static const bool value = sizeof(test<F>(nullptr)) == sizeof(yes);
};

template<class F>
struct has_apply2 {
  using yes = char[1];
  using no = char[2];

  template<class T>
  static yes& test(typename T::template apply<void, void>*);
  template<class T>
  static no& test(...);

  static const bool value = sizeof(test<F>(nullptr)) == sizeof(yes);
};

namespace {
  struct _has_apply_test {
    template<class>
    using apply = int;
  };
  struct _has_apply_test2 {
    template<class, class>
    using apply = int;
  };

  static_assert(has_apply1<has_apply1<void>>::value == false, "has_apply1<void> shouldn't have apply<A>");

  static_assert(has_apply1<_has_apply_test>::value, "has_apply1<_has_apply_test> should have apply<A>");
  static_assert(has_apply1<_has_apply_test2>::value == false, "has_apply1<_has_apply_test2> shouldn't have apply<A>");

  static_assert(has_apply2<_has_apply_test>::value == false, "has_apply1<_has_apply_test> shouldn't have apply<A,B>");
  static_assert(has_apply2<_has_apply_test2>::value, "has_apply1<_has_apply_test2> should have apply<A,B>");
}

#endif