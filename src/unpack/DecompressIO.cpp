#include "DecompressIO.hpp"
#include <cassert>
#include <sys/stat.h>

using namespace std;

int DecompressIO::read(char *buff)
{
    int size;
    ifs.read((char *) &size, FILE_LSIZE);
    ifs.read(buff, size);
    cout << "size: " << size << endl;
    assert(ifs.gcount() == size);
    return size;
}

void DecompressIO::write(char *buff, int length)
{
    outlength = length;
    copy(buff, buff + outlength, outbuff);
    position = 0;
//    string fname("part");
//    fname.insert(fname.length(), to_string(part));
//    ofstream o1(fname, ios::binary);
//    o1.write(outbuff, outlength);
//    o1.flush();
//    o1.close();
    if (part == 6)
        cout << "part = " << part << endl;
    part++;
    while (position != outlength) {
        check_to_read();
        if (!to_read.file_open)
        {
            string path = dirname;
            path.append("/").append(namebuff);
            create_directory(path.c_str());
            ofs.open(path, ios::binary);
            if (!ofs.is_open())
            {
                perror("Error while opening output stream: ");
                std::cout << path << endl;
                exit(0);
            }
            to_read.file_open = true;
        }
        if (to_read.file > outlength - position)
        {
            to_read.file -= outlength - position;
            ofs.write(outbuff + position, outlength - position);
            position = outlength;
        }
        else if (to_read.file > 0)
        {
            std::cout << "part: " << part << " pos: " << position << endl;
            ofs.write(outbuff + position, to_read.file);
            ofs.flush();
            std::cout << ofs.tellp() << endl;
            ofs.close();
            position += to_read.file;
            to_read.namesize = NAME_SIZE;
            to_read.file = 0;
            to_read.file_open = false;
        }
    }
}

void    DecompressIO::check_to_read()
{
    std::cout << "namesize: " << to_read.namesize << endl;
    std::cout << "name: " << to_read.name << endl;
    std::cout << "position: " << position << endl;

    if (to_read.namesize != 0 && read_params(to_read.namesize, to_read.namesize_pos, &to_read.name_tmp))
    {
        to_read.name = to_read.name_tmp;
        std::cout << "name: " << to_read.name << endl;
        std::cout << "position: " << position << endl;
    }
    if (to_read.name != 0 && read_params(to_read.name, to_read.name_pos, namebuff))
    {
        namebuff[to_read.name_tmp] = 0;
        to_read.name_tmp = 0;
        to_read.filesize = FILE_LSIZE;
        std::cout << "position: " << position << endl;
    }
    if (to_read.filesize != 0 && read_params(to_read.filesize, to_read.filesize_pos, &to_read.file_tmp))
    {
        to_read.file = to_read.file_tmp;
        to_read.file_tmp = 0;
        std::cout << "file: " << to_read.file << endl;
        std::cout << "position: " << position << endl;
    }
    std::cout << endl;
}

bool    DecompressIO::read_params(int &length, int &pos, void *param)
{
    int end = position + length;
    if (end > outlength)
        end = outlength;
    length -= end - position;
    copy(outbuff + position, outbuff + end, (char *) param + pos);
    pos += end - position;
    position = end;
    if (length == 0)
        pos = 0;
    return length == 0;
}

bool    folder_exists(const string& path)
{
    struct stat st{};
    stat(path.c_str(), &st);
    return st.st_mode & S_IFDIR;
}

void    DecompressIO::create_directory(const char *fpath)
{

    string path(fpath);
    string curr_path("");
    
    int start = 0;
    size_t pos;
    while ((pos = path.find('/', start)) != std::string::npos)
    {
        curr_path.append("/").append(path.substr(start, pos));
        if (!folder_exists(curr_path))
        {
            mkdir(curr_path.c_str(), 0755);
        }
        ++start;
    }    
}
