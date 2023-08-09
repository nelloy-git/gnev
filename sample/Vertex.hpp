#pragma once

#include "data/Vertex.hpp"

static constexpr gnev::AttribInfo AttribPos(3, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribUV(2, GL_FLOAT, false);
static constexpr gnev::AttribInfo AttribMaterialID(1, GL_INT, false);

using Vertex = gnev::Vertex<AttribPos, AttribUV, AttribMaterialID>;
