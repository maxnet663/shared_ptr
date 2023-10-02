#ifndef SHARED_PTR_H_SENTRY
#define SHARED_PTR_H_SENTRY

#include <cstdlib>
#include <utility>

template<class T>
class SharedPtr;

template<class T>
class ControlBlock {

    /**
     * open the class for SharedPtr
     * friend is a dangerous keyword,
     * but in the context of a smart pointer
     * it would be appropriate
     */
    friend SharedPtr<T>;

    size_t refs_counter;
    T *ptr;

    //constructor for make_shared
    explicit ControlBlock(T *_ptr) noexcept
    : ptr(_ptr), refs_counter(1) {}

    ~ControlBlock() = default;

    size_t getRefsCount() { return refs_counter; }

    void release();

    void add_ref() { refs_counter++; }

    template <class U, class... Args>
    friend SharedPtr<U> make_shared(Args&&... args);

public:
    ControlBlock() = delete;
    ControlBlock(const ControlBlock &other) = delete;
    ControlBlock<T>& operator=(const ControlBlock<T> &other) = delete;
};

template <class T>
class SharedPtr {

    ControlBlock<T> *ptr_cb;

    //private constructor for make_shared
    explicit SharedPtr(ControlBlock<T> *_ptr_cb)
    : ptr_cb(std::move(_ptr_cb)) {}

    void add_ref() { if (ptr_cb) ptr_cb->add_ref(); }

public:

    SharedPtr() : ptr_cb(nullptr) {}

    SharedPtr(const SharedPtr<T> &other);

    SharedPtr(SharedPtr&& other)  noexcept
    : ptr_cb(std::move(other.ptr_cb)) {}

    SharedPtr<T>& operator=(const SharedPtr<T> &right);

    SharedPtr<T>& operator=(SharedPtr<T>&& right) noexcept;

    ~SharedPtr() { if (ptr_cb) ptr_cb->release(); }

    void release()
    { if (ptr_cb) { ptr_cb->release(); ptr_cb = nullptr; } }

    size_t getRefsCount() const
    { return ptr_cb ? ptr_cb->getRefsCount() : 0; }

    T* get() const {return ptr_cb ? ptr_cb->ptr : nullptr; }

    T* operator->();
    T& operator*();
    const T* operator->() const;
    const T& operator*() const;

    template <class U, class... Args>
    friend SharedPtr<U> make_shared(Args&&... args);
};

template <class T>
void ControlBlock<T>::release() {
    if (!--refs_counter) {
        delete ptr;
        delete this;
    }
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &other) {
    ptr_cb = other.ptr_cb;
    add_ref();
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T> &right) {
    //if this and right are the same or point to the same object
    if (&right == this || ptr_cb == right.ptr_cb)
        return *this;
    release(); // we have to release current ptr_cb before grab new
    ptr_cb = right.ptr_cb;
    add_ref();
    return *this;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& right) noexcept {
    std::swap(ptr_cb, right.ptr_cb);
    return *this;
}

template <class T>
T* SharedPtr<T>::operator->() {
    if (ptr_cb)
        return ptr_cb->ptr;
    else
        return nullptr;
}

template<class T>
T& SharedPtr<T>::operator*() {
    if (ptr_cb)
        return *(ptr_cb->ptr);
    else
        throw std::logic_error("Dereference of nullptr");
}

template <class T>
const T* SharedPtr<T>::operator->() const {
    if (ptr_cb)
        return ptr_cb->ptr;
    else
        return nullptr;
}

template<class T>
const T& SharedPtr<T>::operator*() const {
    if (ptr_cb)
        return *(ptr_cb->ptr);
    else
        throw std::logic_error("Dereference of nullptr");
}


// todo how to avoid two new operators?
template <class T, class... Args>
SharedPtr<T> make_shared(Args&&... args) {
    auto ptr_value = new T(std::forward<Args>(args)...);
    auto ptr_cb = new ControlBlock<T>(ptr_value);
    return SharedPtr<T>(ptr_cb);
}


#endif // SHARED_PTR_H_SENTRY
