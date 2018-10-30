//
// Created by gusef on 25.10.2018.
//
#pragma once

#include <array>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "block.hpp"


class pair_hash
{
private:
    std::hash<unsigned long long> hasher;

public:
    std::size_t operator()(const std::pair<int, int> & v) const noexcept
    {
        return hasher((static_cast<unsigned long long>(v.first) << 32) |
                      static_cast<unsigned long long>(v.second));
    }
};


/**
 * \brief                   Class for executing state machine configured in block.
 * \tparam ALPHABET_SIZE    Size of state machine alphabit.
 * \tparam CHARACTER_CASTER Function for comparing number to string symbol.
 */
template <size_t ALPHABET_SIZE, size_t (*CHARACTER_CASTER)(char)>
class state_machine
{
private:
    /**
     * \brief Configured block, describing state machine.
     */
    block<ALPHABET_SIZE, CHARACTER_CASTER>& _block;

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
     * \brief                  Function for getting closure by eps links from some state.
     * \param[in] position     Current position which closure we need to get.
     * \param[in] state_buffer Number of buffer in which we should add new states.
     * \param[in] start_pos    Start position of current position.
     */
    void closure(size_t position, size_t state_buffer, size_t start_pos);

    /**
     * \brief                  Function for getting closure by eps links from some state.
     * \param[in] position     Current position which closure we need to get.
     * \param[in] state_buffer Number of buffer in which we should add new states.
     * \param[in] start_pos    Start position of current position.
     */
    void closure_impl(size_t position, size_t state_buffer, size_t start_pos);

    /**
     * \brief              Function for clearing current state, when need to
     *                     process next string.
     * \param[in] id       Id of states container.
     * \param[in] position Current position in line for this state.
     */
    void clear_states(size_t id, size_t position);

public:
    /**
     * \brief           Constructor from configured block.
     * \param[in] block Configured state machine with vertexes and links.
     */
    explicit state_machine(block<ALPHABET_SIZE, CHARACTER_CASTER>& block);

    /**
     * \brief          Function for finding all end positions of correct sub strings.
     * \param[in] line Text for searching.
     * \param[in] info Flag if should show debug information.
     * \return         Vector with ends of good substrings.
     */
    std::vector<std::pair<size_t,size_t>> find_end_positions(const std::string& line,
                                                             const bool info = false);

    /**
     * \brief          Add this string for previous which was used for search,
     *                 may find strings which starts in previous and finishes
     *                 in this.
     * \param[in] line Text for searching.
     * \param[in] info Flag if should show debug information.
     * \return         Vector with ends of good substrings.
     */
    std::vector<std::pair<size_t,size_t>> append_search(const std::string& line,
                                                        const bool info = false);
};


#include "state_machine.inl"
