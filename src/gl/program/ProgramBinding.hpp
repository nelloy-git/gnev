#pragma once

#include <unordered_map>

#include "util/IndexManager.hpp"

namespace gnev::gl {

template <typename T>
class EXPORT ProgramBinding {
public:
    ProgramBinding(unsigned int capacity)
        : binds(capacity) {}

    // links loc_index -> binding_index -> object
    // return binding_index
    std::optional<unsigned int> set(unsigned int loc_index, const std::optional<T> obj) {
        // Check is loc_index already in use
        auto iter = map.find(loc_index);
        if (iter != map.end()) {
            unsigned int binding = iter->second.first;
            binds.freeIndex(binding);
        }

        if (not obj.has_value()) {
            return std::nullopt;
        }

        auto binding_index_opt = binds.reserveIndex();
        if (not binding_index_opt) {
            return std::nullopt;
        }
        map[loc_index] = {binding_index_opt.value(), obj.value()};
        return binding_index_opt.value();
    }

private:
    IndexManager binds;
    std::unordered_map<unsigned int, std::pair<unsigned int, T>> map;
};

} // namespace gnev::gl