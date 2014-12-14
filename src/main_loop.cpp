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
inline void Main::processEvents () {
    while (this->mainLoopRunning  &&  SDL_PollEvent(&this->event)) {
        switch (this->event.type) {
            case SDL_QUIT:
                this->mainLoopRunning = false;
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
void Main::mainLoop () {
    while (this->mainLoopRunning) {
        this->processEvents();
        this->draw();
        SDL_GL_SwapBuffers();
    }
}
