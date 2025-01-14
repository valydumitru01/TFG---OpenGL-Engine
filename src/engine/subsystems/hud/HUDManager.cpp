
#include "engine/subsystems/hud/HUDManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include "engine/Config.h"
#ifdef GLESC_OPENGL
#include <imgui/imgui_impl_opengl3.h>
#endif
#include "engine/subsystems/hud/HudLookAndFeel.h"
#include "engine/core/low-level-renderer/graphic-api/Gapi.h"

using namespace GLESC::HUD;

HUDManager::HUDManager(SDL_Window& window) :
    window(window) {
    initImGUI();
}

HUDManager::~HUDManager() {
#ifdef GLESC_OPENGL
    ImGui_ImplOpenGL3_Shutdown();
#endif
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void HUDManager::processInput(SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

HUDManager::WindowID HUDManager::addWindow(GLESC::InGameWindow& window) {
    WindowID id = nextWindowID;
    windows.push_back(&window);
    windowIDs.insert(std::make_pair(id, &window));
    nextWindowID++;
    return id;
}

GLESC::InGameWindow& HUDManager::getWindow(WindowID id) {
    return *windowIDs[id];
}

void HUDManager::removeWindow(WindowID id) {
    auto it = windowIDs.find(id);
    if (it != windowIDs.end()) {
        // Remove from vector
        auto win_it = std::find(windows.begin(), windows.end(), it->second);
        if (win_it != windows.end()) {
            windows.erase(win_it);
        }
        // Remove from map
        windowIDs.erase(it);
    } else {
        D_ASSERT_TRUE(false, "Window ID not found");
    }
}

void HUDManager::update() {
    for (auto& window : windows) {
        window->update();
    }
}

void HUDManager::render(double timeOfFrame) {
    newFrame();
    for (auto& window : windows) {
        window->render(static_cast<float>(timeOfFrame));
    }
    endFrame();
}

void HUDManager::newFrame() {
#ifdef GLESC_OPENGL
    ImGui_ImplOpenGL3_NewFrame();
#endif
    ImGui_ImplSDL2_NewFrame(&window);
    ImGui::NewFrame();
}

void HUDManager::endFrame() {
    ImGui::EndFrame();

    ImGui::Render();
#ifdef GLESC_OPENGL
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void HUDManager::initImGUI() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    std::string glslCoreStr = GLESC_GLSL_CORE_PROFILE ? "core" : "";
    std::string glslVersionStr = "#version " + std::to_string(GLESC_GL_MAJOR_VERSION) + "" +
        std::to_string(GLESC_GL_MINOR_VERSION) + "0 " + glslCoreStr + "\n";
#ifdef GLESC_OPENGL
    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(&window, getGAPI().getContext());
    ImGui_ImplOpenGL3_Init(glslVersionStr.c_str());
#endif
    HudLookAndFeel::get().setDefaultFont("PixelIntv");
    HudLookAndFeel::get().setDefaultFontSize(20);
    // This must be called after all the inits
    for (int i = 5; i < 40; i++) {
        HudLookAndFeel::get().addFont(HudLookAndFeel::get().getDefaultFont(), i);
    }
    HudLookAndFeel::get().apply();

}
