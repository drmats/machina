/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#include "main.h"




/**
 *  Event-processing.
 */
inline void Main::process_events () {
    while (this->main_loop_running  &&  SDL_PollEvent(&this->event)) {
        switch (this->event.type) {
            case SDL_QUIT:
                this->main_loop_running = false;
                break;
        }
    }
}




/**
 *  Drawing.
 */
inline void Main::draw () {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
    this->camera.establish();
}




/**
 *  Main application loop.
 */
void Main::main_loop () {
    while (this->main_loop_running) {
        this->process_events();
        this->draw();
        SDL_GL_SwapBuffers();
    }
}
