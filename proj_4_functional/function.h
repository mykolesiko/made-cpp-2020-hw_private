#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <memory>
#include <functional>
#include  <typeinfo>
#include <tuple>
#include <string>



template <typename UnusedType>
class function;

class NoneType {};

template <typename... Args>
struct ArgumentTypes
{
    using Head = NoneType;
    using Tail = NoneType;
};


template <typename H, typename ...Args>
struct ArgumentTypes<H, Args...>
{
    using Head = H;
    using Tail = ArgumentTypes<Args...>;
    using argument_type  = Head;
};

template <>
struct ArgumentTypes<> {
    using argument_type = void;
};


typedef struct ArgumentTypes<> EmptyArgumentTypes;


template<typename H>
struct ArgumentTypes<H>{
    using Head = H;
    using Tail = EmptyArgumentTypes;
    using argument_type  = Head;
};



template <typename R, typename ... Args>
class function <R (Args ...)>
{
public:

    typedef R signature_type (Args ...);
    //using argument_type =  Args ...;
    static const size_t nargs = sizeof...(Args);
    typedef typename ArgumentTypes<Args...>::argument_type argument_type;

    function() noexcept : handler(nullptr) {}

    template <typename F>
    function(F f): handler(new function_general<F>(f))  {}

    //конструктор для функции члена класса
    template <typename F, typename ClassType>
    function(F ClassType::* f) : handler(new function_member<F, Args ...>(f))  {}

    //конструктор копирования
    function(const function & other): handler(other.handler->copy())  {}

    //operator() invokes the target
    R operator ()(Args ... args) const {
        return handler->invoke(args ...);
    }

    void swap( function& other ) noexcept {
        function_base* temp = handler ;
        handler = other.handler;
        other.handler = temp;
    }

    function &operator=(const function &other)  {
       if (this != &other) {
              handler = other.handler->copy();
       }
       return *this;
    };

    function &operator=(function &&other) {
       if (this == &other) {
               return (*this);
       }
       delete handler;
       handler = other.handler;
       other.handler = nullptr;
       return *this;
    };

     ~function() {
            delete handler;
     };

      explicit operator bool() const noexcept {
            return handler != nullptr;

      }

private:

    class function_base {
      public:

        function_base() {}
        virtual ~function_base() {}
        virtual R invoke(Args ... args) = 0;
        virtual function_base* copy() = 0;

      private:
        function_base(const function_base & );
        void operator = (const function_base &);
    };

    template <typename F>
    class function_general : public function_base
    {
      public:

        function_general(F func): function_base(), func(func)  {}

        virtual R invoke(Args ... args) {
            return func(args ...);
        }

        virtual function_base* copy()  {
            return reinterpret_cast<function_base*>(new function_general(func));
        }

      private:
        F func;
    };


    template <typename F, typename ClassType, typename ... MoreArgs>
    class function_member : public function_base
    {
    public:

        typedef F ClassType::* function_member_sign;

        function_member(function_member_sign f) : func(f) {}

        virtual R invoke(ClassType obj, MoreArgs ... moreArgs) {
            return (obj.*func)(moreArgs ...);
        }

        virtual function_base* copy() {
            return reinterpret_cast<function_base*>(new function_member(func));
        }

    private:
        function_member_sign func;
    };

    function_base* handler;
};





#endif // FUNCTION_H
