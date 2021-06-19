#include "ParallelCompressor.hpp"
#include "DecompressIO.hpp"
#include "Decompressor.hpp"
#include "FileInfo.hpp"
#include <iostream>
#include <dirent.h>
#include <map>

using namespace std;

int isFile(const string& path)
{
    DIR* directory = opendir(path.c_str());

    if(directory != nullptr)
    {
     closedir(directory);
     return 0;
    }
    if(errno == ENOTDIR)
    {
     return 1;
    }
    return -1;
}

void    get_all_filenames_within_folder(vector<FileInfo> &files, const string& root_path, const string& path)
{
    string full_path = root_path + "/" + path;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (full_path.c_str())) != nullptr) {
 	/* print all the files and directories within directory */
        while ((ent = readdir (dir)) != nullptr) {
            string name(ent->d_name);
            if (name == "." || name == "..")
                continue;
            int flag = isFile( path + "/" + name);
            if (flag == 1)
            {
                files.emplace_back(root_path, path + "/" + name);
            }
            else if (flag == 0)
                get_all_filenames_within_folder(files, root_path, path + "/" + name);
            else
            {
                string s("An error occured while reading directory ");
                s.append(full_path);
                perror (s.c_str());
                exit(0);
            }
        }
        closedir (dir);
	} else {
        string s("Cannot open directory ");
        s.append(full_path);
        perror (s.c_str());
	  	exit(0);
	}
}

void pack(const string& path)
{
    string root = path.substr(0, path.rfind('/'));
    string name = path.substr(path.rfind('/') + 1, -1);
    vector<FileInfo> files;
    get_all_filenames_within_folder(files, root, name);
    ParallelCompressor compressor(files, "name.av2");

    try {
        compressor.run();
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}

void unpack()
{
    string folder("unpk");
    DecompressIO io("name.av2", folder);
    Decompressor decompressor(io);
    decompressor.run();
}

int main(int argc, char **argv)
{
    
    if (argc == 1)
        pack("./test_data");
    else
        unpack();
    return 0;
}