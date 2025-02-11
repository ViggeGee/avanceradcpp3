#pragma once
#include <cstddef> // For size_t

template <typename T>
class SharedPtr
{
public:
       
    SharedPtr() : ptr_(nullptr), count_(nullptr) {}

    explicit SharedPtr(T* ptr) : ptr_(ptr), count_(ptr ? new size_t(1) : nullptr) {}

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), count_(other.count_) {
        if (count_) ++(*count_);
    }

    // Move constructor
    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_), count_(other.count_) {
        other.ptr_ = nullptr;
        other.count_ = nullptr;
    }

    ~SharedPtr() 
    {
        ptr_ = nullptr;
        count_ = 0;
            delete ptr_;
            delete count_;
    }

    T* get()
    {
        return ptr_;
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    // Conversion to bool: Returns true if ptr_ is not null
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }
	
    bool Invariant() const noexcept {
        if (ptr_ == nullptr) {
            return count_ == nullptr; // count_ must be nullptr when ptr_ is nullptr
        }
        return count_ && *count_ > 0; // count_ must be non-null and positive when ptr_ is not null
    }

    size_t use_count() const noexcept
    {
        if (count_ != nullptr)
            return *count_;
        if (count_ == nullptr)
            return 0;
    }

    


   

private:
    T* ptr_;
    size_t* count_;
};
