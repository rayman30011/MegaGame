#pragma once

#include <d3d9types.h>

class Material
{
public:
    Material();
    
    void SetAmbient(D3DCOLORVALUE color) { _d3dMaterial.Ambient = color; }
    D3DCOLORVALUE GetAmbient() const { return _d3dMaterial.Ambient; }

    void SetSpecular(D3DCOLORVALUE color) { _d3dMaterial.Specular = color; }
    D3DCOLORVALUE GetSpecular() const { return _d3dMaterial.Specular; }

    void SetDiffuse(D3DCOLORVALUE color) { _d3dMaterial.Diffuse = color; }
    D3DCOLORVALUE GetDiffuse() const { return _d3dMaterial.Diffuse; }

    void SetEmissive(D3DCOLORVALUE color) { _d3dMaterial.Emissive = color; }
    D3DCOLORVALUE GetEmissive() const { return _d3dMaterial.Emissive; }

    void SetAlpha(const float alpha) { _d3dMaterial.Diffuse.a = alpha; }
    float GetAlpha() const { return _d3dMaterial.Diffuse.a; }
    
private:
    D3DMATERIAL9 _d3dMaterial;
};
