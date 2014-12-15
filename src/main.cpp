/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_CPP_
#define __MAIN_CPP_ 1

#include "main.h"




/**
 *  Initialize vital application components.
 */
Main::Main (int argc, char **argv):
    command_line{argc, argv}
{
    std::cout << "Hi!" << std::endl;
    this->initialize_sdl();
    this->initialize_surface();
    this->setup_opengl();
    this->main_loop.run();
}




/**
 *  Clean-up.
 */
Main::~Main () {
    std::cout << "Bye!" << std::endl;
}




/**
 *  Obtain SDL versions, initialize SDL and get video driver name.
 */
void Main::initialize_sdl () {
    SDL_VERSION(&this->compile_sdl_version);
    std::cout
        << "Compile-time SDL version: "
        << static_cast<int>(this->compile_sdl_version.major) << "."
        << static_cast<int>(this->compile_sdl_version.minor) << "."
        << static_cast<int>(this->compile_sdl_version.patch)
        << std::endl;

    this->linked_sdl_version = *SDL_Linked_Version();
    std::cout
        << "Runtime SDL version: "
        << static_cast<int>(this->linked_sdl_version.major) << "."
        << static_cast<int>(this->linked_sdl_version.minor) << "."
        << static_cast<int>(this->linked_sdl_version.patch)
        << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr
            << "Unable to initialize SDL: "
            << SDL_GetError()
            << std::endl;
        exit(EXIT_FAILURE);
    }

    atexit(SDL_Quit);

    if (
        SDL_VideoDriverName(
            this->video_driver_name,
            VIDEO_DRIVER_NAME_BUFFER_SIZE
        ) != NULL
    ) {
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
void Main::initialize_surface () {
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    this->screen = SDL_SetVideoMode(
        this->view.width, this->view.height, this->view.bpp,
        SDL_OPENGL
        | SDL_GL_DOUBLEBUFFER
        | SDL_HWPALETTE
        | SDL_HWSURFACE
        | SDL_HWACCEL
    );

    if (screen != NULL) {
        std::cout
            << "Initialized surface: "
            << this->screen->w << "x" << this->screen->h
            << "@" << static_cast<int>(this->screen->format->BitsPerPixel)
            << std::endl;
    } else {
        std::cerr
            << "Unable to initialize surface: "
            << SDL_GetError()
            << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, this->screen->w, this->screen->h);
    SDL_WM_SetCaption(PROGRAM_NAME, NULL);
}




/**
 *  Setup OpenGL parameters.
 */
void Main::setup_opengl () {
    glClearColor(0.1f, 0.1f, 0.3f, 0.0f);
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}




/**
 *  Program entry-point.
 */
int main (int argc, char *argv[]) {
    Main(argc, argv);
    exit(EXIT_SUCCESS);
}




#endif
