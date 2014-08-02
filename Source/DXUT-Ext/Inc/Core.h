#ifndef _CORE_H_
#define _CORE_H_


//////////////////////////////////////////////////////////////////////////

// 系统头文件
#include <assert.h>

// 标准库
#include <vector>
#include <string>
#include <xstring>
#include <map>
#include <set>

// DXUT头文件
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"


// 其他头文件
#include "Build.h"
#include "SmartPtr.h"


/// 各种宏
//////////////////////////////////////////////////////////////////////////

#if DO_CHECK
	#define Check(Expr) { assert(Expr); }
#endif

// 一个很牛逼的宏，获取成员在Struct中的偏移量
#ifdef __GNUC__
#define STRUCT_OFFSET( struc, member )	( ( (PTRINT)&((struc*)0x1)->member ) - 0x1 )
#else
#define STRUCT_OFFSET( struc, member )	( (PTRINT)&((struc*)NULL)->member )
#endif

#ifndef INDEX_NONE
#define INDEX_NONE -1
#endif

// 字符串
#define WString	std::wstring
#define String	std::string

// 各种容器
#define TArray	CGrowableArray
#define TMap	std::map
#define TSet	std::set	
#define TPair	std::pair



#endif