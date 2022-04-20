#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include <string>

class Window
{
public:
    Window(std::string title, int width = 800, int height = 800);
    ~Window();

    auto GetRaw() const -> GLFWwindow* { return _window; }

    auto GetFramebufferSize() const -> glm::ivec2;

    bool ShouldClose();

    void SwapBuffers();

private:
    GLFWwindow* _window;

};
