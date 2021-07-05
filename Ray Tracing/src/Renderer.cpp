#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    bool show = false;
    if (show) {
        std::cout << function <<
            " " << file << ":" << line << std::endl;
    }

    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Resize(GLFWwindow* window, int* width, int* height)
{
    glfwGetWindowSize(window, width, height);

    GLCall(glViewport(0, 0, *width, *height));
}

void Renderer::Clear() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
