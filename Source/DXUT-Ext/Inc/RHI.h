#ifndef _RHI_H_
#define _RHI_H_

#include "Core.h"

/** 
 * Render hardware interface
 */
class DynamicRHI
{
public:
	DynamicRHI();
	~DynamicRHI();

	static void InitRHI();
	static void ReleaseRHI();

#include "RHIMethods.h"

private:
	ID3D11DeviceContext*		m_pd3dImmediateContext;
	ID3D11Device*				m_pd3dDevice;
};

extern DynamicRHI* RHI;

#endif