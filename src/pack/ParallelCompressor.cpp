#include "ParallelCompressor.hpp"

void    ParallelCompressor::run()
{
    for (int i = 0; i < THREAD_COUNT; i++)
        threads[i] = new thread(&Compressor::run, compressors[i]);
    for (int i = 0; i < THREAD_COUNT; i++)
        threads[i]->join();
}