cmake_minimum_required(VERSION 3.25)
project(vkloader)
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_CXX_STANDARD 23)
set(GLFW_BUILD_STATIC_LIBS ON)
include(FindPkgConfig)

if(WIN32 AND DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "")
endif()

# Find Vulkan
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    message(STATUS "Using Vulkan found by find_package")
    include_directories(${Vulkan_INCLUDE_DIRS})
    set(VULKAN_LIBS ${Vulkan_LIBRARIES})
else()
    if(UNIX)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(VULKAN REQUIRED vulkan)
        message(STATUS "Using Vulkan found by pkg-config")
        include_directories(${VULKAN_INCLUDE_DIRS})
        link_directories(${VULKAN_LIBRARY_DIRS})
        set(VULKAN_LIBS ${VULKAN_LIBRARIES})
    else()
        message(FATAL_ERROR "Vulkan not found and pkg-config is not available on Windows")
    endif()
endif()

# Find GLFW
find_package(glfw3 QUIET)
if(NOT GLFW3_FOUND AND UNIX)
    pkg_check_modules(GLFW REQUIRED glfw3)
    message(STATUS "Using GLFW found by pkg-config")
    include_directories(${GLFW_INCLUDE_DIRS})
    link_directories(${GLFW_LIBRARY_DIRS})
    set(GLFW_LIBS ${GLFW_LIBRARIES})
else()
    message(STATUS "Using GLFW found by find_package")
    set(GLFW_LIBS glfw)
endif()

# Find GLM
find_package(glm QUIET)
if(NOT GLM_FOUND AND UNIX)
    pkg_check_modules(GLM REQUIRED glm)
    message(STATUS "Using GLM found by pkg-config")
    include_directories(${GLM_INCLUDE_DIRS})
else()
    message(STATUS "Using GLM found by find_package")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(
                -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wcast-align -Wunused -Woverloaded-virtual
                -Wconversion -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion
                -Wformat=2 -Werror -Wfloat-equal -Wlogical-op -Wredundant-decls -Wmissing-include-dirs
                -Wswitch-default -Wswitch-enum -Wunused-parameter -Wunreachable-code -Wstrict-overflow=5
                -Winline -Werror=return-type -Werror=uninitialized -Werror=maybe-uninitialized
                -Wold-style-cast -Wuseless-cast -Wshadow=local -Wduplicated-cond -Wduplicated-branches
                -Walloc-zero -Wlogical-not-parentheses -Wnull-dereference -Wdangling-else
                -Wmissing-declarations -Wextra-semi -Wimplicit-fallthrough -Wdisabled-optimization
                -Wctor-dtor-privacy -Wnoexcept -Wsign-promo -Wstrict-null-sentinel -Winit-self
                -Wdelete-non-virtual-dtor -Wcatch-value=3 -Wzero-as-null-pointer-constant -Wdeprecated
                -Wconditionally-supported -Wformat-security -Wmissing-noreturn
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/W4 /WX)
    endif()
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    # NO warnings in release mode
endif()

add_executable(
    main
    main.cpp
    engine.cpp
    engine.hpp
    instance.hpp
    config.hpp
    logging.hpp
    instance.cpp
    device.cpp
    device.hpp
    swapchain.cpp
    swapchain.hpp
    queue_families.cpp
    queue_families.hpp
        frame.hpp
    shaders.cpp
    shaders.hpp
    pipeline.cpp
    pipeline.hpp
    framebuffer.cpp
    framebuffer.hpp
    commands.cpp
    commands.hpp
    sync.cpp
    sync.hpp
    app.cpp
    app.hpp
        render_structs.hpp
    scene.cpp
    scene.hpp
    mesh.cpp
    mesh.hpp
    memory.cpp
    memory.hpp
    triangle_mesh.cpp
    triangle_mesh.hpp
    logging.cpp
        quad_mesh.cpp
        quad_mesh.hpp
        CubeMesh.cpp
        CubeMesh.hpp
)
target_link_libraries(
    main
    ${VULKAN_LIBS}
    ${GLFW_LIBS}
)
