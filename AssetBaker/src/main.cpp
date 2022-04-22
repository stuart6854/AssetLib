#include "BinaryIn.h"
#include "BinaryOut.h"
#include "ExporterMesh.h"
#include "MeshImport.h"

#include "Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <portable-file-dialogs/portable-file-dialogs.h>

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <filesystem>

/*struct Player
{
    float x;
    float y;
    std::string name;

    void Print()
    {
        std::cout << "x = " << x << std::endl;
        std::cout << "y = " << y << std::endl;
        std::cout << "name = " << name << std::endl;
    }

    void WriteBinary(Serialisation::BinaryOut& out) const
    {
        out.Write(x);
        out.Write(y);
        out.Write(name);
    }

    void ReadBinary(Serialisation::BinaryIn& in)
    {
        x = in.Read<float>();
        y = in.Read<float>();
        name = in.Read<std::string>();
    }
};*/

#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 6

constexpr int WIN_WIDTH = 1280;
constexpr int WIN_HEIGHT = 720;

constexpr float ORBIT_SPEED = 5.0f;
constexpr float ZOOM_SPEED = 0.5f;

enum eRenderMode : int
{
    eMaterial = 0,
    eObjects,
    eSubmeshes,
    eCount,
};

std::unique_ptr<CameraOrbit> camera;

bool firstClick = true;
double lastX;
double lastY;

float rawModelImportTime = 0.0f;

std::shared_ptr<Shader> materialShader;
std::shared_ptr<Shader> colorShader;

std::vector<std::shared_ptr<Mesh>> meshes;
std::vector<std::shared_ptr<Material>> materials;
std::vector<glm::vec3> colors;
AssetBaker::ImporterModel importer;

int renderMode;

void CameraInput(GLFWwindow* window, CameraOrbit& camera)
{
    // Hold right mouse btn to rotate camera with mouse
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick)
        {
            glfwSetCursorPos(window, WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
            firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        const float rotX = ORBIT_SPEED * static_cast<float>(mouseY - (WIN_HEIGHT / 2.0f)) / WIN_HEIGHT;
        const float rotY = ORBIT_SPEED * static_cast<float>(mouseX - (WIN_WIDTH / 2.0f)) / WIN_WIDTH;

        lastX = mouseX;
        lastY = mouseY;

        camera.RotateAzimuth(rotY);
        camera.RotatePolar(rotX);

        glfwSetCursorPos(window, WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->Zoom(static_cast<float>(yoffset * ZOOM_SPEED));
}

void InitImGui(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void BeginImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EndImGui()
{
    ImGui::Render();
}

void RenderImGui()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool assetBakerOpen = true;
void AssetBakerControlsGUI()
{
    ImGui::Begin("Asset Baker", &assetBakerOpen, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Raw Import Time: %f", rawModelImportTime);
    ImGui::Text("Mesh Count: %i", meshes.size());
    ImGui::Text("Submesh Count: %i", importer.GetSubmeshCount());
    ImGui::Text("Vertices: %i", importer.GetVertexCount());
    ImGui::Text("Triangles: %i", importer.GetTriangleCount());

    if (ImGui::Button("Open"))
    {
        auto selection = pfd::open_file("Choose model to open", ".").result();
        if (!selection.empty())
        {
            auto file = selection[0];
            std::cout << file << std::endl;

            /*Serialisation::BinaryIn in("test.bin");
            player.ReadBinary(in);
            player.Print();*/

            importer.Import(file);
            meshes = importer.GetMeshes();
            materials = importer.GetMaterials();
            for (const auto& material : materials)
            {
                material->SetShader(materialShader);
            }

            colors.clear();
            for (size_t i = 0; i < meshes.size(); i++)
            {
                float hue = glm::linearRand(0.0f, 360.0f);
                float sat = 0.4f;
                float value = 1.0f;
                glm::vec3 color = glm::rgbColor(glm::vec3(hue, sat, value));
                colors.push_back(color);
            }
        }
    }

    if (ImGui::Button("Bake"))
    {
        /*Serialisation::BinaryOut out("test.bin");
        player.WriteBinary(out);*/

        AssetLib::Mesh mesh;
        mesh.vertices = meshes[0]->GetVertices();
        mesh.indices = meshes[0]->GetIndices();
        mesh.submeshes = meshes[0]->GetSubmeshes();

        AssetLib::ExporterMesh exporter;
        exporter.Export(mesh, "test_mesh.mesh");
    }

    if (!meshes.empty())
    {
        // Mesh Controls
        const char* elemNames[eCount] = { "Material", "Objects", "Submeshes" };
        ImGui::Combo("RenderMode", &renderMode, elemNames, eCount);
    }

    ImGui::End();
}

int main()
{
    std::cout << "Asset Baker" << std::endl;

    std::cout << std::filesystem::current_path() << std::endl;

    Window window("AssetBaker", WIN_WIDTH, WIN_HEIGHT);

    glfwSetScrollCallback(window.GetRaw(), scroll_callback);

    {
#pragma region Object Creation

        materialShader = std::make_shared<Shader>("Assets/material.vert", "Assets/material.frag");
        colorShader = std::make_shared<Shader>("Assets/color.vert", "Assets/color.frag");

        const glm::mat4 model = glm::mat4(1.0f);

        constexpr float aspectRatio = static_cast<float>(WIN_WIDTH) / WIN_HEIGHT;
        camera = std::make_unique<CameraOrbit>(60.0f, aspectRatio, 0.1f, 1000.0f);

#pragma endregion

        InitImGui(window.GetRaw());

        /*Player player;
        player.x = 5.86f;
        player.y = 786.876f;
        player.name = "Stuart";*/

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        while (!window.ShouldClose())
        {
            /* Update */

            glfwPollEvents();

            CameraInput(window.GetRaw(), *camera);

            /* Render */

#pragma region ImGui Render

            BeginImGui();

            ImGui::ShowDemoWindow();

            AssetBakerControlsGUI();

            EndImGui();

#pragma endregion

            const auto displaySize = window.GetFramebufferSize();
            glViewport(0, 0, displaySize.x, displaySize.y);

            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            materialShader->Bind();
            materialShader->Set("model", model);
            materialShader->Set("projView", camera->GetProjView());

            int meshIndex = 0;
            for (const auto& mesh : meshes)
            {
                if (mesh == nullptr)
                    continue;

                mesh->Bind();

                for (const auto& submesh : mesh->GetSubmeshes())
                {
                    if (renderMode == eMaterial)
                    {
                        const auto& material = materials[submesh.MaterialIndex];
                        material->Bind();
                    }
                    else if (renderMode == eObjects)
                    {
                        colorShader->Bind();
                        colorShader->Set("model", model);

                        colorShader->Set("projView", camera->GetProjView());
                        colorShader->Set("color", colors[meshIndex]);
                    }
                    else if (renderMode == eSubmeshes)
                    {
                        colorShader->Bind();
                        colorShader->Set("model", model);
                        colorShader->Set("projView", camera->GetProjView());

                        colorShader->Set("color", colors[meshIndex]);
                    }

                    glDrawElements(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.BaseIndex * sizeof(uint32_t)));
                }

                // glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

                ++meshIndex;
            }

            RenderImGui();

            window.SwapBuffers();
        }
    }

    glfwTerminate();
    return 0;
}