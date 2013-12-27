#include "duck_typing.h"
#include <vector>

struct ObjectNil : Object
{
    virtual var send(FnTag f);
    virtual var send(FnTag f, var);
    virtual var send(FnTag f, var, var);

    virtual std::wstring toString() const override
    {
        return L"nil";
    }
};

const var nil = std::make_shared<ObjectNil>();

var ObjectNil::send(FnTag tag) {
    throw std::runtime_error("Called function on nil");
}
var ObjectNil::send(FnTag tag, var) {
    throw std::runtime_error("Called function on nil");
}
var ObjectNil::send(FnTag tag, var, var) {
    throw std::runtime_error("Called function on nil");
}

var::var(const char* t) : obj(std::make_shared<ObjectWrapper<std::string>>(t)) {}
var::var(const wchar_t* t) : obj(std::make_shared<ObjectWrapper<std::wstring>>(t)) {}