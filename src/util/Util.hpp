#pragma once

#include <type_traits>
#include <unordered_set>
#include <variant>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace gnev {

template <typename T>
concept IsTriviallyCopyable = std::is_trivially_copyable_v<T>;

template <std::integral T = std::size_t>
class IdGen {
public:
    IdGen(T start = 0)
        : current(0) {}

    T gen() {
        if (unused.size() > 0) {
            return unused.extract(unused.begin());
        } else {
            return current++;
        }
    }

    void free(T id) { unused.insert(id); }

private:
    T current;
    std::unordered_set<T> unused;
};

} // namespace gnev
