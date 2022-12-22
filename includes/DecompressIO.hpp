#ifndef DECOMPRESSIO_HPP
# define DECOMPRESSIO_HPP

# include "archiver.hpp"
# include <fstream>
# include <cstring>

struct to_read_t
{
    int     namesize = NAME_SIZE;
    int     namesize_pos = 0;
    int     namesize_val = 0;

    int     name = 0;
    int     name_pos = 0;
    int     filesize = 0;
    int     filesize_pos = 0;
    int     filesize_val = 0;
    int     file = 0;
    bool    file_open = false;
};


class DecompressIO
{
private:
    std::ifstream   ifs;
    std::ofstream   ofs;
    to_read_t       to_read;
    std::string     dirname;
    int             position = 0;
    int             part = 0;
    int             outlength{};
    char            *inbuff;
    char            *outbuff;
    char            *namebuff;

public:
    DecompressIO(const std::string& filename, std::string& dname)
    {
        dirname = dname;
        ifs.open(filename, std::ios::binary);
        if (!ifs.is_open())
        {
            std::cout << "Cannot open file " << filename << ": " << strerror(errno) << std::endl;
            exit(0);
        }
        inbuff = new char[BUFF_SIZE * 2];
        outbuff = new char[BUFF_SIZE];
        namebuff = new char[0x1000];
    }
    ~DecompressIO()
    {
        delete[] inbuff;
        delete[] outbuff;
        delete[] namebuff;
        ifs.close();
    }
    int             read(char *buff);
    void            write(char *buff, int length);
    private:
    void            check_to_read();
    bool            read_params(int &length, int &pos, void *param);
    void            create_directory(const char *fpath);
};

#endif