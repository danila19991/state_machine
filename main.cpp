#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>

#include "re_compnent/_re_component.hpp"
#include "vertexes/_vertex_with_e.hpp"
#include "state_machine/memory_optimisation/state_machine_slow.hpp"
#include "state_machine/time_optimisation/state_machine_fast.hpp"


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

using re = _re_component<_alphabet_size, symbol_caster>;
using sm = state_machine_slow<_alphabet_size, symbol_caster>;
using smf = state_machine_fast<_alphabet_size, symbol_caster>;
using sms = state_machine_suf<_alphabet_size, symbol_caster>;

re generate_article_searcher()
{
    re digit(re("0", "1", "2", "3", "4", "5", "6", "7", "8", "9"), '+');

    re digit_and_after_space(digit, re(re(","), '?'), re(" ", "."));

    return re(re("article ", "articles ", "arts "), re(digit_and_after_space, '+'));

}

std::string read_file(const std::string& file_name)
{
    std::ifstream in(file_name);

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
        return "";
    }
    return buffer;
}

// current version - fast compilation, slow execution.
int main()
{
    /*
    setlocale(LC_ALL, "rus");
    auto buffer = read_file("../input.txt");

    re _re_component = generate_article_searcher();

    sm machine(_re_component);

    if(!buffer.empty())
    {
        std::cout << buffer << '\n';


        const auto result = machine.find_end_positions(buffer);

        for(const auto& elem:result)
        {
            std::cout << elem << ' ';
        }
    }
     */

    re block(re("abad", "ad", "ab"),'+');
    re rblock(re("da", "ba", "daba"), '+');

//    block = re(re(re(re("a"),'*'),re(re("a"),'*')), '+');

    smf machine(block, rblock);

    std::cout<<block.size()<<' '<<machine.size()<<'\n';

    machine.print();

    auto result = machine.find_end_positions("adabcad");

    for(const auto& elem:result)
    {
        std::cout << elem << ' ';
    }


    return 0;
}

// ababababa
