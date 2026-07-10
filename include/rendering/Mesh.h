// include/rendering/Mesh.h
#pragma once

namespace gk1
{
    class Mesh
    {
    public:
        Mesh() = default;
        virtual ~Mesh() = default;

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        virtual void draw() const = 0;

    protected:
        virtual void release() noexcept = 0;

        unsigned int m_vao{0};
        unsigned int m_vbo{0};
        unsigned int m_ebo{0};
        int m_indexCount{0};
    };
} // namespace gk1