#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>

#include "block.hpp"
#include "vertex.hpp"
#include "state_machine.hpp"


constexpr size_t _alphabet_size = 40;

size_t symbol_caster(const char c)
{
    if (c == ' ')
    {
        return 0;
    }
    if ('0' <= c && c <= '9')
    {
        return c - '0' + 1u;
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
        return c - 'a' + 11u;
    }
    return c - 'A' + 11u;
}

using re = block<_alphabet_size, symbol_caster>;
using sm = state_machine<_alphabet_size, symbol_caster>;


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
        std::cout << "No input file.\n";
        return 1;
    }

    re digit(re("0", "1", "2", "3", "4", "5", "6", "7", "8", "9"), '+');

    re digit_and_after_space(digit, re(re(","), '?'), re(" ", "."));

    re block(re("article ", "articles ", "arts "), re(digit_and_after_space, '+'));

    sm machine(block);
    std::cout << buffer << '\n';

    const auto result = machine.find_end_positions(buffer);
    std::copy(std::begin(result), std::end(result), std::ostream_iterator<size_t>(std::cout, " "));

    return 0;
}
