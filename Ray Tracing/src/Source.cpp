#include <iostream>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <math.h>

#include "Maths/Polygon.h"
#include "Maths/Sutherland.h"
#include "Drawing/Drawing.h"

#include "Transform.h"
#include "Camera.h"

enum MODE { POLYGON, CLIPPING };

int main(void) {
    // ON SUPPOSE SENS TRIGO POUR LE MOMENT

    std::vector<Vector> points;
    points.push_back(Vector(0.0f, 0.0f));
    points.push_back(Vector(1.0f, 0.0f));
    points.push_back(Vector(1.0f, 1.0f));
    points.push_back(Vector(0.0f, 1.0f));
    Polygon poly(points);

    poly.SetTrigonometric(true);

    bool inside = poly.IsInside(Vector(1.5f, 0.5f));

    Sutherland s;

    // TEST INTERSECTION

    bool inter1 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.5f), Vector(3.0f, 0.5f)); // inter sommet du milieu
    bool inter2 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.0f), Vector(3.0f, 0.0f)); // inter sommet du bas
    bool inter3 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 1.0f), Vector(3.0f, 1.0f)); // inter sommet du haut

    bool inter4 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, -1.0f), Vector(3.0f, -1.0f)); // en dessous
    bool inter5 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 2.0f), Vector(3.0f, 2.0f)); // au dessus

    bool inter6 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.0f), Vector(2.0f, 2.0f)); // paralelle
    bool inter7 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(0.0f, -1.0f), Vector(0.0f, 0.5f)); // confondue

    bool inter8 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 2.0f / 5.0f), Vector(4.0f / 5.0f, 3.0f / 5.0f)); // fenetre oblique
    bool inter9 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 1.0f / 5.0f), Vector(4.0f / 5.0f, 2.0f / 5.0f)); // fenetre oblique, inter bas
    bool inter10 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 0.0f / 5.0f), Vector(4.0f / 5.0f, 1.0f / 5.0f)); // fenetre oblique pas d'inter

    bool inter11 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(-1.0f, 0.5f), Vector(1.0f, 0.5f)); // inter segments

    ASSERT(inter1);
    ASSERT(inter2);
    ASSERT(inter3);
    ASSERT(!inter4);
    ASSERT(!inter5);
    ASSERT(!inter6);
    ASSERT(!inter7);
    ASSERT(inter8);
    ASSERT(inter9);
    ASSERT(!inter10);
    ASSERT(inter11);


    // TEST INTERSECTION CALCUL POINT
    Vector inter1v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.5f), Vector(3.0f, 0.5f)); // inter sommet du milieu
    Vector inter2v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.0f), Vector(3.0f, 0.0f)); // inter sommet du bas
    Vector inter3v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 1.0f), Vector(3.0f, 1.0f)); // inter sommet du haut

    Vector inter8v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 2.0f / 5.0f), Vector(4.0f / 5.0f, 3.0f / 5.0f)); // fenetre oblique
    Vector inter9v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 1.0f / 5.0f), Vector(4.0f / 5.0f, 2.0f / 5.0f)); // fenetre oblique, inter bas

    Vector inter11v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(-1.0f, 0.5f), Vector(1.0f, 0.5f)); // inter segments


    // TEST VISIBLE 
    Polygon subject;

    subject.SetTrigonometric(true);

    subject.Add(Vector(0.0f, 0.0f));
    subject.Add(Vector(1.0f, 0.0f));
    subject.Add(Vector(2.0f, 2.0f));
    subject.Add(Vector(0.5f, 1.0f));
    subject.Add(Vector(-1.0f, 2.0f));

    Polygon clipPolygon;

    clipPolygon.SetTrigonometric(true);

    clipPolygon.Add(Vector(-1.0f, 0.5f));
    clipPolygon.Add(Vector(2.0f, 0.5f));
    clipPolygon.Add(Vector(2.0f, 1.25f));
    clipPolygon.Add(Vector(-1.0f, 1.25f));

    bool visible1 = s.Visible(Vector(0.0f, 0.0f), Vector(-1.0f, 0.5f), Vector(2.0f, 0.5f), clipPolygon); // NOT VISIBLE
    bool visible2 = s.Visible(Vector(0.0f, 0.0f), Vector(2.0f, 0.5f), Vector(2.0f, 1.25f), clipPolygon); // VISIBLE

    bool visible3 = s.Visible(Vector(0.5f, 1.0f), Vector(2.0f, 0.5f), Vector(2.0f, 1.25f), clipPolygon); // VISIBLE
    bool visible4 = s.Visible(Vector(0.5f, 1.0f), Vector(2.0f, 1.25f), Vector(-1.0f, 1.25f), clipPolygon); // VISIBLE
    bool visible5 = s.Visible(Vector(0.5f, 1.0f), Vector(-1.0f, 1.25f), Vector(-1.0f, 0.5f), clipPolygon); // VISIBLE
    bool visible6 = s.Visible(Vector(0.5f, 1.0f), Vector(-1.0f, 0.5f), Vector(2.0f, 0.5f), clipPolygon); // VISIBLE

    bool visible7 = s.Visible(Vector(2.0f, 2.0f), Vector(2.0f, 0.5f), Vector(2.0f, 1.25f), clipPolygon); // VISIBLE
    bool visible8 = s.Visible(Vector(2.0f, 2.0f), Vector(2.0f, 1.25f), Vector(-1.0f, 1.25f), clipPolygon); // NOT VISIBLE
    bool visible9 = s.Visible(Vector(2.0f, 2.0f), Vector(-1.0f, 1.25f), Vector(-1.0f, 0.5f), clipPolygon); // VISIBLE
    bool visible10 = s.Visible(Vector(2.0f, 2.0f), Vector(-1.0f, 0.5f), Vector(2.0f, 0.5f), clipPolygon); // VISIBLE

    ASSERT(!visible1);
    ASSERT(visible2);

    ASSERT(visible3);
    ASSERT(visible4);
    ASSERT(visible5);
    ASSERT(visible6);

    ASSERT(visible7);
    ASSERT(!visible8);
    ASSERT(visible9);
    ASSERT(visible10);

    Polygon cut = s.Clip(subject, clipPolygon);

    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;


    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Clipping and Windowing", NULL, NULL);
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
    Drawing drawing(SCR_WIDTH, SCR_HEIGHT);

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
        for (int i = 0; i < SCR_HEIGHT; i++)
        {
            for (int j = 0; j < SCR_WIDTH; j++)
            {
                drawing.DrawPixel(j, i, Color{ 255, 255, 255, 255 }, renderTexture);
            }
        }
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



        // draw as wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // UNBIND
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // IMGUI

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        bool my_tool_active = true;
        
        Color polygonColor{ 255, 0, 0, 255 }, windowPolygonColor{ 0, 255, 0, 255 }, cutPolygonColor{ 0, 0, 255, 255 };
        Sutherland sutherland;

        bool clicked = false;
        MODE mode = POLYGON;

        bool show_colors = false;
        bool fill = false;

        Transform camTransform, objectTransform;
        Camera cam;
        camTransform.SetPos(0.0f, 0.0f, -10.0f);

        std::vector<Polygon> drawnPolygons;

        drawnPolygons.push_back(Polygon());
        drawnPolygons[0].SetTrigonometric(true);

        std::vector<Polygon> clippingPolygons;

        clippingPolygons.push_back(Polygon());
        clippingPolygons[0].SetTrigonometric(true);



        bool initial_Polygons = false;

        if (initial_Polygons) {
            float factor = 1;

            drawnPolygons[0].Add({ 0 * factor, 40 * factor });
            drawnPolygons[0].Add({ 20 * factor, 20 * factor });
            drawnPolygons[0].Add({ 40 * factor, 20 * factor });
            drawnPolygons[0].Add({ 60 * factor, 40 * factor });

            clippingPolygons[0].Add({ 30, 41 });
            clippingPolygons[0].Add({ 30, 30 });
            clippingPolygons[0].Add({ 60, 30 });


        }

        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            renderer.Resize(window, &width, &height);

            renderer.Clear();
            for (int i = 0; i < SCR_HEIGHT; i++)
            {
                for (int j = 0; j < SCR_WIDTH; j++)
                {
                    drawing.DrawPixel(j, i, Color{ 255, 255, 255, 255 }, renderTexture);
                }
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            for (int i = 0; i < drawnPolygons.size(); i++)
            {
                drawing.DrawPolygon(drawnPolygons[i], polygonColor, renderTexture);
            }

            for (int i = 0; i < clippingPolygons.size(); i++) {
                drawing.DrawPolygon(clippingPolygons[i], windowPolygonColor, renderTexture);
            }

            for (int j = 0; j < clippingPolygons.size(); j++) {
                for (int i = 0; i < drawnPolygons.size(); i++)
                {
                    Polygon cutPolygon = sutherland.Clip(drawnPolygons[i], clippingPolygons[j]);

                    drawing.DrawPolygon(cutPolygon, cutPolygonColor, renderTexture);
                    
                    if (fill) {
                        drawing.Fill(cutPolygon, cutPolygonColor, renderTexture);
                    }
                    
                }
            }

            

            GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderTexture));
            shader.SetUniform1i("u_Texture", slot);
            renderer.Draw(va, ib, shader);



            ImGui::Begin("Polygon clipping and filling", &my_tool_active, ImGuiWindowFlags_MenuBar);

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Show"))
                {
                    if (ImGui::MenuItem("Show Colors", "")) {
                        show_colors = !show_colors;
                    }
                    if (ImGui::MenuItem("Show filling", "")) {
                        fill = !fill;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Add Polygon"))
                {
                    if (ImGui::MenuItem("Add subject polygon", "")) {
                        drawnPolygons.push_back(Polygon());
                        drawnPolygons[drawnPolygons.size() - 1].SetTrigonometric(true);
                    }
                    if (ImGui::MenuItem("Add clipping polygon", "")) {
                        clippingPolygons.push_back(Polygon());
                        drawnPolygons[clippingPolygons.size() - 1].SetTrigonometric(true);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Select Polygon"))
                {
                    if (ImGui::MenuItem("Polygon to cut", "")) {
                        mode = POLYGON;
                        std::cout << "The mode is " << mode << std::endl;
                    }
                    if (ImGui::MenuItem("Draw window", "")) {
                        mode = CLIPPING;
                        std::cout << "The mode is " << mode << std::endl;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Clear"))
                {
                    if (ImGui::MenuItem("All", "")) {
                        clippingPolygons.clear();
                        clippingPolygons.push_back(Polygon());
                        clippingPolygons[clippingPolygons.size() - 1].SetTrigonometric(true);


                        drawnPolygons.clear();
                        drawnPolygons.push_back(Polygon());
                        drawnPolygons[drawnPolygons.size() - 1].SetTrigonometric(true);
                    }
                    if (ImGui::MenuItem("Polygons to cut", "")) {
                        drawnPolygons.clear();
                        drawnPolygons.push_back(Polygon());
                        drawnPolygons[drawnPolygons.size() - 1].SetTrigonometric(true);
                    }
                    if (ImGui::MenuItem("Draw window", "")) {
                        clippingPolygons.clear();
                        clippingPolygons.push_back(Polygon());
                        clippingPolygons[clippingPolygons.size() - 1].SetTrigonometric(true);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            if (mode == POLYGON) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Selected polygon is polygon");
            }
            else if (mode == CLIPPING) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Selected polygon is window");
            }
            ImGui::End();

            if (show_colors) {
                ImGui::Begin("Colors");
                ImGui::SliderInt3("Polygon Color", &polygonColor.r, 0, 255);
                ImGui::SliderInt3("Window Polygon Color", &windowPolygonColor.r, 0, 255);
                ImGui::SliderInt3("Cut Polygon Color", &cutPolygonColor.r, 0, 255);
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            //Swap front and back buffers 
            glfwSwapBuffers(window);

            //Poll for and process events
            glfwPollEvents();

            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
            if (state == GLFW_PRESS)
            {
                if (!clicked) {
                    clicked = true;
                    double xpos, ypos;
                    //getting cursor position
                    glfwGetCursorPos(window, &xpos, &ypos);
                    std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;

                    ypos = SCR_HEIGHT - ypos;
                    std::cout << "Texture Position at (" << xpos << " : " << ypos << std::endl;

                    if (mode == POLYGON) {
                        drawnPolygons[drawnPolygons.size() - 1].Add(Vector(xpos, ypos)); 
                        std::cout << drawnPolygons[drawnPolygons.size() - 1].PointCount() << std::endl;
                    }
                    else if (mode == CLIPPING) {
                        clippingPolygons[clippingPolygons.size() - 1].Add(Vector(xpos, ypos));
                        std::cout << clippingPolygons[clippingPolygons.size() - 1].PointCount() << std::endl;
                    }
                }
            }
            else {
                clicked = false;
            }

        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwTerminate();
    return 0;
}