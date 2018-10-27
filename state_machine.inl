//
// Created by gusef on 25.10.2018.
//

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
state_machine<_alphabet_size, _character_caster>::state_machine(
    block<_alphabet_size, _character_caster>& block)
: _block(block)
, _current_states(0)
{
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
std::vector<size_t> state_machine<_alphabet_size, _character_caster>::find_end_positions(
    const std::string& line, bool info)
{
    if (line.empty())
    {
        return {};
    }

    _active_states.at(_current_states).clear();
    closure(_block.get_root(), _current_states);
    std::vector<size_t> result;
    for (size_t symbol_number = 0; symbol_number < line.size(); ++symbol_number)
    {
        if (info)
        {
            std::cout << symbol_number << '|';
        }
        size_t next_states = _current_states ^1;
        _active_states.at(next_states).clear();
        closure(_block.get_root(), next_states);
        for (auto& state : _active_states.at(_current_states))
        {
            if (info)
            {
                std::cout << state << ' ';
            }
            if (_block.get_vertexes().at(state)._is_finished)
            {
                result.emplace_back(symbol_number - 1);
            }
            size_t next_vertex = _block.get_vertexes().at(state).get_next(
                _character_caster(line[symbol_number]));
            closure(next_vertex, next_states);
        }
        if (info)
        {
            std::cout << '\n';
        }
        _current_states = next_states;
    }
    for (auto& state : _active_states.at(_current_states))
    {
        if (_block.get_vertexes().at(state)._is_finished)
        {
            result.emplace_back(line.size() - 1);
        }
    }
    return result;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void state_machine<_alphabet_size, _character_caster>::closure(
    const size_t position, const size_t state_buffer)
{
    if (!_active_states.at(state_buffer).count(position))
    {
        _active_states.at(state_buffer).insert(position);
        for (auto& vertex : _block.get_vertexes().at(position).get_eps_links())
        {
            closure(vertex, state_buffer);
        }
    }
}
