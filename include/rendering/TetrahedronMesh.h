#pragma once

#include "rendering/Mesh.h"

namespace gk1
{
    class TetrahedronMesh final : public Mesh
    {
    public:
        TetrahedronMesh();
        ~TetrahedronMesh() override;

        void draw() const override;

    private:
        void release() noexcept override;
    };
} // namespace gk1