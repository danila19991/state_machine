#include <iostream>
#include <fstream>

#include "Block.h"
#include "Vertex.h"
#include "StateMachine.h"

constexpr size_t _aphabet_size = 40;

size_t symbol_caster(char c)
{
    if(c==' ')
    {
        return 0;
    }
    if(c >= '0' && c<= '9')
    {
        return static_cast<size_t>(c-'0'+1);
    }
    if(c == ',')
    {
        return 38;
    }
    if(c == '.')
    {
        return 39;
    }
    if(c>='a' && c<='z')
    {
        return static_cast<size_t>(c-'a'+11);
    }
    return static_cast<size_t>(c-'A'+11);
}

typedef Block<_aphabet_size, symbol_caster> re;
typedef StateMachine<_aphabet_size, symbol_caster> sm;

// current version - fast compilation, slow execution.
int main() {
    std::ifstream in("../input.txt");

    std::string buffer;

    if(in.is_open())
    {
        std::string tmp;
        while(in>>tmp)
        {
            buffer += tmp + " ";
        }
    }
    else
    {
        std::cout<<"no such file\n";
    }

    re digit(re("0", "1", "2", "3", "4", "5", "6", "7", "8", "9"), '+');

    re digit_and_after_space(digit, re(re(","), '?'), re(" ", "."));

    re block(re("article ", "articles ", "arts "), re(digit_and_after_space, '+'));
    sm machine(block);
    std::cout<<buffer<<'\n';
    auto tmp = machine.find_end_positions(buffer);
    for(auto& pos:tmp)
    {
        std::cout<<pos<<' ';
    }

    return 0;
}