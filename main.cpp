/**
 * Order matters glad first then sdl2
 */
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>

// SDL + glad initialization
SDL_Window *create_window();
void set_gl_context(SDL_Window *sdlWindow);
void processInput(SDL_Window *sdlWindow);

// cap at monitor refresh rate
int vsync = 1;
bool window_open = true;

int main() {
    Uint32 subsystemInitializationFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    SDL_Init(subsystemInitializationFlags);
    SDL_Window *sdlWindow = create_window();
    set_gl_context(sdlWindow);

    while(window_open) {
        glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(sdlWindow);
        SDL_GL_SwapWindow(sdlWindow);
    }

    SDL_QuitSubSystem(subsystemInitializationFlags);
    SDL_Quit();

    return 0;
}

SDL_Window *create_window() {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Declare display mode structure to be filled in.
    SDL_DisplayMode current{};
    //get main monitor
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window *sdlWindow = SDL_CreateWindow("SDL2_GLAD_GLM",
                                             SDL_WINDOWPOS_CENTERED,
                                             SDL_WINDOWPOS_CENTERED,
                                             current.w,
                                             current.h,
                                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    return sdlWindow;
}

void set_gl_context(SDL_Window *sdlWindow) {
    SDL_GL_CreateContext(sdlWindow);
    if (!gladLoadGL()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLAD failed to load gl functions");
    }
    SDL_Log("OpenGL: %s", glGetString(GL_VERSION));
    SDL_Log("Shading Language: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    SDL_Log("Renderer: %s", glGetString(GL_RENDERER));
    SDL_Log("Vendor: %s", glGetString(GL_VENDOR));
    int result = SDL_GL_SetSwapInterval(vsync);
    if (result != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SwapInterval on %i failed: %s", vsync, SDL_GetError());
    } else {
        SDL_Log("SwapInterval set to %i", vsync);
    }
}

void processInput(SDL_Window *sdlWindow) {
    SDL_Event inputEvent;
    while (SDL_PollEvent(&inputEvent)) {
        switch (inputEvent.type) {
            case SDL_KEYDOWN:
                if (inputEvent.key.keysym.scancode == SDL_SCANCODE_Z) {
                    SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                if (inputEvent.key.keysym.scancode == SDL_SCANCODE_X) {
                    SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN);
                }
                if (inputEvent.key.keysym.scancode == SDL_SCANCODE_C) {
                    SDL_SetWindowFullscreen(sdlWindow, 0);
                }
                break;
            case SDL_QUIT:
                window_open = false;
                break;
            default:
                break;
        }
    }
}
