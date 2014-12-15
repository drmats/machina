/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_H_
#define __MAIN_H_ 1

#include "m3d.h"
#include "camera.h"

#include <iostream>
#include <cstdlib>
#include <functional>

#include "sdl_opengl.h"




#define VIDEO_DRIVER_NAME_BUFFER_SIZE 64
#define PROGRAM_NAME "machina"




/**
 *  Main program class.
 */
class Main {

private:

    // command-line parameters
    int argc;
    char **argv;


    // SDL library versions
    SDL_version
        compile_sdl_version,
        linked_sdl_version;


    // drawing surface
    SDL_Surface *screen;


    // current video driver
    char video_driver_name[VIDEO_DRIVER_NAME_BUFFER_SIZE];


    // viewport parameters
    struct view_t {
        int width = 960;
        int height = 540;
        int bpp = 32;
    } view;


    // event storage
    SDL_Event event;


    // event handlers
    struct handle_t {
        std::function< void (const SDL_Event &) > mouse_motion =
            [] (const SDL_Event &) -> void { return; };
        std::function< void (const SDL_Event &) > mouse_buttons =
            [] (const SDL_Event &) -> void { return; };
        std::function< void (const SDL_Event &) > keyboard =
            [] (const SDL_Event &) -> void { return; };
    } handle;


    // main camera
    Camera< GLfloat > camera;


    // main loop sustaining variable
    bool main_loop_running = true;




public:

    /**
     *  Initialize vital application components.
     */
    Main (int argc, char *argv[]);


    /**
     *  Clean-up.
     */
    ~Main ();


    /**
     *  Obtain SDL versions, initialize SDL and get video driver name.
     */
    void initialize_sdl ();


    /**
     *  Initialize viewport/drawing surface.
     */
    void initialize_surface ();


    /**
     *  Assign default handlers (mouse/keyboard).
     */
    void assign_default_handlers ();


    /**
     *  Setup OpenGL parameters.
     */
    void setup_opengl ();


    /**
     *  Gracefully finish main loop.
     */
    void terminate_main_loop ();


    /**
     *  Event-processing.
     */
    inline void process_events ();


    /**
     *  Drawing.
     */
    inline void draw ();


    /**
     *  Main application loop.
     */
    void main_loop ();

};




#endif
