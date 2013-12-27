#pragma once

#include "fake_ptr.h"

template<class Ty, bool STACK_ALLOCATED>
struct read_ptr_impl {
    Ty t;
};

template<class Ty>
struct read_ptr_impl<Ty, false> {
    
};

template<class Ty>
using read_ptr = read_ptr_impl<Ty, sizeof(Ty) <= sizeof(void*)>;
