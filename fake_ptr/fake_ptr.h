#pragma once

#include <utility>

// The fake_ptr class tries to act like a ptr for purposes of dereference but is actually a stack allocated object.
// This is so that you might (hypothetically) switch between unique_ptr, shared_ptr, or fake_ptr depending on how you want storage to work without changing the using implementation.

template<class Ty>
struct fake_ptr {
    using element_type = Ty;

    // Allow copy construction if underlying type supports it
    template<class Y>
    fake_ptr(fake_ptr<Y> const& fp);
    // Allow move construction if underlying type supports it
    template<class Y>
    fake_ptr(fake_ptr<Y>&& fp);
    // Otherwise, use whatever constructors the underlying type has
    template<class...Args>
    explicit fake_ptr(Args&&...args);

    // Allow assignment if the underlying type supports it
    template<class Y>
    fake_ptr& operator=(const fake_ptr<Y>&);
    template<class Y>
    fake_ptr& operator=(fake_ptr<Y>&&);

    // I'm a pointer, I promise!
    Ty& operator*();
    Ty const& operator*() const;
    // Still a pointer!
    Ty* operator->();
    Ty const* operator->() const;

    // Mimic shared_ptr's get() functionality
    Ty* get();
    Ty const* get() const;

private:
    Ty element;
};

// A clone of make_shared and make_unique
template<class Ty, class...Args>
fake_ptr<Ty> make_fake(Args&&...args);

// Implementations in a separate file -- Let's pretend this is C!
#include "fake_ptr_impl.h"