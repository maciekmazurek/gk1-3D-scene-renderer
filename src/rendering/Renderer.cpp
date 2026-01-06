#include "rendering/Renderer.h"

#include <glad/glad.h>

#include "rendering/ShaderProgram.h"
#include "rendering/TetrahedronMesh.h"

namespace gk1
{
    Renderer::Renderer() : m_clearColor{0.0F, 0.0F, 0.0F, 1.0F} {}

    void Renderer::setClearColor(const std::array<float, 4>& color)
    {
        m_clearColor = color;
    }

    void Renderer::beginFrame() const
    {
        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::render(const TetrahedronMesh& mesh, const ShaderProgram& shader) const
    {
        shader.use();
        mesh.draw();
    }
} // namespace gk1