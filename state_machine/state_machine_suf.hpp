//
// Created by gusef on 02.11.2018.
//

#ifndef STATE_MACHINE_STATE_MACHINE_SUF_HPP
#define STATE_MACHINE_STATE_MACHINE_SUF_HPP

#include "../vertexes/_vertex.hpp"
#include "../re_compnent/_re_component.hpp"

#include <vector>
#include <string>
#include <set>

class set_hash {
    std::hash<unsigned long long> hasher;
public:
    inline std::size_t operator()(const std::set<size_t> & v) const {
        std::size_t res = 0;
        for(const auto& it:v)
        {
            auto loc_hash = hasher(it);
            res = res*1373 + ((loc_hash<<7)^(loc_hash>>7));
        }
        return res;
    }
};

template <size_t _alphabet_size, size_t (* _character_caster)(char)>
class state_machine_suf
{
    std::vector<_vertex<_alphabet_size>> _vertexes;

    size_t _root;

    size_t _stop_vertex;

    size_t create_vertex();

    std::set<size_t> closure(const _re_component<_alphabet_size, _character_caster>& block, size_t id);

    void optimise();

public:

    state_machine_suf(const _re_component<_alphabet_size, _character_caster>& block);

    std::vector<size_t> find_start_positions(const std::string& line, size_t index);

    void print();

    size_t size() const noexcept;
};

#include "state_machine_suf.inl"

#endif //STATE_MACHINE_STATE_MACHINE_SUF_HPP
