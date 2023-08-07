#ifndef SHARED_PTR_DEF_H_SENTRY
#define SHARED_PTR_DEF_H_SENTRY
#include "shared_ptr.h"

template <class T>
void ControlBlock<T>::release() {
    if (!--refs_counter) {
        delete ptr;
        delete this;
    }
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &other) {
    cb_ptr = other.cb_ptr;
    add_ref();
}

// todo maybe I should define the assignment operator for ControlBlock
// to keep track of the amount of references
// todo what if they point to the same object?
template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T> &right) {
    if (&right == this)
        return *this;
    release(); // we have to release current cb_ptr before grab new
    cb_ptr = right.cb_ptr;
    add_ref();
    return *this;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& right) noexcept {
    cb_ptr = std::move(right.cb_ptr);
    return *this;

}

#endif //SHARED_PTR_DEF_H_SENTRY
