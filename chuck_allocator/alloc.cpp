#ifndef ALLOC_H
#define ALLOC_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <iostream>

using namespace std;

class Chunk {
public:
    Chunk(const std::size_t size) : next(nullptr), ptr_begin((uint8_t*) this), ptr_tek((uint8_t*) this) {}
    Chunk* next;
    uint8_t* ptr_tek;
    uint8_t* ptr_begin;
};

class Chunks_storage {
 public:
    Chunks_storage(): first(nullptr), end(nullptr), n_chunks(0) {}
    ~Chunks_storage();
 private:
    Chunk* first;
    Chunk* end;
    int n_chunks;
    const int chank_size = 100;
 public:
    uint8_t* allocate(const std::size_t size);
    Chunk* add_new_chunk();

};

Chunk* Chunks_storage :: add_new_chunk() {
    if (!first) {
        end = first = new Chunk(chank_size);
        n_chunks = 1;
    } else {
        Chunk* end_old = end;
        end = new Chunk(chank_size);
        end_old->next = end;
        n_chunks ++;
    }
    return(end);
}

uint8_t* Chunks_storage :: allocate(const std::size_t size) {

    if (size > chank_size) {
        //!!!
    }
    Chunk* chunk_cur = first;
    if (first == nullptr)
       chunk_cur =  add_new_chunk();
    while (chunk_cur != nullptr) {
        if ((chank_size - (chunk_cur->ptr_tek - chunk_cur->ptr_begin)) > size) {
            uint8_t* ptr_tek_old = chunk_cur->ptr_tek;
            chunk_cur->ptr_tek += size;
            return ptr_tek_old;
        }
    }
    chunk_cur = add_new_chunk();
    return chunk_cur->ptr_tek;
}

template< class T >
struct allocator_my {
    using value_type = T;
    using pointer =  T*;
    using const_pointer  = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    Chunks_storage* chunks;
    static int  ref_counts;

    //rebind (deprecated in C++17)(removed in C++20)	template< class U > struct rebind { typedef allocator<U> other; };




 public:
    //creates a new allocator instance
    //(public member function)
    allocator_my() : chunks(new Chunks_storage()) { ref_counts = 1; }
    ~allocator_my();


    //(public member function)
    //allocates uninitialized storage
    T* allocate(const size_type n );


    //(public member function)
    //deallocates storage
    void deallocate(T* p, size_t size);

    template<class T, class... Args>
       void construct(T *p, Args &&...args) {
           new ((void *) p) T(std::forward<Args>(args)...);
       }

    //destructs an object in allocated storage
    //(public member function)
    void destroy(T* ptr);



};

template<class T> int allocator_my<T> :: ref_counts = 0;


template< class T > T* allocator_my<T> :: allocate(const size_type size) {
    return reinterpret_cast<T*>(chunks->allocate(size * sizeof(value_type)));
};



template< class T > void allocator_my<T> :: deallocate(T* p, size_t size) {

}

template< class T > void allocator_my<T> :: destroy(T* ptr) {
    ptr->~T();
}


//template< class T, class... Args > void  allocator_my<T> :: construct( T* ptr, Args&&... args ) {
//    return (new (ptr) T(args...));
//}


template< class T > allocator_my<T> :: ~allocator_my() {
    if (ref_counts <= 1) {
        delete chunks;
    }
    ref_counts--;
}


Chunks_storage :: ~Chunks_storage() {
    Chunk* chunk_cur = first;
    while (chunk_cur != nullptr) {
        Chunk* chunk_cur_old = chunk_cur;
        chunk_cur = chunk_cur_old->next;
        delete(chunk_cur_old);
    }

}


#endif // ALLOC_H
