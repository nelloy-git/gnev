#pragma once

#include "data/Vertex.hpp"
#include "data/VertexInfo.hpp"

static constexpr gnev::VertexAttributeInfo AttribPos(3, GL_FLOAT, false);
static constexpr gnev::VertexAttributeInfo AttribUV(2, GL_FLOAT, false);
static constexpr gnev::VertexAttributeInfo AttribMaterialID(1, GL_INT, false);

using Vertex = gnev::Vertex<AttribPos, AttribUV, AttribMaterialID>;
static constexpr gnev::VertexInfo VertInfo = Vertex::info;
