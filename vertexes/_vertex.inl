
template <size_t _alphabet_size>
_vertex<_alphabet_size>::_vertex()
    : _links()
    , _is_finished(false)
{
}

template <size_t _alphabet_size>
constexpr void _vertex<_alphabet_size>::set_link(const size_t position, const size_t destination)
{
    _links.at(position) = destination;
}

template <size_t _alphabet_size>
constexpr size_t _vertex<_alphabet_size>::get_link(const size_t position) const
{
    return _links.at(position);
}