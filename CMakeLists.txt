cmake_minimum_required(VERSION 3.20)

project("VisualizadorDeAlgortimos-VCPKG-CLION-WIN11-IMGUI-CMAKE" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

if (OVERRIDE_VCPKG_ROOT)
    message(STATUS "VCPKG nao foi encontrado ou sera sobrescrito.")
    execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg.git
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    if (CMAKE_HOST_WIN32)
        execute_process(
                COMMAND ${CMAKE_BINARY_DIR}/vcpkg/bootstrap-vcpkg.bat
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg
        )
    elseif (CMAKE_HOST_UNIX OR CMAKE_HOST_APPLE)
        execute_process(
                COMMAND ./bootstrap-vcpkg.sh
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg
        )
    endif ()

    set(ENV{VCPKG_ROOT} ${CMAKE_BINARY_DIR}/vcpkg)
endif()

if (CMAKE_HOST_WIN32)
    execute_process(
            COMMAND ${CMAKE_BINARY_DIR}/vcpkg/vcpkg.exe install
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg
    )
elseif (CMAKE_HOST_UNIX OR CMAKE_HOST_APPLE)
    execute_process(
            COMMAND ./vcpkg install
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg
    )
endif ()

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/share)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.hpp" "${SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE range-v3::meta range-v3::concepts range-v3::range-v3 glfw glad::glad glm::glm OpenGL::GL GLEW::GLEW imgui::imgui TBB::tbb)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/glfw3.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)