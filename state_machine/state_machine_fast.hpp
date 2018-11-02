//
// Created by gusef on 02.11.2018.
//

#ifndef STATE_MACHINE_STATE_MACHINE_FAST_HPP
#define STATE_MACHINE_STATE_MACHINE_FAST_HPP

#include "../vertexes/_vertex.hpp"
#include "../re_compnent/_re_component.hpp"
#include "state_machine_suf.hpp"

#include <vector>
#include <string>
#include <set>

/**
 * \brief                    Class for executing state machine configured in block.
 * \details                  Before executing transform it to dfsm.
 * \tparam _alphabet_size    Size of state machine alphabet.
 * \tparam _character_caster Function for comparing number to string symbol.
 */
template <size_t _alphabet_size, size_t (* _character_caster)(char)>
class state_machine_fast
{
    std::vector<_vertex<_alphabet_size>> _vertexes;

    state_machine_suf<_alphabet_size, _character_caster> _start_taker;

    size_t _root;

    size_t create_vertex();

    std::set<size_t> closure(const _re_component<_alphabet_size, _character_caster>& block, size_t id);

    void optimise();

public:

    state_machine_fast(const _re_component<_alphabet_size, _character_caster>& block, const _re_component<_alphabet_size, _character_caster>& block_reverse);

    std::vector<std::pair<size_t,size_t>> find_end_positions(const std::string& line);

    void print();

    size_t size() const noexcept;
};

#include "state_machine_fast.inl"

#endif //STATE_MACHINE_STATE_MACHINE_FAST_HPP
