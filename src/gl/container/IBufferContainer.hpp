#pragma once

#include "gl/Buffer.hpp"
#include "util/Util.hpp"

namespace gnev {

class IBufferContainerBase {
public:
    virtual gl::Buffer& getBuffer() = 0;
    virtual const gl::Buffer& getBuffer() const = 0;

    virtual unsigned size() const = 0;
};

template <typename T>
class IBufferContainer : public IBufferContainerBase {};

GNEV_TEMPLATE_CONCEPT(IBufferContainer, typename);

} // namespace gnev