#pragma once

#include <filesystem>
#include <string>

#include <glm/mat4x4.hpp>

namespace gk1
{
    class ShaderProgram final
    {
    public:
        ShaderProgram(const std::filesystem::path& vertexPath,
                      const std::filesystem::path& fragmentPath);
        ~ShaderProgram();

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        void use() const noexcept;

        void setMat4(const std::string& uniformName, const glm::mat4& matrix) const;

    private:
        void release() noexcept;

        unsigned int m_programId{0};
    };
} // namespace gk1