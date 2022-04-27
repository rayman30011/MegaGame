#pragma once

class IGameView
{
public:
    virtual void OnRender(float time, float elapsedTime) = 0;
};

class Resource;
class ResHandler;

class IResourceLoader
{
public:
    virtual std::string GetPattern() = 0;
    virtual bool UseRawFile() = 0;
    virtual size_t GetLoadedResourceSize(char* rawBuffer, size_t rawSize) = 0;
    virtual bool LoadResource(char* rawBuffer, uint32_t rawSize, shared_ptr<ResHandler> handle) = 0;
};

class IResourceFile
{
public:
    virtual bool Open() = 0;
    virtual int GetRawResourceSize(const Resource& resource) = 0;
    virtual int GetRawResource(const Resource& resource, char* buffer) = 0;
    virtual int GetNumResources() const = 0;
    virtual std::string GetResourceName(int num) const = 0;
};