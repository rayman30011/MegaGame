#include "MegaEngineStd.h"

#include "XmlResourceLoader.h"

void XmlResourceExtraData::ParseXml(const char* rawBuffer)
{
    const string str = rawBuffer;
    const auto result = _document.Parse(str.c_str());
    if (result != xml::XML_SUCCESS)
    {
        int a = 5;
    }
}

bool XmlResourceLoader::LoadResource(char* rawBuffer, size_t rawSize, shared_ptr<ResHandler> handle)
{
    if (rawSize <= 0) return false;

    const auto extra = std::make_shared<XmlResourceExtraData>();
    extra->ParseXml(rawBuffer);

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
