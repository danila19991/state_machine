//
// Created by gusef on 25.10.2018.
//

template <size_t _alphabet_size>
vertex<_alphabet_size>::vertex()
: _eps_links()
, _links()
, _is_finished(false)
{
}

template <size_t _alphabet_size>
constexpr void vertex<_alphabet_size>::set_link(size_t position, size_t destination)
{
    _links.at(position) = destination;
}

template <size_t _alphabet_size>
constexpr size_t vertex<_alphabet_size>::get_next(size_t position) const
{
    return _links.at(position);
}

template <size_t _alphabet_size>
void vertex<_alphabet_size>::add_eps_link(size_t destination)
{
    _eps_links.emplace_back(destination);
}

template <size_t _alphabet_size>
std::vector<size_t> vertex<_alphabet_size>::get_eps_links() const
{
    return _eps_links;
}
