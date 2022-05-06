#include "MegaEngineStd.h"

#include "ResCache.h"
#include "Utilities/Strings.h"

ResHandler::ResHandler(Resource& resource, char* buffer, size_t size, ResCache* resCache) : _resource(resource)
{
    _buffer = buffer;
    _size = size;
    _extra = nullptr;
    _resCache = resCache;
}

ResHandler::~ResHandler()
{
    if (_buffer)
        delete[] _buffer;

    _buffer = nullptr;

    _resCache->MemoryHasBeenFreed(_size);
}


ResCache::ResCache(const size_t sizeMb, IResourceFile* resFile)
{
    _cacheSize = sizeMb * 1024 * 1024;
    _allocated = 0;
    _file = resFile;
}

ResCache::~ResCache()
{
    while (!_lru.empty())
    {
        FreeOneResource();
    }

    if (_file)
    {
        delete _file;
        _file = nullptr;
    }
}

bool ResCache::Init()
{
    bool result = false;
    if (_file->Open())
    {
        RegisterLoader(shared_ptr<IResourceLoader>(_NEW DefaulResourceLoader()));
        result = true;
    }

    return result;
}

void ResCache::RegisterLoader(shared_ptr<IResourceLoader> loader)
{
    _resourceLoaders.push_front(loader);
}

shared_ptr<ResHandler> ResCache::GetHandle(Resource* resource)
{
    shared_ptr handler(Find(resource));
    if (!handler)
        handler = Load(resource);
    else
        Update(handler);

    return handler;
}

int ResCache::Preload(const string pattern, void(* progressCallback)(int, bool&))
{
    if (_file == nullptr)
        return 0;

    int numFiles = _file->GetNumResources();
    int loaded = 0;
    bool cancel = false;
    for (int i = 0; i < numFiles; ++i)
    {
        Resource resource(_file->GetResourceName(i));
        if (Utilities::Strings::WildcardMatch(pattern.c_str(), resource.Name.c_str()))
        {
            shared_ptr<ResHandler> handle = GApp->GetResourceCahce()->GetHandle(&resource);
            ++loaded;
        }

        if (progressCallback != nullptr)
            progressCallback(i / 100 / numFiles, cancel);
    }
    
    return loaded;
}

void ResCache::Flush()
{
}

shared_ptr<ResHandler> ResCache::Find(const Resource* resource)
{
    const auto handler = _resources.find(resource->Name);
    if (handler == _resources.end())
        return shared_ptr<ResHandler>();

    return handler->second;
}

void ResCache::Update(shared_ptr<ResHandler> handler)
{
    _lru.remove(handler);
    _lru.push_front(handler);
}

shared_ptr<ResHandler> ResCache::Load(Resource* resource)
{
    shared_ptr<IResourceLoader> loader;
    shared_ptr<ResHandler> handler;

    for (auto it = _resourceLoaders.begin(); it != _resourceLoaders.end(); ++it)
    {
        shared_ptr<IResourceLoader> testLoader = *it;
        if (Utilities::Strings::WildcardMatch(testLoader->GetPattern().c_str(), resource->Name.c_str()))
        {
            loader = testLoader;
            break;
        }
    }

    if (!loader)
    {
        assert(loader && _T("Default resource loader not found"));
        return handler;
    }

    size_t rawSize = _file->GetRawResourceSize(*resource);
    size_t alloc = rawSize + (loader->AddNullZero() ? 1 : 0);
    char* rawBuffer = loader->UseRawFile() ? Allocate(alloc) : _NEW char[alloc];

    if (rawBuffer == nullptr)
        return shared_ptr<ResHandler>();

    _file->GetRawResource(*resource, rawBuffer);
    char* buffer = nullptr;
    size_t size = 0;
    
    if (loader->UseRawFile())
    {
        buffer = rawBuffer;
        handler = std::make_shared<ResHandler>(*resource, buffer, rawSize, this);
    }
    else
    {
        size = loader->GetLoadedResourceSize(rawBuffer, rawSize);
        buffer = Allocate(size);
        if (rawBuffer == nullptr || buffer == nullptr)
            return shared_ptr<ResHandler>();

        handler = std::make_shared<ResHandler>(*resource, buffer, size, this);
        bool success = loader->LoadResource(rawBuffer, rawSize, handler);

        if (!success)
            return shared_ptr<ResHandler>();
    }

    if (handler)
    {
        _lru.push_front(handler);
        _resources[resource->Name] = handler;
    }

    
    assert(loader && _T("Default resource loader not found!"));
    return handler;
}

void ResCache::Free(shared_ptr<ResHandler> gonner)
{
}

bool ResCache::MakeRoom(size_t size)
{
    if (size > _cacheSize)
        return false;

    while (size > (_cacheSize - _allocated))
    {
        if (_lru.empty())
            return false;

        FreeOneResource();
    }
    return true;
}

char* ResCache::Allocate(size_t size)
{
    if (!MakeRoom(size))
        return nullptr;

    char* mem = _NEW char[size];
    if (mem)
        _allocated += size;

    return mem;
}

void ResCache::FreeOneResource()
{
    const auto end = _lru.end();
    shared_ptr<ResHandler> handler = *end;

    _lru.pop_back();
    _resources.erase(handler->_resource.Name);
}

void ResCache::MemoryHasBeenFreed(size_t size)
{
}



