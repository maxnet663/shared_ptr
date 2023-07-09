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

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T> &other) {
    if (&other == this)
        return *this;
    release(); // we have to release current cb_ptr before grab new
    cb_ptr = other.cb_ptr;
    add_ref();
    return *this;
}

#endif //SHARED_PTR_DEF_H_SENTRY
