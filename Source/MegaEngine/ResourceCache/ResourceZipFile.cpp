#include <MegaEngineStd.h>

#include "ResourceZipFile.h"
#include "ResCache.h"
#include "ZipFile.h"

ResourceZipFile::~ResourceZipFile()
{
    if (_zipFile)
    {
        delete _zipFile;
        _zipFile = nullptr;
    }
}

bool ResourceZipFile::Open()
{
    _zipFile = _NEW ZipFile();
    if (_zipFile)
        return _zipFile->Init(_name);
    return false;
}

int ResourceZipFile::GetRawResourceSize(const Resource& resource)
{
    const auto resourceNum = _zipFile->Find(resource.Name.c_str());
    if (resourceNum == std::nullopt)
        return -1;

    return _zipFile->GetFileLength(resourceNum.value());
}

int ResourceZipFile::GetRawResource(const Resource& resource, char* buffer)
{
    int size = 0;
    const auto resourceNum = _zipFile->Find(resource.Name.c_str());
    if (resourceNum)
    {
        size = _zipFile->GetFileLength(resourceNum.value());
        _zipFile->ReadFile(resourceNum.value(), buffer);
    }
    return size;
}

int ResourceZipFile::GetNumResources() const
{
    return _zipFile == nullptr ? 0 : _zipFile->GetNumFiles();
}

std::string ResourceZipFile::GetResourceName(int num) const
{
    std::string resName = "";
    if (_zipFile != nullptr && num >= 0 && num < _zipFile->GetNumFiles())
    {
        resName = _zipFile->GetFileName(num); 
    }
    return resName;
}
