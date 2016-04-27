#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "Utils.h"
#include <fstream>
#include <iterator>

class FileManager
{
    public:
        FileManager();
        virtual ~FileManager();
        bool Load(FileContents &content, String fileName);
        bool Save(std::vector<String> &content, String fileName);
    protected:
        Utils utils;
        String version;
        String extension;
    private:
};

#endif // FILEMANAGER_H
