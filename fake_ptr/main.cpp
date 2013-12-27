#include "fake_ptr.h"
#include <memory>
#include <cassert>

int main() {
    auto i1 = make_fake<int>(1);
    auto i2 = std::make_shared<int>(1);

    *i2 = 1;
    *i1 = *i2;
    assert(*i1 == 1);
    *i1 = 5;
    *i2 = *i1;
    assert(*i2 == 5);
}