#ifndef _CORE_H_
#define _CORE_H_


//////////////////////////////////////////////////////////////////////////

// 系统头文件
#include <assert.h>

// 标准库
#include <vector>
#include <string>

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

#endif