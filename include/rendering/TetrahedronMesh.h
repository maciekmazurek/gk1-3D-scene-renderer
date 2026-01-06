#pragma once

namespace gk1
{
    class TetrahedronMesh final
    {
    public:
        TetrahedronMesh();
        ~TetrahedronMesh();

        TetrahedronMesh(const TetrahedronMesh&) = delete;
        TetrahedronMesh& operator=(const TetrahedronMesh&) = delete;

        void draw() const;

    private:
        void release() noexcept;

        unsigned int m_vao{0};
        unsigned int m_vbo{0};
        unsigned int m_ebo{0};
        int m_indexCount{0};
    };
} // namespace gk1