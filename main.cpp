//
// Created by Admin on 2022/12/18.
//

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"

#include <string>

const int WNDW_WIDTH = 1600;
const int WNDW_HEIGHT = 900;

bgfx::ShaderHandle loadShader(const char *FILENAME)
{
    std::string shaderPath = "???";

    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:    shaderPath = "shaders/dx11/"; break;
        case bgfx::RendererType::Vulkan:        shaderPath = "shaders/spirv/"; break;
        default:                                shaderPath = "???";
    }

    shaderPath += FILENAME;

    FILE *file = fopen(shaderPath.c_str(), "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WNDW_WIDTH, WNDW_HEIGHT, "GLFW + bgfx", nullptr, nullptr);

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    bgfx::Init bgfxInit;
    bgfxInit.platformData.nwh = glfwGetWin32Window(window);
//    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
//    bgfxInit.type = bgfx::RendererType::Vulkan;
    bgfxInit.type = bgfx::RendererType::Direct3D12;
    bgfxInit.resolution.width = WNDW_WIDTH;
    bgfxInit.resolution.height = WNDW_HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(bgfxInit);

    struct PosColorVertex
    {
        float x;
        float y;
        float z;
        uint32_t abgr;
    };

    static PosColorVertex cubeVertices[] =
            {
                    {-1.0f,  1.0f,  1.0f, 0xff000000 },
                    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
                    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
                    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
                    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
                    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
                    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
                    { 1.0f, -1.0f, -1.0f, 0xffffffff },
            };

    static const uint16_t cubeTriList[] =
            {
                    0, 1, 2,
                    1, 3, 2,
                    4, 6, 5,
                    5, 6, 7,
                    0, 2, 4,
                    4, 2, 6,
                    1, 5, 3,
                    5, 7, 3,
                    0, 4, 1,
                    4, 5, 1,
                    2, 3, 6,
                    6, 3, 7,
            };

    bgfx::VertexLayout pcvDecl;
    pcvDecl.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

    bgfx::ShaderHandle vsh = loadShader("vs_cubes.bin");
    bgfx::ShaderHandle fsh = loadShader("fs_cubes.bin");
    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

    // Rendering Loop
    unsigned int counter = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, WNDW_WIDTH, WNDW_HEIGHT);

        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        bgfx::touch(0);

        const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(WNDW_WIDTH)/float(WNDW_HEIGHT), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
        float mtx[16];
        bx::mtxRotateXY(mtx, counter * 0.01f, counter * 0.01f);
        bgfx::setTransform(mtx);

        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        bgfx::submit(0, program);
        bgfx::frame();

        counter++;
    }

//    bgfx::shutdown();
    glfwTerminate();
    return EXIT_SUCCESS;
}
