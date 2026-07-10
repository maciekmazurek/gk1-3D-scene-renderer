#pragma once

#include <array>

namespace gk1
{
    class Mesh;
    class ShaderProgram;

    class Renderer final
    {
    public:
        Renderer();

        void setClearColor(const std::array<float, 4>& color);
        void beginFrame() const;
        void render(const Mesh& mesh, const ShaderProgram& shader) const;

    private:
        std::array<float, 4> m_clearColor;
    };
} // namespace gk1