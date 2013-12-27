#pragma once
// This line is to improve intellisense on supported IDE's
#include "fake_ptr.h"

// Implementation for make_fake_ptr
template<class Ty, class...Args>
fake_ptr<Ty> make_fake(Args&&...args)
{
    return fake_ptr<Ty>(std::forward<Args>(args)...);
}

// Implementations for fake_ptr<T>
template<class Ty>
template<class Y>
fake_ptr<Ty>::fake_ptr(fake_ptr<Y> const& fp) : element(fp.element) {}
template<class Ty>
template<class Y>
fake_ptr<Ty>::fake_ptr(fake_ptr<Y>&& fp) : element(std::move(fp.element)) {}
template<class Ty>
template<class...Args>
fake_ptr<Ty>::fake_ptr(Args&&...args) : element(std::forward<Args>(args)...) {}

template<class Ty>
template<class Y>
fake_ptr<Ty>& fake_ptr<Ty>::operator=(const fake_ptr<Y>& r)
{
    element = r.element;
    return *this;
}

template<class Ty>
template<class Y>
fake_ptr<Ty>& fake_ptr<Ty>::operator=(fake_ptr<Y>&& r)
{
    element = std::move(r.element);
    return *this;
}

template<class Ty>
Ty& fake_ptr<Ty>::operator*() { return element; }
template<class Ty>
Ty const& fake_ptr<Ty>::operator*() const { return element; }

template<class Ty>
Ty* fake_ptr<Ty>::operator->() { return &element; }
template<class Ty>
Ty const* fake_ptr<Ty>::operator->() const { return &element; }

template<class Ty>
Ty* fake_ptr<Ty>::get() { return &element; }
template<class Ty>
Ty const* fake_ptr<Ty>::get() const { return &element; }