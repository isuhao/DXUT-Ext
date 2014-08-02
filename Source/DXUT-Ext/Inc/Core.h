#ifndef _CORE_H_
#define _CORE_H_


//////////////////////////////////////////////////////////////////////////

// ϵͳͷ�ļ�
#include <assert.h>

// ��׼��
#include <vector>
#include <string>

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

#endif