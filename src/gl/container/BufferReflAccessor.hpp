#pragma once

#include <cstddef>
#include <memory>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "Ctx.hpp"
#include "gl/container/IBufferRawAccessor.hpp"
#include "pfr/core.hpp"
#include "pfr/core_name.hpp"
#include "pfr/tuple_size.hpp"
#include "quill/PatternFormatter.h"
#include "util/CtString.hpp"
#include "util/Logger.hpp"
#include "util/Reflection.hpp"
#include "util/SrcLoc.hpp"

namespace gnev::gl {

template <typename T>
concept IsTriviallyCopyable = std::is_trivially_copyable_v<T>;

namespace details {

template <IsTriviallyCopyable T>
class BufferReflAccessorImpl {
public:
    template <typename V>
    using Changer = std::function<void(V&)>;

    BufferReflAccessorImpl(const std::shared_ptr<IBufferRawAccessor>& accessor,
                           std::size_t base_offset)
        : accessor{accessor}
        , base_offset{base_offset} {}

    GLuint handle() const { return accessor->getBuffer()->handle(); }

    void set(const T& value) {
        GNEV_HANDLER_LOG_L1(getTypeName<T>());
        bool success = accessor->set(base_offset, sizeof(T), &value);
        if (not success) {
            // GNEV_LOG_ERROR(fmt, ...)
            // GNEV_HANDLER_LOG_ERROR("")
            // Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    T get() const {
        GNEV_HANDLER_LOG_L1();
        T dst;
        bool success = accessor->get(base_offset, sizeof(T), &dst);
        if (not success) {
            // Ctx::Get().getLogger().log<LogLevel::ERROR, "Failed">();
        }
        return dst;
    }

    void change(const Changer<T>& changer) const {
        GNEV_HANDLER_LOG_L1(getTypeName<Changer<T>>());
        bool success = accessor->change(base_offset, sizeof(T), changer);
        if (not success) {
            // Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    void copy(const BufferReflAccessorImpl<T>& src) {
        GNEV_HANDLER_LOG_L1(getTypeName<BufferReflAccessorImpl<T>>());
        bool success = accessor->copy(src.base_offset, base_offset, sizeof(T));
        if (not success) {
            // Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

protected:
    std::shared_ptr<IBufferRawAccessor> accessor;
    const std::size_t base_offset;
};

} // namespace details

template <IsTriviallyCopyable T>
class BufferReflAccessor : public details::BufferReflAccessorImpl<T> {
public:
    BufferReflAccessor(const std::shared_ptr<IBufferRawAccessor>& accessor,
                       std::size_t base_offset)
        : details::BufferReflAccessorImpl<T>{accessor, base_offset} {}
};

template <IsTriviallyCopyable T>
    requires(refl::IsReflectable<T>)
class BufferReflAccessor<T> : public details::BufferReflAccessorImpl<T> {
public:
    using Meta = refl::Meta<T>;

    template <refl::Key... Keys>
    using MemberT = Meta::template DeduceMemberInfo<Keys...>::Type::Type;

    using details::BufferReflAccessorImpl<T>::handle;
    using details::BufferReflAccessorImpl<T>::get;
    using details::BufferReflAccessorImpl<T>::set;
    using details::BufferReflAccessorImpl<T>::change;
    using details::BufferReflAccessorImpl<T>::copy;

    BufferReflAccessor(const std::shared_ptr<IBufferRawAccessor>& accessor,
                       std::size_t base_offset)
        : details::BufferReflAccessorImpl<T>{accessor, base_offset} {}

    template <refl::Key... Keys>
        requires(sizeof...(Keys) > 0)
    auto sub() {
        GNEV_HANDLER_LOG_L1();
        return BufferReflAccessor<MemberT<Keys...>>{
            details::BufferReflAccessorImpl<T>::accessor,
            details::BufferReflAccessorImpl<T>::base_offset +
                Meta::template MemberOffset<Keys...>};
    }

    template <refl::Key... Keys>
        requires(sizeof...(Keys) > 0)
    void set(const MemberT<Keys...>& value) {
        GNEV_HANDLER_LOG_L1(getTypeName<MemberT<Keys...>>(), Keys.template name<T>()...);
        bool success = details::BufferReflAccessorImpl<T>::accessor
                           ->set(details::BufferReflAccessorImpl<T>::base_offset +
                                     Meta::template MemberOffset<Keys...>(),
                                 sizeof(MemberT<Keys...>),
                                 &value);
        if (not success) {
            // Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }

    template <refl::Key... Keys>
        requires(sizeof...(Keys) > 0)
    auto get() const {
        using MemberT = Meta::template MemberType<Keys...>;
        MemberT dst;
        bool success = details::BufferReflAccessorImpl<T>::accessor
                           ->get(details::BufferReflAccessorImpl<T>::base_offset +
                                     Meta::template MemberOffset<Keys...>(),
                                 sizeof(MemberT),
                                 &dst);
        if (not success) {
            // Ctx::Get().getLogger().log<LogLevel::ERROR, "Failed">();
        }
        return dst;
    }

    template <auto... Keys>
        requires(sizeof...(Keys) > 0)
    void
    change(const details::BufferReflAccessorImpl<T>::template Changer<MemberT<Keys...>>&
               changer) {
        bool success = details::BufferReflAccessorImpl<T>::accessor
                           ->change(details::BufferReflAccessorImpl<T>::base_offset +
                                        Meta::template MemberOffset<Keys...>(),
                                    sizeof(MemberT<Keys...>),
                                    changer);
        if (not success) {
            // Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
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
            // Ctx::Get().getLogger().logMsg<LogLevel::ERROR, "Failed">();
        }
    }
};

} // namespace gnev::gl