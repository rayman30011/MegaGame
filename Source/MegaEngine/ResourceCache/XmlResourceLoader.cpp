#include "MegaEngineStd.h"

#include "XmlResourceLoader.h"

bool XmlResourceExtraData::ParseXml(const char* rawBuffer, size_t rawSize)
{
    const auto result = _document.Parse(rawBuffer, rawSize);
    if (result != xml::XML_SUCCESS)
        return false;

    return true;
}

bool XmlResourceLoader::LoadResource(char* rawBuffer, size_t rawSize, shared_ptr<ResHandler> handle)
{
    if (rawSize <= 0) return false;

    const auto extra = std::make_shared<XmlResourceExtraData>();
    if (!extra->ParseXml(rawBuffer, rawSize))
        return false;

    handle->SetExtra(extra);

    return true;
}

shared_ptr<IResourceLoader> CreateXmlResourceLoader() { return std::make_shared<XmlResourceLoader>(); }

xml::XMLElement* XmlResourceLoader::LoadAndReturnRoot(const char* resourceString)
{
    Resource resource(resourceString);
    const auto handler = GApp->GetResourceCahce()->GetHandle(&resource);
    const auto extraData = std::static_pointer_cast<XmlResourceExtraData>(handler->GetExtra());
    return extraData->GetRoot();
}
