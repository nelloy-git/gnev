#pragma once

#include <unordered_set>
#include <variant>

#define EXPORT __declspec(dllexport)

namespace gnev {

template<std::integral T = size_t>
class IdGen {
public:
    IdGen(T start = 0)
        : current(0)
    {
    }

    T gen()
    {
        if (unused.size() > 0){
            return unused.extract(unused.begin());
        } else {
            return current++;
        }
    }

    void free(T id)
    {
        unused.insert(id);
    }

private:
    T current;
    std::unordered_set<T> unused;
};

}
