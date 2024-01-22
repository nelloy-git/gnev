#pragma once

#include <unordered_map>

#include "util/IndexStorage.hpp"
#include "util/Log.hpp"
#include "util/Ref.hpp"

namespace gnev::gl {

template <typename T>
class ProgramBinding {
public:
    ProgramBinding(unsigned int capacity)
        : binds(capacity) {}

    // binds loc_index -> binding_index -> object
    // return binding_index
    std::optional<unsigned int> set(unsigned int loc_index, const Ptr<T> obj) {
        // Check is loc_index already in use
        auto iter = map.find(loc_index);
        if (iter != map.end()) {
            unsigned int binding = iter->second.first;
            binds.freeIndex(binding);
        }

        auto binding_index_opt = binds.useIndex();
        if (not binding_index_opt) {
            return std::nullopt;
        }
        map[loc_index] = {binding_index_opt.value(), obj};
        return binding_index_opt.value();
    }

private:
    IndexStorage binds;
    std::unordered_map<unsigned int, std::pair<unsigned int, Ptr<T>>> map;
};

} // namespace gnev::gl