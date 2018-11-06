//
// Created by gusef on 25.10.2018.
//
#pragma once

#include <array>
#include <vector>

#include "_vertex.hpp"


/**
 * \brief                     Vertex in state machine with eps-links.
 * \tparam[in] _alphabet_size Size of alphabet for state machine.
 */
template <size_t _alphabet_size>
class _vertex_with_e: public _vertex<_alphabet_size>
{
protected:

    /**
     * \brief   Vector of eps links.
     * \details Vector because i don't know number of such links.
     */
    std::vector<size_t> _eps_links;

public:

    /**
     * \breif Default constructor.
     */
    _vertex_with_e();

    /**
     * \brief                 Function for adding new link.
     * \param[in] destination New destination for eps link.
     */
    void add_eps_link(const size_t destination);

    /**
     * \brief  Getter for eps links.
     * \return Vector with eps links of this vertex.
     */
    std::vector<size_t> get_eps_links() const;
};

#include "_vertex_with_e.inl"
