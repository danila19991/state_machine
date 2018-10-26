//
// Created by gusef on 25.10.2018.
//

#ifndef STATEMACHINE_VERTEX_HPP
#define STATEMACHINE_VERTEX_HPP

#include <array>
#include <vector>

/**
 * \brief                       Vertex in state machine.
 * \tparam[in] _alphabet_size   Size of alpabet for state machine.
 */
template <size_t _alphabet_size>
class Vertex{

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
     * \breif                  Default constructor.
     */
    constexpr Vertex();

    /**
     * \brief                  Setter for vertex link.
     * \param[in] position     Id of link.
     * \param[in] destination  New value for link.
     */
    constexpr void setLink(size_t position, size_t destination);

    /**
     * \brief                  Getter for vertex link.
     * \param[in] position     Id of link.
     * \return                 Link on id position.
     */
    constexpr size_t getNext(size_t position) const;

    /**
     * \brief                  Function for adding new link.
     * \param[in] destination  New destination for eps link.
     */
    constexpr void add_eps_link(size_t destination);

    /**
     * \brief                  Getter for eps links.
     * \return                 Vector with eps links of this vertex.
     */
    std::vector<size_t> get_eps_links() const;

    /**
     * \brief Flag if this vertex is finish vertex of state machine.
     */
    bool _is_finished;
};

#include "Vertex.hpp"

#endif //STATEMACHINE_VERTEX_HPP
