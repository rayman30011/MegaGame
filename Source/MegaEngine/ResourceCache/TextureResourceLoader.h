#pragma once

#include "ResCache.h"

class TextureResourceLoader : public IResourceLoader
{
public:
    bool UseRawFile() override { return false; }
    bool DiscardRawBufferAfterLoad() { return true; }
    uint32_t GetLoadedResourceSize(char* rawBuffer, size_t rawSize) override { return 0; }
    bool LoadResource(char* rawBuffer, uint32_t rawSize, shared_ptr<ResHandler> handle) override;
private:
};

class D3DTextureResourceExtraData : public IResourceExtraData
{
    friend class TextureResourceLoader;
public:
    D3DTextureResourceExtraData();
    virtual ~D3DTextureResourceExtraData()
    {
        SAFE_RELEASE(_texture);
        SAFE_RELEASE(_samplerLinear);
    }

    std::string ToString() override { return "“D3DTextureResourceExtraData"; }
    ID3D11ShaderResourceView* const* GetTexture() const { return &_texture; }
    ID3D11SamplerState* const* GetSampler() const { return &_samplerLinear; }
    
private:
    ID3D11ShaderResourceView* _texture;
    ID3D11SamplerState* _samplerLinear;
};

