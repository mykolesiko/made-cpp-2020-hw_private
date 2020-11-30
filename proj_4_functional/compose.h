#ifndef COMPOSE_H
#define COMPOSE_H

#include <iostream>
#include <memory>
#include <functional>
#include  <typeinfo>
#include <tuple>
#include <string>
#include "function.h"

template <typename T>
class argument_type_checker
{
        typedef char true_type;
        typedef long false_type;
        template <typename U> static true_type check(typename U::argument_type*) {};
        template <typename U> static false_type  check(...) {};
    public:
        static constexpr bool value = sizeof(check<T>(0)) == sizeof(true_type);
};

template<typename ... Fs>
struct compose_obj
{
    compose_obj(Fs&& ... fs) : ftuple(std::forward_as_tuple(fs ...)) {}
    template<std::size_t> struct counter{};
    template<size_t N, typename ...Ts>
    auto invoke(counter<N>, Ts&&... ts)
    {
        auto return_value = invoke(counter<N+1>(),std::forward<Ts>(ts)...);
        auto f = std::get<N>(ftuple);
        constexpr bool value = argument_type_checker<decltype(f)>::value;

        if constexpr(value == true) {
            static_assert(std::is_same<decltype(return_value), typename decltype(f)::argument_type>::value, "types are not the same in for arguments function");
        }
        else {
            static_assert(std::is_same<decltype(return_value), typename function<typename std::remove_pointer<decltype(f)>::type>::argument_type>::value, "types are not the same for ordinary function");
        }
        auto return_value_new = f(return_value);
        return (return_value_new);
    }
    static const size_t size = sizeof ... (Fs);
    template<typename ...Ts>
    auto invoke(counter<size-1>, Ts&& ...ts) {
        return std::get<size-1>(ftuple)(std::forward<Ts>(ts)...);
    }
    template<typename ...Ts>
    auto operator()(Ts&&... ts) {
       return invoke(counter<0>(), std::forward<Ts>(ts)...);
    }
    std::tuple<Fs ...> ftuple;
};


template<typename ... Fs>
auto compose(Fs&& ... fs){
    return compose_obj<Fs ...>(std::forward<Fs>(fs) ...);
}


#endif // COMPOSE_H
