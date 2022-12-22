#ifndef COMPRESSIO_HPP
# define COMPRESSIO_HPP

# include "archiver.hpp"
# include "FileInfo.hpp"
# include <fstream>
# include <memory>
# include <thread>
# include <chrono>
# include <atomic>

using namespace std;

class CompressIO {

    private:
    ifstream                    ifs;
    ofstream                    ofs;
    char                        *buffer;
    vector<char>                outbuffers[THREAD_COUNT];
    vector<FileInfo>            files;
    vector<FileInfo>::iterator  file_iterator;
    unique_ptr<thread>          rw_thread;
    atomic_int                  position;
    atomic_int                  read_counter;
    atomic_int                  write_counter;
    atomic_bool                 read_flags[THREAD_COUNT];
    atomic_bool                 write_flags[THREAD_COUNT];  
    atomic_bool                 reading;
    atomic_bool                 writing;
    atomic_bool                 is_finish;
    atomic_bool                 rw_finish;
    
    public:
    CompressIO(vector<FileInfo> &_files, const string& archive_name) :
        position(0), read_counter(0), write_counter(0), 
        reading(true), writing(false), is_finish(false), rw_finish(false)
    {
        files = _files;
        buffer = new char[THREAD_COUNT * BUFF_SIZE];
        fill(read_flags, read_flags + THREAD_COUNT, true);
        fill(write_flags, write_flags + THREAD_COUNT, false);
        file_iterator = files.begin();
        ofs.open(archive_name, ios::binary);
        rw_thread.reset(new thread(&CompressIO::rw_loop, this));
    }
    ~CompressIO()
    {

        writing.store(true);
        this_thread::sleep_for(chrono::milliseconds(10));
        rw_finish.store(true);
        rw_thread->join();
        cout << "wf[0] = " << write_flags[0] << endl;
        ofs.close();
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