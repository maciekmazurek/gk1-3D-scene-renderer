#include "rendering/ShaderProgram.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

namespace gk1
{
    namespace
    {
        std::string readFileOrThrow(const std::filesystem::path& path)
        {
            std::ifstream file(path, std::ios::binary);
            if (!file)
            {
                throw std::runtime_error("Unable to open shader file: " + path.string());
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        unsigned int compileShader(GLenum type,
                                   const std::string& source,
                                   const std::filesystem::path& label)
        {
            unsigned int shader = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            int success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                char infoLog[1024]{};
                glGetShaderInfoLog(shader, static_cast<GLsizei>(std::size(infoLog)), nullptr, infoLog);
                glDeleteShader(shader);
                throw std::runtime_error("Shader compilation failed (" + label.string() + "): " + infoLog);
            }

            return shader;
        }
    } // namespace

    ShaderProgram::ShaderProgram(const std::filesystem::path& vertexPath,
                                 const std::filesystem::path& fragmentPath)
    {
        const std::string vertexSource = readFileOrThrow(vertexPath);
        const std::string fragmentSource = readFileOrThrow(fragmentPath);

        const unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource, vertexPath);
        const unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource, fragmentPath);

        m_programId = glCreateProgram();
        glAttachShader(m_programId, vertexShader);
        glAttachShader(m_programId, fragmentShader);
        glLinkProgram(m_programId);

        int success = 0;
        glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            char infoLog[1024]{};
            glGetProgramInfoLog(m_programId, static_cast<GLsizei>(std::size(infoLog)), nullptr, infoLog);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            release();
            throw std::runtime_error(std::string("Program link failed: ") + infoLog);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ShaderProgram::~ShaderProgram()
    {
        release();
    }

    void ShaderProgram::use() const noexcept
    {
        glUseProgram(m_programId);
    }

    void ShaderProgram::setMat4(const std::string& uniformName, const glm::mat4& matrix) const
    {
        const int location = glGetUniformLocation(m_programId, uniformName.c_str());
        if (location != -1)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
        }
    }

    void ShaderProgram::release() noexcept
    {
        if (m_programId != 0)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }
    }
} // namespace gk1