#include <MegaEngineStd.h>

#include "Material.h"

Material::Material()
{
    ZeroMemory(&_d3dMaterial, sizeof(D3DMATERIAL9));
    const auto white = DirectX::Colors::White;
    const auto black = DirectX::Colors::Black;
    _d3dMaterial.Diffuse = {white[0], white[1], white[2], white[3]};
    _d3dMaterial.Specular = {white[0], white[1], white[2], white[3]};
    _d3dMaterial.Emissive = {black[0], black[1], black[2], black[3]};
    _d3dMaterial.Ambient =  { 0.1f,0.1f,0.1f,1.f };
}
