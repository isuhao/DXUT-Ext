#ifndef _QUICK_MULTI_PASS_H_
#define _QUICK_MULTI_PASS_H_

#include "AppFramework.h"


class FQuickMultiPass : public FAppFramework
{
public:
	FQuickMultiPass(const WString& AppName = L"GameApp", const String& ResPath = L"")
		: FAppFramework(AppName, ResPath)
	{
	}

	~FQuickMultiPass()
	{
	}



};

#endif