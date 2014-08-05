#include "Color.h"

// Common colors.
const FLinearColor FLinearColor::White(1, 1, 1);
const FLinearColor FLinearColor::Black(0, 0, 0);

FLinearColor::FLinearColor()
: R(0.f), G(0.f), B(0.f), A(0.f)
{
}

FLinearColor::FLinearColor(float InR, float InG, float InB, float InA)
: R(InR), G(InG), B(InB), A(InA)
{
}
