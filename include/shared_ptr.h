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
public:
    ControlBlock(const ControlBlock &other) = delete;
    //default constructor
    ControlBlock() : ptr(0), refs_counter(0) {}
    //constructor for make_shared
    ControlBlock(T *in_ptr) : ptr(in_ptr), refs_counter(1) {}
    ControlBlock(T &obj) : ptr(new T(obj)), refs_counter(1) {}
    ~ControlBlock() {}
    size_t getRefsCount() { return refs_counter; }
    void release();

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
    SharedPtr(T &obj) : cb_ptr(new ControlBlock<T>(obj)) {}
    ~SharedPtr() { if (cb_ptr) cb_ptr->release(); }

    void release() { if (cb_ptr) cb_ptr->release(); cb_ptr = 0; }
    size_t getRefsCount() { return cb_ptr ? cb_ptr->getRefsCount() : 0; }
    SharedPtr<T>& operator=(const SharedPtr<T> &other);
    const T* operator->() { return cb_ptr->ptr; }
    const T& operator*() { return *(cb_ptr->ptr); }
    template<class U, class... Args>
    friend SharedPtr<U> make_shared(Args&&... args);
};

template<class U, class... Args>
SharedPtr<U> make_shared(Args&&... args) {
    auto p = new ControlBlock<U>(new U(std::forward<Args>(args)...));
    return SharedPtr<U>(p);
}

template<class T>
SharedPtr<T> make_shared(const T& obj) {
    return SharedPtr<T>(&obj);
}

template<class T>
SharedPtr<T> make_shared(const SharedPtr<T>& other) {
    return SharedPtr<T>(other);
}

#endif // SHARED_PTR_H_SENTRY