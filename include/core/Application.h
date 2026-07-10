#pragma once

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

struct GLFWwindow;

#include "core/Window.h"
#include "rendering/Renderer.h"

namespace gk1
{
    class ShaderProgram;
    class Mesh;
    class TetrahedronMesh;
    class SphereMesh;

    class Application final
    {
    public:
        Application();
        ~Application();

        int run();

    private:
        void initialize();
        void setupGLFWGlad();
        void setupCoordSystemTransforms();
        void setupStaticCubes();
        void loadContent();
        void mainLoop();
        void updateTransformsOnFrame(double currentTime);
        void updateProjection(int width, int height);
        void shutdown();
        void processInput();

        static void handleFramebufferResize(GLFWwindow* window, int width, int height);
        void onFramebufferResized(int width, int height);

        WindowConfig m_windowConfig;
        Renderer m_renderer;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<ShaderProgram> m_shader;
        std::unique_ptr<TetrahedronMesh> m_tetrahedron;
        std::unique_ptr<SphereMesh> m_sphere;
        glm::mat4 m_modelMatrix{1.0F};
        glm::mat4 m_viewMatrix{1.0F};
        glm::mat4 m_projectionMatrix{1.0F};
        std::vector<glm::mat4> m_staticCubeMatrices;
        double m_lastFrameTime{0.0};
        bool m_glfwInitialized{false};
        bool m_initialized{false};
    };
} // namespace gk1