#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#include "DXUT-Ext.h"

/**
 * App��ܻ���
 */
class FAppFramework : public FGameApp
{
public:
	FAppFramework(const WString& AppName = L"GameApp", const WString& ResPath = L"")
		: FGameApp(AppName, ResPath)
	{
	}

	~FAppFramework()
	{
	}
};

#endif