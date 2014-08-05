#ifndef _COLOR_H_
#define _COLOR_H_

#include "DXUT.h"

enum EColorChannel
{
	ECC_R,
	ECC_G,
	ECC_B,
	ECC_A,
	ECC_Num
};

class FLinearColor
{
public:
	float R, G, B, A;
	
	FLinearColor();
	explicit FLinearColor(float InR, float InG, float InB, float InA = 1.f);

	// Common colors.	
	static const FLinearColor White;
	static const FLinearColor Black;
};

#endif