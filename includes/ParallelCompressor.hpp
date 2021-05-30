#ifndef PARALLEL_COMPRESSOR_HPP
# define PARALLEL_COMPRESSOR_HPP

# include "CompressIO.hpp"
# include "Compressor.hpp"

class ParallelCompressor
{
private:
    CompressIO      io;
    Compressor      *compressors[THREAD_COUNT];
    thread          *threads[THREAD_COUNT];

public:
    ParallelCompressor(vector<string> &filenames, string archive_name) : io(filenames, archive_name)
    {
        for (int i = 0; i < THREAD_COUNT; i++)
            compressors[i] = new Compressor(io, i);
    }
    ~ParallelCompressor()
    {
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            delete compressors[i];
            delete threads[i];
        }
    }
    void    run();
};

#endif