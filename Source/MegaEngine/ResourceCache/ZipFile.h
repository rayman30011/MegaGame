#pragma once
#include <MegaEngineStd.h>
#include <optional>

class ZipFile
{
public:
    ZipFile();

    bool Init(std::wstring &resFileName);
    void End();

    int GetNumFiles() const { return _numFiles; }
    std::string GetFileName(int index) const;
    int GetFileLength(int index) const;
    bool ReadFile(int i, void* buf) const;

    bool ReadLargeFile(int index, void* buf, void(*callback)(int, bool&));
    std::optional<int> Find(const std::string& path);

    std::map<std::string, int> ZipContent;
private:
    struct ZipDirHeader;
    struct ZipFileHeader;
    struct ZipLocalHeader;

    FILE* _file;
    char* _dirData;
    int _numFiles;

    const ZipFileHeader **_papDir;
};
