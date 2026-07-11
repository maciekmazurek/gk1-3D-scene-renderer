#pragma once

#include "rendering/Mesh.h"

namespace gk1
{
    class CubeMesh final : public Mesh
    {
    public:
        CubeMesh();
        ~CubeMesh() override;
    };
} // namespace gk1