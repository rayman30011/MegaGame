#pragma once
#include "MegaEngineStd.h"

#include "ResCache.h"


class XmlResourceExtraData : public IResourceExtraData
{
public:
    std::string ToString() override { return "XmlResourceExtraData"; }
    bool ParseXml(const char* rawBuffer, size_t rawSize);
    xml::XMLElement* GetRoot() { return _document.RootElement(); }

private:
    xml::XMLDocument _document;
};

class XmlResourceLoader : public IResourceLoader
{
public:
    std::string GetPattern() override {return "*.xml";}
    bool UseRawFile() override { return false; }
    bool AddNullZero() override { return true; }
    size_t GetLoadedResourceSize(char* rawBuffer, size_t rawSize) override { return rawSize; }
    bool LoadResource(char* rawBuffer, size_t rawSize, shared_ptr<ResHandler> handle) override;

    static xml::XMLElement* LoadAndReturnRoot(const char* resourceString);
};