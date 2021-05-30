#include "Decompressor.hpp"
#include <cassert>

Decompressor::~Decompressor()
{
    delete inbuff;
    delete outbuff;
}

void    Decompressor::run()
{
    while((length_in = io.read(inbuff)) > 0)
    {
        assert(length_in % 4 == 0);
        decompress();
        io.write(outbuff, position);
        dictionary.clear();
        position = 0;
    }
}

void    Decompressor::decompress()
{
    int i = 0;
    while (i < length_in / 4)
    {
        position += dictionary.writeChain(int_buff[i], outbuff, position);
        i++;
        if (i == length_in / 4)
            --position;
    }
}