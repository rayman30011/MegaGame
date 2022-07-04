#pragma once

namespace DirectX
{
    HRESULT CompileShader(LPCSTR pSrcData,SIZE_T SrcDataLen,LPCSTR pFileName,LPCSTR szEntryPoint,LPCSTR szShaderModel,ID3DBlob** ppBlobOut);
}
