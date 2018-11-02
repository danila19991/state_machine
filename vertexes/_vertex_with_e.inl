//
// Created by gusef on 25.10.2018.
//

template <size_t _alphabet_size>
_vertex_with_e<_alphabet_size>::_vertex_with_e()
: _eps_links()
, _links()
, _is_finished(false)
{
}

template <size_t _alphabet_size>
constexpr void _vertex_with_e<_alphabet_size>::set_link(const size_t position, const size_t destination)
{
    _links.at(position) = destination;
}

template <size_t _alphabet_size>
constexpr size_t _vertex_with_e<_alphabet_size>::get_link(const size_t position) const
{
    return _links.at(position);
}

template <size_t _alphabet_size>
void _vertex_with_e<_alphabet_size>::add_eps_link(const size_t destination)
{
    _eps_links.emplace_back(destination);
}

template <size_t _alphabet_size>
std::vector<size_t> _vertex_with_e<_alphabet_size>::get_eps_links() const
{
    return _eps_links;
}
