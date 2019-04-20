#pragma once

template <typename T>
class observer_ptr
{
public:
	observer_ptr(T* naked) { ptr_ = naked; }
	observer_ptr() = default;
	~observer_ptr() = default;
	observer_ptr(const observer_ptr&) = default;
	observer_ptr(observer_ptr&&) = default;
	observer_ptr& operator=(const observer_ptr&) = default;
	observer_ptr& operator=(observer_ptr&&) = default;
	explicit operator bool() const {
		return static_cast<bool>(ptr_);
	}
	explicit operator T*() const {
		return ptr_;
	}
	bool operator!=(observer_ptr<T> other)
	{
		return ptr_ != other.ptr_;
	}
	void assign(observer_ptr<T> ptr)
	{
		assert(!ptr_);
		ptr_ = ptr.ptr_;
	}
	observer_ptr<T> transfer()
	{
		assert(ptr_);
		auto out = ptr_;
		ptr_ = nullptr;
		return observer_ptr{ ptr_ };
	}
	void free()
	{
		assert(ptr_);
		ptr_ = nullptr;
	}
	T* operator->() const
	{
		return ptr_;
	}
	T* get() const { return ptr_; }
	bool is_free() const { return ptr_ != nullptr; }
protected:
	T* ptr_ = nullptr;
};