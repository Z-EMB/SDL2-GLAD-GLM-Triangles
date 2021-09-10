/**
 * Order matters glad first then sdl2
 */
#include <glad/glad.h>

#define SDL_MAIN_HANDLED

#include <SDL.h>

// SDL + glad initialization
SDL_Window *create_window();

void set_gl_context(SDL_Window *sdlWindow);

void process_input(SDL_Window *sdlWindow);

// load up and initialize gpu state
void shader_compilation_log(GLuint shaderId, const char *shaderType);

void program_linking_log(GLuint programId);

void load_to_gpu(GLuint &shaderProgramId, GLuint &vertexArrayObjectId);

void draw(const GLuint &shaderProgramId, const GLuint &vertexArrayObjectId);

// cap at monitor refresh rate
int vsync = 1;
bool window_open = true;

int main() {
    Uint32 subsystemInitializationFlags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    SDL_Init(subsystemInitializationFlags);
    SDL_Window *sdlWindow = create_window();
    set_gl_context(sdlWindow);

    // open gl state
    GLuint shaderProgramId, vertexArrayObjectId;
    load_to_gpu(shaderProgramId, vertexArrayObjectId);

    while (window_open) {
        glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw(shaderProgramId, vertexArrayObjectId);

        process_input(sdlWindow);
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

void process_input(SDL_Window *sdlWindow) {
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

void program_linking_log(GLuint programId) {
    GLint status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (!status) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Open Gl error linking program %s", infoLog);
    }
}

void shader_compilation_log(GLuint shaderId, const char *shaderType) {
    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Open Gl error compiling shader %s %s", shaderType, infoLog);
    }
}

void load_to_gpu(GLuint &shaderProgramId, GLuint &vertexArrayObjectId) {
    // Triangles setup
    const GLfloat triangle[] = {
            0.5f, 0.5f, 0.0f, //top right 0
            -0.5f, 0.5f, 0.0f, //top left 1
            0.5f, -0.5f, 0.0f, //bottom right 2
            -0.5f, -0.5f, 0.0f, //bottom left 3
    };

    const GLuint indices[] = {
            2, 0, 1,
            2, 1, 3
    };

    const char *vertexShader = "#version 330 core\n"
                               "  \n"
                               "layout (location = 0) in vec3 position;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                               "}";

    const char *fragmentShader = "#version 330 core\n"
                                 "\n"
                                 "out vec4 color;\n"
                                 "\n"
                                 "void main()\n"
                                 "{\n"
                                 "    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                 "} ";

    //shader creation
    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsID, 1, &vertexShader, NULL);
    glCompileShader(vsID);

    shader_compilation_log(vsID, "Vertex");

    GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsId, 1, &fragmentShader, NULL);
    glCompileShader(fsId);

    shader_compilation_log(fsId, "Fragment");

    //bind fragment and vertex shader to a single id;
    // set shaderProgramId
    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vsID);
    glAttachShader(shaderProgramId, fsId);
    glLinkProgram(shaderProgramId);
    program_linking_log(shaderProgramId);
    glDeleteShader(vsID);
    glDeleteShader(fsId);

    glUseProgram(shaderProgramId);
    // set vaoId
    glGenVertexArrays(1, &vertexArrayObjectId);

    //bind vertices to video memory
    GLuint vertexArrayBufferID;
    glGenBuffers(1, &vertexArrayBufferID);

    //bind vertices to video memory
    GLuint vertexIndicesBufferID;
    glGenBuffers(1, &vertexIndicesBufferID);

    //start permanent copy of vertices to video memory
    glBindVertexArray(vertexArrayObjectId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndicesBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void draw(const GLuint &shaderProgramId, const GLuint &vertexArrayObjectId) {
    glUseProgram(shaderProgramId);
    glBindVertexArray(vertexArrayObjectId);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
