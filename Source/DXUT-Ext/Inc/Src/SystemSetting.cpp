#include "SystemSetting.h"

FSystemSettings GSystemSettings;

FSystemSettings::FSystemSettings()
:	MSAACount	(1)
,	MSAAQuality	(0)
,	MaxConstantBufferSize(Align(2560, 16))
{

}