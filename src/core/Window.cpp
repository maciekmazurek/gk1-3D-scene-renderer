#include "core/Window.h"

#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace gk1
{
    Window::Window(const WindowConfig& config)
        : m_width(config.width)
        , m_height(config.height)
    {
        m_handle = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
        if (m_handle == nullptr)
        {
            throw std::runtime_error("Failed to create GLFW window.");
        }
    }

    Window::~Window()
    {
        if (m_handle != nullptr)
        {
            glfwDestroyWindow(m_handle);
            m_handle = nullptr;
        }
    }

    GLFWwindow* Window::handle() const noexcept
    {
        return m_handle;
    }

    bool Window::shouldClose() const noexcept
    {
        return glfwWindowShouldClose(m_handle) == GLFW_TRUE;
    }

    void Window::requestClose() noexcept
    {
        glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
    }

    int Window::width() const noexcept
    {
        return m_width;
    }

    int Window::height() const noexcept
    {
        return m_height;
    }

    void Window::updateSize(int width, int height) noexcept
    {
        m_width = width;
        m_height = height;
    }
} // namespace gk1