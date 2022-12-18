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
            int flag = isFile( full_path + "/" + name);
            if (flag == 1)
            {
                files.emplace_back(root_path, path + "/" + name);
            }
            else if (flag == 0)
                get_all_filenames_within_folder(files, root_path, path + "/" += name);
            else
            {
                string s("An error occurred while reading directory ");
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

void pack(const vector<string>& paths)
{
    string root;
    string name;
    vector<FileInfo> files;

    for (int i = 1; i < paths.size() - 1; ++i)
    {
        const string& path = paths[i];
        if (path.find('/') == -1)
        {
            root = ".";
            name = path;
        } else {
            root = path.substr(0, path.rfind('/'));
            name = path.substr(path.rfind('/') + 1, -1);
        }
        if (isFile(path) == 1)
            files.emplace_back(root, name);
        else
            get_all_filenames_within_folder(files, root, name);
    }
    string archive_name = paths.at(paths.size() - 1);
    if (archive_name.find(".av2") != archive_name.length() - 4)
    {
        cout << "Invalid archive name: " << archive_name  << endl;
        exit(0);
    }
    ParallelCompressor compressor(files, archive_name);
    try {
        compressor.run();
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}

void unpack(string &file, string &folder)
{
    DecompressIO io(file, folder);
    Decompressor decompressor(io);
    decompressor.run();
}

void unpack_argv_process(vector<string> &args)
{
    if (args.size() > 3)
    {
        cout << "To many arguments for unpacking" << endl;
        exit(0);
    }
    string dir;
    if (args.size() == 2)
        dir = ".";
    if (args.size() == 3)
    {
        dir = args[2];
        if (isFile(dir) != -1)
        {
            cout << "The file " << dir << " already exists" << endl;
            exit(0);
        }
    }
    unpack(args[1], dir);
}

void argv_process(int argc, char **argv)
{
    vector<string> args;

    if (argc == 1)
    {
        cout << USAGE << endl;
        exit(0);
    }
    args.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        args.emplace_back(argv[i]);
    }
    int pos = args[1].find(".av2");
    if (pos > 0 && pos == args[1].length() - 4)
    {
        unpack_argv_process(args);
    } else {
        pack(args);
    }
}

int main(int argc, char **argv)
{
    argv_process(argc, argv);
    return 0;
}