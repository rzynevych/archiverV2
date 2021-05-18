#ifndef DECOMPRESSOR_HPP
# define DECOMPRESSOR_HPP

# include "archiver.hpp"
# include "DecompressIO.hpp"
# include "Dictionary.hpp"

using namespace std;

class Decompressor
{
private:
    DecompressIO    &io;
    Dictionary      dictionary;
    char            *inbuff;
    int             *int_buff;
    char            *outbuff;
    int             length_in;
    int             position = 0;
    
public:
    Decompressor(DecompressIO &decompressIO) : io(decompressIO) {
        inbuff = new char[2 * BUFF_SIZE];
        int_buff = (int *)inbuff;
        outbuff = new char[BUFF_SIZE];
    }
    ~Decompressor();
    void            run();
    void            decompress();
};

#endif