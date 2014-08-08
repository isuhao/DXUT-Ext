#ifndef _CORE_H_
#define _CORE_H_


//////////////////////////////////////////////////////////////////////////

// Build
#include "Build.h"

// ϵͳͷ�ļ�
#include <assert.h>

// ��׼��
#include <iostream>
#include <vector>
#include <string>
#include <xstring>
#include <map>
#include <set>

// DXͷ�ļ�
#include "d3d11.h"
#include "D3D10.h"
#include "d3d9.h"

// DXUTͷ�ļ�
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"

// ����ͷ�ļ�
#include "Types.h"
#include "Util.h"

/// ���ֺ�
//////////////////////////////////////////////////////////////////////////

#if DO_CHECK
	#define Check(Expr) { assert(Expr); }
#endif

// ǰ������
#define PRE_DECLARE_CLASS(ClassType) \
	class ClassType;

#define PRE_DECLARE_STRUCT(StructType) \
	class StructType;

#define PRE_DECLARE_TEMPLATE_CLASS(T, ClassType) \
	template<typename T> \
	class ClassType;

#define PRE_DECLARE_TEMPLATE_CLASS2(T1, T2, ClassType) \
	template<typename T1, typename T2> \
class ClassType;


// һ����ţ�Ƶĺ꣬��ȡ��Ա��Struct�е�ƫ����
#ifdef __GNUC__
#define STRUCT_OFFSET( struc, member )	( ( (PTRINT)&((struc*)0x1)->member ) - 0x1 )
#else
#define STRUCT_OFFSET( struc, member )	( (PTRINT)&((struc*)NULL)->member )
#endif

// ��Ч������
#ifndef INDEX_NONE
#define INDEX_NONE -1
#endif

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V2
#define V2(x)           { if( FAILED(x) ) { Check(0); } }
#endif
#else
#ifndef V2
#define V2(x)           { x; }
#endif
#endif

#endif