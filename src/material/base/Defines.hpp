#pragma once

#include "gl/buffer/BufStorageIndexMap.hpp"
#include "gl/texture/TexStorageIndexMap.hpp"

namespace gnev::base {

using MaterialIndex = unsigned int;
template<IsTriviallyCopyable T>
using MaterialDataStorage = gl::BufStorageIndexMap<T>;
using MaterialTexStorage = gl::TexStorageIndexMap;

} // namespace gnev::base