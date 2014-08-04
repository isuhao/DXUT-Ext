#include "Color.h"

FLinearColor::FLinearColor()
: R(0.f), G(0.f), B(0.f), A(0.f)
{
}

FLinearColor::FLinearColor(float InR, float InG, float InB, float InA)
: R(InR), G(InG), B(InB), A(InA)
{
}
