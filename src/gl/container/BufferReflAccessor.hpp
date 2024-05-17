#pragma once

#include <cstddef>
#include <memory>

#include "boost/preprocessor/comparison/greater.hpp"
#include "gl/container/IBufferAccessor.hpp"
#include "util/Log.hpp"
#include "util/Reflection.hpp"
#include "util/SrcLoc.hpp"

#define GNEV_REFL_ACCESSOR_LOG(Level, Type, ...)                                            \
    BOOST_PP_CAT(GNEV_LOG_, Level)                                                          \
    (BOOST_PP_IF(BOOST_PP_GREATER(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 0),                  \
                 "{}#{}::{}<{}>|" GNEV_REPEAT_STR("{}",                                     \
                                                  "->",                                     \
                                                  BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)) "|", \
                 "{}#{}::{}<{}>"),                                                          \
     GNEV_GET_TYPE_NAME(std::remove_pointer_t<decltype(this)>),                             \
     this->getBuffer().handle(),                                                            \
     GNEV_GET_FUNC_NAME,                                                                    \
     GNEV_GET_TYPE_NAME(Type),                                                              \
     ##__VA_ARGS__)

namespace gnev::gl {

namespace details {

template <IsTriviallyCopyable T>
class BufferReflAccessorImpl {
public:
    template <typename V>
    using Changer = std::function<void(V&)>;

    BufferReflAccessorImpl(Buffer& buffer,
                           IBufferAccessor& accessor,
                           std::size_t base_offset)
        : buffer{buffer}
        , accessor{accessor}
        , base_offset{base_offset} {}

    Buffer& getBuffer() { return buffer; }

    const Buffer& getBuffer() const { return buffer; }

    void set(const T& value) {
        GNEV_REFL_ACCESSOR_LOG(L1, T);
        bool success = accessor.set(buffer, base_offset, sizeof(T), &value);
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
    }

    T get() const {
        GNEV_REFL_ACCESSOR_LOG(L1, T);
        T dst;
        bool success = accessor.get(buffer, base_offset, sizeof(T), &dst);
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
        return dst;
    }

    void change(const Changer<T>& changer) const {
        GNEV_REFL_ACCESSOR_LOG(L1, T);
        bool success = accessor.change(buffer, base_offset, sizeof(T), changer);
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
    }

    void copy(const BufferReflAccessorImpl<T>& src) {
        GNEV_REFL_ACCESSOR_LOG(L1, T);
        bool success = accessor.copy(buffer, src.base_offset, base_offset, sizeof(T));
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
    }

protected:
    Buffer& buffer;
    IBufferAccessor& accessor;
    const std::size_t base_offset;
};

} // namespace details

template <IsTriviallyCopyable T>
class BufferReflAccessor : public details::BufferReflAccessorImpl<T> {
public:
    BufferReflAccessor(Buffer& buffer, IBufferAccessor& accessor, std::size_t base_offset)
        : details::BufferReflAccessorImpl<T>{buffer, accessor, base_offset} {}

    BufferReflAccessor* operator->() { return this; }

    const BufferReflAccessor* operator->() const { return this; }
};

template <IsTriviallyCopyable T>
    requires(refl::IsReflectable<T>)
class BufferReflAccessor<T> : public details::BufferReflAccessorImpl<T> {
public:
    using Meta = refl::Meta<T>;

    template <refl::Key... Keys>
    using MemberT = Meta::template DeduceMemberInfo<Keys...>::Type::Type;

    using details::BufferReflAccessorImpl<T>::get;
    using details::BufferReflAccessorImpl<T>::set;
    using details::BufferReflAccessorImpl<T>::change;
    using details::BufferReflAccessorImpl<T>::copy;

    BufferReflAccessor(Buffer& buffer, IBufferAccessor& accessor, std::size_t base_offset)
        : details::BufferReflAccessorImpl<T>{buffer, accessor, base_offset} {}

    BufferReflAccessor* operator->() { return this; }

    const BufferReflAccessor* operator->() const { return this; }

    template <refl::Key... Keys>
        requires(sizeof...(Keys) > 0)
    auto sub() {
        GNEV_REFL_ACCESSOR_LOG(L1, MemberT<Keys...>, Keys.template name<T>()...);
        return BufferReflAccessor<MemberT<Keys...>>{
            details::BufferReflAccessorImpl<T>::buffer,
            details::BufferReflAccessorImpl<T>::accessor,
            details::BufferReflAccessorImpl<T>::base_offset +
                Meta::template MemberOffset<Keys...>};
    }

    template <refl::Key... Keys>
        requires(sizeof...(Keys) > 0)
    void set(const MemberT<Keys...>& value) {
        GNEV_REFL_ACCESSOR_LOG(L1, MemberT<Keys...>, Keys.template name<T>()...);
        bool success = details::BufferReflAccessorImpl<T>::accessor
                           ->set(details::BufferReflAccessorImpl<T>::base_offset +
                                     Meta::template MemberOffset<Keys...>(),
                                 sizeof(MemberT<Keys...>),
                                 &value);
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
    }

    template <refl::Key... Keys>
        requires(sizeof...(Keys) > 0)
    auto get() const {
        using MemberT = Meta::template MemberType<Keys...>;
        GNEV_REFL_ACCESSOR_LOG(L1, MemberT, Keys.template name<T>()...);
        MemberT dst;
        bool success = details::BufferReflAccessorImpl<T>::accessor
                           ->get(details::BufferReflAccessorImpl<T>::base_offset +
                                     Meta::template MemberOffset<Keys...>(),
                                 sizeof(MemberT),
                                 &dst);
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
        return dst;
    }

    template <auto... Keys>
        requires(sizeof...(Keys) > 0)
    void
    change(const details::BufferReflAccessorImpl<T>::template Changer<MemberT<Keys...>>&
               changer) {
        GNEV_REFL_ACCESSOR_LOG(L1, MemberT<Keys...>, Keys.template name<T>()...);
        bool success = details::BufferReflAccessorImpl<T>::accessor
                           ->change(details::BufferReflAccessorImpl<T>::base_offset +
                                        Meta::template MemberOffset<Keys...>(),
                                    sizeof(MemberT<Keys...>),
                                    changer);
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
    }

    template <auto... Keys>
        requires(sizeof...(Keys) > 0)
    void copy(const BufferReflAccessor<T>& src) {
        bool success =
            details::BufferReflAccessorImpl<T>::accessor
                ->copy(src.base_offset + Meta::template MemberOffset<Keys...>(),
                       details::BufferReflAccessorImpl<T>::base_offset +
                           Meta::template MemberOffset<Keys...>(),
                       sizeof(MemberT<Keys...>));
        if (not success) {
            GNEV_LOG_ERROR("Failed");
        }
    }
};

} // namespace gnev::gl