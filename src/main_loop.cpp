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
            case SDL_MOUSEMOTION:
                this->handle.mouse_motion(this->event);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                this->handle.mouse_buttons(this->event);
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                this->handle.keyboard(this->event);
                break;

            case SDL_QUIT:
                this->terminate_main_loop();
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
