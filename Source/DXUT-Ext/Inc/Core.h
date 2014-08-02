#ifndef _CORE_H_
#define _CORE_H_


//////////////////////////////////////////////////////////////////////////

// ϵͳͷ�ļ�
#include <assert.h>

// ��׼��
#include <vector>
#include <string>
#include <xstring>
#include <map>
#include <set>

// DXUTͷ�ļ�
#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"


// ����ͷ�ļ�
#include "Build.h"
#include "SmartPtr.h"


/// ���ֺ�
//////////////////////////////////////////////////////////////////////////

#if DO_CHECK
	#define Check(Expr) { assert(Expr); }
#endif

// һ����ţ�Ƶĺ꣬��ȡ��Ա��Struct�е�ƫ����
#ifdef __GNUC__
#define STRUCT_OFFSET( struc, member )	( ( (PTRINT)&((struc*)0x1)->member ) - 0x1 )
#else
#define STRUCT_OFFSET( struc, member )	( (PTRINT)&((struc*)NULL)->member )
#endif

#ifndef INDEX_NONE
#define INDEX_NONE -1
#endif

// �ַ���
#define WString	std::wstring
#define String	std::string

// ��������
#define TArray	CGrowableArray
#define TMap	std::map
#define TSet	std::set	
#define TPair	std::pair



#endif