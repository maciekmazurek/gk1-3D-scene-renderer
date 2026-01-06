#include "core/Application.h"

#include <algorithm>
#include <stdexcept>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rendering/ShaderProgram.h"
#include "rendering/TetrahedronMesh.h"

namespace gk1
{
    Application::Application() = default;

    Application::~Application()
    {
        if (m_initialized || m_glfwInitialized)
        {
            shutdown();
        }
    }

    int Application::run()
    {
        initialize();
        mainLoop();
        shutdown();

        return EXIT_SUCCESS;
    }

    void Application::initialize()
    {
        if (m_initialized)
        {
            return;
        }

        if (glfwInit() == GLFW_FALSE)
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        m_glfwInitialized = true;
		setupGLFWGlad();
		setupCoordSystemTransforms();
        loadContent();
        m_initialized = true;
    }

    void Application::setupGLFWGlad()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        m_window = std::make_unique<Window>(m_windowConfig);
        GLFWwindow* handle = m_window->handle();
        glfwMakeContextCurrent(handle);
        glfwSwapInterval(m_windowConfig.vsync ? 1 : 0);

        glfwSetWindowUserPointer(handle, this);
        glfwSetFramebufferSizeCallback(handle, Application::handleFramebufferResize);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            throw std::runtime_error("Failed to initialize GLAD.");
        }

        glViewport(0, 0, m_windowConfig.width, m_windowConfig.height);
        glEnable(GL_DEPTH_TEST);
	}

    void Application::setupCoordSystemTransforms()
    {
        m_modelMatrix = glm::mat4(1.0F);
        m_viewMatrix = glm::lookAt(glm::vec3(2.5F, 2.0F, 3.0F),
            glm::vec3(0.0F, 0.0F, 0.0F),
            glm::vec3(0.0F, 0.0F, 1.0F));
        updateProjection(m_windowConfig.width, m_windowConfig.height);
	}

    void Application::loadContent()
    {
        m_shader = std::make_unique<ShaderProgram>(
            "assets/shaders/tetrahedron.vert",
            "assets/shaders/tetrahedron.frag");
        m_tetrahedron = std::make_unique<TetrahedronMesh>();
    }

    void Application::mainLoop()
    {
        while (!m_window->shouldClose())
        {
            const double currentTime = glfwGetTime();
            const double deltaTime = currentTime - m_lastFrameTime;
            m_lastFrameTime = currentTime;

            processInput();
            updateTransformsOnFrame(deltaTime);
            m_renderer.beginFrame();

            if (m_shader && m_tetrahedron)
            {
                m_shader->use();
                m_shader->setMat4("model", m_modelMatrix);
                m_shader->setMat4("view", m_viewMatrix);
                m_shader->setMat4("projection", m_projectionMatrix);
                m_renderer.render(*m_tetrahedron, *m_shader);
            }

            glfwSwapBuffers(m_window->handle());
            glfwPollEvents();
        }
    }

    void Application::updateTransformsOnFrame(double deltaTime)
    {
        constexpr float rotationSpeed = glm::radians(45.0F);
        m_rotationAngle += static_cast<float>(deltaTime) * rotationSpeed;
        if (m_rotationAngle > glm::two_pi<float>())
        {
            m_rotationAngle -= glm::two_pi<float>();
        }

        m_modelMatrix = glm::rotate(glm::mat4(1.0F),
                                    m_rotationAngle,
                                    glm::vec3(0.0F, 1.0F, 0.0F));
    }

    void Application::updateProjection(int width, int height)
    {
        const int clampedHeight = std::max(height, 1);
        const float aspect = static_cast<float>(width) / static_cast<float>(clampedHeight);
        m_projectionMatrix = glm::perspective(glm::radians(70.0F), aspect, 0.1F, 100.0F);
    }

    void Application::processInput()
    {
        GLFWwindow* handle = m_window->handle();
        if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            m_window->requestClose();
        }
    }

    void Application::shutdown()
    {
        m_tetrahedron.reset();
        m_shader.reset();
        m_window.reset();

        if (m_glfwInitialized)
        {
            glfwTerminate();
            m_glfwInitialized = false;
        }

        m_initialized = false;
    }

    void Application::handleFramebufferResize(GLFWwindow* window, int width, int height)
    {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app != nullptr)
        {
            glViewport(0, 0, width, height);
            app->onFramebufferResized(width, height);
        }
    }

    void Application::onFramebufferResized(int width, int height)
    {
        m_windowConfig.width = width;
        m_windowConfig.height = height;
        updateProjection(width, height);

        if (m_window)
        {
            m_window->updateSize(width, height);
        }
    }
} // namespace gk1