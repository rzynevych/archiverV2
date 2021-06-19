#ifndef FILE_INFO_HPP
# define FILE_INFO_HPP

# include <iostream>

class FileInfo
{
private:
    std::string     root_path;
    std::string     path;
    std::string     full_name;
public:
    FileInfo(const std::string& rp, const std::string& p)
    {
        root_path = rp;
        path = p;
        full_name = root_path + "/" + path;
    }
    std::string getRootPath() {return root_path;}
    std::string getPath() {return path;}
    std::string getFullName() {return root_path + "/" + path;}
    size_t        length() {return path.length();}
    size_t        getFullLength() {return full_name.length();}
};

#endif