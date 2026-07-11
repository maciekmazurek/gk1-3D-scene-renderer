#include "core/Application.h"

#include <algorithm>
#include <stdexcept>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "rendering/ShaderProgram.h"
#include "rendering/CubeMesh.h"
#include "rendering/SphereMesh.h"

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
        setupStaticCubes();
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

    void Application::setupStaticCubes()
    {
        m_staticCubeMatrices.push_back(
            glm::translate(glm::mat4(1.0F), glm::vec3(-5.5F, 4.0F, 0.0F))
        );
        m_staticCubeMatrices.push_back(
            glm::translate(glm::mat4(1.0F), glm::vec3(5.5F, -3.0F, 0.0F))
        );
        m_staticCubeMatrices.push_back(
            glm::translate(glm::mat4(1.0F), glm::vec3(0.0F, 0.5F, 6.0F))
        );
    }

    void Application::setupCoordSystemTransforms()
    {
        m_modelMatrix = glm::mat4(1.0F);
        
        m_viewMatrix = glm::lookAt(glm::vec3(8.0F, 4.0F, 8.0F),
                                   glm::vec3(0.0F, 0.0F, 0.0F),
                                   glm::vec3(0.0F, 1.0F, 0.0F));
        updateProjection(m_windowConfig.width, m_windowConfig.height);
	}

    void Application::loadContent()
    {
        m_shader = std::make_unique<ShaderProgram>("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
        m_cube = std::make_unique<CubeMesh>();
        m_sphere = std::make_unique<SphereMesh>(1.5F, 20, 20);
    }

    void Application::mainLoop()
    {
        while (!m_window->shouldClose())
        {
            const double currentTime = glfwGetTime();
            const double deltaTime = currentTime - m_lastFrameTime;
            m_lastFrameTime = currentTime;

            processInput();
            updateTransformsOnFrame(currentTime);
            m_renderer.beginFrame();

            if (m_shader && m_cube && m_sphere)
            {
                m_shader->use();
                m_shader->setMat4("view", m_viewMatrix);
                m_shader->setMat4("projection", m_projectionMatrix);
                
                // Światło punktowe
                m_shader->setVec3("u_lightPos", glm::vec3(3.0F, 3.0F, 3.0F));
                m_shader->setVec3("u_lightColor", glm::vec3(1.0F));
                m_shader->setVec3("u_viewPos", glm::vec3(0.0F, 0.0F, 5.0F));
                
                // Reflektor na sześcianie
                glm::vec3 spotlightPos = m_cubePosition + glm::vec3(0.0F, 1.0F, 0.0F);
                glm::vec3 spotlightDir = glm::normalize(glm::vec3(0.0F, 0.0F, 0.0F) - spotlightPos);  // Patrzy na środek sceny
                m_shader->setVec3("u_spotlightPos", spotlightPos);
                m_shader->setVec3("u_spotlightDir", spotlightDir);
                m_shader->setVec3("u_spotlightColor", glm::vec3(1.0F, 0.8F, 0.6F));
                m_shader->setFloat("u_spotlightAngle", glm::cos(glm::radians(25.0F)));

                // Render dynamic rotating cube - SKIP in FPP mode
                if (m_cameraMode != CameraMode::FirstPerson)
                {
                    m_shader->setMat4("model", m_modelMatrix);
                    m_renderer.render(*m_cube, *m_shader);
                }

                // Render static cubes
                for (const auto& staticMatrix : m_staticCubeMatrices)
                {
                    m_shader->setMat4("model", staticMatrix);
                    m_renderer.render(*m_cube, *m_shader);
                }

                // Render sphere
                m_shader->setMat4("model", glm::mat4(1.0F));
                m_renderer.render(*m_sphere, *m_shader);
            }

            glfwSwapBuffers(m_window->handle());
            glfwPollEvents();
        }
    }

    void Application::processInput()
    {
        GLFWwindow* handle = m_window->handle();

        if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(handle, GLFW_TRUE);
        }

        // Zmiana kamery: klawisz 1 - kamera statyczna, 2 - FPP, 3 - śledząca
        if (glfwGetKey(handle, GLFW_KEY_1) == GLFW_PRESS)
        {
            m_cameraMode = CameraMode::Static;
        }
        if (glfwGetKey(handle, GLFW_KEY_2) == GLFW_PRESS)
        {
            m_cameraMode = CameraMode::FirstPerson;
        }
        if (glfwGetKey(handle, GLFW_KEY_3) == GLFW_PRESS)
        {
            m_cameraMode = CameraMode::Following;
        }
    }

    void Application::updateTransformsOnFrame(double currentTime)
    {
        float angle = static_cast<float>(currentTime);
        
        // Rotacja wokół własnej osi
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0F), angle, glm::vec3(0.0F, 1.0F, 0.0F));
        
        // Pozycja sześcianu na orbicie
        m_cubePosition = glm::vec3(std::sin(angle * 0.5F) * 4.0F, 0.0F, std::cos(angle * 0.5F) * 4.0F);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0F), m_cubePosition);
        
        m_modelMatrix = translationMatrix * rotationMatrix;

        // Aktualizuj kamerę w zależności od trybu
        updateCameraForMode();
    }

    void Application::updateCameraForMode()
    {
        if (m_cameraMode == CameraMode::Static)
        {
            // Kamera statyczna
            m_viewMatrix = glm::lookAt(
                glm::vec3(8.0F, 4.0F, 8.0F),  // Pozycja kamery
                glm::vec3(0.0F, 0.0F, 0.0F),  // Cel
                glm::vec3(0.0F, 1.0F, 0.0F)   // Up vector
            );
        }
        else if (m_cameraMode == CameraMode::FirstPerson)
        {
            // FPP - kamera na pozycji sześcianu, patrzy w kierunku (0, 0, 1) w lokalnym układzie
            glm::vec3 cameraPos = m_cubePosition + glm::vec3(0.0F, 1.0F, 0.0F);
            
            // Kierunek patrzenia (środek globalnego układu)
            glm::vec3 cameraTarget = glm::vec3(0.0F, 0.0F, 0.0F);

            m_viewMatrix = glm::lookAt(
                cameraPos,      // Pozycja kamery
                cameraTarget,   // Cel
                glm::vec3(0.0F, 1.0F, 0.0F)  // Up vector
            );
        }
        else if (m_cameraMode == CameraMode::Following)
        {
            // Kamera śledząca - za sześcianem, z góry
            glm::vec3 cameraOffset = glm::vec3(0.0F, 2.0F, -5.0F);
            glm::vec3 cameraPos = m_cubePosition + cameraOffset;
            glm::vec3 cameraTarget = m_cubePosition; // Patrzy na sześcian

            m_viewMatrix = glm::lookAt(
                cameraPos,      // Pozycja kamery
                cameraTarget,   // Cel
                glm::vec3(0.0F, 1.0F, 0.0F)  // Up vector
            );
        }
    }

    void Application::updateProjection(int width, int height)
    {
        const int clampedHeight = std::max(height, 1);
        const float aspect = static_cast<float>(width) / static_cast<float>(clampedHeight);
        m_projectionMatrix = glm::perspective(glm::radians(60.0F), aspect, 0.1F, 100.0F);
    }

    void Application::shutdown()
    {
        m_cube.reset();
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