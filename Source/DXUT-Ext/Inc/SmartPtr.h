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


#define CreateRHIResourcePtr( )

#endif