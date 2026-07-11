#pragma once

#include <glad/glad.h>
#include <vector>

namespace gk1
{
    class Mesh
    {
    public:
        Mesh() = default;
        virtual ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void draw() const;

    protected:
        void release() noexcept;
        void createVAOVBOEBO(std::vector<float> vertices, std::vector<unsigned int> indices);

        unsigned int m_vao{0};
        unsigned int m_vbo{0};
        unsigned int m_ebo{0};
        int m_indexCount{0};
    };
} // namespace gk1