#include "ParallelCompressor.hpp"
#include "CompressIO.hpp"
#include "DecompressIO.hpp"
#include "Decompressor.hpp"
#include <iostream>
#include <dirent.h>

using namespace std;

vector<string> get_all_files_names_within_folder(string folder)
{
    vector<string> names;
    string search_path = folder;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (search_path.c_str())) != NULL) {
 	/* print all the files and directories within directory */
 	while ((ent = readdir (dir)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
            continue;
    	names.push_back(ent->d_name);
  	}
  	closedir (dir);
	} else {
	  	perror ("");
	  	exit(0);
	}
    return names;
}

void pack()
{
    vector<string> names = get_all_files_names_within_folder("./test_data");
    for (int i = 0; i < names.size(); ++i)
    {
        names[i] = "test_data/" + names[i];
    }
    ParallelCompressor compressor(names, "name.av2");

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
        pack();
    else
        unpack();
    return 0;
}