//
// Created by gusef on 02.11.2018.
//

#ifndef STATE_MACHINE_STATE_MACHINE_FAST_HPP
#define STATE_MACHINE_STATE_MACHINE_FAST_HPP

#include "../../vertexes/_vertex.hpp"
#include "../../re_compnent/_re_component.hpp"
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
    /**
     * \brief Vertexes of state machine.
     */
    std::vector<_vertex<_alphabet_size>> _vertexes;

    /**
     * \brief State machine for getting all starts of accepting strings.
     */
    state_machine_suf<_alphabet_size, _character_caster> _start_taker;

    /**
     * \brief Root of state machine current graph.
     */
    size_t _root;

    /**
     * \brief   Function for creating vertex.
     * \return  Id of new vertex.
     */
    size_t create_vertex();

    /**
     * \brief           Function for creating closure of 'id' vertex in given re.
     * \param[in] block Re for making closure.
     * \param[in] id    Id of vertex.
     * \return          Set with vertexes in closure.
     */
    std::set<size_t> closure(const _re_component<_alphabet_size, _character_caster>& block, size_t id);

    /**
     * \brief Function for minimizing number of states in state machine.
     */
    void optimise();

public:

    /**
     * \brief                   Constructor for create state machine from configured re.
     * \param[in] block         Re for accepting sub strings moving from start to end.
     * \param[in] block_reverse Re for accepting sub strings moving from end to start.
     */
    state_machine_fast(const _re_component<_alphabet_size, _character_caster>& block, const _re_component<_alphabet_size, _character_caster>& block_reverse);

    /**
     * \brief           Function for finding all starts and ends of accepting sub strings.
     * \param[in] line  String for scanning.
     * \return          Vector with pairs of accepting strings.
     */
    std::vector<std::pair<size_t,size_t>> find_end_positions(const std::string& line);

    /**
     * \brief Function for outputting this state machine to stdout.
     */
    void print();

    /**
     * \brief   Function fro getting size of graph.
     * \return  Number of vertexes in graph.
     */
    size_t size() const noexcept;
};

#include "state_machine_fast.inl"

#endif //STATE_MACHINE_STATE_MACHINE_FAST_HPP
