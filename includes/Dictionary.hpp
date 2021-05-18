#ifndef DICTIONARY_HPP
# define DICTIONARY_HPP

# include "archiver.hpp"

struct dict_elem
{
    char addition;
    int position;
    int parent;
    std::vector<int> children;
};

class Dictionary {
private:
    std::vector<dict_elem>  additions;
    std::vector<char>       chain_buff;
    unsigned int current = -1;
    
public:    
    Dictionary();
    void    insertAddition(char addition); 
    bool    existsAddidion(char addidion);
    int     getLastAddition();
    int     writeChain(int elem, char *buff, int position);
    void    clear();
};

#endif