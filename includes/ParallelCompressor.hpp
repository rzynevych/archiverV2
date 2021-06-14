#ifndef PARALLEL_COMPRESSOR_HPP
# define PARALLEL_COMPRESSOR_HPP

# include "CompressIO.hpp"
# include "Compressor.hpp"
# include "FileInfo.hpp"

class ParallelCompressor
{
private:
    CompressIO      io;
    Compressor      *compressors[THREAD_COUNT];
    thread          *threads[THREAD_COUNT];

public:
    ParallelCompressor(vector<FileInfo> &files, string root, string archive_name) : io(files, root, archive_name)
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