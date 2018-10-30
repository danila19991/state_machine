//
// Created by gusef on 25.10.2018.
//

// Overloaded shift operator for printing founded patters.
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::pair<T, T>& elem)
{
    out << '[' << elem.first << ',' << elem.second << ']';
    return out;
}

template <size_t ALPHABET_SIZE, size_t (*CHARACTER_CASTER)(char)>
state_machine<ALPHABET_SIZE, CHARACTER_CASTER>::state_machine(
    block<ALPHABET_SIZE, CHARACTER_CASTER>& block)
: _block(block)
, _current_states(0)
{
}

template <size_t ALPHABET_SIZE, size_t (*CHARACTER_CASTER)(char)>
std::vector<std::pair<size_t,size_t>> state_machine<ALPHABET_SIZE, CHARACTER_CASTER>::find_end_positions(
    const std::string& line, const bool info)
{
    _current_length = 0;
    clear_states(_current_states, _current_length);
    return append_search(line, info);
}

template <size_t ALPHABET_SIZE, size_t (* CHARACTER_CASTER)(char)>
void state_machine<ALPHABET_SIZE, CHARACTER_CASTER>::closure(
    const size_t position, const size_t state_buffer, const size_t start_pos)
{
    if (position != _block.get_root())
    {
        closure_impl(position, state_buffer, start_pos);
    }
}

template<size_t ALPHABET_SIZE, size_t (*CHARACTER_CASTER)(char)>
void state_machine<ALPHABET_SIZE, CHARACTER_CASTER>::clear_states(const size_t id, const size_t position)
{
    _active_states.at(id).clear();
    closure_impl(_block.get_root(), id, position);
}

template<size_t ALPHABET_SIZE, size_t (*CHARACTER_CASTER)(char)>
std::vector<std::pair<size_t,size_t>> state_machine<ALPHABET_SIZE, CHARACTER_CASTER>::append_search(
    const std::string& line, const bool info)
{
    if (line.empty())
    {
        return {};
    }
    std::vector<std::pair<size_t,size_t>> result;
    for (size_t symbol_number = 0; symbol_number < line.size(); ++symbol_number)
    {
        const size_t next_states = _current_states^1;
        clear_states(next_states, _current_length+1);
        for(const auto& state : _active_states.at(_current_states))
        {
            const size_t next_vertex = _block.get_vertexes().at(state.first).get_next(
                CHARACTER_CASTER(line[symbol_number])
            );
            closure(next_vertex, next_states, state.second);
        }

        if(info)
        {
            std::cout << _current_length << '|';
        }
        for(const auto& state : _active_states.at(next_states))
        {
            if(info)
            {
                std::cout << state << ' ';
            }
            if(_block.get_vertexes().at(state.first)._is_finished)
            {
                result.emplace_back(std::make_pair(state.second, _current_length));
            }
        }
        if(info)
        {
            std::cout << '\n';
        }

        _current_states = next_states;
        ++_current_length;

    }
    return result;
}

template<size_t ALPHABET_SIZE, size_t (*CHARACTER_CASTER)(char)>
void state_machine<ALPHABET_SIZE, CHARACTER_CASTER>::closure_impl(
    const size_t position, const size_t state_buffer, const size_t start_pos)
{
    if (!_active_states.at(state_buffer).count(std::make_pair(position,start_pos)))
    {
        _active_states.at(state_buffer).insert(std::make_pair(position,start_pos));
        for (const auto& vertex : _block.get_vertexes().at(position).get_eps_links())
        {
            closure(vertex, state_buffer, start_pos);
        }
    }
}
