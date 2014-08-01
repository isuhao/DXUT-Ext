#ifndef _SMART_PTR_H_
#define _SMART_PTR_H_

#include <memory>
#include <functional>


// ����ָ��
#define TSharedPtr std::shared_ptr

// COM���������ָ��
// �������ͷŵ�ʱ�����Release
template <typename T>
inline TSharedPtr<T> MakeCOMPtr(T* Ptr)
{
	return Ptr ? TSharedPtr<T>(Ptr, std::mem_fn(&T::Release)) : TSharedPtr<T>();
}


#define CreateRHIResourcePtr( )

#endif