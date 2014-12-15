/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_LOOP_H_
#define __MAIN_LOOP_H_ 1

#include "sdl_opengl.h"
#include <functional>
#include "camera.h"




/**
 *  Work-horse.
 */
class MainLoop {

private:

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


    // loop sustaining variable
    bool running = false;




public:

    /**
     *  Initialize vital components.
     */
    MainLoop ();


    /**
     *  Clean-up.
     */
    ~MainLoop ();


    /**
     *  Assign default handlers (mouse/keyboard).
     */
    void assign_default_handlers ();


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
    void run ();


    /**
     *  Gracefully finish.
     */
    void terminate ();

};




#endif