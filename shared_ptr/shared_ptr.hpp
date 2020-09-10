#include <cassert>

namespace lfs {

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) :
    _ptr(ptr), _ref(new RefCount) {
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& ptr) :
    _ptr(ptr._ptr), _ref(ptr._ref) {
    Ref();
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    Deref();
}

template <typename T>
T& SharedPtr<T>::operator*() const {
    return *_ptr;
}

template <typename T>
T* SharedPtr<T>::operator->() const {
    return _ptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& ptr) {
    Deref();
    _ptr = ptr._ptr;

    _ref = ptr._ref;
    Ref();

    return *this;
}


template <typename T>
int SharedPtr<T>::Count() const {
    return _ref->count.load(std::memory_order_relaxed);
}

template <typename T>
void SharedPtr<T>::Ref() const {
    _ref->count.fetch_add(1, std::memory_order_relaxed);
}

template <typename T>
void SharedPtr<T>::Deref() const {
    int c = _ref->count.fetch_sub(1, std::memory_order_relaxed);
    if (c == 1 && _ptr) {
        delete _ptr;
        delete _ref;
    }
}

} // namespace lfs
