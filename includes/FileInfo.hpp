#ifndef FILE_INFO_HPP
# define FILE_INFO_HPP

# include <iostream>

class FileInfo
{
private:
    std::string     root_path;
    std::string     path;
    std::string     name;
    std::string     packName;
public:
    FileInfo(std::string rp, std::string p, std::string n)
    {
        root_path = rp;
        path = p;
        name = n;
        packName = path + "/" + name;
    }
    std::string getRootPath() {return root_path;}
    std::string getPath() {return path;}
    std::string getName() {return name;}
    std::string &getPackName() {return packName;}
    std::string getFullName() {return root_path + "/" + packName;}
    int         length() {return packName.length();}
};


#endif