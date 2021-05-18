#include "Compressor.hpp"
#include "CompressIO.hpp"

Compressor::Compressor(CompressIO *io)
{
    this->io = io;
    in_buff = new char[BUFF_SIZE];
}

Compressor::~Compressor()
{
    delete in_buff;
}

void    Compressor::run()
{
    while((length_in = io->read(in_buff)) > 0)
    {
        compress();
        cout << "w_start\n";
        io->write(out_buff);
        cout << "w_end\n";
    }
}

void    Compressor::compress()
{
    int i = 0;
    while (i < length_in)
    {
        int i1 = i;
        while(i < length_in && dictionary.existsAddidion(in_buff[i]))
            ++i;
        int i2 = i;
        int length = i2 - i1;
        // if (length > 10)
        //     printf("len: %d\n", length);
        int add = dictionary.getLastAddition();
        char *padd = (char *) &add;
        out_buff.insert(out_buff.end(), padd, padd + 3);
        out_buff.insert(out_buff.end(), in_buff[i]);
        ++i;
    }
}