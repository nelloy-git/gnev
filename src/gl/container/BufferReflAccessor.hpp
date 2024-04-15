#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>

#include "gl/container/IBufferRawAccessor.hpp"
#include "util/Logger.hpp"
#include "util/Reflection.hpp"

namespace gnev::gl {

template <typename T>
static constexpr bool HasDefaultAlignmentV =
    sizeof(T) == sizeof(decltype(pfr::structure_to_tuple<T>(std::declval<T>())));

template <typename T>
concept IsReflectible =
    pfr::is_implicitly_reflectable_v<T, void> and std::is_trivially_copyable_v<T> and HasDefaultAlignmentV<T>;

template <IsReflectible T>
class BufferReflAccessor {
public:
    using Meta = refl::Meta<T>;
    template <auto... Keys>
    using SubType = typename Meta::template DeduceMember<Keys...>::type;
    template <auto... Keys>
    static constexpr std::size_t SubTypeOffset = Meta::template Offset<Keys...>();

    template <typename V>
    using Changer = std::function<void(V&)>;

    BufferReflAccessor(const std::shared_ptr<IBufferRawAccessor>& accessor,
                       std::size_t base_offset)
        : accessor{accessor}
        , base_offset{base_offset} {}

    template<auto... Keys>
        requires(sizeof...(Keys) > 0)
    auto sub(){
        return BufferReflAccessor<SubType<Keys...>>{accessor, base_offset + SubTypeOffset<Keys...>};
    }

    void set(const T& value) {
        bool success = accessor->set(base_offset, sizeof(T), &value);
        if (not success) {
            Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    template <auto... Keys>
        requires(sizeof...(Keys) > 0)
    void set(const SubType<Keys...>& value) {
        bool success = accessor->set(base_offset + SubTypeOffset<Keys...>,
                                     sizeof(SubType<Keys...>),
                                     &value);
        if (not success) {
            Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    T get() const {
        T dst;
        bool success = accessor->get(base_offset, sizeof(T), &dst);
        if (not success) {
            Ctx::Get().getLogger().log<LogLevel::ERROR, "Failed">();
        }
        return dst;
    }

    template <auto... Keys>
        requires(sizeof...(Keys) > 0)
    auto get() const {
        SubType<Keys...> dst;
        bool success = accessor->get(base_offset + SubTypeOffset<Keys...>,
                                     sizeof(SubType<Keys...>),
                                     &dst);
        if (not success) {
            Ctx::Get().getLogger().log<LogLevel::ERROR, "Failed">();
        }
        return dst;
    }

    void change(const Changer<T>& changer) const {
        bool success = accessor->change(base_offset, sizeof(T), changer);
        if (not success) {
            Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    template <auto... Keys>
        requires(sizeof...(Keys) > 0)
    void change(const Changer<SubType<Keys...>>& changer) {
        bool success = accessor->change(base_offset + SubTypeOffset<Keys...>,
                                        sizeof(SubType<Keys...>),
                                        changer);
        if (not success) {
            Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    void copy(const BufferReflAccessor<T>& src) {
        bool success = accessor->copy(src.base_offset, base_offset, sizeof(T));
        if (not success) {
            Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    template <auto... Keys>
        requires(sizeof...(Keys) > 0)
    void copy(const BufferReflAccessor<T>& src) {
        bool success = accessor->copy(src.base_offset + SubTypeOffset<Keys...>,
                                      base_offset + SubTypeOffset<Keys...>,
                                      sizeof(SubType<Keys...>));
        if (not success) {
            Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

private:
    std::shared_ptr<IBufferRawAccessor> accessor;
    const std::size_t base_offset;
};

} // namespace gnev::gl