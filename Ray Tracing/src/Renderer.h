#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(fun) GLClearError();\
                    fun;\
                    ASSERT(GLLogCall(#fun, __FILE__, __LINE__))  

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
    void Resize(GLFWwindow* window, int* width, int* height);
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};