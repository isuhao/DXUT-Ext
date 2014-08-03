#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

#include <memory>
#include <functional>


// 智能指针
#define TSharedPtr std::shared_ptr

// COM对象的智能指针
// 可以在释放的时候调用Release
template <typename T>
inline TSharedPtr<T> MakeCOMPtr(T* Ptr)
{
	return Ptr ? TSharedPtr<T>(Ptr, std::mem_fn(&T::Release)) : TSharedPtr<T>();
}

template <typename T>
inline TSharedPtr<T> MakeSharedPtr()
{
	return TSharedPtr<T>(new T);
}

template <typename T, typename A1>
inline TSharedPtr<T> MakeSharedPtr(A1 const & a1)
{
	return TSharedPtr<T>(new T(a1));
}

template <typename T, typename A1>
inline TSharedPtr<T> MakeSharedPtr(A1& a1)
{
	return TSharedPtr<T>(new T(a1));
}

template <typename T, typename A1, typename A2>
inline TSharedPtr<T> MakeSharedPtr(A1 const & a1, A2 const & a2)
{
	return TSharedPtr<T>(new T(a1, a2));
}

template <typename T, typename A1, typename A2>
inline TSharedPtr<T> MakeSharedPtr(A1& a1, A2& a2)
{
	return TSharedPtr<T>(new T(a1, a2));
}

#endif