#include <iostream>
#include <fstream>

#include "block.hpp"
#include "vertex.hpp"
#include "state_machine.hpp"


constexpr size_t _alphabet_size = 40;

size_t symbol_caster(char c)
{
    if (c == ' ')
    {
        return 0;
    }
    if ('0' <= c && c <= '9')
    {
        return static_cast<size_t>(c - '0' + 1);
    }
    if (c == ',')
    {
        return 38;
    }
    if (c == '.')
    {
        return 39;
    }
    if ('a' <= c && c <= 'z')
    {
        return static_cast<size_t>(c - 'a' + 11);
    }
    return static_cast<size_t>(c - 'A' + 11);
}

typedef block<_alphabet_size, symbol_caster> re;
typedef state_machine<_alphabet_size, symbol_caster> sm;


// current version - fast compilation, slow execution.
int main()
{
    std::ifstream in("../input.txt");

    std::string buffer;

    if (in.is_open())
    {
        std::string tmp;
        while (in >> tmp)
        {
            buffer += tmp + " ";
        }
    }
    else
    {
        std::cout << "no such file\n";
    }

    re digit(re("0", "1", "2", "3", "4", "5", "6", "7", "8", "9"), '+');

    re digit_and_after_space(digit, re(re(","), '?'), re(" ", "."));

    re block(re("article ", "articles ", "arts "), re(digit_and_after_space, '+'));
    sm machine(block);
    std::cout << buffer << '\n';
    auto tmp = machine.find_end_positions(buffer);
    for (auto& pos : tmp)
    {
        std::cout << pos << ' ';
    }

  return 0;
}