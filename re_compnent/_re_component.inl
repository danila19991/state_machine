//
// Created by gusef on 25.10.2018.
//

#include <algorithm>
#include <deque>
#include <unordered_map>


template <size_t _alphabet_size, size_t (* _character_caster)(char)>
_re_component<_alphabet_size, _character_caster>::_re_component(const std::vector<std::string>& lines):
    _vertexes(1)
{
    _root = new_vertex();
    _finish_vertex = new_vertex();
    _vertexes.at(_finish_vertex)._is_finished = true;
    for (const auto& line : lines)
    {
        add_string_impl(line);
    }
    optimise();
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
template <size_t n>
_re_component<_alphabet_size, _character_caster>::_re_component(const std::array<std::string, n>& lines):
    _vertexes(1)
{
    _root = new_vertex();
    _finish_vertex = new_vertex();
    _vertexes.at(_finish_vertex)._is_finished = true;
    for (const auto& line : lines)
    {
        add_string_impl(line);
    }
    optimise();
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
_re_component<_alphabet_size, _character_caster>::_re_component(
    const std::vector<_re_component<_alphabet_size, _character_caster>>& blocks):
    _vertexes(1)
{
    if (!blocks.empty())
    {
        _re_component tmp_block(blocks.front());

        for (size_t bloc_number = 1; bloc_number < blocks.size(); ++bloc_number)
        {
            tmp_block.concatenate_impl(blocks.at(bloc_number));
        }

        std::swap(tmp_block, *this);
    }
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
template <size_t n>
_re_component<_alphabet_size, _character_caster>::_re_component(
    const std::array<_re_component<_alphabet_size, _character_caster>, n>& blocks):
    _vertexes(1)
{
    if (!blocks.empty())
    {
        _re_component tmp_block(blocks.front());

        for (size_t block_number = 1; block_number < blocks.size(); ++block_number)
        {
            tmp_block.concatenate_impl(blocks.at(block_number));
        }

        std::swap(tmp_block, *this);
    }
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
template <typename... Args>
_re_component<_alphabet_size, _character_caster>::_re_component(const std::string& line1, const Args&... args):
    _vertexes(1)
{
    _root = new_vertex();
    _finish_vertex = new_vertex();
    _vertexes.at(_finish_vertex)._is_finished = true;
    add_string_impl(line1);
    add_string(args...);
    optimise();
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
_re_component<_alphabet_size, _character_caster>::_re_component(const _re_component& other, const char type)
{
    _re_component tmp_block(other);
    tmp_block.add_closure(type);
    std::swap(tmp_block, *this);
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
_re_component<_alphabet_size, _character_caster>::_re_component(_re_component&& other, const char type) noexcept
{
    _re_component tmp_block(other);
    tmp_block.add_closure(type);
    std::swap(tmp_block, *this);
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
template <typename... Args>
_re_component<_alphabet_size, _character_caster>::_re_component(
    const _re_component<_alphabet_size, _character_caster>& other, const Args&... args)
{
    _re_component tmp_block(other);
    tmp_block.concatenate(args...);
    std::swap(tmp_block, *this);
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
template <typename... Args>
_re_component<_alphabet_size, _character_caster>::_re_component(
    _re_component<_alphabet_size, _character_caster>&& other, Args&&... args) noexcept
{
    _re_component tmp_block(other);
    tmp_block.concatenate(std::forward<Args>(args)...);
    std::swap(tmp_block, *this);
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
size_t _re_component<_alphabet_size, _character_caster>::new_vertex()
{
    _vertexes.emplace_back(_vertex_with_e<_alphabet_size>());
    return _vertexes.size() - 1;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::optimise_vertexes()
{
    std::vector<_vertex_with_e<_alphabet_size>> new_vertexes(2);

    std::deque<bool> was_in_vertex(_vertexes.size(), false);
    was_in_vertex.at(_root) = true;
    was_in_vertex.at(0) = true;

    const size_t root = 1;

    std::unordered_map<size_t, size_t> vertexes_dict{{0, 0}, {_root, root}};

    std::deque<size_t> q_vertexes{_root}; // use deque as queue.
    while (!q_vertexes.empty())
    {
        const size_t vertex_front = q_vertexes.front();
        const size_t new_vertex = vertexes_dict[vertex_front];
        q_vertexes.pop_front();

        const auto current_vertexes = _vertexes.at(vertex_front);
        for (size_t i = 0; i < _alphabet_size; ++i)
        {
            if (!was_in_vertex.at(current_vertexes.get_link(i)))
            {
                new_vertexes.emplace_back(_vertex_with_e<_alphabet_size>());
                vertexes_dict[current_vertexes.get_link(i)] = new_vertexes.size() - 1;
                was_in_vertex.at(current_vertexes.get_link(i)) = true;
                q_vertexes.push_front(current_vertexes.get_link(i));
            }
            new_vertexes.at(new_vertex).set_link(i, vertexes_dict[current_vertexes.get_link(i)]);
        }

        // move to range-base for
        const auto eps_links = current_vertexes.get_eps_links();
        for (size_t link_number = 0; link_number < eps_links.size(); ++link_number)
        {
            if (!was_in_vertex.at(eps_links.at(link_number)))
            {
                new_vertexes.emplace_back(_vertex_with_e<_alphabet_size>());
                vertexes_dict[eps_links.at(link_number)] = new_vertexes.size() - 1;
                was_in_vertex.at(eps_links.at(link_number)) = true;
                q_vertexes.push_front(eps_links.at(link_number));
            }
            new_vertexes.at(new_vertex).add_eps_link(vertexes_dict[eps_links.at(link_number)]);
        }
    }

    _root = root;
    _finish_vertex = vertexes_dict[_finish_vertex];
    _vertexes = new_vertexes;
    _vertexes.at(_finish_vertex)._is_finished = true;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
bool _re_component<_alphabet_size, _character_caster>::is_equal_sub_trees(const size_t vertex1,
                                                                  const size_t vertex2)
{
    if (vertex1 == vertex2) return true;

    // check if need to check vertexes with root
    if ((vertex1 == _root && vertex2 != _root) || (vertex1 != _root && vertex2 == _root) ||
        (vertex1 == 0 && vertex2 != 0) || (vertex1 != 0 && vertex2 == 0))
        return false;

    auto eps_links1 = _vertexes.at(vertex1).get_eps_links();
    auto eps_links2 = _vertexes.at(vertex2).get_eps_links();

    if (eps_links1.size() != eps_links2.size()) return false;

    std::sort(eps_links1.begin(), eps_links1.end());
    std::sort(eps_links2.begin(), eps_links2.end());

    if (eps_links1 != eps_links2) return false;

    for (size_t i = 0; i < _alphabet_size; ++i)
    {
        if (_vertexes.at(vertex1).get_link(i) != _vertexes.at(vertex2).get_link(i) &&
            !is_equal_sub_trees(_vertexes.at(vertex1).get_link(i), _vertexes.at(vertex2).get_link(i)))
            return false;
    }

    return true;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::optimise_links()
{
    _sub_tree_hashes.clear();
    optimise_links_crawler(_root);
    _sub_tree_hashes.clear();
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
unsigned long long _re_component<_alphabet_size, _character_caster>::optimise_links_crawler(
    const size_t position)
{
    unsigned long long current_hash = 1;
    for (size_t alphabet_index = 0; alphabet_index < _alphabet_size; ++alphabet_index)
    {
        if (_vertexes.at(position).get_link(alphabet_index) != _root &&
            _vertexes.at(position).get_link(alphabet_index) != 0)
        {
            const auto sub_tree_hash = optimise_links_crawler(
                _vertexes.at(position).get_link(alphabet_index)
            );

            bool flag = false;
            for (const auto& may_be_clone : _sub_tree_hashes[sub_tree_hash])
            {
                if (is_equal_sub_trees(may_be_clone,
                                       _vertexes.at(position).get_link(alphabet_index)))
                {
                    flag = true;
                    _vertexes.at(position).set_link(alphabet_index, may_be_clone);
                    break;
                }
            }
            if (!flag)
            {
                _sub_tree_hashes[sub_tree_hash].emplace_back(
                    _vertexes.at(position).get_link(alphabet_index)
                );
            }

            // may be change to other type of hashing
            current_hash += BASE1 * alphabet_index + BASE2 * sub_tree_hash;
        }
    }
    return current_hash;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::optimise()
{
    optimise_links();
    optimise_vertexes();
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::print() const
{
    std::cout << "root: " << _root << " finish state: " << _finish_vertex << '\n';
    for (size_t vertex = 0; vertex < _vertexes.size(); ++vertex)
    {
        std::cout << vertex << '|' << _vertexes.at(vertex)._is_finished << '|';
        for (size_t j = 0; j < _alphabet_size; ++j)
        {
            std::cout << _vertexes.at(vertex).get_link(j) << ' ';
        }
        std::cout << '|';
        for (const auto& eps_link : _vertexes.at(vertex).get_eps_links())
        {
            std::cout << eps_link << ' ';
        }
        std::cout << '\n';
    }
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
std::vector<_vertex_with_e<_alphabet_size>> _re_component<_alphabet_size, _character_caster>::get_vertexes() const
{
    return _vertexes;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
size_t _re_component<_alphabet_size, _character_caster>::get_root() const noexcept
{
    return _root;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
size_t _re_component<_alphabet_size, _character_caster>::size() const noexcept
{
    return _vertexes.size();
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::concatenate_impl(const _re_component& block)
{
    _vertexes.reserve(size() + block.size());

    std::deque<bool> was_in_vertex(block.size(), false);
    was_in_vertex.at(block.get_root()) = true;

    std::unordered_map<size_t, size_t> vertexes_dict{{0, 0}, {block.get_root(), _root}};
    _vertexes.at(_finish_vertex)._is_finished = false;

    std::deque<size_t> q_vertexes{block.get_root()}; // use deque as queue.
    while (!q_vertexes.empty())
    {
        const size_t vertex_from_other = q_vertexes.front();
        size_t current_vertex = vertexes_dict[vertex_from_other];
        if (vertex_from_other == block.get_root())
        {
            current_vertex = _finish_vertex;
        }
        q_vertexes.pop_front();

        const auto current_vertexes = block.get_vertexes().at(vertex_from_other);
        for (size_t alphabet_index = 0; alphabet_index < _alphabet_size; ++alphabet_index)
        {
            if (!was_in_vertex.at(current_vertexes.get_link(alphabet_index)))
            {
                vertexes_dict[current_vertexes.get_link(alphabet_index)] = new_vertex();
                was_in_vertex.at(current_vertexes.get_link(alphabet_index)) = true;
                q_vertexes.push_front(current_vertexes.get_link(alphabet_index));
            }
            _vertexes.at(current_vertex).set_link(
                alphabet_index, vertexes_dict[current_vertexes.get_link(alphabet_index)]
            );
        }

        const auto eps_links = current_vertexes.get_eps_links();
        for (size_t eps_link = 0; eps_link < eps_links.size(); ++eps_link)
        {
            if (!was_in_vertex.at(eps_links.at(eps_link)))
            {
                vertexes_dict[eps_links.at(eps_link)] = new_vertex();
                was_in_vertex.at(eps_links.at(eps_link)) = true;
                q_vertexes.push_front(eps_links.at(eps_link));
            }
            if (vertexes_dict[eps_links.at(eps_link)] != _root)
            {
                _vertexes.at(current_vertex).add_eps_link(vertexes_dict[eps_links.at(eps_link)]);
            }
            else
            {
                _vertexes.at(current_vertex).add_eps_link(_finish_vertex);
            }
        }
    }
    _finish_vertex = vertexes_dict[block._finish_vertex];
    _vertexes.at(_finish_vertex)._is_finished = true;
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::add_closure(const char type)
{
    switch(type)
    {
        case '?':
            _vertexes.at(_root).add_eps_link(_finish_vertex);
            break;

        case '+':
            _vertexes.at(_finish_vertex).add_eps_link(_root);
            break;

        case '*':
            _vertexes.at(_root).add_eps_link(_finish_vertex);
            _vertexes.at(_finish_vertex).add_eps_link(_root);
            break;

        default:
            std::cout << "Not supported closure type!\n";
            break;
    }
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::concatenate()
{
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
template <typename... Args>
void _re_component<_alphabet_size, _character_caster>::concatenate(
    const _re_component<_alphabet_size, _character_caster>& other, const Args&... args)
{
    concatenate_impl(other);
    concatenate(args...);
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::add_string()
{
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
template <typename... Args>
void _re_component<_alphabet_size, _character_caster>::add_string(const std::string& line,
                                                          const Args&... args)
{
    add_string_impl(line);
    add_string(args...);
}

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
void _re_component<_alphabet_size, _character_caster>::add_string_impl(const std::string& line)
{
    size_t pos = _root;
    for (const auto& symbol : line)
    {
        const size_t symbol_id = _character_caster(symbol);
        if (_vertexes.at(pos).get_link(symbol_id) == 0)
        {
            const size_t next_vertex = new_vertex();
            _vertexes.at(pos).set_link(symbol_id, next_vertex);
            pos = next_vertex;
        }
        else
        {
            pos = _vertexes.at(pos).get_link(symbol_id);
        }
    }
    _vertexes.at(pos).add_eps_link(_finish_vertex);
}
