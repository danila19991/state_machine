//
// Created by gusef on 25.10.2018.
//
#pragma once

#include <array>
#include <string>
#include <vector>
#include <unordered_map>

#include "vertex.hpp"


/**
 * \brief                    Class for creating configuration for state machine.
 * \tparam _alphabet_size    Size of alphabit in state machine.
 * \tparam _character_caster Function for casting symbol to size_t.
 */
template <size_t _alphabet_size, size_t (* _character_caster)(char)>
class block
{
    /**
     * \brief First base for hashing.
     */
    constexpr static unsigned long long BASE1 = 37;

    /**
     * \brief Second base for hashing.
     */
    constexpr static unsigned long long BASE2 = 1373;

    /**
     * \brief Container for vertexes in state machine.
     */
    std::vector<Vertex<_alphabet_size>> _vertexes;

    /**
     * \brief Start position of state machine.
     */
    size_t _root;

    /**
     * \brief Finish position of state machine.
     */
    size_t _finish_vertex;

    /**
     * Function of adding new vertex to state machine.
     * \return Id of new vertex.
     */
    size_t new_vertex();

    /**
     * \brief Hash table of hashes of sub trees with roots of this subtrees.
     */
    std::unordered_map<unsigned long long, std::vector<size_t>> _sub_tree_hashes;

    /**
     * \brief Function for optimising number of vertexes, after optimising links.
     */
    void optimise_vertexes();

    /**
     * \brief               Function for checking if 2 vertexes has equal sub trees.
     * \param[in] vertex1   First vertex for checking.
     * \param[in] vertex2   Second vertex for checking.
     * \return              True if sub trees from these vertexes equal, false otherwise.
     */
    bool is_equal_sub_trees(size_t vertex1, size_t vertex2);

    /**
     * \brief               Deep first search for finding equal sub trees.
     * \param[in] position  Current position of dfs.
     * \return              Hash of current sub tree.
     */
    unsigned long long optimise_links_crowler(size_t position);

    /**
     * \brief Function for optimising links in state machine.
     */
    void optimise_links();

    /**
     * \brief Function for optimising state machine.
     */
    void optimise();

    /**
     * \brief           Function for concatenating 2 blocks.
     * \param[in] block New block for concatenating.
     */
    void concatenate_impl(const block& block);

    /**
     * \brief Function for concatenate many blocks.
     */
    void concatenate();

    /***
     * \brief Function for adding many strings.
     */
    void add_string();

    /**
     * \brief           Function for adding string to accepted list of this block.
     * \param[in] line  New string.
     */
    void add_string_impl(const std::string& line);

    /**
     * \brief               Function for adding many strings.
     * \tparam args         Other strings in stack.
     * \param[in] line      New string for adding.
     * \param[in] Args      Other strings for adding.
     */
    template <typename ...args>
    void add_string(const std::string& line, args ...Args);

public:

    /**
     * \brief               Constructor from vector of strings.
     * \param[in] lines     Vector of accepting strings.
     */
    explicit block(const std::vector<std::string>& lines);

    /**
     * \brief           Constructor from array of strings.
     * \tparam n        Number of strings in array.
     * \param[in] lines Strings for adding.
     */
    template <size_t n>
    explicit block(const std::array<std::string, n>& lines);

    /**
     * \brief           Constructor from many strings.
     * \tparam args     Other strings in stack.
     * \param[in] line1 First string in accepting lines.
     * \param[in] Args  Other strings.
     */
    template <typename ...args>
    explicit block(const std::string& line1, args ...Args);

    /**
     * \brief           Copy constructor from many blocks.
     * \tparam args     Other blocks in stack.
     * \param[in] other First block for constructing.
     * \param[in] Args  Other blocks.
     */
    template <typename ...args>
    explicit block(const block& other, args ...Args);

    /**
     * \brief           Move constructor from many blocks.
     * \tparam args     Other blocks in stack.
     * \param[in] other First block for constructing.
     * \param[in] Args  Other blocks.
     */
    template <typename ...args>
    explicit block(block&& other, args ...Args) noexcept;

    /**
     * \brief               Constructor from vector of blocks.
     * \param[in] blocks    Vector of blocks, which should be concatinate.
     */
    explicit block(const std::vector<block>& blocks);

    /**
     * \brief            Constructor from array of blocks.
     * \tparam n         Size of array.
     * \param[in] blocks Array of blocks, which should be concatinate.
     */
    template <size_t n>
    explicit block(const std::array<block, n>& blocks);

    /**
     * \brief           Copy constructor for making closure of block.
     * \param[in] other Block from which should be constructed.
     * \param[in] type  Type of closure.
     */
    block(const block& other, char type);

    /**
     * \brief           Move constructor for making closure of block.
     * \param[in] other Block from which should be constructed.
     * \param[in] type  Type of closure.
     */
    block(block&& other, char type) noexcept;

    /**
     * \brief Function for printing structure of state machine.
     */
    void print() const;

    /**
     * \brief           Function for concatinate many blocks to this.
     * \tparam args     Other blocks for concatinate.
     * \param[in] other First block for concatinate.
     * \param[in] Args  Other blocks.
     */
    template <typename ...args>
    void concatenate(const block& other, args ...Args);

    /**
     * \brief   Function for getting vertexes of state machine.
     * \return  Vector of vertexes.
     */
    std::vector<Vertex<_alphabet_size>> get_vertexes() const;

    /**
     * \brief   Function for getting root of state machine.
     * \return  Id of root.
     */
    size_t get_root() const noexcept;

    /**
     * \brief   Function for getting size of state machine.
     * \return  Number of vertexes in state machine.
     */
    size_t size() const noexcept;

    /**
     * \brief           Function for setting closure to these block.
     * \details         '?' - for 1 or 0 repetitions;
     *                  '*' - for 0 or inf repetitions.
     *                  '+' - for 1 or inf repetitions.
     * \param[in] type  Type of closure.
     */
    void add_closure(char type);
};

#include "block.inl"
