#include "CompressIO.hpp"
#include <cassert>

void CompressIO::read()
{
    position = 0;
    while (position < THREAD_COUNT * BUFF_SIZE && !is_finish)
    {
        int count;
        do {
            ifs.read(buffer + position, THREAD_COUNT * BUFF_SIZE - position);
            count = ifs.gcount();
            position += count;
        } while(count > 0);
        int length = file_iterator->length();
        if (count == 0 && position + length + NAME_SIZE < THREAD_COUNT * BUFF_SIZE)
        {
            ifs.close();
            open_new_file();
            ++file_iterator;
        } else
            break;
    }
}

void    CompressIO::rw_loop()
{
    while (!rw_finish)
    {
        if (reading)
        {
            read();
            fill(read_flags, read_flags + THREAD_COUNT, false);
            reading = false;
            read_counter = 0;
        }
        if (writing)
        {
            for (int i = 0; i < THREAD_COUNT; i++) {
                ofs.write(outbuffers[i].data(), outbuffers[i].size());
                outbuffers[i].clear();
            }
            fill(write_flags, write_flags + THREAD_COUNT, false);
            writing = false;
            write_counter = 0;
        }
        this_thread::sleep_for(chrono::microseconds(100));
    }
}

void    CompressIO::open_new_file()
{
    if (is_finish)
        return ;
    if (file_iterator == files.end())
    {
        is_finish = true;
        return ;
    }
    int name_length = file_iterator->length();
    *(short *)(buffer + position) = (short) name_length;
    position += NAME_SIZE;

    string path = file_iterator->getPath();
    const char *name_data = path.data();
    copy(name_data, name_data + name_length, buffer + position);
    position += file_iterator->length();
    ifs.open(file_iterator->getFullName(), ios::binary);
    if (!ifs.is_open())
    {
        perror("");
        cout << file_iterator->getFullName() << endl;
        exit(0);
    }
    ifs.seekg(0, ios::end);
    size_t filesize = ifs.tellg();
    ifs.seekg(0, ios::beg);
    char *p = (char *) &filesize;
    copy(p, p + FILE_LSIZE, buffer + position);
    position += FILE_LSIZE;
}

int     CompressIO::get_block(char *buff, int thr)
{
    while (read_flags[thr])
    {
        if (is_finish && write_flags[thr])
            return 0;
        this_thread::sleep_for(chrono::microseconds(100));
    }
    if (position < BUFF_SIZE * thr)
        return -1;
    int count = position - BUFF_SIZE * thr > BUFF_SIZE ? BUFF_SIZE : position - BUFF_SIZE * thr;
    char *end = buffer + BUFF_SIZE * thr + count;
    copy(buffer + BUFF_SIZE * thr, end, buff);
    read_flags[thr] = true;
    ++read_counter;
    if (read_counter == THREAD_COUNT)
        reading = true;
    return count;
}

void    CompressIO::write_buff(const vector<char> &buff, int thr)
{
    while (write_flags[thr])
    {
        this_thread::sleep_for(chrono::microseconds(100));
    }
    outbuffers[thr].insert(outbuffers[thr].begin(), buff.begin(), buff.end());
    write_flags[thr] = true;
    ++write_counter;
    if (write_counter == THREAD_COUNT)
        writing = true;
}