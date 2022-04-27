class ResCache;

#include "ZipFile.h"

class IResourceExtraData
{
public:
    virtual std::string ToString()=0;
};

class Resource
{
public:
    std::string Name;

    Resource(const std::string& name)
    {
        Name = name;
        std::transform(Name.begin(), Name.end(), Name.begin(), std::tolower);
    }
};

class ResCache;

class ResHandler
{
    friend class ResCache;
public:
    ResHandler(Resource& resource, char* buffer, size_t size, ResCache* resCache);
    virtual ~ResHandler();

    size_t Size() const { return _size; }
    char* Buffer() const { return _buffer; }
    char* WritableBuffer() { return _buffer; }
    shared_ptr<IResourceExtraData> GetExtra() { return _extra; }
    void SetExtra(shared_ptr<IResourceExtraData> extra) { _extra = extra; }
    
protected:
    Resource _resource;
    char* _buffer;
    size_t _size;
    shared_ptr<IResourceExtraData> _extra;
    ResCache* _resCache;
};

class DefaulResourceLoader : public IResourceLoader
{
public:
    std::string GetPattern() override { return "*"; }
    bool UseRawFile() override { return true; }
    uint32_t GetLoadedResourceSize(char* rawBuffer, size_t rawSize) override { return rawSize; }
    bool LoadResource(char* rawBuffer, size_t rawSize, shared_ptr<ResHandler> handle) override { return true; }
};

class ResCache
{
public:
    ResCache(const size_t sizeMb, IResourceFile* resFile);
    ~ResCache();

    bool Init();
    void RegisterLoader(shared_ptr<IResourceLoader> loader);

    shared_ptr<ResHandler> GetHandle(Resource* resource);
    int Preload(const string pattern, void (*progressCallback)(int, bool&));
    void Flush();
    
protected:
    std::list<shared_ptr<ResHandler>> _lru;
    std::map<string, shared_ptr<ResHandler>> _resources;
    std::list<shared_ptr<IResourceLoader>> _resourceLoaders;

    IResourceFile* _file;

    size_t _cacheSize;
    size_t _allocated;

    shared_ptr<ResHandler> Find(Resource* resource);
    const void* Update(shared_ptr<ResHandler> handler);
    shared_ptr<ResHandler> Load(Resource* resource);
    void Free(shared_ptr<ResHandler> gonner);

    bool MakeRoom(size_t size);
    char* Allocate(size_t size);
    void FreeOneResource();
    void MemoryHasBeenFreed(size_t size);
};
