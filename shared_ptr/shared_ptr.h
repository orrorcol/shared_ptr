#include <atomic>

namespace lfs {

struct RefCount {
    RefCount() : count(1) {}

    std::atomic<int> count;
};

template <typename T>
class SharedPtr {
public:
    SharedPtr(T* ptr = nullptr);
    SharedPtr(const SharedPtr& ptr);

    ~SharedPtr();
    
    T& operator*() const;
    T* operator->() const;

    SharedPtr& operator=(const SharedPtr& ptr);

    int Count() const;

private:
    void Ref() const;
    void Deref() const;

    T* _ptr;
    RefCount* _ref;
};

} // namespace lfs

#include "shared_ptr.hpp"
