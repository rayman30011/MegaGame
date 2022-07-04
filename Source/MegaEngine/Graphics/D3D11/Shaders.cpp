#include "MegaEngineStd.h"

#include "Shaders.h"

#include "Common.h"
#include "ResourceCache/ResCache.h"
#include "VertexTypes.h"
#include "ResourceCache/TextureResourceLoader.h"

HlslVertexShader::HlslVertexShader()
{
    _vertexShader = nullptr;
    _vertexLayout = nullptr;
    _bufferMatrices = nullptr;
    _bufferMaterial = nullptr;
    _bufferLighting = nullptr;
    _enableLights = true;
}

HlslVertexShader::~HlslVertexShader()
{
    SAFE_RELEASE(_vertexLayout);
    SAFE_RELEASE(_vertexShader);
    SAFE_RELEASE(_bufferMatrices);
    SAFE_RELEASE(_bufferMaterial);
    SAFE_RELEASE(_bufferLighting);
}

HRESULT HlslVertexShader::OnRestore(Scene* scene)
{
    HRESULT hr;
    
    SAFE_RELEASE(_vertexLayout);
    SAFE_RELEASE(_vertexShader);
    SAFE_RELEASE(_bufferMatrices);
    SAFE_RELEASE(_bufferMaterial);
    SAFE_RELEASE(_bufferLighting);

    ID3DBlob* vertexShaderBuffer = nullptr;
    const std::string hlslFileName = "Effects\\VertexShader.hlsl";
    Resource resource(hlslFileName.c_str());
    const auto resourceHandler = GApp->GetResourceCache()->GetHandle(&resource);

    if (FAILED(DirectX::CompileShader(resourceHandler->Buffer(), resourceHandler->Size(), hlslFileName.c_str(), "VSMain", "vs_4_0_level_9_1", &vertexShaderBuffer)))
    {
        SAFE_RELEASE(vertexShaderBuffer);
        return E_FAIL;
    }

    if (FAILED(DXUTGetD3D11Device()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &_vertexShader)))
    {
        SAFE_RELEASE(vertexShaderBuffer);
        return E_FAIL;
    }

    if (SUCCEEDED(DXUTGetD3D11Device()->CreateInputLayout(
        DirectX::VertexPositionNormalTexture::InputElements, 
        ARRAYSIZE(DirectX::VertexPositionNormalTexture::InputElements),
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        &_vertexLayout))
    )
    {
        D3D11_BUFFER_DESC desc;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.ByteWidth = sizeof(CBMatrices);
        V_RETURN(DXUTGetD3D11Device()->CreateBuffer(&desc, nullptr, &_bufferMatrices));

        desc.ByteWidth = sizeof(CBMaterial);
        V_RETURN(DXUTGetD3D11Device()->CreateBuffer(&desc, nullptr, &_bufferMaterial));

        desc.ByteWidth = sizeof(CBLighting);
        V_RETURN(DXUTGetD3D11Device()->CreateBuffer(&desc, nullptr, &_bufferLighting));
    }

    SAFE_RELEASE(vertexShaderBuffer);
    return S_OK;
}

HRESULT HlslVertexShader::SetupRender(Scene* scene, const SceneNode* node)
{
    HRESULT hr;

    DXUTGetD3D11DeviceContext()->VSSetShader(_vertexShader, nullptr, 0);
    DXUTGetD3D11DeviceContext()->IASetInputLayout(_vertexLayout);

    Matrix worldViewProjection;
    //Matrix worldViewProjection = scene->GetCamera()->GetWorldViewProj();
    Matrix world;
    // Matrix world = scene->GetTopMatrix();
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;

    
    V(DXUTGetD3D11DeviceContext()->Map(_bufferMatrices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource));
    CBMatrices* matrices = (CBMatrices*)mappedSubresource.pData;
    worldViewProjection.Transpose(matrices->WorldViewProj);
    world.Transpose(matrices->World);
    DXUTGetD3D11DeviceContext()->Unmap(_bufferMatrices, 0);

    
    V(DXUTGetD3D11DeviceContext()->Map(_bufferLighting, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource));
    CBLighting* lighting = (CBLighting*)mappedSubresource.pData;
    if (_enableLights)
    {
        //scene->GetLightManager()->CalcLighting(lighting, node);
    }
    else
    {
        lighting->NumLights = 0;
        lighting->LightAmbient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    DXUTGetD3D11DeviceContext()->Unmap(_bufferLighting, 0);

    
    V(DXUTGetD3D11DeviceContext()->Map(_bufferMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource));
    CBMaterial* material = (CBMaterial*)mappedSubresource.pData;

    Color color;
    // Color color = pNode->VGet()->GetMaterial().GetDiffuse();
    material->DiffuseColor = color;
    // color = (_enableLights) ?
    //     node->VGet()->GetMaterial().GetAmbient() :
    //     Color(1.0f, 1.0f, 1.0f, 1.0f);

    color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    material->AmbientColor = color;
    material->HasTexture = false;
    DXUTGetD3D11DeviceContext()->Unmap(_bufferMaterial, 0);

    DXUTGetD3D11DeviceContext()->VSSetConstantBuffers(0, 1, &_bufferMatrices);
    DXUTGetD3D11DeviceContext()->VSSetConstantBuffers(1, 1, &_bufferLighting);
    DXUTGetD3D11DeviceContext()->VSSetConstantBuffers(2, 1, &_bufferMaterial);

    return S_OK;
}




HlslPixelShader::HlslPixelShader(const std::string& textureResource)
{
    _textureResource = textureResource;
    _pixelShader = nullptr;
    _bufferMaterial = nullptr;
}

HlslPixelShader::~HlslPixelShader()
{
    SAFE_RELEASE(_pixelShader);
    SAFE_RELEASE(_bufferMaterial);
}

HRESULT HlslPixelShader::OnRestore(Scene* scene)
{
    HRESULT hr;
    
    SAFE_RELEASE(_pixelShader);
    SAFE_RELEASE(_bufferMaterial);

    ID3DBlob* pixelShaderBuffer = nullptr;
    const std::string hlslFileName = "Effects\\PixelShader.hlsl";
    Resource resource(hlslFileName.c_str());
    const auto resourceHandler = GApp->GetResourceCache()->GetHandle(&resource);

    if (FAILED(DirectX::CompileShader(resourceHandler->Buffer(), resourceHandler->Size(), hlslFileName.c_str(), "Main", "ps_4_0_level_9_1", &pixelShaderBuffer)))
    {
        SAFE_RELEASE(pixelShaderBuffer);
        return E_FAIL;
    }

    if (SUCCEEDED(DXUTGetD3D11Device()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &_pixelShader)))
    {
        D3D11_BUFFER_DESC desc;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.ByteWidth = sizeof(CBMaterial);

        hr = DXUTGetD3D11Device()->CreateBuffer(&desc, nullptr, &_bufferMaterial);
    }

    SAFE_RELEASE(pixelShaderBuffer);
    return hr;
}

HRESULT HlslPixelShader::SetupRender(Scene* scene, const SceneNode* node)
{
    HRESULT hr;
    DXUTGetD3D11DeviceContext()->PSSetShader(_pixelShader, nullptr, 0);

    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    V(DXUTGetD3D11DeviceContext()->Map(_bufferMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource));
    auto* material = (CBMaterial*)mappedSubresource.pData;

    Color color;
    // Color color = node->Get()->GetMaterial().GetDiffuse();
    material->DiffuseColor = color;

    if (_textureResource.empty())
        material->HasTexture = false;
    else
        material->HasTexture = true;

    DXUTGetD3D11DeviceContext()->Unmap(_bufferMaterial, 0);
    DXUTGetD3D11DeviceContext()->PSSetConstantBuffers(0, 1, &_bufferMaterial);
    SetTexture(_textureResource);
    
    return S_OK;
}

HRESULT HlslPixelShader::SetTexture(std::string& textureName)
{
    _textureResource = textureName;
    if (!_textureResource.empty())
    {
        Resource resource(textureName);
        auto texture = GApp->GetResourceCache()->GetHandle(&resource);
        if (texture)
        {
            shared_ptr<D3DTextureResourceExtraData> extra = std::static_pointer_cast<D3DTextureResourceExtraData>(texture->GetExtra());
            SetTexture(extra->GetTexture(), extra->GetSampler());
        }
    }

    return S_OK;
}

HRESULT HlslPixelShader::SetTexture(ID3D11ShaderResourceView* const* diffuseRv, ID3D11SamplerState* const* samplers)
{
    DXUTGetD3D11DeviceContext()->PSSetShaderResources(0, 1, diffuseRv);
    DXUTGetD3D11DeviceContext()->PSSetSamplers(0, 1, samplers);
    
    return S_OK;
}
