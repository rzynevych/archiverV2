#ifndef COMPRESSOR_HPP
# define COMPRESSOR_HPP

# include "archiver.hpp"
# include "CompressIO.hpp"
# include "Dictionary.hpp"

class Compressor {

    private:
    CompressIO      *io;
    Dictionary      dictionary;
    char            *in_buff;
    vector<char>    out_buff;
    int             length_in;

    public:
    Compressor(CompressIO *io);
    ~Compressor();
    void            run();
    void            compress();
};

#endif