#include <iostream>
#include <chrono>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Ray Tracing Lib/Image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Transform.h"
#include "Application.h"

#include "Ray Tracing Lib/Scene.h"
#include "Ray Tracing Lib/Shapes/Sphere.h"
#include "Ray Tracing Lib/Shapes/Plane.h"
#include "Ray Tracing Lib/RayTracerCamera.h"

using namespace std::chrono;

void CleanScreen(Image& image, GLubyte  renderTexture[SCR_WIDTH][SCR_HEIGHT][4])
{
    for (int i = 0; i < SCR_HEIGHT; i++)
    {
        for (int j = 0; j < SCR_WIDTH; j++)
        {
            image.DrawPixel(j, i, Color{ 255, 255, 255 }, renderTexture);
        }
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ray Tracing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current 
    glfwMakeContextCurrent(window);

    // vsync
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    GLCall(glEnable(GL_DEPTH_TEST));

    Image image(SCR_WIDTH, SCR_HEIGHT);
    {
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
        };

        const unsigned int indices[] = {
            0, 1, 2, 3, 4, 5
        };

        // BLENDING

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VERTEX ARRAY

        VertexArray va;

        // VERTEX BUFFER

        VertexBuffer vb(quadVertices, sizeof(quadVertices));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        // INDEX BUFFER
        IndexBuffer ib(indices, 6);

        // RENDER TEXTURE
        unsigned int renderTextureId;
        static GLubyte renderTexture[SCR_HEIGHT][SCR_WIDTH][4];
        CleanScreen(image, renderTexture);
        unsigned int slot = 0;
        GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GLCall(glGenTextures(1, &renderTextureId));
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, renderTextureId));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, renderTexture));


        // SHADER
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        // UNBIND
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        //IM GUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        Vector3 camPos(0.0f, 0.0f, -3.0f);
        PerspectiveCamera camera(camPos,
            Vector3(0.0f, 0.0f, 0.0f), Vector3::Up(), 50.0f * PI / 180.0f,
            (float)SCR_WIDTH / (float)SCR_HEIGHT);

        Scene scene;

        Plane floor(Vector3(0.0f, 0.0f, 0.0f), Vector3::Up(), Color(128, 255, 128));
        Vector3 spherePos(-1.0f, 0.0f, 0.0f);
        Sphere sphere(spherePos, 1.0f, Color(128, 128, 255));
        Sphere sphere2(spherePos - Vector3::Forward(), 0.5f, Color(0, 128, 255));

        scene.AddShape(&floor);
        scene.AddShape(&sphere);
        scene.AddShape(&sphere2);

        bool my_tool_active = true;


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            renderer.Resize(window, &width, &height);

            renderer.Clear();

            //CleanScreen(image, renderTexture);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            for (int x = 0; x < image.GetWidth(); x++) {
                for (int y = 0; y < image.GetHeight(); y++) {
                    Vector3 screenCoord((2.0f * x) / image.GetWidth() - 1.0f, (-2.0f * y) / image.GetHeight() + 1.0f, 0.0f);

                    //std::cout << x << " " << y << "  ";
                    //screenCoord.Print();

                    Ray ray = (&camera)->CreateRay(screenCoord);

                    Color color;
                    RayHit rayHit(ray);

                    if ((&scene)->Intersect(rayHit)) {
                        color = rayHit.color;
                    }
                    else
                    {
                        color = Color();
                    }

                    image.DrawPixel(x, y, color, renderTexture);
                }
            }

            GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderTexture));
            shader.SetUniform1i("u_Texture", slot);
            renderer.Draw(va, ib, shader);

            //IMGUI HERE
            ImGui::Begin("Ray Tracing", &my_tool_active, ImGuiWindowFlags_MenuBar);
            ImGui::SliderFloat3("Sphere Pos", &spherePos.x, -1.0f, 1.0f);
            ImGui::SliderFloat3("Cam Pos", &camPos.x, -5.0f, 5.0f);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            sphere.SetCenter(spherePos);
            camera.SetPosition(camPos);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwTerminate();
    return 0;
}