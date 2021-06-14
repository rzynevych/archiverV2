#include "ParallelCompressor.hpp"
#include "CompressIO.hpp"
#include "DecompressIO.hpp"
#include "Decompressor.hpp"
#include "FileInfo.hpp"
#include <iostream>
#include <dirent.h>
#include <map>

using namespace std;

int isFile(string path)
{
    DIR* directory = opendir(path.c_str());

    if(directory != NULL)
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

void    get_all_filenames_within_folder(vector<FileInfo> &files, string root_path, string path)
{
    string full_path = root_path + "/" + path;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) {
 	/* print all the files and directories within directory */
 	while ((ent = readdir (dir)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
            continue;
        int flag = isFile( path + "/" + string(ent->d_name));
        if (flag == 0)
        {
            
    	    files.push_back(FileInfo(root_path, path, ent->d_name));
        }
        else if (flag == 1)
            get_all_filenames_within_folder(files, root_path, path);
        else
        {
            perror ("An error occured while reading directory\n");
            exit(0);
        }
  	}
  	closedir (dir);
	} else {
	  	perror ("Cannot open directory");
	  	exit(0);
	}
}

void pack(string root_folder)
{
    vector<FileInfo> files;
    get_all_filenames_within_folder(files, root_folder, "");
    ParallelCompressor compressor(files, root_folder, "name.av2");

    try {
        compressor.run();
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}

void unpack()
{
    DecompressIO io("name.av2", "unpk/");
    Decompressor decompessor(io);
    decompessor.run();
}

int main(int argc, char **argv)
{
    
    if (argc == 1)
        pack("./test_data");
    else
        unpack();
    return 0;
}