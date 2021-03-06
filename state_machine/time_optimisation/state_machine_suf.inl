#include <queue>
#include <deque>


template<size_t _alphabet_size, size_t (*_character_caster)(char)>
state_machine_suf<_alphabet_size, _character_caster>::state_machine_suf(
    const _re_component<_alphabet_size, _character_caster>& block)
{
    std::vector<std::set<size_t>> from_new_to_old;
    std::unordered_map<std::set<size_t>, size_t, set_hash> from_old_to_new;

    _stop_vertex = create_vertex();
    _root = create_vertex();
    from_new_to_old.emplace_back(std::set<size_t>());
    from_new_to_old.emplace_back(closure(block, block.get_root()));
    from_old_to_new[from_new_to_old[_root]] = _stop_vertex;

    std::queue<size_t> q;
    q.push(_root);

    while(!q.empty())
    {
        auto v = q.front();
        q.pop();

        auto v_closure = from_new_to_old[v];

        for(size_t char_index=0; char_index<_alphabet_size; ++char_index)
        {
            std::set<size_t> new_vertexes;

            for(const auto& it: v_closure)
            {
                if(block.get_vertexes().at(it)._is_finished)
                {
                    _vertexes.at(v)._is_finished=true;
                }

                auto local_set = closure(block, block.get_vertexes().at(it).get_link(char_index));
                for(const auto& elem: local_set)
                {
                    new_vertexes.insert(elem);
                }
            }

            if(from_old_to_new.count(new_vertexes))
            {
                _vertexes.at(v).set_link(char_index, from_old_to_new[new_vertexes]);
            }
            else
            {
                auto nv = create_vertex();
                from_new_to_old.emplace_back(new_vertexes);
                from_old_to_new[new_vertexes] = nv;
                _vertexes.at(v).set_link(char_index, nv);
                q.push(nv);
            }
        }

    }

    optimise();
}

template<size_t _alphabet_size, size_t (*_character_caster)(char)>
void state_machine_suf<_alphabet_size, _character_caster>::print()
{
    std::cout<<"root: "<<_root<<" end:"<<_stop_vertex<<'\n';
    for(size_t id=0;id<_vertexes.size();++id){
        std::cout<<id<<'|'<<_vertexes.at(id)._is_finished<<'|';
        for(size_t char_index = 0; char_index < _alphabet_size; ++char_index)
        {
            std::cout<<_vertexes.at(id).get_link(char_index)<<' ';
        }
        std::cout<<'\n';
    }
}

template<size_t _alphabet_size, size_t (*_character_caster)(char)>
std::vector<size_t>
state_machine_suf<_alphabet_size, _character_caster>::find_start_positions(
    const std::string& line, size_t index)
{
    std::vector<size_t> res;

    size_t pos = _root;

    while(pos != _stop_vertex && index >0)
    {
        pos = _vertexes.at(pos).get_link(_character_caster(line.at(index)));

        if(_vertexes.at(pos)._is_finished)
        {
            res.emplace_back(index);
        }
        --index;
    }
    if(index == 0)
    {
        pos = _vertexes.at(pos).get_link(_character_caster(line.at(index)));

        if(_vertexes.at(pos)._is_finished)
        {
            res.emplace_back(index);
        }
    }

    return res;
}

template<size_t _alphabet_size, size_t (*_character_caster)(char)>
void state_machine_suf<_alphabet_size, _character_caster>::optimise()
{
    std::vector<std::vector<bool>> is_equal(size());
    bool was_change = true;

    for(size_t id=1;id<is_equal.size();++id)
    {
        is_equal.at(id).resize(id,true);
    }

    for(size_t idl=0; idl<size();++idl)
    {
        for(size_t idr = idl+1; idr<size();++idr)
        {
            if(_vertexes.at(idl)._is_finished ^ _vertexes.at(idr)._is_finished)
            {
                is_equal.at(idr).at(idl) = false;
            }
        }
    }

    while(was_change)
    {
        was_change = false;
        for(size_t idl=0; idl<size();++idl)
        {
            for(size_t idr = idl+1; idr<size();++idr)
            {
                for(size_t char_id=0; char_id<_alphabet_size; ++char_id)
                {
                    auto l = _vertexes.at(idl).get_link(char_id);
                    auto r = _vertexes.at(idr).get_link(char_id);
                    if(l>r)
                    {
                        std::swap(l,r);
                    }

                    if(l!=r && !is_equal.at(r).at(l) && is_equal.at(idr).at(idl))
                    {
                        is_equal.at(idr).at(idl) = false;
                        was_change = true;
                    }
                }
            }
        }
    }

    std::vector<size_t> dict(size());
    std::deque<bool> was(size(), false);
    std::deque<bool> finish;
    std::vector<size_t> rdict;

    size_t new_size = 0;

    for(size_t id=0;id<dict.size();++id)
    {
        if(!was.at(id))
        {
            rdict.push_back(id);
            was[id] = true;
            dict[id] = new_size;
            finish.push_back(_vertexes.at(id)._is_finished);
            for(size_t id2 = id+1; id2<dict.size(); ++id2)
            {
                if(is_equal.at(id2).at(id))
                {
                    finish.back() |= _vertexes.at(id2)._is_finished;
                    was[id2] = true;
                    dict[id2] = new_size;
                }
            }
            ++new_size;
        }
    }

    std::vector<_vertex<_alphabet_size>> new_vertexes(new_size);

    for(size_t id=0; id < new_size;++id)
    {
        new_vertexes.at(id)._is_finished = finish.at(id);
        for(size_t char_id=0; char_id < _alphabet_size; ++char_id)
        {
            new_vertexes.at(id).set_link(char_id, dict[_vertexes.at(rdict[id]).get_link(char_id)]);
        }
    }

    _root = dict[_root];
    _stop_vertex = dict[_stop_vertex];
    _vertexes = new_vertexes;

}

template<size_t _alphabet_size, size_t (*_character_caster)(char)>
std::set<size_t> state_machine_suf<_alphabet_size, _character_caster>::closure(
    const _re_component<_alphabet_size, _character_caster>& block, size_t id)
{
    if(id == 0)
    {
        return {};
    }

    // optimise it more memory less queue
    std::set<size_t> res;
    std::queue<size_t> q;
    q.push(id);

    while(!q.empty())
    {
        auto v = q.front();
        q.pop();

        if(!res.count(v))
        {
            res.insert(v);
            for(const auto& new_vertex:block.get_vertexes().at(v).get_eps_links())
            {
                if(new_vertex)
                {
                    q.push(new_vertex);
                }
            }
        }
    }

    return res;
}

template<size_t _alphabet_size, size_t (*_character_caster)(char)>
size_t state_machine_suf<_alphabet_size, _character_caster>::create_vertex()
{
    _vertexes.emplace_back(_vertex<_alphabet_size>());
    return _vertexes.size()-1;
}

template<size_t _alphabet_size, size_t (*_character_caster)(char)>
size_t
state_machine_suf<_alphabet_size, _character_caster>::size() const noexcept
{
    return _vertexes.size();
}
