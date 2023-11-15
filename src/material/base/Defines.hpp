#pragma once

#include "gl/buffer/BufStorageIndexMap.hpp"
#include "gl/texture/TexStorageIndexMap.hpp"
#include "gl/texture/TexStorageIterator.hpp"

namespace gnev::base {

template <IsTriviallyCopyable T>
using MaterialDataStorage = gl::BufStorageIndexMap<T>;
using MaterialTexStorage = gl::TexStorageIndexMap;
using MaterialTex = gl::TexStorageIterator;

} // namespace gnev::base