#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "Core.h"
#include "Shader.h"

//--------------------------------------------------------------------------------------
// ±‡“ÎShader
//--------------------------------------------------------------------------------------
extern HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

#endif