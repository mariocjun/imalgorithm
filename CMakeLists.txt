cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)

# Nome do projeto
set(PROJECT_NAME "VisualizadorDeAlgortimos-VCPKG-CLION-WIN11-IMGUI-CMAKE")
project(${PROJECT_NAME})

# Adiciona a flag de debug
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(cmake-build-debug/vcpkg_installed/x64-windows)
include_directories(include)
include_directories(include/utils)
include_directories(include/algorithms)
include_directories(include/algorithms/graphs/djikstra)
include_directories(include/algorithms/sorts/bubblesort)
include_directories(include/algorithms/sorts/quicksort)

# Adiciona o diretório src com os arquivos fonte do projeto
file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.hpp" "src/*.cpp")

# Adiciona os arquivos fonte ao projeto, e cria um executável
add_executable(${PROJECT_NAME} ${SRC_FILES})

FIND_PACKAGE(glfw3 CONFIG REQUIRED)
FIND_PACKAGE(glad CONFIG REQUIRED)
FIND_PACKAGE(glm CONFIG REQUIRED)
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(glew CONFIG REQUIRED)
FIND_PACKAGE(imgui CONFIG REQUIRED)
FIND_PACKAGE(TBB CONFIG REQUIRED)
FIND_PACKAGE(range-v3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE range-v3::meta range-v3::concepts range-v3::range-v3 glfw glad::glad glm::glm OpenGL::GL GLEW::GLEW imgui::imgui TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)






