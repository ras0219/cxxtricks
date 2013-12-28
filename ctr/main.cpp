#include "ctr.hpp"
#include <vector>
#include <iostream>

using namespace std;

struct storage_visitor {
  template<class...Args>
  void operator()(Storage<Args...>& s) { s.visit(*this); }
};


template<class T>
struct is_port {
  enum { value = false };
};

struct start_cancel_visitor : storage_visitor {
  template<class T>
  enable_if_t<is_port<T>::value> operator()(T& t) {
    t.start_cancel();
  }

  template<class T>
  enable_if_t<!is_port<T>::value> operator()(T&) {}

  //template<class...Args>
  //void operator()(Storage<Args...>& s) { s.visit(*this); }
};

struct little_port {
  little_port(bool) {}

  void start_cancel() { std::cout << "start_cancel()" << std::endl; }
};
template<> struct is_port<little_port> { enum { value = true }; };

struct big_port : Storage<little_port, int, vector<int>, little_port> {
  template<class Tuple>
  big_port(Tuple&& tup) : Storage(std::forward<Tuple>(tup)) {}

  void start_cancel() {
    visit(start_cancel_visitor());
  }
private:
};
template<> struct is_port<big_port> { enum { value = true }; };

struct really_big_port : Storage<big_port, big_port, big_port> {
  template<class Tuple>
  really_big_port(Tuple&& tup) : Storage(std::forward<Tuple>(tup)) {}

  void start_cancel() {
    visit(start_cancel_visitor());
  }
};
template<> struct is_port<really_big_port> { enum { value = true }; };

int main()
{
  big_port bp(make_tuple(true, 1, vector<int>(), false));
  bp.start_cancel();

  cout << "next" << endl;
  really_big_port rbp(make_tuple(make_tuple(true, 1, vector<int>(), false),
    make_tuple(true, 1, vector<int>(), false),
    make_tuple(true, 1, vector<int>(), false)));
  rbp.start_cancel();

  cin.get();
}