cmake_minimum_required(VERSION 3.21)
project(hello_bgfx)

set(CMAKE_CXX_STANDARD 20)

# GLFW
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
add_subdirectory(3rdparty/glfw)

# BGFX
option(BGFX_DEBUG_LIBS "Use debug version of bgfx" ON)
add_library(bgfxlib INTERFACE)
target_include_directories(bgfxlib INTERFACE 3rdparty/bgfx/include)
target_link_directories(bgfxlib INTERFACE 3rdparty/bgfx/.build/win64_vs2022/bin)
if(${BGFX_DEBUG_LIBS})
    target_link_libraries(bgfxlib INTERFACE bgfxDebug bimgDebug bimg_decodeDebug bxDebug)
else()
    target_link_libraries(bgfxlib INTERFACE bgfxRelease bimgRelease bimg_decodeRelease bxRelease)
endif()

add_executable(hello_bgfx main.cpp)
target_link_libraries(hello_bgfx glfw bgfxlib)
set_property(TARGET hello_bgfx PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")