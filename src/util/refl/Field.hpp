#pragma once

#include "util/refl/FieldMeta.hpp"
#include "util/refl/Reserve.hpp"

#ifdef WIN32
#define GNEV_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define GNEV_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace gnev::refl {

template <IsFieldLeafMeta Meta_>
struct FieldLeaf {
    using Meta = Meta_;
    Meta::Type value;

private:
    [[msvc::no_unique_address]] Reserve<Meta::Size - sizeof(Meta::Type)> _;
};

template <IsFieldArrayMeta Array>
struct FieldArray;

template <IsFieldMapMeta Map>
struct FieldMap;

template <IsFieldMeta... Metas>
struct FieldArrayElem;

template <>
struct FieldArrayElem<> {
    template <std::size_t I>
    void get() {}
};

namespace details {
template <IsFieldLeafMeta Meta>
consteval FieldLeaf<Meta> deduceType();

template <IsFieldArrayMeta Meta>
consteval FieldArray<Meta> deduceType();

template <IsFieldMapMeta Meta>
consteval FieldMap<Meta> deduceType();
} // namespace details

template <IsFieldMeta Cur, IsFieldMeta... Others>
struct FieldArrayElem<Cur, Others...> {
    template <std::size_t I>
    auto& get() {
        if constexpr (I == 0) {
            return current;
        } else {
            return next.get<I - 1>();
        }
    }

    template <std::size_t I>
    const auto& get() const {
        if constexpr (I == 0) {
            return current;
        } else {
            return next.get<I - 1>();
        }
    }

    decltype(details::deduceType<Cur>()) current;
    GNEV_NO_UNIQUE_ADDRESS FieldArrayElem<Others...> next;
};

template <IsFieldArrayMeta Meta_>
struct FieldArrayList {
    using Meta = Meta_;

    template <std::size_t I>
    auto& get() {
        return first.get<I>();
    }

    template <std::size_t I>
    const auto& get() const {
        return first.get<I>();
    }

private:
    static consteval auto deduceFirstElem() {
        return []<std::size_t... I>(std::index_sequence<I...>) {
            return FieldArrayElem<std::tuple_element_t<I, typename Meta_::SubMetas>...>{};
        }(std::make_index_sequence<Meta::N>{});
    }

    decltype(deduceFirstElem()) first;
};

template <IsFieldMapMeta Meta_>
struct FieldMap {
    using Meta = Meta_;

    template <CtString Key>
    auto& get() {
        return list.get<KeyIndex<Key>>();
    }

    template <CtString Key>
    const auto& get() const {
        return list.get<KeyIndex<Key>>();
    }

private:
    static consteval auto deduceInfoList() {
        return []<std::size_t... I>(std::index_sequence<I...>) {
            return FieldArrayMeta<
                typename std::tuple_element_t<I, typename Meta_::SubMetas>::Value...>{};
        }(std::make_index_sequence<Meta::N>{});
    }

    template <CtString Key>
    static constexpr std::size_t KeyIndex =
        Meta::findKeyIndex<Key>(std::make_index_sequence<Meta::N>{});

    using InfoList = decltype(deduceInfoList());
    FieldArrayList<InfoList> list;
};

namespace details {
template <IsFieldLeafMeta Meta>
consteval FieldLeaf<Meta> deduceField();

template <IsFieldArrayMeta Meta>
consteval FieldArray<Meta> deduceField();

template <IsFieldMapMeta Meta>
consteval FieldMap<Meta> deduceField();
}; // namespace details

template <IsFieldMeta Meta>
using Struct = decltype(details::deduceField<Meta>());

} // namespace gnev::refl