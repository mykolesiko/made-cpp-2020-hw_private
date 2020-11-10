#pragma once

#

#include <cstddef>
#include <utility>



namespace task {

template <class T> class WeakPtr;


template<typename T>
class UniquePtr {
private:
    T* ptr = nullptr;

public:
    UniquePtr() noexcept {}

    UniquePtr(T* _ptr) noexcept: ptr(_ptr) {}

    UniquePtr(UniquePtr&& other) noexcept: ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator = (UniquePtr&& other) {
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    ~UniquePtr() {
        delete ptr;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }

    T& operator * () const {
        return (*ptr);
    }

    T* operator -> () const noexcept {
        return ptr;
    }

    T* release() noexcept {
        T* _ptr = ptr;
        ptr = nullptr;
        return _ptr;
    }

    void reset(T* _ptr) {
        delete ptr;
        ptr = _ptr;
    }

    T* get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return (ptr != nullptr);
    }

    UniquePtr(const UniquePtr& other) = delete;

    UniquePtr& operator = (const UniquePtr &other) = delete;
};

struct counter{
    counter(): countShared(1), countWeak(0) {}
    size_t countShared;
    size_t countWeak;
};

template<typename T>
class SharedPtr {
private:
    T* ptr = nullptr;
    counter* refPtr = nullptr;

    void removeRef() {
        if (ptr) {
            if (--(refPtr->countShared) == 0) {
                delete ptr;
                //delete refPtr;
            }
        }
    }

 public:
    SharedPtr() {}

    explicit SharedPtr(T* _ptr): ptr(_ptr) {
        if (ptr) {
            refPtr = new counter();
        }
    }

    SharedPtr(const SharedPtr& other): ptr(other.ptr), refPtr(other.refPtr) {
        if (ptr) {
            ++(refPtr->countShared);
        }
    }

    SharedPtr(SharedPtr&& other) {
        swap(other);
    }

    SharedPtr(const WeakPtr<T>& p) {
        ptr = p.get();
        refPtr = p.get_ref();
        if (refPtr) {
            (refPtr->countShared)++;
        }
    }

    long use_count() const noexcept {
        if (refPtr == nullptr) {
            return (0);
        } else {
            return(refPtr->countShared);
        }
    }

    void swap(SharedPtr& other) {
        std::swap(ptr, other.ptr);
        std::swap(refPtr, other.refPtr);
    }

    SharedPtr& operator = (T* _ptr) {
        reset(_ptr);
        return *this;
    }

    SharedPtr& operator = (const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }
        removeRef();
        ptr = other.ptr;
        refPtr = other.refPtr;
        if (ptr) {
            ++(refPtr->countShared);
        }
        return *this;
    }

    SharedPtr& operator = (SharedPtr&& other) {
        if (this == &other) {
            return *this;
        }
        removeRef();
        ptr = other.ptr;
        refPtr = other.refPtr;
        other.ptr = nullptr;
        other.refPtr = nullptr;
        return *this;
    }

    ~SharedPtr() {
        removeRef();
        if (refPtr != nullptr) {
            if ((refPtr->countShared == 0) && (refPtr->countWeak == 0)) {
                delete(refPtr);
            }
        }
    }

    T& operator * () {
        return *ptr;
    }

    const T& operator * () const {
        return *ptr;
    }

    T* operator -> () const {
        return ptr;
    }

    void reset(T* _ptr) {
        removeRef();
        ptr = _ptr;
        if (ptr) {
            refPtr = new counter();
        } else {
            refPtr = nullptr;
        }
    }

    void reset() noexcept {
        removeRef();
        ptr = nullptr;
        refPtr = nullptr;
    }

    T* get() const {
        return ptr;
    }
    counter* get_ref() const {
        return refPtr;
    }

    explicit operator bool() const {
        return (ptr != nullptr);
    }
};

template <class T>
class WeakPtr{

public:
    WeakPtr(): ptr(nullptr), refPtr(nullptr) {}
    WeakPtr( const SharedPtr<T>& r ) noexcept {
        refPtr = r.get_ref();
        refPtr->countWeak++;
    }
    WeakPtr( WeakPtr&& r ) noexcept {
        refPtr  = r.get_ref();
        r.reset();
    }

    ~WeakPtr() {
        if (refPtr) {
           refPtr->countWeak--;
           if ((refPtr->countShared == 0) && (refPtr->countWeak == 0)) {
               delete(refPtr);
           }
        }
    }

    T* ptr = nullptr;
    counter* refPtr = nullptr;

    counter* get_ref() const {
        return refPtr;
    }

    T* get() const {
        return ptr;
    }

    WeakPtr& operator=( const SharedPtr<T>& r ) noexcept {
        refPtr = r.get_ref();
        if (refPtr) {
            refPtr->countWeak++;
        }
        return(*this);
    }
    WeakPtr( const WeakPtr<T>& r ) noexcept {
        refPtr = r.get_ref();
        if (refPtr) {
            refPtr->countWeak++;
        }
    }

    WeakPtr& operator=( const WeakPtr<T>& r ) noexcept {
        refPtr = r.get_ref();
        if (refPtr) {
            refPtr->countWeak++;
        }
        return(*this);
    }
    WeakPtr& operator=( WeakPtr&& r ) noexcept {
        refPtr  = r.get_ref();
        r.reset();

    }
    long use_count() const noexcept {
        if (refPtr == nullptr) {
            return(0);
        }
        return (refPtr->countShared);
    }

    bool expired() const noexcept {
        if (refPtr)
        {
            return (refPtr->countShared == 0);
        }
    }

    void reset() noexcept {
        refPtr = nullptr;
    }

    void swap( WeakPtr& r ) noexcept {
        std::swap(r.refPtr, refPtr);
    }
    SharedPtr<T> lock() const noexcept {
        return (expired() ? SharedPtr<T>() : SharedPtr<T>(*this));
    }
};


// Your template function definitions may go here...

}  // namespace task


#include "smart_pointers.tpp"
