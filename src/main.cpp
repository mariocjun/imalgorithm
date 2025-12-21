// ImAlgorithm - Visualizador de Algoritmos
// Janela fixa 1920x1080 com fonte grande

#include <stdio.h>
#include <memory>
#include <optional>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "algorithm_gui.hpp"
#include "algorithms/graphs/dijkstra/dijkstra_gui.hpp"
#include "algorithms/sorts/bubblesort/bubblesort_gui.hpp"
#include "algorithms/sorts/quicksort/quicksort_gui.hpp"

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // Configuração OpenGL
#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // ═══════════════════════════════════════════════════════════
    // JANELA FIXA 1920x1080
    // ═══════════════════════════════════════════════════════════
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int WINDOW_HEIGHT = 1080;
    
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // Não permite redimensionar
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
                                          "ImAlgorithm", nullptr, nullptr);
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // VSync

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // ═══════════════════════════════════════════════════════════
    // ESTILO DARK COM CORES VIBRANTES
    // ═══════════════════════════════════════════════════════════
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Cores mais vibrantes
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.40f, 0.70f, 0.80f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.50f, 0.80f, 0.90f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.55f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.45f, 0.75f, 0.80f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.55f, 0.85f, 0.90f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.60f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.60f, 0.90f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.70f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.80f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.60f, 0.90f, 1.00f);
    
    // Bordas arredondadas
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(8, 6);
    style.ItemSpacing = ImVec2(10, 8);
    
    // ═══════════════════════════════════════════════════════════
    // FONTE GRANDE (2x o tamanho padrão)
    // ═══════════════════════════════════════════════════════════
    ImFontConfig fontConfig;
    fontConfig.SizePixels = 26.0f;  // Padrão é 13, então 2x = 26
    fontConfig.OversampleH = 2;
    fontConfig.OversampleV = 2;
    io.Fonts->AddFontDefault(&fontConfig);

    // Setup backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Estado da aplicação
    std::optional<std::unique_ptr<ImAlgorithm::AlgorithmGUI>> algorithm;
    ImVec4 clear_color = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);

    // ═══════════════════════════════════════════════════════════
    // LOOP PRINCIPAL
    // ═══════════════════════════════════════════════════════════
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Menu principal
        float menu_bar_height = 0;
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Algorithms")) {
                if (ImGui::BeginMenu("Sorts")) {
                    if (ImGui::MenuItem("Bubble Sort")) {
                        algorithm.emplace(
                            std::make_unique<ImAlgorithm::bubblesort::BubbleSortGUI>());
                    }
                    if (ImGui::BeginMenu("Quick Sort")) {
                        if (ImGui::MenuItem("Lomuto Partition")) {
                            algorithm.emplace(
                                std::make_unique<ImAlgorithm::quicksort::QuickSortLomutoGUI>());
                        }
                        if (ImGui::MenuItem("Hoare Partition")) {
                            algorithm.emplace(
                                std::make_unique<ImAlgorithm::quicksort::QuickSortHoareGUI>());
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Graphs")) {
                    if (ImGui::MenuItem("Dijkstra")) {
                        algorithm.emplace(
                            std::make_unique<ImAlgorithm::dijkstra::DijkstraGUI>());
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            menu_bar_height = ImGui::GetWindowSize().y;
            ImGui::EndMainMenuBar();
        }

        // Área de visualização (tela inteira menos menu)
        if (algorithm.has_value()) {
            glfwSetWindowTitle(window, algorithm->get()->name());
            algorithm->get()->show(
                ImVec2(0, menu_bar_height),
                ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT - menu_bar_height));
        } else {
            // Tela inicial
            ImGui::SetNextWindowPos(ImVec2(0, menu_bar_height));
            ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT - menu_bar_height));
            ImGui::Begin("Welcome", nullptr, 
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
            
            ImVec2 center = ImVec2(WINDOW_WIDTH / 2.0f, (WINDOW_HEIGHT - menu_bar_height) / 2.0f);
            ImGui::SetCursorPos(ImVec2(center.x - 200, center.y - 50));
            ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "ImAlgorithm");
            ImGui::SetCursorPos(ImVec2(center.x - 250, center.y + 20));
            ImGui::TextDisabled("Select an algorithm from the menu above");
            
            ImGui::End();
        }

        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}