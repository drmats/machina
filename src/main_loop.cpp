/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_LOOP_CPP_
#define __MAIN_LOOP_CPP_ 1

#include "main_loop.h"




/**
 *  Initialize vital components.
 */
MainLoop::MainLoop () {
    this->assign_default_handlers();
}




/**
 *  Clean-up.
 */
MainLoop::~MainLoop () {}




/**
 *  Assign default handlers (mouse/keyboard).
 */
void MainLoop::assign_default_handlers () {
    // ...
}




/**
 *  Event-processing.
 */
inline void MainLoop::process_events () {
    while (this->running  &&  SDL_PollEvent(&this->event)) {
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
                this->terminate();
                break;
        }
    }
}




/**
 *  Drawing.
 */
inline void MainLoop::draw () {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
    this->camera.establish();
}




/**
 *  Main application loop.
 */
void MainLoop::run () {
    this->running = true;
    while (this->running) {
        this->process_events();
        this->draw();
        SDL_GL_SwapBuffers();
    }
}




/**
 *  Gracefully finish.
 */
void MainLoop::terminate () {
    this->running = false;
}




#endif
