// Dear ImGui: standalone example application for GLFW + OpenGL 3, using
// programmable pipeline (GLFW is a cross-platform general purpose library for
// handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation,
// etc.) If you are new to Dear ImGui, read documentation from the docs/ folder
// + read the top of imgui.cpp. Read online:
// https://github.com/ocornut/imgui/tree/master/docs

#include <cstdio>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to
// maximize ease of testing and compatibility with old VS compilers. To link
// with VS2010-era libraries, VS2015+ requires linking with
// legacy_stdio_definitions.lib, which we do using this pragma. Your own project
// should not be affected, as you are likely to link with a newer binary of GLFW
// that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

#include <memory>
#include <optional>

#include "algorithm_gui.hpp"
#include "algorithms/graphs/dijkstra/dijkstra_gui.hpp"
#include "algorithms/sorts/bubblesort/bubblesort_gui.hpp"
#include "algorithms/sorts/quicksort/quicksort_gui.hpp"

int main(int, char **) {
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char * glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char * glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

  // Create window with graphics context
  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "ImAlgorithm", NULL, NULL);
  if (window == NULL) return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Carregar Fontes
  // - Se nenhuma fonte for carregada, dear imgui usará a fonte padrão. Você pode também
  // carregar várias fontes e usar ImGui::PushFont()/PopFont() para selecioná-las.
  // - AddFontFromFileTTF() retornará ImFont* para que você possa armazená-la caso precise
  // selecionar a fonte entre várias.
  // - Se o arquivo não puder ser carregado, a função retornará NULL. Por favor, trate esses
  // erros na sua aplicação (por exemplo, use uma asserção, ou exiba um erro e encerre).
  // - As fontes serão rasterizadas em um tamanho definido (com sobreamostragem) e
  // armazenadas numa textura quando chamar ImFontAtlas::Build()/GetTexDataAsXXXX(), que
  // ImGui_ImplXXXX_NewFrame abaixo irá chamar.
  // - Leia 'docs/FONTS.md' para mais instruções e detalhes.
  // - Lembre-se de que em C/C++ se você quiser incluir uma barra invertida \ em uma string
  // literal, você precisa escrever uma barra invertida dupla \\!
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

  // Our state
  std::optional<std::unique_ptr<ImAlgorithm::AlgorithmGUI>> algorithm;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    /*
        Utilizamos expressões regulares para encontrar e substituir texto específico em nosso código.
        As expressões regulares são uma ferramenta poderosa para manipulação de strings. No nosso caso,
        queríamos encontrar todas as linhas correspondentes a 'ImGui::Text("Algoritmo ainda não implementado");',
        onde 'Algoritmo' pode ser qualquer string.

        Para isso, implementamos a seguinte expressão regular:

        ImGui::Text\("(.*?) (ainda não implementado)"\);

        Explicando a expressão:
        - 'ImGui::Text\("' corresponde exatamente a esses caracteres.
        - (.*?) é um grupo de captura que corresponde a qualquer sequência de caracteres, mas de forma não gananciosa.
        Esta parte capturará o nome do algoritmo.
        - ' (ainda não implementado)' corresponde exatamente a essa frase. Há um espaço antes de 'ainda'.
        - '"\);' corresponde exatamente a esses caracteres.

        Após encontrar as linhas correspondentes, queríamos inserir um comentário T O D O abaixo de cada uma delas.
        Para substituir pelo texto desejado, usamos a seguinte expressão regular:

        $0\n//T O D O: Implementar $1

        Explicando:
        - $0 refere-se à linha completa encontrada pela busca.
        - \n adiciona uma quebra de linha.
        - '//T O D O: Implementar' é o texto que queríamos adicionar ao nosso código.
        - $1 se refere ao primeiro grupo de captura na busca, que é o nome do algoritmo.

        Com isso, conseguimos acrescentar automaticamente comentários T O D O referentes à implementação de
        determinados algoritmos em nosso código.

    */
    float menu_bar_height;
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Algorithms")) {
        if (ImGui::BeginMenu("Data Structures")) {
          if (ImGui::BeginMenu("Trees")) {
            if (ImGui::MenuItem("Binary Search Tree")) {
              ImGui::Text("Binary Search Tree ainda não implementado");
              //TODO: Implementar Binary Search Tree
            }
            if (ImGui::MenuItem("AVL Tree")) {
              ImGui::Text("AVL Tree ainda não implementado");
              //TODO: Implementar AVL Tree
            }
            if (ImGui::MenuItem("Red Black Tree")) {
              ImGui::Text("Red Black Tree ainda não implementado");
              //TODO: Implementar Red Black Tree
            }
            if (ImGui::MenuItem("B Tree")) {
              ImGui::Text("B Tree ainda não implementado");
              //TODO: Implementar B Tree
            }
            if (ImGui::MenuItem("B+ Tree")) {
              ImGui::Text("B+ Tree ainda não implementado");
              //TODO: Implementar B+ Tree
            }
            ImGui::EndMenu();
          }
          if (ImGui::BeginMenu("Graphs")) {
            if (ImGui::MenuItem("BFS")) {
              ImGui::Text("BFS ainda não implementado");
              //TODO: Implementar BFS
            }
            if (ImGui::MenuItem("DFS")) {
              ImGui::Text("DFS ainda não implementado");
              //TODO: Implementar DFS
            }
            if (ImGui::MenuItem("Prim")) {
              ImGui::Text("Prim ainda não implementado");
              //TODO: Implementar Prim
            }
            if (ImGui::MenuItem("Kruskal")) {
              ImGui::Text("Kruskal ainda não implementado");
              //TODO: Implementar Kruskal
            }
            if (ImGui::MenuItem("Dijkstra")) {
              ImGui::Text("Dijkstra ainda não implementado");
              //TODO: Implementar Dijkstra
            }
            if (ImGui::MenuItem("Bellman-Ford")) {
              ImGui::Text("Bellman-Ford ainda não implementado");
              //TODO: Implementar Bellman-Ford
            }
            if (ImGui::MenuItem("Floyd-Warshall")) {
              ImGui::Text("Floyd-Warshall ainda não implementado");
              //TODO: Implementar Floyd-Warshall
            }
            if (ImGui::MenuItem("Topological Sort")) {
              ImGui::Text("Topological Sort ainda não implementado");
              //TODO: Implementar Topological Sort
            }
            if (ImGui::MenuItem("Edmonds-Karp")) {
              ImGui::Text("Edmonds-Karp ainda não implementado");
              //TODO: Implementar Edmonds-Karp
            }
            if (ImGui::MenuItem("Ford-Fulkerson")) {
              ImGui::Text("Ford-Fulkerson ainda não implementado");
              //TODO: Implementar Ford-Fulkerson
            }
            if (ImGui::MenuItem("Dinic")) {
              ImGui::Text("Dinic ainda não implementado");
              //TODO: Implementar Dinic
            }
            if (ImGui::MenuItem("A*")) {
              ImGui::Text("A* ainda não implementado");
              //TODO: Implementar A*
            }
            if (ImGui::MenuItem("IDA*")) {
              ImGui::Text("IDA* ainda não implementado");
              //TODO: Implementar IDA*
            }
            ImGui::EndMenu();
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Search Algorithms")) {
          if (ImGui::MenuItem("Linear Search")) {
            ImGui::Text("Linear Search ainda não implementado");
            //TODO: Implementar Linear Search
          }
          if (ImGui::MenuItem("Binary Search")) {
            ImGui::Text("Binary Search ainda não implementado");
            //TODO: Implementar Binary Search
          }
          if (ImGui::MenuItem("Jump Search")) {
            ImGui::Text("Jump Search ainda não implementado");
            //TODO: Implementar Jump Search
          }
          if (ImGui::MenuItem("Interpolation Search")) {
            ImGui::Text("Interpolation Search ainda não implementado");
            //TODO: Implementar Interpolation Search
          }
          if (ImGui::MenuItem("Exponential Search")) {
            ImGui::Text("Exponential Search ainda não implementado");
            //TODO: Implementar Exponential Search
          }
          if (ImGui::MenuItem("Ternary Search")) {
            ImGui::Text("Ternary Search ainda não implementado");
            //TODO: Implementar Ternary Search
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Sorting Algorithms")) {
          if (ImGui::MenuItem("Bubble Sort")) {
            algorithm.emplace(std::make_unique<ImAlgorithm::bubblesort::BubbleSortGUI>());
          }
          if (ImGui::MenuItem("Selection Sort")) {
            ImGui::Text("Selection Sort ainda não implementado");
            //TODO: Implementar Selection Sort
          }
          if (ImGui::MenuItem("Insertion Sort")) {
            ImGui::Text("Insertion Sort ainda não implementado");
            //TODO: Implementar Insertion Sort
          }
          if (ImGui::MenuItem("Merge Sort")) {
            ImGui::Text("Merge Sort ainda não implementado");
            //TODO: Implementar Merge Sort
          }
          if (ImGui::BeginMenu("Quick Sort")) {
            if (ImGui::MenuItem("Lomuto")) {
              algorithm.emplace(std::make_unique<ImAlgorithm::quicksort::QuickSortLomutoGUI>());
            }
            if (ImGui::MenuItem("Hoare")) {
              algorithm.emplace(std::make_unique<ImAlgorithm::quicksort::QuickSortHoareGUI>());
            }
            ImGui::EndMenu();
          }
          if (ImGui::MenuItem("Heap Sort")) {
            ImGui::Text("Heap Sort ainda não implementado");
            //TODO: Implementar Heap Sort
          }
          if (ImGui::MenuItem("Counting Sort")) {
            ImGui::Text("Counting Sort ainda não implementado");
            //TODO: Implementar Counting Sort
          }
          if (ImGui::MenuItem("Radix Sort")) {
            ImGui::Text("Radix Sort ainda não implementado");
            //TODO: Implementar Radix Sort
          }
          if (ImGui::MenuItem("Bucket Sort")) {
            ImGui::Text("Bucket Sort ainda não implementado");
            //TODO: Implementar Bucket Sort
          }
          if (ImGui::MenuItem("Shell Sort")) {
            ImGui::Text("Shell Sort ainda não implementado");
            //TODO: Implementar Shell Sort
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Dynamic Programming")) {
          if (ImGui::MenuItem("Longest Common Subsequence")) {
            ImGui::Text("Longest Common Subsequence ainda não implementado");
            //TODO: Implementar Longest Common Subsequence
          }
          if (ImGui::MenuItem("Knapsack")) {
            ImGui::Text("Knapsack ainda não implementado");
            //TODO: Implementar Knapsack
          }
          if (ImGui::MenuItem("0-1 Knapsack")) {
            ImGui::Text("0-1 Knapsack ainda não implementado");
            //TODO: Implementar 0-1 Knapsack
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Backtracking")) {
          if (ImGui::MenuItem("N-Queens")) {
            ImGui::Text("N-Queens ainda não implementado");
            //TODO: Implementar N-Queens
          }
          if (ImGui::MenuItem("Sudoku")) {
            ImGui::Text("Sudoku ainda não implementado");
            //TODO: Implementar Sudoku
          }
          if (ImGui::MenuItem("Graph Coloring")) {
            ImGui::Text("Graph Coloring ainda não implementado");
            //TODO: Implementar Graph Coloring
          }
          if (ImGui::MenuItem("Rat in a Maze")) {
            ImGui::Text("Rat in a Maze ainda não implementado");
            //TODO: Implementar Rat in a Maze
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Geometry Algorithms")) {
          if (ImGui::MenuItem("Convex Hull")) {
            ImGui::Text("Convex Hull ainda não implementado");
            //TODO: Implementar Convex Hull
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Help")) {
        ImGui::EndMenu();
      }
      menu_bar_height = ImGui::GetWindowSize().y;
      ImGui::EndMainMenuBar();
    }

    if (algorithm.has_value()) {
      glfwSetWindowTitle(window, algorithm->get()->name());
      ImGuiIO &io = ImGui::GetIO();
      algorithm->get()->show(ImVec2(0, menu_bar_height), ImVec2(io.DisplaySize.x, io.DisplaySize.y - menu_bar_height));
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
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