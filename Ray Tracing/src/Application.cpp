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
#include "Ray Tracing Lib/Shapes/Cube.h"
#include "Ray Tracing Lib/Shapes/Square.h"
#include "Ray Tracing Lib/Shapes/Triangle.h"
#include "Ray Tracing Lib/RayTracerCamera.h"
#include "Ray Tracing Lib/Ray.h"
#include "Ray Tracing Lib/RayHit.h"
#include "Ray Tracing Lib/Background.h"
#include "Ray Tracing Lib/JsonReader.h"

#include <typeinfo>

#include "Texture.h"



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
    window = glfwCreateWindow(1000, 1000, "Ray Tracing", NULL, NULL);
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

        Texture texture("res/textures/tricolor.png");

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

        JsonReader reader;
        json sceneJson = reader.GetJsonFile("res/scenes/scene1.json");

        auto jCamera = sceneJson.at("camera");
        auto jAmbiant = sceneJson.at("ambiant");

        Vector3 camPos(jCamera.at("position").at(0), jCamera.at("position").at(1), jCamera.at("position").at(2));
        Vector3 camLookAt(jCamera.at("lookAt").at(0), jCamera.at("lookAt").at(1), jCamera.at("lookAt").at(2));
        PerspectiveCamera camera(camPos,
            camLookAt, Vector3::Up(), jCamera.at("fov") * PI / 180.0f,
            (float)SCR_WIDTH / (float)SCR_HEIGHT);

        Scene scene;
        bool useShadow = true;
        scene.shadowFactor = 0.5f;
        scene.SetAmbiant(Color(jAmbiant.at(0), jAmbiant.at(1), jAmbiant.at(2)));

        //TODO utiliser des new et désallouer à la fin du programme
        /*
        auto jLights = sceneJson.at("lights");
        for (int i = 0; i < jLights.size(); i++)
        {
            auto light = jLights.at(i);
            if (light.at("type").get<std::string>().compare("point") == 0) {
                Color lightSpec = Color(light.at("is").at(0), light.at("is").at(1), light.at("is").at(2));
                Color lightColor = Color(light.at("id").at(0), light.at("id").at(1), light.at("id").at(2));

                Vector3 lightPos(light.at("position").at(0), light.at("position").at(1), light.at("position").at(2));
                Light light(lightPos, lightColor, lightSpec);
                scene.AddLight(&light);
            }
        }*/
        /*
        auto jShapes = sceneJson.at("shapes");
        for (int i = 0; i < jShapes.size(); i++)
        {
            auto shape = jShapes.at(i);
            Color dColor = Color(shape.at("material").at("diffuse").at(0), shape.at("material").at("diffuse").at(1), shape.at("material").at("diffuse").at(2));
            Color sColor = Color(shape.at("material").at("specular").at(0), shape.at("material").at("specular").at(1), shape.at("material").at("specular").at(2));
            Color aColor = Color(shape.at("material").at("ambiant").at(0), shape.at("material").at("ambiant").at(1), shape.at("material").at("ambiant").at(2));
            Material mat = Material(dColor, sColor, aColor, shape.at("material").at("shininess"));
            if (shape.at("type").get<std::string>().compare("plane") == 0) {
                Vector3 pos(shape.at("position").at(0), shape.at("position").at(1), shape.at("position").at(2));
                Vector3 normal(shape.at("normal").at(0), shape.at("normal").at(1), shape.at("normal").at(2));
                Plane plane = new Plane(pos, normal, mat);
                scene.AddShape(&plane);
            }
        }*/

        Light light(Vector3(3,1,-3), Color(1.0f, 1.0f, 1.0f), Color(1.0f,1.0f,1.0f));
        scene.AddLight(&light);

        float shininess = 50;
        Color matAmbiant = Color(1.5f, 1.5f, 1.5f);
        Color matSpec = Color(255, 255, 255);

        Plane floor(Vector3(0, 0, 0), Vector3(0, 1, 0), Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));

        Sphere sphere(Vector3(0.0f, 90.0f, 90.0f), Vector3(-3.0f, 1.0f, 3.0f), 1.0f, Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));
        Cube cube(Vector3(0.0f, 45.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), 1.0f, Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));
        Square square(Vector3(0.0f, 20.0f, 0.0f), Vector3(3.0f, 4.0f, 2.0f), 0.5f, Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));
        Triangle tri(Vector3(0.0f, -20.0f, 0.0f), Vector3(0.0f, 3.0f, 3.0f), 1.0f, Vector3(0.0f, 1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));

        scene.AddShape(&floor);
        scene.AddShape(&sphere);
        scene.AddShape(&cube);
        scene.AddShape(&square);
        scene.AddShape(&tri);

        Background background;

        int samples = 1;

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
                    float r = 0.0f, g = 0.0f, b = 0.0f;
                    Color color;
                    for (int i = 0; i < samples; i++)
                    {
                        float newX = (float)x + float(i) / (float)samples + 0.25f;
                        for (int j = 0; j < samples; j++)
                        {
                            float newY = (float)y + float(j) / (float)samples + 0.25f;
                            Vector3 screenCoord((2.0f * newX) / image.GetWidth() - 1.0f, (-2.0f * newY) / image.GetHeight() + 1.0f, 0.0f);
                            Vector3 debugScreenCoord((2.0f * 150) / image.GetWidth() - 1.0f, (-2.0f * 150) / image.GetHeight() + 1.0f, 0.0f);
                            Ray ray = (&camera)->CreateRay(screenCoord);
                            //Ray ray = (&camera)->CreateRay(debugScreenCoord);
                            RayHit rayHit(ray);

                            if ((&scene)->Intersect(rayHit, useShadow)) {
                                color = rayHit.color;
                            }
                            else
                            {
                                color = background.Get(rayHit.Direction());
                            }

                            r += color.r;
                            g += color.g;
                            b += color.b;
                        }
                    }

                    color = Color(r / (float)(samples * samples), g / (float)(samples * samples), b / (float)(samples * samples));

                    image.DrawPixel(x, y, color, renderTexture);
                }
            }

            GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderTexture));
            shader.SetUniform1i("u_Texture", slot);
            renderer.Draw(va, ib, shader);

            //IMGUI HERE
            ImGui::Begin("Ray Tracing", &my_tool_active, ImGuiWindowFlags_MenuBar);
            ImGui::SliderInt("AA Samples", &samples, 1, 2);
            ImGui::SliderFloat3("Cam Pos", &camPos.x, -5.0f, 5.0f);
            ImGui::SliderFloat("Shadow Factor", &scene.shadowFactor, 0.0f, 1.0f);
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Show"))
                {
                    if (ImGui::MenuItem("Show Shadows", "")) {
                        useShadow = !useShadow;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            //sphere.SetCenter(spherePos);
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