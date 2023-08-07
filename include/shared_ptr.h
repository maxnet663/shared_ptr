#ifndef SHARED_PTR_H_SENTRY
#define SHARED_PTR_H_SENTRY

#include <cstdlib>
#include <utility>

template<class T>
class SharedPtr;

template<class T>
class ControlBlock {

    // open the class for SharedPtr
    friend SharedPtr<T>;
    size_t refs_counter;
    T *ptr;

    ControlBlock(const ControlBlock &other); //banned

    //constructor for make_shared
    ControlBlock(T *in_ptr) : ptr(std::move(in_ptr)), refs_counter(1) {}

    ~ControlBlock() {}

    size_t getRefsCount() { return refs_counter; }
    void release();
    void add_ref() { refs_counter++; }

    template <class U, class... Args>
    friend SharedPtr<U> make_shared(Args&&... args);
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

    //move constructor
    SharedPtr(SharedPtr&& other)  noexcept : cb_ptr(std::move(other.cb_ptr)) {}

    ~SharedPtr() { if (cb_ptr) cb_ptr->release(); }

    void release() { if (cb_ptr) cb_ptr->release(); cb_ptr = 0; }

    size_t getRefsCount() const { return cb_ptr ? cb_ptr->getRefsCount() : 0; }

    T* get() const {return cb_ptr ? cb_ptr->ptr : 0; }

    SharedPtr<T>& operator=(const SharedPtr<T> &right);

    //move assignment operator
    SharedPtr<T>& operator=(SharedPtr<T>&& right) noexcept;

    T* operator->() { return cb_ptr->ptr; }
    T& operator*() { return *(cb_ptr->ptr); }
    const T* operator->() const { return cb_ptr->ptr; }
    const T& operator*() const { return *(cb_ptr->ptr); }

    template <class U, class... Args>
    friend SharedPtr<U> make_shared(Args&&... args);
};

// todo how to avoid two new operators? Another constructor for ControlBlock?
template <class T, class... Args>
SharedPtr<T> make_shared(Args&&... args) {
    auto p_t = new T(std::forward<Args>(args)...);
    auto p_cb = new ControlBlock<T>(p_t);
    return SharedPtr<T>(p_cb);
}


#endif // SHARED_PTR_H_SENTRY