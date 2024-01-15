cmake_minimum_required(VERSION 3.25)
project(3DLoaderVk)
set(CMAKE_CXX_STANDARD 23)
find_package(PkgConfig REQUIRED)
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    message(STATUS "Using Vulkan found by find_package")
    include_directories(${Vulkan_INCLUDE_DIRS})
    set(VULKAN_LIBS ${Vulkan_LIBRARIES})
else ()
    pkg_check_modules(VULKAN REQUIRED vulkan)
    message(STATUS "Using Vulkan found by pkg-config")
    include_directories(${VULKAN_INCLUDE_DIRS})
    link_directories(${VULKAN_LIBRARY_DIRS})
    set(VULKAN_LIBS ${VULKAN_LIBRARIES})
endif()
find_package(GLFW3 QUIET)
if(GLFW3_FOUND)
    message(STATUS "Using GLFW found by find_package")
    set(GLFW_LIBS glfw)
else()
    pkg_check_modules(GLFW REQUIRED glfw3)
    message(STATUS "Using GLFW found by pkg-config")
    include_directories(${GLFW_INCLUDE_DIRS})
    link_directories(${GLFW_LIBRARY_DIRS})
    set(GLFW_LIBS ${GLFW_LIBRARIES})
endif()
find_package(GLM QUIET)
if(GLM_FOUND)
    message(STATUS "Using GLM found by find_package")
    include_directories(${GLM_INCLUDE_DIRS})
else()
    pkg_check_modules(GLM REQUIRED glm)
    message(STATUS "Using GLM found by pkg-config")
    include_directories(${GLM_INCLUDE_DIRS})
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wmisleading-indentation -Wnull-dereference -Wdouble-promotion -Wformat=2 -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
else()
    message(
        WARNING "Unknown compiler. No specific warning flags set."
    )
endif()
add_executable(
    3DLoaderVk
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
    frame.cpp frame.hpp
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
    render_structs.cpp
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
)
target_link_libraries(
    3DLoaderVk
    ${VULKAN_LIBS}
    ${GLFW_LIBS}
)