#ifndef ALLOC_H
#define ALLOC_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <iostream>
#include <new>

using namespace std;

class Chunk {
public:
    Chunk(const std::size_t size);
    Chunk* next;
    uint8_t* ptr_tek; //текущий указатель на свободное место
    uint8_t* ptr_begin; //указатель на начало чанка
};

Chunk :: Chunk(const std::size_t size) {
    next = nullptr;
    ptr_begin = static_cast<uint8_t*>(operator new (size));
    ptr_tek = ptr_begin;
}


class Chunks_storage {
 public:
    Chunks_storage(): first(nullptr), end(nullptr), n_chunks(0), ref_counts(1) {}
    ~Chunks_storage();
 private:
    Chunk* first;
    Chunk* end;
    int n_chunks;
    const int chunk_size = 10000;

 public:
    int ref_counts;
    uint8_t* allocate(const std::size_t size);
    Chunk* add_new_chunk();
};

Chunk* Chunks_storage :: add_new_chunk() {
    if (!first) {
        end = first = new Chunk(chunk_size);
        n_chunks = 1;
    } else {
        Chunk* end_old = end;
        end = new Chunk(chunk_size);
        end_old->next = end;
        n_chunks ++;
    }
    return(end);
}

uint8_t* Chunks_storage :: allocate(const std::size_t size) {
    if (size > chunk_size) {
        throw(std::bad_alloc());
    }
    Chunk* chunk_cur = first;
    if (first == nullptr)
       chunk_cur =  add_new_chunk();
    while (chunk_cur != nullptr) {
        if ((chunk_size - (chunk_cur->ptr_tek - chunk_cur->ptr_begin)) > size) {
            uint8_t* ptr_tek_old = chunk_cur->ptr_tek;
            chunk_cur->ptr_tek += size;
            return ptr_tek_old;
        } else {
           chunk_cur =  chunk_cur->next;
        }
    }
    chunk_cur = add_new_chunk();
    return chunk_cur->ptr_tek;
}

template< class T >
class allocator_my {
  public:
    using value_type = T;
    using pointer =  T*;
    using const_pointer  = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    template< class U >
    struct rebind {
        using other =  allocator_my<U>;
        //typedef allocator_my<U> other;
    };
    Chunks_storage* chunks;
 public:
    //creates a new allocator instance
    //(public member function)
    allocator_my() : chunks(new Chunks_storage()) {}
    allocator_my(const allocator_my<T>& copy);
    ~allocator_my();
    allocator_my<T>& operator=(const allocator_my<T>& copy);
    //(public member function)
    //allocates uninitialized storage
    T* allocate(const size_type n );
    //(public member function)
    //deallocates storage
    void deallocate(T* p, size_t size) {};
    template <class... Args>
       void construct(T *p, Args &&...args) {
           new ((void *) p) T(std::forward<Args>(args)...);
       }
    //destructs an object in allocated storage
    //(public member function)
    void destroy(T* ptr);
};

template< class T > T* allocator_my<T> :: allocate(const size_type size) {
    return reinterpret_cast<T*>(chunks->allocate(size * sizeof(value_type)));
};

template< class T > void allocator_my<T> :: destroy(T* ptr) {
    ptr->~T();
}

template< class T > allocator_my<T> :: allocator_my(const allocator_my<T>& copy) {
    chunks = copy.chunks;
    chunks->ref_counts++;
}

template< class T > allocator_my<T>& allocator_my<T> :: operator=(const allocator_my<T>& copy) {
    if (chunks->ref_counts <= 1) {
        chunks->ref_counts--;
        delete chunks;
    } else {
        chunks->ref_counts--;
    }
    chunks = copy.chunks;
    chunks->ref_counts++;
    return (*this);
}

template< class T > allocator_my<T> :: ~allocator_my() {
    chunks->ref_counts--;
    if (chunks->ref_counts == 0) {
        delete chunks;
    }
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
