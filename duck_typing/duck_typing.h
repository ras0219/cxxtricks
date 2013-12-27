#pragma once

#include <typeinfo>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <tuple>
#include <locale>
#include <codecvt>

template<class T>
std::wstring to_wstring(const T& t)
{
    std::wstringstream ss;
    ss << t;
    return ss.str();
}

inline std::wstring to_wstring(const std::string& t)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(t);
}

struct var;

struct FnTag_t {
    operator FnTag_t*() { return this; }
};
using FnTag = FnTag_t*;

template<class T>
struct FnTagTable
{
    static std::unordered_map<FnTag, std::tuple<size_t, var(*)()>> table;

    static void add_function(FnTag tag, var(*func)(T&)) {
        table.emplace(tag, std::make_pair(0, reinterpret_cast<var(*)()>(func)));
    }
    static void add_function(FnTag tag, var(*func)(T&, var)) {
        table.emplace(tag, std::make_pair(1, reinterpret_cast<var(*)()>(func)));
    }
    static void add_function(FnTag tag, var(*func)(T&, var, var)) {
        table.emplace(tag, std::make_pair(2, reinterpret_cast<var(*)()>(func)));
    }
    static void add_function(FnTag tag, var(*func)(T&, var, var, var)) {
        table.emplace(tag, std::make_pair(3, reinterpret_cast<var(*)()>(func)));
    }
};

template<class T>
std::unordered_map<FnTag, std::tuple<size_t, var(*)()>> FnTagTable<T>::table;


struct Object
{
    virtual ~Object() {}

    virtual var send(FnTag f) = 0;
    virtual var send(FnTag f, var) = 0;
    virtual var send(FnTag f, var, var) = 0;
    virtual std::wstring toString() const = 0;
};

template<class T>
struct ObjectWrapper;

struct var
{
    var(var&& v) : obj(std::move(v.obj)) {}
    var(const var& v) : obj(v.obj) {}
    var(const char* t);
    var(const wchar_t* t);

    template<class T>
    var(std::shared_ptr<T> ptr) : obj(ptr) {}

    template<class T>
    var(T&& t) : obj(std::make_shared<ObjectWrapper<T>>(std::forward<T>(t))) {
        static_assert(!std::is_same<var, typename std::decay<T>::type>::value, "Cannot wrap var");
    }

    template<class...Args>
    var operator()(FnTag fntag, Args...args)
    {
        return 5;
    }

    std::wstring toString() const {
        return obj->toString();
    }

private:
    std::shared_ptr<Object> obj;
};

extern const var nil;

template<class T>
struct ObjectWrapper : Object
{
    template<class T>
    ObjectWrapper(T&& t) : t(std::forward<T>(t)) {
        static_assert(!std::is_same<var, typename std::decay<T>::type>::value, "Cannot wrap var type");
    }

    virtual var send(FnTag f)
    {
        auto it = FnTagTable<T>::table.find(f);
        if (it == FnTagTable<T>::table.end())
            throw std::runtime_error("Function not found");

        switch (std::get<0>(it->second))
        {
        case 0: return reinterpret_cast<var(*const)(T&)>(std::get<1>(it->second))(t);
        case 1: return reinterpret_cast<var(*const)(T&, var)>(std::get<1>(it->second))(t, nil);
        case 2: return reinterpret_cast<var(*const)(T&, var, var)>(std::get<1>(it->second))(t, nil, nil);
        case 3: return reinterpret_cast<var(*const)(T&, var, var, var)>(std::get<1>(it->second))(t, nil, nil, nil);
        default:
            throw std::runtime_error("Too many function arguments");
        }
    }
    virtual var send(FnTag f, var) { return nil; }
    virtual var send(FnTag f, var, var) { return nil; }

    virtual std::wstring toString() const override
    {
        return to_wstring(t);
    }

private:
    T t;
};

#include <iostream>

inline std::wostream& operator<<(std::wostream& os, const var& v)
{
    return os << v.toString();
}