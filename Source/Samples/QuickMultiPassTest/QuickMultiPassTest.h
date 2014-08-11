#ifndef _QUICK_MULTI_PASS_TEST_H_
#define _QUICK_MULTI_PASS_TEST_H_

#include "MultiPassRenderer.h"

class FQuickMultiPassTest : public FMultiPassRenderer
{
public:
	FQuickMultiPassTest();

	virtual void RegisterResources();
};

#endif