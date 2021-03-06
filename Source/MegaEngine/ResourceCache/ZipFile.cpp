#include <MegaEngineStd.h>

#include "ZipFile.h"
#include <zlib.h>

typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char byte;

#pragma pack(1)

struct ZipFile::ZipLocalHeader
{
    enum
    {
        SIGNATURE = 0x04034b50,
    };
    dword   sig;
    word    version;
    word    flag;
    word    compression;      // Z_NO_COMPRESSION or Z_DEFLATED
    word    modTime;
    word    modDate;
    dword   crc32;
    dword   cSize;
    dword   ucSize;
    word    fnameLen;         // Filename string follows header.
    word    xtraLen;          // Extra field follows filename.
};

struct ZipFile::ZipDirHeader
{
    enum
    {
        SIGNATURE = 0x06054b50
    };
    dword   sig;
    word    nDisk;
    word    nStartDisk;
    word    nDirEntries;
    word    totalDirEntries;
    dword   dirSize;
    dword   dirOffset;
    word    cmntLen;
};

struct ZipFile::ZipDirFileHeader
{
    enum
    {
        SIGNATURE   = 0x02014b50
    };
    dword   sig;
    word    verMade;
    word    verNeeded;
    word    flag;
    word    compression;      // COMP_xxxx
    word    modTime;
    word    modDate;
    dword   crc32;
    dword   cSize;            // Compressed size
    dword   ucSize;           // Uncompressed size
    word    fnameLen;         // Filename string follows header.
    word    xtraLen;          // Extra field follows filename.
    word    cmntLen;          // Comment field follows extra field.
    word    diskStart;
    word    intAttr;
    dword   extAttr;
    dword   hdrOffset;

    char *GetName   () const { return (char*)(this + 1);   }
    char *GetExtra  () const { return GetName() + fnameLen; }
    char *GetComment() const { return GetExtra() + xtraLen; }
};

#pragma pack()

bool ZipFile::Init(const std::wstring& resFileName)
{
    _wfopen_s(&_file, resFileName.c_str(), _T("rb"));
    if (!_file)
        return false;

    // Assuming no extra comment at the end, read the whole end record.
    ZipDirHeader dh;

    fseek(_file, -(int)sizeof(dh), SEEK_END);
    long dhOffset = ftell(_file);
    memset(&dh, 0, sizeof(dh));
    fread(&dh, sizeof(dh), 1, _file);

    // Check
    if (dh.sig != ZipDirHeader::SIGNATURE)
        return false;

    // Go to the beginning of the directory.
    fseek(_file, dhOffset - dh.dirSize, SEEK_SET);

    // Allocate the data buffer, and read the whole thing.
    _dirData = _NEW char[dh.dirSize + dh.nDirEntries*sizeof(*_papDir)];
    if (!_dirData)
        return false;
    memset(_dirData, 0, dh.dirSize + dh.nDirEntries * sizeof(*_papDir));
    fread(_dirData, dh.dirSize, 1, _file);

    // Now process each entry.
    char *pfh = _dirData;
    _papDir = (const ZipDirFileHeader **)(_dirData + dh.dirSize);

    bool success = true;

    for (int i = 0; i < dh.nDirEntries && success; i++)
    {
        ZipDirFileHeader &fh = *(ZipDirFileHeader*)pfh;

        // Store the address of nth file for quicker access.
        _papDir[i] = &fh;

        // Check the directory entry integrity.
        if (fh.sig != ZipDirFileHeader::SIGNATURE)
            success = false;
        else
        {
            pfh += sizeof(fh);

            // Convert UNIX slashes to DOS backlashes.
            for (int j = 0; j < fh.fnameLen; j++)
                if (pfh[j] == '/')
                    pfh[j] = '\\';

            char fileName[_MAX_PATH];
            memcpy(fileName, pfh, fh.fnameLen);
            fileName[fh.fnameLen]=0;
            _strlwr_s(fileName, _MAX_PATH);
            std::string spath = fileName;
            ZipContent[spath] = i;

            // Skip name, extra and comment fields.
            pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
        }
    }
    if (!success)
    {
        SAFE_DELETE_ARRAY(_dirData);
    }
    else
    {
        _numFiles = dh.nDirEntries;
    }

    return success;
}

std::optional<int> ZipFile::Find(const std::string& path)
{
    std::string lowerCase = path;
    std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
    const auto i = ZipContent.find(lowerCase);
    if (i == ZipContent.end())
        return std::nullopt;

    return i->second;
}

void ZipFile::End()
{
    ZipContent.clear();
    SAFE_DELETE_ARRAY(_dirData);
    _numFiles = 0;
}

std::string ZipFile::GetFileName(int index) const
{
    std::string fileName = "";
    if (index >=0 && index < _numFiles)
    {
        char pszDest[_MAX_PATH];
        memcpy(pszDest, _papDir[index]->GetName(), _papDir[index]->fnameLen);
        pszDest[_papDir[index]->fnameLen] = '\0';
        fileName = pszDest;
    }

    return fileName;
}

int ZipFile::GetFileLength(int index) const
{
    if (index < 0 || index >= _numFiles)
        return -1;
    
    return _papDir[index]->ucSize;
}

bool ZipFile::ReadFile(int i, void* buf) const
{
    if (buf == nullptr || i < 0 || i > _numFiles)
        return false;

    fseek(_file, _papDir[i]->hdrOffset, SEEK_SET);
    ZipLocalHeader header;

    memset(&header, 0, sizeof(header));
    fread(&header, sizeof(header), 1, _file);
    if (header.sig != ZipLocalHeader::SIGNATURE)
        return false;

    fseek(_file, header.fnameLen + header.xtraLen, SEEK_CUR);
    if (header.compression == Z_NO_COMPRESSION)
    {
        fread(buf, header.cSize, 1, _file);
        return true;
    }
    else if (header.compression != Z_DEFAULT_COMPRESSION)
        return false;

    char* data = _NEW char[header.cSize];
    if (!data)
        return false;

    memset(data, 0, header.cSize);
    fread(data, header.cSize, 1, _file);

    bool ret = true;

    // Setup the inflate stream.
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)data;
    stream.avail_in = (uInt)header.cSize;
    stream.next_out = (Bytef*)buf;
    stream.avail_out = header.ucSize;
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit2(&stream, -MAX_WBITS);
    if (err == Z_OK)
    {
        err = inflate(&stream, Z_FINISH);
        inflateEnd(&stream);
        if (err == Z_STREAM_END)
            err = Z_OK;
        inflateEnd(&stream);
    }
    if (err != Z_OK)
        ret = false;

    delete[] data;
    return ret;
}

bool ZipFile::ReadLargeFile(int i, void* buf, void(* callback)(int, bool&))
{
    if (buf == nullptr || i < 0 || i > _numFiles)
        return false;

    fseek(_file, _papDir[i]->hdrOffset, SEEK_SET);
    ZipLocalHeader header;

    memset(&header, 0, sizeof(header));
    fread(&header, sizeof(header), 1, _file);
    if (header.sig != ZipLocalHeader::SIGNATURE)
        return false;

    fseek(_file, header.fnameLen + header.xtraLen, SEEK_CUR);
    if (header.compression == Z_NO_COMPRESSION)
    {
        fread(buf, header.cSize, 1, _file);
        return true;
    }
    else if (header.compression != Z_DEFAULT_COMPRESSION)
        return false;

    char* data = _NEW char[header.cSize];
    if (!data)
        return false;

    memset(data, 0, header.cSize);
    fread(data, header.cSize, 1, _file);

    bool ret = true;

    // Setup the inflate stream.
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)data;
    stream.avail_in = (uInt)header.cSize;
    stream.next_out = (Bytef*)buf;
    stream.avail_out = (128 * 1024);
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit2(&stream, -MAX_WBITS);

    if (err == Z_OK)
    {
        uInt count = 0;
        bool cancel = false;

        while (stream.total_in < (uInt)header.cSize && !cancel)
        {
            err = inflate(&stream, Z_SYNC_FLUSH);
            if (err == Z_STREAM_END)
            {
                err = Z_OK;
                break;
            }
            else if (err != Z_OK)
            {
                _ASSERT(TEXT("Something happened"));
                break;
            }

            stream.avail_out = (128 * 1024); 
            stream.next_out += stream.total_out;

            callback(count * 100 / header.cSize, cancel);
        }
    }

    if (err != Z_OK)
        ret = false;

    delete[] data;
    return ret;
}




