#include "FileManager.h"

FileManager::FileManager()
{
    version = "DES_version_1.0.0.0";
    extension = ".des";
}

FileManager::~FileManager()
{

}

bool FileManager::Load(FileContents &content, String fileName)
{
    content.clear();

    std::ifstream file((fileName + extension).c_str());
    if(!file.is_open())
        return false;
    while(!file.eof())
    {
        String s;
        std::getline(file, s);
        //split each word
        std::stringstream line(s);
        std::istream_iterator<String> begin(line);
        std::istream_iterator<String> end;
        std::vector<String> vstrings(begin, end);

        content.push_back(vstrings);
    }
    file.close();
    if(!content.size())
        return false;
    if(!content[0].size())
        return false;
    if(content[0][0] != version)
        return false;
    return true;
}

bool FileManager::Save(std::vector<String> &content, String fileName)
{
    std::ofstream file;
    std::ifstream exists((fileName + extension).c_str());
    if(exists.is_open())
    {
        int i = 0;
        exists.close();
        exists.open((fileName + utils.ToString(i) + extension).c_str());
        while(exists.is_open())
        {
            i++;
            exists.close();
            exists.open((fileName + utils.ToString(i) + extension).c_str());
        }
        file.open((fileName + utils.ToString(i) + extension).c_str());
    }
    else
        file.open((fileName + extension).c_str());

    if(!file.is_open())
        return false;

#ifdef DEBUG
    std::cout << std::endl;
    for(unsigned i = 0; i < content.size(); i++)
        std::cout << content[i] << std::endl;
#endif // DEBUG

    file << version << std::endl;

    for(unsigned i = 0; i < content.size(); i++)
    {
        file << content[i] << std::endl;
    }
    file.close();
    return true;
}
