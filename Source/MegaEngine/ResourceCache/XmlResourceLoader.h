#pragma once
#include "MegaEngineStd.h"

#include "ResCache.h"


class XmlResourceExtraData : public IResourceExtraData
{
public:
    std::string ToString() override { return "XmlResourceExtraData"; }
    void ParseXml(const char* rawBuffer);
    xml::XMLElement* GetRoot() { return _document.RootElement(); }

private:
    xml::XMLDocument _document;
};

class XmlResourceLoader : public IResourceLoader
{
public:
    std::string GetPattern() override {return "*.xml";}
    bool UseRawFile() override { return false; }
    size_t GetLoadedResourceSize(char* rawBuffer, size_t rawSize) override { return rawSize; }
    bool LoadResource(char* rawBuffer, size_t rawSize, shared_ptr<ResHandler> handle) override;

    static xml::XMLElement* LoadAndReturnRoot(const char* resourceString);
};