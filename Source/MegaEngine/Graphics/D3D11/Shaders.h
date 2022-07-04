#pragma once

using namespace DirectX::SimpleMath;

constexpr int MAX_LIGHT_SUPPORT = 8;

struct CBMatrices
{
    Matrix WorldViewProj;
    Matrix World;
};

struct CBMaterial
{
    Vector4 DiffuseColor;
    Vector4 AmbientColor;
    BOOL HasTexture;
    Vector3 Unused;
};

struct CBLighting
{
    Vector4 LightDiffuse[MAX_LIGHT_SUPPORT];
    Vector4 LightDirection[MAX_LIGHT_SUPPORT];
    Vector4 LightAmbient;
    uint32_t NumLights;
    Vector3 Unused;
};


class Scene;
class SceneNode;

class HlslVertexShader
{
public:
    HlslVertexShader();
    ~HlslVertexShader();

    HRESULT OnRestore(Scene* scene);
    HRESULT SetupRender(Scene* scene, const SceneNode* node);
    
protected:
    ID3D11VertexShader* _vertexShader;
    ID3D11InputLayout* _vertexLayout;
    ID3D11Buffer* _bufferMatrices;
    ID3D11Buffer* _bufferMaterial;
    ID3D11Buffer* _bufferLighting;
    bool _enableLights;
};

class HlslPixelShader
{
public:
    HlslPixelShader(const std::string& textureResource);
    ~HlslPixelShader();
    
    HRESULT OnRestore(Scene* scene);
    HRESULT SetupRender(Scene* scene, const SceneNode* node);

    HRESULT SetTexture(std::string& textureName);
    HRESULT SetTexture(ID3D11ShaderResourceView* const* diffuseRv, ID3D11SamplerState* const* samplers);

    void EnableLights(bool val) { _enableLights = val; }
    
protected:
    bool _enableLights;
    
    ID3D11PixelShader* _pixelShader;
    ID3D11Buffer* _bufferMaterial;
    std::string _textureResource;
};