//
// Created by gusef on 25.10.2018.
//
#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "../../re_compnent/_re_component.hpp"

class pair_hash {
    std::hash<unsigned long long> hasher;
public:
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return hasher((static_cast<unsigned long long>(v.first)<<32)|
                             static_cast<unsigned long long>(v.second));
    }
};


/**
 * \brief                    Class for executing state machine configured in block.
 * \tparam _alphabet_size    Size of state machine alphabit.
 * \tparam _character_caster Function for comparing number to string symbol.
 */
template <size_t _alphabet_size, size_t (* _character_caster)(char)>
class state_machine_slow
{

    /**
     * \brief Configured block, describing state machine.
     */
    _re_component<_alphabet_size, _character_caster>& _block;

    /**
     * \brief   Current length of string, need for multiple search call of one
     *          string.
     */
    size_t _current_length;

    /**
     * \breif Buffer for storing state machine's states.
     */
    std::array<std::unordered_set<std::pair<size_t,size_t>, pair_hash>, 2> _active_states;

    /**
     * \brief State buffer which we use now.
     */
    size_t _current_states;

    /**
     * \brief                   Function for getting closure by eps links from some state.
     * \param[in] position      Current position which closure we need to get.
     * \param[in] state_buffer  Number of buffer in which we should add new states.
     * \param[in] start_pos     Start position of current position.
     */
    void closure(size_t position, size_t state_buffer, size_t start_pos);

    /**
     * \brief                   Function for getting closure by eps links from some state.
     * \param[in] position      Current position which closure we need to get.
     * \param[in] state_buffer  Number of buffer in which we should add new states.
     * \param[in] start_pos     Start position of current position.
     */
    void closure_impl(size_t position, size_t state_buffer, size_t start_pos);

    /**
     * \brief               Function for clearing current state, when need to
     *                      process next string.
     * \param[in] id        Id of states container.
     * \param[in] position  Current position in line for this state.
     */
    void clear_states(size_t id, size_t position);

public:

    /**
     * \brief           Constructor from configured block.
     * \param[in] block Configured state machine with vertexes and links.
     */
    explicit state_machine_slow(_re_component<_alphabet_size, _character_caster>& block);

    /**
     * \brief          Function for finding all end positions of correct sub strings.
     * \param[in] line Text for searching.
     * \param[in] info Flag if should show debug information.
     * \return         Vector with ends of good substrings.
     */
    std::vector<std::pair<size_t,size_t>> find_end_positions(const std::string& line, bool info = false);

    /**
     * \brief          Add this string for previous which was used for search,
     *                 may find strings which starts in previous and finishes
     *                 in this.
     * \param[in] line Text for searching.
     * \param[in] info Flag if should show debug information.
     * \return         Vector with ends of good substrings.
     */
    std::vector<std::pair<size_t,size_t>> append_search(const std::string& line, bool info = false);

};


#include "state_machine_slow.inl"
