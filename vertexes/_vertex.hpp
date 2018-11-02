//
// Created by gusef on 02.11.2018.
//

#ifndef STATE_MACHINE_VERTEX_HPP
#define STATE_MACHINE_VERTEX_HPP

#include <array>

/**
 * \brief                     Vertex in state machine.
 * \tparam[in] _alphabet_size Size of alphabet for state machine.
 */
template <size_t _alphabet_size>
class _vertex
{
    /**
     * \brief   Array of links to next states.
     * \details Default value is start state.
     */
    std::array<size_t, _alphabet_size> _links;

public:

    /**
     * \breif Default constructor.
     */
    _vertex();

    /**
     * \brief                 Setter for vertex link.
     * \param[in] position    Id of link.
     * \param[in] destination New value for link.
     */
    constexpr void set_link(const size_t position, const size_t destination);

    /**
     * \brief              Getter for vertex link.
     * \param[in] position Id of link.
     * \return             Link on id position.
     */
    constexpr size_t get_link(const size_t position) const;

    /**
     * \brief Flag if this vertex is finish vertex of state machine.
     */
    bool _is_finished;
};

#include "_vertex.inl"

#endif //STATE_MACHINE_VERTEX_HPP
