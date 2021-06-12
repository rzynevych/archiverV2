#ifndef COMPRESSOR_HPP
# define COMPRESSOR_HPP

# include "archiver.hpp"
# include "CompressIO.hpp"
# include "Dictionary.hpp"

class Compressor {

    private:
    CompressIO      &io;
    Dictionary      dictionary;
    char            *in_buff;
    vector<char>    out_buff;
    int             length_in;
    int             thr_num;

    public:
    Compressor(CompressIO &ioref, int num) : io(ioref)
    {
        in_buff = new char[BUFF_SIZE];
        thr_num = num;
    }
    ~Compressor()
    {
        delete in_buff;
    }
    void            run();
    void            compress();
    void            write_addition(char *addr, char addition);
};

#endif