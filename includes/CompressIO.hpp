#ifndef COMPRESSIO_HPP
# define COMPRESSIO_HPP

# include "archiver.hpp"
# include <fstream>
# include <thread>
# include <chrono>

using namespace std;

class CompressIO {

    private:
    ifstream                    ifs;
    ofstream                    ofs;
    char                        *buffer;
    vector<char>                outbuffers[THREAD_COUNT];
    vector<string>              &file_names;
    vector<string>::iterator    file_iterator;
    volatile int                position = 0;
    volatile int                read_counter = 0;
    volatile int                write_counter = 0;
    volatile bool               read_flags[THREAD_COUNT];
    volatile bool               write_flags[THREAD_COUNT];  
    volatile bool               reading = true;
    volatile bool               writing = false;
    volatile bool               is_finish = false;
    volatile bool               rw_finish = false;
    
    public:
    CompressIO(vector<string> &names, string archive_name) : file_names(names)
    {
        buffer = new char[THREAD_COUNT * BUFF_SIZE];
        fill(read_flags, read_flags + THREAD_COUNT, true);
        fill(write_flags, write_flags + THREAD_COUNT, false);
        file_iterator = file_names.begin();
        ofs.open(archive_name, ios::binary);
        thread(CompressIO::rw_loop, this).detach();
    }
    ~CompressIO()
    {
        for (int i = 0; i < THREAD_COUNT && write_flags[i]; i++)
            ofs.write(outbuffers[i].data(), outbuffers[i].size());
        ofs.close();
        rw_finish = true;
        delete buffer;
    }
    int     get_block(char *buff, int thr);
    void    write_buff(const vector<char> &buff, int thr);
    private:
    void    read();
    void    read_file();
    void    open_new_file();
    void    write();
    void    rw_loop();

};

#endif