/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MACHINA_CPP_
#define __MACHINA_CPP_ 1

#include "machina.hpp"

namespace machina {




/**
 *  Initialize vital application components.
 */
Machina::Machina (int argc, char **argv):
    command_line_params{argc, argv}
{
    std::cout << "Hi!" << std::endl;

    this->initialize_sdl();
    this->initialize_surface();

    this->main_loop = std::make_shared<MainLoop>(this);
}




/**
 *  Clean-up.
 */
Machina::~Machina () {
    if (this->gl_context != NULL) {
        SDL_GL_DeleteContext(this->gl_context);
        std::cout
            << "Deleted OpenGL context."
            << std::endl;
    }

    if (this->main_window != NULL) {
        SDL_DestroyWindow(this->main_window);
        std::cout
            << "Destroyed surface."
            << std::endl;
    }

    std::cout << "Bye!" << std::endl;
}




/**
 *  Obtain SDL versions, initialize SDL and get video driver name.
 */
void Machina::initialize_sdl () {
    SDL_VERSION(&this->compile_sdl_version);
    std::cout
        << "Compile-time SDL version: "
        << static_cast<int>(this->compile_sdl_version.major) << "."
        << static_cast<int>(this->compile_sdl_version.minor) << "."
        << static_cast<int>(this->compile_sdl_version.patch)
        << std::endl;

    SDL_GetVersion(&this->linked_sdl_version);
    std::cout
        << "Runtime SDL version: "
        << static_cast<int>(this->linked_sdl_version.major) << "."
        << static_cast<int>(this->linked_sdl_version.minor) << "."
        << static_cast<int>(this->linked_sdl_version.patch)
        << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) == 0) {
        SDL_Delay(this->init_delay);
        std::atexit(SDL_Quit);
    } else {
        std::cerr
            << "Unable to initialize SDL: "
            << SDL_GetError()
            << std::endl;
        std::exit(EXIT_FAILURE);
    }

    this->video_driver_name = SDL_GetCurrentVideoDriver();
    if (this->video_driver_name != NULL) {
        std::cout
            << "Video driver: "
            << this->video_driver_name
            << std::endl;
    } else {
        std::cerr
            << "Failed to obtain the video driver name."
            << std::endl;
    }
}




/**
 *  Initialize viewport/drawing surface.
 */
void Machina::initialize_surface () {
    // main window
    this->main_window = SDL_CreateWindow(
        PROGRAM_NAME.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        this->viewport.width, this->viewport.height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (this->main_window != NULL) {
        SDL_Delay(this->init_delay);
        std::cout
            << "Initialized surface: "
            << this->viewport.width << "x" << this->viewport.height
            << std::endl;
    } else {
        std::cerr
            << "Unable to initialize surface: "
            << SDL_GetError()
            << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // main GL context
    this->gl_context = SDL_GL_CreateContext(this->main_window);
    if (this->gl_context != NULL) {
        SDL_Delay(this->init_delay);
        std::cout
            << "Created OpenGL context."
            << std::endl;
    } else {
        std::cerr
            << "Unable to create OpenGL context "
            << SDL_GetError()
            << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // GLEW initialization
    glewExperimental = GL_TRUE;
    this->glew_status = glewInit();
    if (this->glew_status == GLEW_OK) {
        SDL_Delay(this->init_delay);
        std::cout
            << "GLEW version: "
            << glewGetString(GLEW_VERSION)
            << std::endl;
    } else {
        std::cerr
            << "Unable to initialize GLEW: "
            << glewGetErrorString(this->glew_status)
            << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // GL version
    glGetIntegerv(GL_MAJOR_VERSION, &this->opengl_major_version);
    glGetIntegerv(GL_MINOR_VERSION, &this->opengl_minor_version);
    this->opengl_version_string =
        reinterpret_cast<const char *>(glGetString(GL_VERSION));
    std::cout
        << "OpenGL version: "
        << this->opengl_major_version << "."
        << this->opengl_minor_version << " "
        << "(" << this->opengl_version_string << ")"
        << std::endl
        << "OpenGL vendor: "
        << reinterpret_cast<const char *>(glGetString(GL_VENDOR))
        << std::endl
        << "OpenGL renderer: "
        << reinterpret_cast<const char *>(glGetString(GL_RENDERER))
        << std::endl
        << "GLSL version: "
        << reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION))
        << std::endl;

    // GL extensions
    glGetIntegerv(GL_NUM_EXTENSIONS, &this->opengl_num_extensions);
    std::cout
        << "OpenGL extensions available: "
        << this->opengl_num_extensions
        << std::endl;

    // NV available GPU memory
    if (glewIsSupported("GL_NVX_gpu_memory_info")) {
        glGetIntegerv(
            GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
            &this->total_gpu_memory
        );
        glGetIntegerv(
            GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
            &this->available_gpu_memory
        );
        std::cout
            << "Total GPU memory: "
            << this->total_gpu_memory << "kB" << std::endl
            << "Available GPU memory: "
            << this->available_gpu_memory << "kB" << std::endl;
    }
}




/**
 *  Print known OpenGL extenstions to stdout.
 */
void Machina::list_gl_extensions () {
    std::cout
        << "------ available OpenGL extension names ------"
        << std::endl;
    for (int i = 0;  i < this->opengl_num_extensions;  i++) {
        std::cout
            << reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i))
            << std::endl;
    }
}




/**
 *  Run the app.
 */
void Machina::run () {
    this->main_loop->run();
}




} // namespace machina

#endif
