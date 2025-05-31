#include <iostream>
#include <cmath>
#include <SDL3/SDL.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlgpu3.h"
#include "imgui/implot.h"
#include <unistd.h>
#include <algorithm>
#include "Body.hpp"
#include "World.hpp"

ImPlotPoint getPoint(int idx, void *data) {
    Body *a = (Body *)data;
    return {a->getPos().getX(), a->getPos().getY()};
}

int main()
{
    //SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDL_Window* window = SDL_CreateWindow("Project Gravity", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_VULKAN);

    SDL_GPUDevice* device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);

    SDL_ClaimWindowForGPUDevice(device, window);
    SDL_SetGPUSwapchainParameters(device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();
    ImPlot::CreateContext();

    ImGui_ImplSDL3_InitForSDLGPU(window);
    {
        ImGui_ImplSDLGPU3_InitInfo info;
        info.GpuDevice = device;
        info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
        info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
        ImGui_ImplSDLGPU3_Init(&info);
    }

    World world;
    world.addBody(new Body("a", State(Vector3(0, 0, 0), Vector3()), 500, 4));
    world.addBody(new Body("b", State(Vector3(46, 0, 0), Vector3(5, 9,0)), 0.7, 1));
    world.addBody(new Body("c", State(Vector3(-40, 0, 0), Vector3(5, -9,0)), 0.9, 1));
    world.addBody(new Body("d", State(Vector3(46, -40, 0), Vector3(-1250, -1250,0)), 0.7, 1));
    world.addBody(new Body("e", State(Vector3(-40, -20, 0), Vector3(1250, -1250,0)), 0.9, 1));
    world.addBody(new Body("f", State(Vector3(46, 30, 0), Vector3(5, 9,0)), 1.3, 1));
    world.addBody(new Body("g", State(Vector3(-40, 20, 0), Vector3(5, -9,0)), 0.9, 1));

    bool quit = false;
    while(!quit) {

        SDL_Event evt;
        while(SDL_PollEvent(&evt)) {
            ImGui_ImplSDL3_ProcessEvent(&evt);
            switch(evt.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                default: break;
            }
        }

        SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUTexture *swapchain_tex;
        SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, window, &swapchain_tex, NULL, NULL);

        if(swapchain_tex) {
            ImGui_ImplSDLGPU3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            SDL_GPUColorTargetInfo info = {0};
            info.clear_color = SDL_FColor {0.0f, 0.0f, 0.0f, 0.0f};
            info.texture = swapchain_tex;
            info.load_op = SDL_GPU_LOADOP_CLEAR;
            info.store_op = SDL_GPU_STOREOP_STORE;

            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            ImGui::SetNextWindowSize(ImVec2(w, h));
            ImGui::SetNextWindowPos(ImVec2(0,0));
            if(ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
            {
                if(ImPlot::BeginPlot("Body plot", ImVec2(w-15, h-70))) {

                    for (auto * b : world.getBodies())
                    {
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, b->getRadius() * 3);
                        ImPlot::PlotScatterG(b->getName().c_str(), getPoint, b, 1);
                    }

                    ImPlot::EndPlot();
                }
                if (ImGui::Button("Reset")) {
                    world.reset();
                }
            }

            ImGui::End();

            SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(cmdbuf, &info, 1, NULL);
            ImGui::Render();
            ImDrawData* draw_data = ImGui::GetDrawData();
            Imgui_ImplSDLGPU3_PrepareDrawData(draw_data, cmdbuf);
            ImGui_ImplSDLGPU3_RenderDrawData(draw_data, cmdbuf, render_pass);
            SDL_EndGPURenderPass(render_pass);
        }

        SDL_SubmitGPUCommandBuffer(cmdbuf);
        world.tick();
    }

    SDL_WaitForGPUIdle(device);
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui::DestroyContext();

    SDL_ReleaseWindowFromGPUDevice(device, window);
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
