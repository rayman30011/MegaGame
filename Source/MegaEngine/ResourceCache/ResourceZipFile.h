#pragma once

class ResourceZipFile : public IResourceFile
{
public:
    ResourceZipFile(const std::wstring& name)
    {
        _name = name;
        _zipFile = nullptr;
    }
    
    virtual ~ResourceZipFile();
    
    bool Open() override;
    int GetRawResourceSize(const Resource& resource) override;
    int GetRawResource(const Resource& resource, char* buffer) override;
    int GetNumResources() const override;
    std::string GetResourceName(int num) const override;
private:
    std::wstring _name;
    class ZipFile* _zipFile;
};
