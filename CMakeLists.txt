# Define o mínimo necessário.
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 23)

# Define o nome do projeto e o idioma.
project("VisualizadorDeAlgortimos" LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Usar a pasta build do cmake para diretório de saída.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Incluir diretórios.
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Adicionar o diretório de scripts de construção do VCPKG ao arquivo CMAKE_TOOLCHAIN
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Encontrar pacotes necessários.
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

# Pastas que contêm arquivos fonte.
set(SOURCE_DIR
        ${CMAKE_SOURCE_DIR}/src
)

# Recolher todos os arquivos de origem nos diretórios fornecidos.
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.hpp" "${SOURCE_DIR}/*.cpp")

# Definir o destino de saída.
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Linkar as bibliotecas ao executável do projeto.
target_link_libraries(${PROJECT_NAME} PRIVATE
        range-v3::meta
        range-v3::concepts
        range-v3::range-v3
        glfw
        glad::glad
        glm::glm
        OpenGL::GL
        GLEW::GLEW
        imgui::imgui
        TBB::tbb
)

# Adiciona flags de desempenho ao compilador
if(COMPILE_FOR_NATIVE)
    add_compile_options(
            $<$<CXX_COMPILER_ID:GNU>:-O3 -march=native>
            $<$<CXX_COMPILER_ID:Clang>: -O3 -march=native>
            $<$<CXX_COMPILER_ID:MSVC>:/O2 /arch:AVX>
    )
endif()


option(WARNINGS "" ON)
option(HARDCORE_WARNINGS "" ON)
option(COMPILE_FOR_NATIVE "" ON)
option(COMPILE_WITH_LTO "" ON)

set(WARNINGS_LIST "-Wall;-Wextra;")
set(HARDCORE_WARNINGS_LIST
        "-Wall;-Wextra;-Wshadow;-Wpedantic;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Woverloaded-virtual;-Wpedantic;-Wconversion;-Wsign-conversion;-Wmisleading-indentation;-Wduplicated-cond;-Wduplicated-branches;-Wlogical-op;-Wnull-dereference;-Wuseless-cast;-Wdouble-promotion;-Wformat=2;-Wlifetime"
)

if(HARDCORE_WARNINGS)
    add_compile_definitions(${HARDCORE_WARNINGS_LIST})
elseif(WARNINGS)
    add_compile_definitions(${WARNINGS_LIST})
endif()

if(COMPILE_WITH_LTO AND (CMAKE_BUILD_TYPE MATCHES Release))
    cmake_policy(SET CMP0069 NEW)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if(result)
        set(COMPILE_WITH_LTO ON)
    else()
        set(COMPILE_WITH_LTO OFF)
    endif()
endif()