//
// Created by gusef on 25.10.2018.
//
#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "block.hpp"


/**
 * \brief                       Class for executing state machine configured in block.
 * \tparam _alphabet_size       Size of state machine alphabit.
 * \tparam _character_caster    Function for comparing number to string symbol.
 */
template <size_t _alphabet_size, size_t (* _character_caster)(char)>
class StateMachine
{

    /**
     * \brief Configured block, describing state machine.
     */
    block<_alphabet_size, _character_caster>& _block;

    /**
     * \breif Buffer for storing state machine's states.
     */
    std::array<std::unordered_set<size_t>, 2> _active_states;

    /**
     * \brief State buffer which we use now.
     */
    size_t _current_states;

    /**
     * \brief                     Function for getting closure by eps links from some state.
     * \param[in] position        Current position which closure we need to get.
     * \param[in] state_buffer    Number of buffer in which we should add new states.
     */
    void closure(size_t position, size_t state_buffer);

public:

    /**
     * \brief             Constructor from configured block.
     * \param[in] block   Configured state machine with vertexes and links.
     */
    explicit StateMachine(block<_alphabet_size, _character_caster>& block);

    /***
     * \brief             Function for finding all end positions of correct sub strings.
     * \param[in] line    Text for searching.
     * \param[in] info    Flag if should show debug information.
     * \return            Vector with ends of good substrings.
     */
    std::vector<size_t> find_end_positions(const std::string& line, bool info = false);

};


#include "state_machine.inl"
