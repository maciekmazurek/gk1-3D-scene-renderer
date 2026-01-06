#pragma once

#include <string>

struct GLFWwindow;

namespace gk1
{
    struct WindowConfig
    {
        int width{800};
        int height{600};
        std::string title{"gk1-3D-scene-renderer"};
        bool vsync{true};
    };

    class Window final
    {
    public:
        explicit Window(const WindowConfig& config);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) noexcept = delete;
        Window& operator=(Window&&) noexcept = delete;

        GLFWwindow* handle() const noexcept;
        bool shouldClose() const noexcept;
        void requestClose() noexcept;

        int width() const noexcept;
        int height() const noexcept;
        void updateSize(int width, int height) noexcept;

    private:
        GLFWwindow* m_handle{nullptr};
        int m_width{0};
        int m_height{0};
    };
} // namespace gk1