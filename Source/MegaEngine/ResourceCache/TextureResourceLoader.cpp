#include "MegaEngineStd.h"

#include "TextureResourceLoader.h"
#include "DDSTextureLoader.h"

bool TextureResourceLoader::LoadResource(char* rawBuffer, uint32_t rawSize, shared_ptr<ResHandler> handle)
{
    const auto extra = std::make_shared<D3DTextureResourceExtraData>();
    if (FAILED(DirectX::CreateDDSTextureFromMemory(DXUTGetD3D11Device(), (uint8_t*)rawBuffer, rawSize, nullptr, &extra->_texture)))
        return false;

    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof samplerDesc);
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MaxLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    if (FAILED(DXUTGetD3D11Device()->CreateSamplerState(&samplerDesc, &extra->_samplerLinear)))
        return false;

    handle->SetExtra(shared_ptr(extra));
    return true;
}
