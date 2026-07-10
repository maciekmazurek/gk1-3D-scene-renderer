// include/rendering/SphereMesh.h
#pragma once

#include "rendering/Mesh.h"

namespace gk1
{
    class SphereMesh final : public Mesh
    {
    public:
        explicit SphereMesh(float radius = 1.0F, int stacks = 20, int slices = 20);
        ~SphereMesh() override;

        void draw() const override;

    private:
        void generateSphere(float radius, int stacks, int slices);
        void release() noexcept override;
    };
} // namespace gk1