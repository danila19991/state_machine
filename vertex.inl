//
// Created by gusef on 25.10.2018.
//

template <size_t ALPHABET_SIZE>
vertex<ALPHABET_SIZE>::vertex()
: _eps_links()
, _links()
, _is_finished(false)
{
}

template <size_t ALPHABET_SIZE>
constexpr void vertex<ALPHABET_SIZE>::set_link(const size_t position, const size_t destination)
{
    _links.at(position) = destination;
}

template <size_t ALPHABET_SIZE>
constexpr size_t vertex<ALPHABET_SIZE>::get_next(const size_t position) const
{
    return _links.at(position);
}

template <size_t ALPHABET_SIZE>
void vertex<ALPHABET_SIZE>::add_eps_link(const size_t destination)
{
    _eps_links.emplace_back(destination);
}

template <size_t ALPHABET_SIZE>
std::vector<size_t> vertex<ALPHABET_SIZE>::get_eps_links() const
{
    return _eps_links;
}
