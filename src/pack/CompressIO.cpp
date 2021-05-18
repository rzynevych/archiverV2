#include "CompressIO.hpp"

CompressIO::CompressIO(vector<string> *fileNames)
{
    this->fileNames = fileNames;
    fileIterator = fileNames->begin();
    ifs.open(*fileIterator, ifstream::binary);
    ofs.open("file.av2", ofstream::binary);
}

int CompressIO::read(char *buff)
{
    ifs.read(buff, BUFF_SIZE);
    return ifs.gcount();
}

void CompressIO::write(const vector<char> &buff)
{
    ofs.write(buff.data(), buff.size());
}