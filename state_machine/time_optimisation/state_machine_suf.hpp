//
// Created by gusef on 02.11.2018.
//

#ifndef STATE_MACHINE_STATE_MACHINE_SUF_HPP
#define STATE_MACHINE_STATE_MACHINE_SUF_HPP

#include "../../vertexes/_vertex.hpp"
#include "../../re_compnent/_re_component.hpp"

#include <vector>
#include <string>
#include <set>

/**
 * Hash for set of objects.
 */
class set_hash {
    std::hash<unsigned long long> hasher;
public:
    inline std::size_t operator()(const std::set<size_t> & v) const {
        std::size_t res = 0;
        for(const auto& it:v)
        {
            auto loc_hash = hasher(it);
            res = res*1373 + ((loc_hash<<7)^(loc_hash>>7));
        }
        return res;
    }
};

/**
 * \brief                       State machine for accepting suffix which was configured in
 * \tparam _alphabet_size    Size of state machine alphabet.
 * \tparam _character_caster Function for comparing number to string symbol.
 */
template <size_t _alphabet_size, size_t (* _character_caster)(char)>
class state_machine_suf
{
    /**
     * \brief Vertexes of state machine.
     */
    std::vector<_vertex<_alphabet_size>> _vertexes;

    /**
     * \brief Root of state machine current graph.
     */
    size_t _root;

    /**
     * \brief Vertex which would represent missing of link.
     */
    size_t _stop_vertex;

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
     * \brief               Constructor from configured re.
     * \param[in] block     Re which represents suffixes of accepting sub strings.
     */
    state_machine_suf(const _re_component<_alphabet_size, _character_caster>& block);

    /**
     * \brief               Function for finding all starts of sub strings, which re accepts.
     * \param[in] line      Line for searching.
     * \param[in] index     Index where sub strings will end.
     * \return              Vector with start positions.
     */
    std::vector<size_t> find_start_positions(const std::string& line, size_t index);

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

#include "state_machine_suf.inl"

#endif //STATE_MACHINE_STATE_MACHINE_SUF_HPP
