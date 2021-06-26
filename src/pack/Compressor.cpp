#include "Compressor.hpp"
#include "CompressIO.hpp"

void    Compressor::run()
{
    while((length_in = io.get_block(in_buff, thr_num)) > 0)
    {
        out_buff.clear();
        out_buff.insert(out_buff.begin(), 4, 0);
        compress();
        int *p = (int *) out_buff.data();
        *p = out_buff.size() - 4;
        cout << "w_start\n";
        io.write_buff(out_buff, thr_num);
        dictionary.clear();
        cout << "w_end\n";
    }
}

void    Compressor::compress()
{
    int i = 0;
    bool last;
    while (i < length_in)
    {
        int i1 = i;
        while(i < length_in && (last = dictionary.existsAddition(in_buff[i])))
            ++i;
        int i2 = i;
        int length = i2 - i1;
        // if (length > 10)
        //     printf("len: %d\n", length);
        int addr = dictionary.getLastAddition();
        write_addition((char *) &addr, in_buff[i]);
        ++i;
    }
    out_buff.push_back((char) !last);
}

void    Compressor::write_addition(char *addr, char addition)
{
    out_buff.insert(out_buff.end(), addr, addr + 3);
    out_buff.insert(out_buff.end(), addition);
}