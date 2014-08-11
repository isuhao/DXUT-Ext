#ifndef _TYPES_H_
#define _TYPES_H_

// 基本类型
typedef UINT			uint;

// D3D
typedef D3DXVECTOR4		Vector4;
typedef D3DXVECTOR3		Vector3;
typedef D3DXVECTOR2		Vector2;

typedef D3DXVECTOR4		float4;
typedef D3DXVECTOR3		float3;
typedef D3DXVECTOR2		float2;

typedef D3DXMATRIXA16	float4x4;
typedef D3DXMATRIXA16	FMatrix;


// 字符串
#define WString	std::wstring
#define String	std::string

// 各种容器
#define TArray	std::vector
#define TMap	std::map
#define TSet	std::set	
#define TPair	std::pair

#endif