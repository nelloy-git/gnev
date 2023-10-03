#pragma once

#include "gl/buffer/ClientStorage.hpp"
#include "util/Util.hpp"

namespace gnev::gl::buffer {

template<typename K, IsTriviallyCopyable T>
class ClientMap {};

}