#include "DecompressIO.hpp"

DecompressIO::DecompressIO(string filename)
{
    ifs.open(filename, ios::binary);
    ofs.open("decompressed.jpg", ios::binary);
    inbuff = new char[BUFF_SIZE * 2];
    outbuff = new char[BUFF_SIZE];
}

DecompressIO::~DecompressIO()
{
    delete inbuff;
    delete outbuff;
    ifs.close();
    ofs.close();
}

int DecompressIO::read(char *buff)
{
    ifs.read(buff, BUFF_SIZE);
    return ifs.gcount();
}

void DecompressIO::write(char *buff, int length)
{
    ofs.write(buff, length);
}