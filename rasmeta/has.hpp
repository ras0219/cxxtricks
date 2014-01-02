#pragma once

#define MAKE_HAS_MEMBER_MACRO_EX(MEMBER, TYPE, SUFFIX) \
  template<class T, class U = TYPE>           \
  struct has_member_ ## MEMBER ## SUFFIX {                           \
    static const bool value = false;                \
  };                                                \
  template<class T>                                 \
  struct has_member_ ## MEMBER ## SUFFIX <T, decltype(T::MEMBER)> {      \
    static const bool value = true;                 \
  };

#define MAKE_HAS_MEMBER_MACRO(MEMBER, TYPE) MAKE_HAS_MEMBER_MACRO_EX(MEMBER, TYPE,)

#define MAKE_HAS_TYPEDEF_MACRO_EX(MEMBER, SUFFIX) \
  template<class T>           \
struct has_typedef_ ## MEMBER ## SUFFIX{ \
  using yes = char[2]; \
  using no = char[1]; \
  template<class C> \
static yes& test(C::MEMBER*); \
  template<class C> \
  static no& test(...); \
  static const bool value = sizeof(test<T>(0)) == sizeof(yes);                \
  };

#define MAKE_HAS_TYPEDEF_MACRO(MEMBER) MAKE_HAS_TYPEDEF_MACRO_EX(MEMBER,)
