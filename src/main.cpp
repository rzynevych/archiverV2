#include "ParallelCompressor.hpp"
#include "CompressIO.hpp"
#include "DecompressIO.hpp"
#include "Decompressor.hpp"
#include <iostream>
#include <Windows.h>

using namespace std;

vector<string> get_all_files_names_within_folder(string folder)
{
    vector<string> names;
    string search_path = folder + "/*.*";
    WIN32_FIND_DATA fd; 
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
        do { 
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
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