#pragma once

#include "rendering/Mesh.h"

namespace gk1
{
    class TetrahedronMesh final : public Mesh
    {
    public:
        TetrahedronMesh();
        ~TetrahedronMesh() override;
    };
} // namespace gk1