// #pragma once

// #include <memory>

// #include "gl/buffer/IBufferAccessor.hpp"
// #include "gl/buffer/IBufferMemManager.hpp"
// #include "gl/buffer/ManagedSubBuffer.hpp"

// namespace gnev::gl {

// class EXPORT ManagedBuffer {
// public:
//     enum Accessor {
//         MappedCoherent
//     };

//     enum Manager {
//         Fragmented
//     };

//     ManagedBuffer(std::unique_ptr<IBufferAccessor>&& accessor,
//                   std::unique_ptr<IBufferMemManager>&& manager)
//         : accessor{std::move(accessor)}
//         , manager{std::move(manager)} {};

//     ManagedBuffer(std::size_t size, Accessor accessor_type, Manager manager_type);

//     template <IsReflStruct T>
//     std::optional<ManagedSubBuffer<T>> createSub() {
//         auto mem = manager->allocate(sizeof(T));
//         if (not mem.has_value()){
//             return std::nullopt;
//         }
//         return ManagedSubBuffer<T>(mem.value(), accessor);
//     }

// private:
//     std::unique_ptr<Buffer> buffer;
//     std::shared_ptr<IBufferAccessor> accessor;
//     std::unique_ptr<IBufferMemManager> manager;
// };

// } // namespace gnev::gl