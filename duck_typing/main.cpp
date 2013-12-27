#include "duck_typing.h"

#include <iostream>

FnTag_t increment;

var increment_int(int& i) {
    ++i;
    return nil;
}

int main()
{
    FnTagTable<int>::add_function(increment, increment_int);

    var v = 5;
    var x = "hello, world!";

    std::wcout << x << std::endl;
    std::wcout << v << std::endl;

    x(increment);

    std::wcout << x << std::endl;

    std::wcin.get();
}