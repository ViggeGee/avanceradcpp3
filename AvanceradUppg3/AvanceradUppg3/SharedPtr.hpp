#pragma once
#include <cstddef> // For size_t
#include <utility> // For std::swap

template <typename T>
class SharedPtr
{
public:

	SharedPtr() noexcept : ptr_(nullptr), count_(nullptr) {}

	explicit SharedPtr(T* ptr) : ptr_(ptr), count_(ptr ? new size_t(1) : nullptr) {}

	//Copy constructor
	SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), count_(other.count_)
	{
		if (count_)
			++(*count_);
	}

	// Move constructor
	SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), count_(other.count_)
	{
		other.ptr_ = nullptr;
		other.count_ = nullptr;
	}

	~SharedPtr()
	{
		reset();
	}

	void reset()
	{
		--count_;
		if (*count_ == 0)
		{
			delete count_;
			delete ptr_;
		}
		count_ = nullptr;
		ptr_ = nullptr;
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

	bool operator==(const SharedPtr& other)
	{
		if (get() == other.get())
		{
				return true;
		}
		return false;
	}

	bool operator<(const SharedPtr& other)
	{
		if (ptr_() < other.ptr_())
			return true;

		return false;
	}

	bool operator>(const SharedPtr& other)
	{
		if (this < other || this == other)
			return false;
		return true;
	}

	// Copy Assignment Operator
	SharedPtr& operator=(const SharedPtr& other)
	{
		if (this != &other)
		{
			// Release old ownership
			if (count_ && --(*count_) == 0)
			{
				reset();
			}

			// Share new ownership
			ptr_ = other.ptr_;
			count_ = other.count_;
			if (count_)
			{
				++(*count_);
			}
		}
		return *this;
	}

	// Move Assignment Operator
	SharedPtr& operator=(SharedPtr&& other) noexcept
	{
		if (this != &other) {
			// Release old ownership
			if (count_ && --(*count_) == 0) {
				reset();
			}

			// Steal resources from 'other'
			ptr_ = other.ptr_;
			count_ = other.count_;

			// Leave other in a valid null state
			other.ptr_ = nullptr;
			other.count_ = nullptr;
		}
		return *this;
	}

	// Swap function
	void swap(SharedPtr& other) noexcept
	{
		using std::swap;
		swap(ptr_, other.ptr_);
		swap(count_, other.count_);
	}

	// Conversion to bool: Returns true if ptr_ is not null
	explicit operator bool() const noexcept
	{
		return ptr_ != nullptr;
	}


	bool Invariant() const noexcept
	{
		if (ptr_ == nullptr)
		{
			return count_ == nullptr;
		}
		return count_ && *count_ > 0;
	}

	size_t use_count() const noexcept
	{
		if (count_ != nullptr)
			return *count_;
		if (count_ == nullptr)
			return 0;
	}








	T* ptr_;
	size_t* count_;
private:
};

template<typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
	return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept
{
	using std::swap;
	swap(lhs.ptr_, rhs.ptr_);
	swap(lhs.count_, rhs.count_);
}
