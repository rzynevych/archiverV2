#ifndef DECOMPRESSIO_HPP
# define DECOMPRESSIO_HPP

# include "archiver.hpp"
# include <fstream>

using namespace std;

class DecompressIO
{
private:
    ifstream    ifs;
    ofstream    ofs;
    char        *inbuff;
    char        *outbuff;

public:
    DecompressIO(string filename);
    ~DecompressIO();
    int         read(char *buff);
    void        write(char *buff, int length);
};

#endif