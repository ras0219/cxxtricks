#pragma once

#include "mt_expr.hpp"
#include "mt_type.hpp"
#include "tcheck_structs.hpp"

#include <iostream>

//// Prettyprinting of types
template<long long N>
std::ostream& operator<<(std::ostream& os, rasmeta::tvar<N>) {
  return os << 't' << N;
}

template<class T1, class T2>
std::ostream& operator<<(std::ostream& os, rasmeta::arr<T1, T2>) {
  return os << T1() << " -> " << T2();
}

template<class T1a, class T1b, class T2>
std::ostream& operator<<(std::ostream& os, rasmeta::arr<rasmeta::arr<T1a, T1b>, T2>) {
  return os << '(' << rasmeta::arr<T1a, T1b>() << ") -> " << T2();
}

template<class T>
std::ostream& operator<<(std::ostream& os, rasmeta::forall<T>) {
  return os /*<< "\\. "*/ << T();
}

template<long long N>
std::ostream& operator<<(std::ostream& os, rasmeta::tref<N>) {
  if (N >= 0 && N < 26)
    return os << (char)('a' + N);
  return os << 'r' << N;
}

std::ostream& operator<<(std::ostream& os, rasmeta::concrete_type<int>) {
  return os << "int";
}

std::ostream& operator<<(std::ostream& os, rasmeta::concrete_type<bool>) {
  return os << "bool";
}

template<class T, T N>
std::ostream& operator<<(std::ostream& os, rasmeta::constant_c<T, N>) {
  return os << N;
}
//// Prettyprinting of asts
template<class E>
std::ostream& operator<<(std::ostream& os, rasmeta::lambda<rasmeta::lambda<E>>) {
  return os << "\\." << rasmeta::lambda<E>();
}
template<class E>
std::ostream& operator<<(std::ostream& os, rasmeta::lambda<E>) {
  return os << "\\. " << E();
}
template<class E1, class E2>
std::ostream& operator<<(std::ostream& os, rasmeta::app<E1, E2>) {
  return os << E1() << " " << E2();
}
template<class E1, class E2>
std::ostream& operator<<(std::ostream& os, rasmeta::app<rasmeta::lambda<E1>, E2>) {
  return os << "(" << rasmeta::lambda<E1>() << ") " << E2();
}
template<class E1, class E2, class E3>
std::ostream& operator<<(std::ostream& os, rasmeta::app<E1, rasmeta::app<E2, E3>>) {
  return os << E1() << " (" << rasmeta::app<E2, E3>() << ")";
}
template<class E1, class E2, class E3>
std::ostream& operator<<(std::ostream& os, rasmeta::app<rasmeta::lambda<E1>, rasmeta::app<E2, E3>>) {
  return os << "(" << rasmeta::lambda<E1>() << ") " << "(" << rasmeta::app<E2, E3>() << ")";
}
template<long long N>
std::ostream& operator<<(std::ostream& os, rasmeta::ref<N>) {
  return os << 'r' << N;
}
template<class E>
std::ostream& operator<<(std::ostream& os, rasmeta::compile<E>) {
  return os << "<compiled>";
}
//// Super prettyprinting of asts
template<class LamStack, char C, class E>
void prettyprint1_lamdamode(std::ostream& os, rasmeta::lambda<E>) {
  os << C << ' ';
  prettyprint1_lambdamode<rasmeta::tcheck::push<LamStack, constant_c<char, C>>, C + 1>(os, E());
}
template<class LamStack, char C, class E>
void prettyprint1_lamdamode(std::ostream& os, E) {
  os << "-> ";
  prettyprint1<LamStack, C>(os, E());
}

template<class LamStack, char C, class E>
void prettyprint1(std::ostream& os, rasmeta::lambda<E>) {
  os << '\\';
  prettyprint1_lamdamode<LamStack, C>(os, rasmeta::lambda<E>());
}
template<class LamStack, char C, class E1, class E2>
void prettyprint1(std::ostream& os, rasmeta::app<E1, E2>) {
  prettyprint1<LamStack, C>(os, E1());
  os << ' ';
  prettyprint1<LamStack, C>(os, E2());
}
template<class LamStack, char C, class E1, class E2>
void prettyprint1(std::ostream& os, rasmeta::app<rasmeta::lambda<E1>, E2>) {
  os << '(';
  prettyprint1<LamStack, C>(os, rasmeta::lambda<E1>());
  os << ") ";
  prettyprint1<LamStack, C>(os, E2());
}
template<class LamStack, char C, class E1, class E2, class E3>
void prettyprint1(std::ostream& os, rasmeta::app<E1, rasmeta::app<E2, E3>>) {
  prettyprint1<LamStack, C>(os, E1());
  os << " (";
  prettyprint1<LamStack, C>(os, rasmeta::app<E2, E3>());
  os << ")";
}
template<class LamStack, char C, class E1, class E2, class E3>
void prettyprint1(std::ostream& os, rasmeta::app<rasmeta::lambda<E1>, rasmeta::app<E2, E3>>) {
  os << '(';
  prettyprint1<LamStack, C>(os, rasmeta::lambda<E1>());
  os << ") (";
  prettyprint1<LamStack, C>(os, rasmeta::app<E2, E3>());
  os << ")";
}
template<class LamStack, char C, class E1, class E2>
void prettyprint1(std::ostream& os, rasmeta::app<rasmeta::lambda<E1>, rasmeta::lambda<E2>>) {
  os << '(';
  prettyprint1<LamStack, C>(os, rasmeta::lambda<E1>());
  os << ") (";
  prettyprint1<LamStack, C>(os, rasmeta::lambda<E2>());
  os << ")";
}
template<class LamStack, char C, class E1, class E2>
void prettyprint1(std::ostream& os, rasmeta::app<E1, rasmeta::lambda<E2>>) {
  prettyprint1<LamStack, C>(os, E1());
  os << " (";
  prettyprint1<LamStack, C>(os, rasmeta::lambda<E2>());
  os << ")";
}

template<class LamStack, char C, long long N>
void prettyprint1(std::ostream& os, rasmeta::ref<N>) {
  os << rasmeta::tcheck::at<LamStack, N>::value;
}
template<class LamStack, char C, class E>
void prettyprint1(std::ostream& os, rasmeta::compile<E>) {
  os << "<compiled>";
}
template<class LamStack, char C>
void prettyprint1(std::ostream& os, rasmeta::bool_c<true>) {
  os << "True";
}
template<class LamStack, char C>
void prettyprint1(std::ostream& os, rasmeta::bool_c<false>) {
  os << "False";
}
template<class LamStack, char C, class T, T t>
void prettyprint1(std::ostream& os, rasmeta::constant_c<T, t>) {
  os << t;
}

template<class E>
void prettyprint(std::ostream& os, E) {
  prettyprint1<rasmeta::tcheck::variadic<>, 'a'>(os, E());
}

//// Ultra-awesome prettyprinting
template<class LamStack, char C, class E>
void ultra_pp1(std::ostream& os, rasmeta::lambda<E>) {
  os << C << ' ';
  ultra_pp1<rasmeta::tcheck::push<LamStack, constant_c<char, C>>, C + 1>(os, E());
}
template<class LamStack, char C, class E>
void ultra_pp1(std::ostream& os, E) {
  os << "= ";
  prettyprint1<LamStack, C>(os, E());
}

template<class E>
void ultra_pp(std::ostream& os, E) {
  ultra_pp1<rasmeta::tcheck::variadic<>, 'a'>(os, E());
}
