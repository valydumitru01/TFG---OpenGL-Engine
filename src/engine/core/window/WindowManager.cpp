#include "engine/core/math/algebra/vector/Vector.h"
#include "engine/core/logger/Logger.h"
#include "engine/core/low-level-renderer/graphic-api/Gapi.h"
#include "engine/core/asserts/Asserts.h"
#include "engine/core/window/WindowManager.h"

#include "engine/core/file-system/BinPath.h"

using namespace GLESC;

WindowManager::WindowManager() {
    initSDL();
    getGAPI().preWindowCreationInit();
    window = createWindow(GLESC_WINDOW_TITLE);
    auto x = GLESC_WINDOW_X;
    auto y = GLESC_WINDOW_Y;
    auto width = static_cast<unsigned short>(GLESC_WINDOW_WIDTH);
    auto height = static_cast<unsigned short>(GLESC_WINDOW_HEIGHT);
    auto isFullscreen = GLESC_WINDOW_FULLSCREEN;

    setFullscreen(isFullscreen);
    setSize(width, height);
    setPosition(x, y);
    getGAPI().createContext(*window);
    getGAPI().postWindowCreationInit();

    // Enable mouse relative mode
    // This will make the mouse cursor invisible and locked in the middle of the screen
    //setMouseRelative(true);
    setIcon("images/icons/TinyLogo.bmp");
}


void WindowManager::setSize(uint16_t windowWidth, uint16_t windowHeight) {
    if (isResizable()) {
        SDLCall(SDL_SetWindowSize(window, windowWidth, windowHeight));
        getGAPI().setViewport(0, 0, windowWidth, windowHeight);
    }
}

void WindowManager::setWindowTitle(const std::string& title) {
    SDLCall(SDL_SetWindowTitle(window, title.c_str()));
}


void WindowManager::setFullscreen(bool isFullScreen) {
    Uint32 flag = isFullScreen ? SDL_WINDOW_FULLSCREEN : 0;
    SDLCall(SDL_SetWindowFullscreen(window, flag));
}

void WindowManager::setFullscreenBorderless(bool isFullScreen) {
    Uint32 flag = isFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    SDLCall(SDL_SetWindowFullscreen(window, flag));
}

bool WindowManager::isFullscreenBorderless() {
    auto flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
}


void WindowManager::setBorderlessWindow(bool isBorderless) {
    SDLCall(SDL_SetWindowBordered(window, isBorderless ? SDL_FALSE : SDL_TRUE));
}

bool WindowManager::isBorderlessWindow() {
    auto flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_BORDERLESS) != 0;
}

void WindowManager::setResizable(bool isResizable) {
    SDLCall(SDL_SetWindowResizable(window, static_cast<SDL_bool>(isResizable)));
}

bool WindowManager::isResizable() {
    auto flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_RESIZABLE) != 0;
}

bool WindowManager::isFullscreen() {
    auto flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_FULLSCREEN) != 0;
}


void WindowManager::setPosition(unsigned int windowX, unsigned int windowY) {
    SDLCall(SDL_SetWindowPosition(window, static_cast<int>(windowX), static_cast<int>(windowY)));
}

Vec2I WindowManager::getPosition() {
    int x, y;
    SDLCall(SDL_GetWindowPosition(window, &x, &y));
    return Vec2I{x, y};
}

void WindowManager::setIcon(const std::string& iconFile) {
    std::string iconPath = BinPath::getExecutableDirectory() + std::string(ASSETS_PATH) + "/" + iconFile;
    Logger::get().info("Loading icon: " + iconPath);

    SDL_Surface* iconSurface = SDL_LoadBMP(iconPath.c_str());
    D_ASSERT_NOT_NULLPTR(iconSurface, "Unable to load icon with path: " + iconPath + " SDL_Error: " +
                         std::string(SDL_GetError()));

    SDLCall(SDL_SetWindowIcon(window, iconSurface));
    SDLCall(SDL_FreeSurface(iconSurface));
}


SDL_Window& WindowManager::getWindow() {
    return *window;
}

GLESC::WindowDimensions WindowManager::getSize() const {
    WindowDimensions dimensions{};
    Uint32 flags = SDL_GetWindowFlags(window);
    if (flags & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_HIDDEN)) {
        return dimensions;
    }
    if (window == nullptr) {
        return dimensions;
    }
    SDLCall(SDL_GetWindowSize(window, &dimensions.width, &dimensions.height));
    return dimensions;
}

Vec2UI WindowManager::getWindowCenter() const {
    auto dimensions = getSize();
    return Vec2UI{dimensions.width / 2, dimensions.height / 2};
}

uint32_t WindowManager::getRaisedFlags() {
    // Flags that are needed to be passed to the windowManager * to configure it.
    // To add more flags we need to use the binary OR ( | )
    // More info: https://wiki.libsdl.org/SDL_WindowFlags

    uint32_t flags = 0;
#if GLESC_RENDER_API == OpenGLAPI
    // Flag to allow SDL windowManager work with OpenGL
    flags |= SDL_WINDOW_OPENGL;
#elif GLESC_RENDER_API == VulkanAPI
    flags |= SDL_WINDOW_VULKAN;
#endif
    // Window has no borders
    // flags |= SDL_WINDOW_BORDERLESS;
    // Window grabs input focus
    // flags |= SDL_WINDOW_INPUT_GRABBED;
    // Window enables High pixel density if supported by monitor
    flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    // Window is resizable
    flags |= SDL_WINDOW_RESIZABLE;

    return flags;
}


void WindowManager::initSDL() {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    postSDLCall;
    D_ASSERT_EQUAL(result, 0, "Unable to initialize SDL: " + std::string(SDL_GetError()));
    Logger::get().success("SDL Initialized!");
}


SDL_Window* WindowManager::createWindow(const char* title) {
    uint32_t flags = getRaisedFlags();
    SDL_Window* tempWindow = SDL_CreateWindow(title, GLESC_WINDOW_X, GLESC_WINDOW_Y,
                                              GLESC_WINDOW_WIDTH, GLESC_WINDOW_HEIGHT, flags);
    postSDLCall;
    D_ASSERT_NOT_NULLPTR(tempWindow, "Unable to create windowManager: " + std::string(SDL_GetError()));
    Logger::get().success("Window created!");
    SDLCall(SDL_SetWindowMinimumSize(tempWindow, windowMinWidth, windowMinHeight));

    return tempWindow;
}

void WindowManager::destroyWindow() {
    getGAPI().deleteContext();
    SDLCall(SDL_DestroyWindow(window));
    SDLCall(SDL_Quit());
    Logger::get().success("Window destroyed!");
}
