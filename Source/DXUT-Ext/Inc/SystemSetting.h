#ifndef _SYSTEM_SETTING_H_
#define _SYSTEM_SETTING_H_

#include "Core.h"

class FSystemSettings
{
public:
	FSystemSettings();


public:
	uint MSAACount;
	uint MSAAQuality;
	uint MaxConstantBufferSize;
};

extern FSystemSettings GSystemSettings;

#endif