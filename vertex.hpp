//
// Created by gusef on 25.10.2018.
//
#pragma once

#include <array>
#include <vector>


/**
 * \brief                     Vertex in state machine.
 * \tparam[in] _alphabet_size Size of alphabet for state machine.
 */
template <size_t _alphabet_size>
class Vertex
{

    /**
     * \brief   Vector of eps links.
     * \details Vector because i don't know number of such links.
     */
    std::vector<size_t> _eps_links;

    /**
     * \brief   Array of links to next states.
     * \details Default value is start state.
     */
    std::array<size_t, _alphabet_size> _links;

public:

    /**
     * \breif Default constructor.
     */
    Vertex();

    /**
     * \brief                 Setter for vertex link.
     * \param[in] position    Id of link.
     * \param[in] destination New value for link.
     */
    constexpr void set_link(size_t position, size_t destination);

    /**
     * \brief              Getter for vertex link.
     * \param[in] position Id of link.
     * \return             Link on id position.
     */
    constexpr size_t get_next(size_t position) const;

    /**
     * \brief                 Function for adding new link.
     * \param[in] destination New destination for eps link.
     */
    void add_eps_link(size_t destination);

    /**
     * \brief  Getter for eps links.
     * \return Vector with eps links of this vertex.
     */
    std::vector<size_t> get_eps_links() const;

    /**
     * \brief Flag if this vertex is finish vertex of state machine.
     */
    bool _is_finished;
};

#include "vertex.inl"
