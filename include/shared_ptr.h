#ifndef SHARED_PTR_H_SENTRY
#define SHARED_PTR_H_SENTRY

#include <cstdlib>
#include <utility>

template<class T>
class SharedPtr;

template<class T>
class ControlBlock {
    friend SharedPtr<T>;
    size_t refs_counter;
    T *ptr;
    void add_ref() { refs_counter++; }
    ControlBlock(const ControlBlock &other); //banned
    //default constructor
    ControlBlock() : ptr(0), refs_counter(0) {}
    //constructor for make_shared
    ControlBlock(T *in_ptr) : ptr(std::move(in_ptr)), refs_counter(1) {}
    //for following constructor T must have copy constructor
    ControlBlock(T &obj) : ptr(new T(std::move(obj))), refs_counter(1) {}
    ~ControlBlock() {}
    size_t getRefsCount() { return refs_counter; }
    void release();

    //he is really friendly
    template <class U>
    friend SharedPtr<U> make_shared(const U& obj);
    template <class U>
    friend SharedPtr<U> make_shared(const SharedPtr<U>& other);
    template <class U>
    friend SharedPtr<U> make_shared(U *ptr);
};

template <class T>
class SharedPtr {
    ControlBlock<T> *cb_ptr;

    //private constructor for make_shared
    SharedPtr(ControlBlock<T> *in_cb_ptr) : cb_ptr(in_cb_ptr) {}

    void add_ref() { if( cb_ptr) cb_ptr->add_ref(); }
public:
    SharedPtr() : cb_ptr(0) {}

    SharedPtr(const SharedPtr<T> &other);

    // for following constructor T must have copy constructor
    SharedPtr(T &obj) : cb_ptr(new ControlBlock<T>(obj)) {} // std::move in new?

    ~SharedPtr() { if (cb_ptr) cb_ptr->release(); }

    void release() { if (cb_ptr) cb_ptr->release(); cb_ptr = 0; }

    size_t getRefsCount() const { return cb_ptr ? cb_ptr->getRefsCount() : 0; }

    T* get() const {return cb_ptr ? cb_ptr->ptr : 0; }

    SharedPtr<T>& operator=(const SharedPtr<T> &other);

    T* operator->() { return cb_ptr->ptr; }
    T& operator*() { return *(cb_ptr->ptr); }
    const T* operator->() const { return cb_ptr->ptr; }
    const T& operator*() const { return *(cb_ptr->ptr); }

    template <class U>
    friend SharedPtr<U> make_shared(const U& obj);
    template <class U>
    friend SharedPtr<U> make_shared(const SharedPtr<U>& other);
    template <class U>
    friend SharedPtr<U> make_shared(U *ptr);
};

// T must have copy constructor
template <class T>
SharedPtr<T> make_shared(const T& obj) {
    auto p = new ControlBlock<T>(new T(std::move(obj)));// std::move in new?
    return SharedPtr<T>(p);
}

template <class T>
SharedPtr<T> make_shared(const SharedPtr<T>& other) {
    return SharedPtr<T>(other);
}

//following func assumes the pointer came from heap
// if it is not, it will be sad
template <class T>
SharedPtr<T> make_shared(T *ptr) {
    auto p = new ControlBlock<T>(ptr);
    return SharedPtr<T>(p);
}

//will be soon
//template <class T, class... Args>
//SharedPtr<T> make_share(Args&&... args) {
//    auto p = ne
//}


#endif // SHARED_PTR_H_SENTRY