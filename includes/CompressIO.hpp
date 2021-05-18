#ifndef COMPRESSIO_HPP
# define COMPRESSIO_HPP

# include "archiver.hpp"
# include <fstream>

using namespace std;

class CompressIO {

    private:
    ifstream                    ifs;
    ofstream                    ofs;
    char                        *buffer;
    vector<string>              *fileNames;
    vector<string>::iterator    fileIterator;


    bool    openNewFile();
    
    public:
    CompressIO(vector<string> *fileNames);
    ~CompressIO();
    int     read(char *buff);
    void    write(const vector<char> &buff);
};

#endif