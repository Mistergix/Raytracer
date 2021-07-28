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
#include "Ray Tracing Lib/Shapes/Cylinder.h"
#include "Ray Tracing Lib/Shapes/Cone.h"
#include "Ray Tracing Lib/RayTracerCamera.h"
#include "Ray Tracing Lib/Ray.h"
#include "Ray Tracing Lib/RayHit.h"
#include "Ray Tracing Lib/Background.h"
#include "Ray Tracing Lib/JsonReader.h"

#include <typeinfo>
#include <cstdlib>
#include "Texture.h"

#include "stb_image/stb_image_write.h"

Vector3 Max(Vector3& a, Vector3& b) {
    Vector3 res(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    return res;
}

Vector3 fresnelSchlick(float cosTheta, Vector3 F0)
{
    return F0 + (Vector3::One() - F0) * pow(std::clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

Vector3 Reflect(Vector3 direction, Vector3 normal) {
    return direction - (2.0f * dot(direction, normal) * normal);
}

Color Shade(Texture& skybox, bool trace, RayHit& rayHit, Ray& ray) {
    Color color;
    if (trace) {
        
        Color specular = rayHit.shape->GetMaterial(rayHit.HitPoint()).ks * (1.0f / 255.0f);
        Vector3 spec(specular.r, specular.g, specular.b);
        Vector3  F = fresnelSchlick(std::max(dot(rayHit.Normal().direction.normalized(), -ray.direction), 0.0f), spec);

        ray.origin = rayHit.HitPoint() + rayHit.Normal().direction.normalized() * 0.001f;
        ray.direction = Reflect(ray.direction, rayHit.Normal().direction.normalized());
        ray.energy = ray.energy * F;

        color = rayHit.color;
    }
    else
    {
        ray.energy = Vector3::Zero();
        float theta = atan2(ray.direction.x, ray.direction.z);
        float r = 1;
        float phi = acos(ray.direction.y / r);
        float u = theta / (2 * PI);
        u = 0.5f - u;
        float v = 1 - phi / PI;
        color = skybox.Sample(Vector3(u, v, 0.0f));
    }

    return color;
}


Color GetColor(int x, int y, int samples, int w, int h, int reflexionMax, PerspectiveCamera& camera, Texture& skybox, Scene& scene, bool useShadow) {
    float r = 0.0f, g = 0.0f, b = 0.0f;
    Color color;
    for (int i = 0; i < samples; i++)
    {
        float newX = (float)x + float(i) / (float)samples + 0.25f;
        for (int j = 0; j < samples; j++)
        {
            float newY = (float)y + float(j) / (float)samples + 0.25f;
            Vector3 screenCoord((2.0f * newX) / w - 1.0f, (-2.0f * newY) / h + 1.0f, 0.0f);
            Vector3 debugScreenCoord((2.0f * 150) / w - 1.0f, (-2.0f * 150) / h + 1.0f, 0.0f);
            Ray ray = (&camera)->CreateRay(screenCoord);
            //Ray ray = (&camera)->CreateRay(debugScreenCoord);
            RayHit rayHit(ray);

            color = Color();

            for (size_t i = 0; i < reflexionMax; i++)
            {
                // TRACE
                bool trace = (&scene)->Intersect(rayHit, useShadow);
                Vector3 energy = ray.energy;
                color += energy * Shade(skybox, trace, rayHit, ray);
                rayHit.ray = ray;

                if (ray.energy.magnitude() <= 0.01f) { break; }
            }
            r += color.r;
            g += color.g;
            b += color.b;
        }
    }

    color = Color(r / (float)(samples * samples), g / (float)(samples * samples), b / (float)(samples * samples));
    return color;
}


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

int main(int argc, char* argv[])
{
    std::cout << "You have entered " << argc
        << " arguments:" << "\n";

    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << "\n";

    std::string arg1 = argv[1];
    std::string arg2 = argv[2];
    bool realTime = arg1 == "true" ? true : false;
    bool useShadow = arg2 == "true" ? true : false;
    int renderWidth = atoi(argv[3]);
    int renderHeight = atoi(argv[4]);
    std::string sceneName = argv[5];
    int samples = atoi(argv[6]);
    std::string jpegName = argv[7];
    int reflexionMax = atoi(argv[8]);

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
        json sceneJson = reader.GetJsonFile("res/scenes/" + sceneName);

        Texture skybox(sceneJson.at("skybox"));

        auto jCamera = sceneJson.at("camera");
        auto jAmbiant = sceneJson.at("ambiant");

        Vector3 camPos(jCamera.at("position").at(0), jCamera.at("position").at(1), jCamera.at("position").at(2));
        Vector3 camLookAt(jCamera.at("lookAt").at(0), jCamera.at("lookAt").at(1), jCamera.at("lookAt").at(2));
        PerspectiveCamera camera(camPos, camLookAt, Vector3::Up(), jCamera.at("fov") * PI / 180.0f,
           ( realTime ? (float)SCR_WIDTH : (float)renderWidth) / (realTime ? (float)SCR_HEIGHT : (float)renderHeight));

        Scene scene;
        scene.shadowFactor = 0.5f;
        scene.SetAmbiant(Color(jAmbiant.at(0), jAmbiant.at(1), jAmbiant.at(2)));

        //Lights
        auto jLights = sceneJson.at("lights");
        for (int i = 0; i < jLights.size(); i++)
        {
            auto light = jLights.at(i);
            if (light.at("type").get<std::string>().compare("point") == 0) {
                Vector3 lightPos(light.at("position").at(0), light.at("position").at(1), light.at("position").at(2));
                Color lightColor = Color(light.at("id").at(0), light.at("id").at(1), light.at("id").at(2));
                Color lightSpec = Color(light.at("is").at(0), light.at("is").at(1), light.at("is").at(2));

                Light light(lightPos, lightColor, lightSpec);
                scene.AddLight(&light);
            }
        }

        //Shapes
        auto jShapes = sceneJson.at("shapes");
        const int size = jShapes.size();
        Texture* texture = new Texture [size];
        for (int i = 0; i < jShapes.size(); i++) {
            auto shape = jShapes.at(i);
            Color dColor = Color(shape.at("material").at("diffuse").at(0), shape.at("material").at("diffuse").at(1), shape.at("material").at("diffuse").at(2));
            Color sColor = Color(shape.at("material").at("specular").at(0), shape.at("material").at("specular").at(1), shape.at("material").at("specular").at(2));
            Color aColor = Color(shape.at("material").at("ambiant").at(0), shape.at("material").at("ambiant").at(1), shape.at("material").at("ambiant").at(2));
            float shininess = shape.at("material").at("shininess");
            texture[i].load_image(shape.at("material").at("texture"));
            bool useTexture = shape.at("material").at("useTexture");
            Material mat = Material(dColor, sColor, aColor, shininess, &texture[i], useTexture);
            if (shape.at("type").get<std::string>().compare("plane") == 0) {
                Vector3 pos(shape.at("position").at(0), shape.at("position").at(1), shape.at("position").at(2));
                Vector3 normal(shape.at("normal").at(0), shape.at("normal").at(1), shape.at("normal").at(2));
                Plane* plane = new Plane(pos, normal, mat);
                scene.AddShape(plane);
            }
            else if (shape.at("type").get<std::string>().compare("sphere") == 0) {
                Vector3 rot(shape.at("rotation").at(0), shape.at("rotation").at(1), shape.at("rotation").at(2));
                Vector3 center(shape.at("center").at(0), shape.at("center").at(1), shape.at("center").at(2));
                float radius = shape.at("radius");
                Sphere* sphere = new Sphere(rot, center, radius, mat);
                scene.AddShape(sphere);
            }
            else if (shape.at("type").get<std::string>().compare("cube") == 0) {
                Vector3 rot(shape.at("rotation").at(0), shape.at("rotation").at(1), shape.at("rotation").at(2));
                Vector3 center(shape.at("center").at(0), shape.at("center").at(1), shape.at("center").at(2));
                float size = shape.at("size");
                Cube* cube = new Cube(rot, center, size, mat);
                scene.AddShape(cube);
            }
            else if (shape.at("type").get<std::string>().compare("square") == 0) {
                Vector3 rot(shape.at("rotation").at(0), shape.at("rotation").at(1), shape.at("rotation").at(2));
                Vector3 center(shape.at("center").at(0), shape.at("center").at(1), shape.at("center").at(2));
                float size = shape.at("size");
                Square* square = new Square(rot, center, size, mat);
                scene.AddShape(square);
            }
            else if (shape.at("type").get<std::string>().compare("triangle") == 0) {
                Vector3 rot(shape.at("rotation").at(0), shape.at("rotation").at(1), shape.at("rotation").at(2));
                Vector3 center(shape.at("center").at(0), shape.at("center").at(1), shape.at("center").at(2));
                float size = shape.at("size");
                Vector3 vertex1(shape.at("vertex1").at(0), shape.at("vertex1").at(1), shape.at("vertex1").at(2));
                Vector3 vertex2(shape.at("vertex2").at(0), shape.at("vertex2").at(1), shape.at("vertex2").at(2));
                Vector3 vertex3(shape.at("vertex3").at(0), shape.at("vertex3").at(1), shape.at("vertex3").at(2));
                Triangle* triangle = new Triangle(rot, center, size, vertex1, vertex2, vertex3, mat);
                scene.AddShape(triangle);
            }
            else if (shape.at("type").get<std::string>().compare("cylinder") == 0) {
                Vector3 rot(shape.at("rotation").at(0), shape.at("rotation").at(1), shape.at("rotation").at(2));
                Vector3 center(shape.at("center").at(0), shape.at("center").at(1), shape.at("center").at(2));
                float size = shape.at("size");
                Cylinder* cylinder = new Cylinder(rot, center, size, mat);
                scene.AddShape(cylinder);
            }
            else if (shape.at("type").get<std::string>().compare("cone") == 0) {
                Vector3 rot(shape.at("rotation").at(0), shape.at("rotation").at(1), shape.at("rotation").at(2));
                Vector3 center(shape.at("center").at(0), shape.at("center").at(1), shape.at("center").at(2));
                float radius = shape.at("radius");
                float height = shape.at("height");
                Cone* cone = new Cone(rot, center, radius, height, mat);
                scene.AddShape(cone);
            }
        }

        /*Light light(Vector3(3, 2, -3), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f));
        scene.AddLight(&light);

        float shininess = 50;
        Color matAmbiant = Color(1.5f, 1.5f, 1.5f);
        Color matSpec = Color(255, 255, 255);

        Plane floor(Vector3(0, 0, 0), Vector3(0, 1, 0), Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));

        Sphere sphere(Vector3(0.0f, 0, 0), Vector3(-3, 1.0f, 3), 1.0f, Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));
        Cube cube(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 2.0f, 0.0f), 1.0f, Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));
        Square square(Vector3(0.0f, 20.0f, 0.0f), Vector3(3.0f, 4.0f, 2.0f), 0.5f, Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));
        Triangle tri(Vector3(0.0f, -20.0f, 0.0f), Vector3(0.0f, 3.0f, 3.0f), 1.0f, Vector3(0.0f, 1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Material(Color(0, 0, 0), matSpec, matAmbiant, shininess, &texture, true));

        scene.AddShape(&floor);
        scene.AddShape(&sphere);
        scene.AddShape(&cube);
        scene.AddShape(&square);
        scene.AddShape(&tri);*/

        //Background background;

        bool my_tool_active = true;

        if (realTime) {
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

                int w = image.GetWidth();
                int h = image.GetHeight();

                for (int x = 0; x < w; x++) {
                    for (int y = 0; y < h; y++) {
                        Color color = GetColor(x,y, samples, w, h, reflexionMax, camera, skybox, scene, useShadow);
                        image.DrawPixel(x, y, color, renderTexture);
                    }
                }

                GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderTexture));
                shader.SetUniform1i("u_Texture", slot);
                renderer.Draw(va, ib, shader);

                //IMGUI HERE
                ImGui::Begin("Ray Tracing", &my_tool_active, ImGuiWindowFlags_MenuBar);
                ImGui::SliderInt("AA Samples", &samples, 1, 2);
                ImGui::SliderInt("Reflection", &reflexionMax, 1, 8);
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
        else
        {
            unsigned char* buffer = new unsigned char[renderWidth * renderHeight * 4];

            int w = renderWidth;
            int h = renderHeight;

            for (int x = 0; x < w; x++) {
                for (int y = 0; y < h; y++) {
                    Color color = GetColor(x, y, samples, w, h, reflexionMax, camera, skybox, scene, useShadow);
                    size_t index = 4 * ((renderHeight - y - 1) * renderWidth + x);
                    buffer[index + 0] = color.r;
                    buffer[index + 1] = color.g;
                    buffer[index + 2] = color.b;
                    buffer[index + 3] = 255;
                }
            }

            int quality = 100;

            int success = stbi_write_jpg(("res/renders/" + jpegName).c_str(), renderWidth, renderHeight, 4, buffer, quality);
            delete[] buffer;
        }
        

        
    }

    glfwTerminate();
    return 0;
}