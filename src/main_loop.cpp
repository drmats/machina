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
#include "machina.h"
#include "primitives.h"

namespace machina {




/**
 *  Default mouse-motion event handler (do-nothing).
 */
void MainLoop::default_handler_t::empty_mouse_motion (
    MainLoop *ml, const SDL_Event &e
) {
    return;
}




/**
 *  Mouse-motion event handler (look-around).
 */
void MainLoop::default_handler_t::look_around_camera (
    MainLoop *ml, const SDL_Event &e, const GLfloat yaw_direction
) {
    static const auto positive_fmod =
        [] (GLfloat val, GLfloat denom) -> GLfloat {
            return std::fmod(std::fmod(val, denom) + denom, denom);
        };
    ml->camera.pitch = positive_fmod(
        ml->camera.pitch + e.motion.yrel * 0.3f, 360.0f
    );
    ml->camera.yaw = positive_fmod(
        ml->camera.yaw + yaw_direction * e.motion.xrel * 0.3f, 360.0f
    );
    ml->camera.recompute_rotation();
}




/**
 *  Mouse-motion event handler (move-around).
 */
void MainLoop::default_handler_t::move_around_camera (
    MainLoop *ml, const SDL_Event &e
) {
    ml->camera.relative_translate(
        ml->camera.strafe, 1.0,
        e.motion.xrel * ml->camera.dist * 0.00215f
    );
    ml->camera.relative_translate(
        ml->camera.up, -1.0,
        e.motion.yrel * ml->camera.dist * 0.00215f
    );
}




/**
 *  Mouse-wheel default event handler (distance-to-target).
 */
void MainLoop::default_handler_t::mouse_wheel (
    MainLoop *ml, const SDL_Event &e
) {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_LCTRL] == 1) {
        // set fovy
        ml->camera.projection.set_all([ml, e] () {
            auto all = ml->camera.projection.get_all();
            all[0] -= e.wheel.y*4;
            return all;
        }());
    } else {
        // set camera distance-to-target
        ml->camera.dist =
            10.0f + std::exp2(std::fabs(
                std::log2(ml->camera.dist - 10.0f) - e.wheel.y*0.2f
            ));
        ml->camera.recompute_rotation();
    }
}




/**
 *  Default mouse-buttons event handler.
 */
void MainLoop::default_handler_t::mouse_buttons (
    MainLoop *ml, const SDL_Event &e
) {
    switch (e.button.button) {

        // look-around camera
        case SDL_BUTTON_MIDDLE:
        case SDL_BUTTON_RIGHT:
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (
                    ml->camera.pitch > 90.0f  &&
                    ml->camera.pitch <= 270.0f
                ) {
                    ml->handle.mouse_motion =
                        [ml] (const SDL_Event &e) -> void {
                            return ml->default_handler.look_around_camera(
                                ml, e, -1.0f
                            );
                        };
                } else {
                    ml->handle.mouse_motion =
                        [ml] (const SDL_Event &e) -> void {
                            return ml->default_handler.look_around_camera(
                                ml, e, 1.0f
                            );
                        };
                }
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                ml->handle.mouse_motion =
                    [ml] (const SDL_Event &e) -> void {
                        return ml->default_handler.empty_mouse_motion(ml, e);
                    };
            }
            break;

        // move-around camera
        case SDL_BUTTON_LEFT:
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                ml->handle.mouse_motion =
                    [ml] (const SDL_Event &e) -> void {
                        return ml->default_handler.move_around_camera(ml, e);
                    };
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                ml->handle.mouse_motion =
                    [ml] (const SDL_Event &e) -> void {
                        return ml->default_handler.empty_mouse_motion(ml, e);
                    };
            }
            break;

        default:
            break;
    }
}




/**
 *  Default keyboard event handler.
 */
void MainLoop::default_handler_t::keyboard (
    MainLoop *ml, const SDL_Event &e
) {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_ESCAPE] == 1) {
        ml->terminate();
        return;
    }
    switch (e.key.keysym.sym) {

        case SDLK_q:
            if (e.key.state == SDL_PRESSED) {
                ml->terminate();
            }
            break;

        case SDLK_c:
            if (e.key.state == SDL_PRESSED) {
                // reset position
                ml->camera.translation.load_identity();
                // reset camera.projection.fovy
                ml->camera.projection.set_all([ml] () {
                    auto all = ml->camera.projection.get_all();
                    all[0] = 60.0f;
                    return all;
                }());
            }

        default:
            break;
    }
}




/**
 *  Default window event handler.
 */
void MainLoop::default_handler_t::window (
    MainLoop *ml, const SDL_Event &e
) {
    switch (e.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            ml->root->viewport.width = e.window.data1;
            ml->root->viewport.height = e.window.data2;
            ml->adjust_camera_aspect();
            break;
        case SDL_WINDOWEVENT_RESIZED:
            std::cout
                << "Resized surface: "
                << e.window.data1 << "x"
                << e.window.data2
                << std::endl;
            break;
        default:
            break;
    }
}




/**
 *  Initialize vital components.
 */
MainLoop::MainLoop (Machina *root):
    root{root}
{
    this->assign_default_handlers();
    this->setup_opengl();
}




/**
 *  Assign default handlers (mouse/keyboard).
 */
void MainLoop::assign_default_handlers () {
    this->handle.mouse_motion =
        [this] (const SDL_Event &e) -> void {
            return this->default_handler.empty_mouse_motion(this, e);
        };
    this->handle.mouse_wheel =
        [this] (const SDL_Event &e) -> void {
            return this->default_handler.mouse_wheel(this, e);
        };
    this->handle.mouse_buttons =
        [this] (const SDL_Event &e) -> void {
            return this->default_handler.mouse_buttons(this, e);
        };
    this->handle.keyboard =
        [this] (const SDL_Event &e) -> void {
            return this->default_handler.keyboard(this, e);
        };
    this->handle.window =
        [this] (const SDL_Event &e) -> void {
            return this->default_handler.window(this, e);
        };
}




/**
 *  Adjust camera aspect ratio.
 */
inline void MainLoop::adjust_camera_aspect () {
    glViewport(
        0, 0,
        this->root->viewport.width, this->root->viewport.height
    );
    this->camera.projection.set_all([this] () {
        auto all = this->camera.projection.get_all();
        all[1] =
            static_cast<GLfloat>(this->root->viewport.width) /
            static_cast<GLfloat>(this->root->viewport.height);
        return all;
    }());
}





/**
 *  Setup initial OpenGL parameters.
 */
void MainLoop::setup_opengl () {
    this->adjust_camera_aspect();

    glClearColor(0.05f, 0.05f, 0.15f, 0.0f);
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}




/**
 *  Event-processing.
 */
inline void MainLoop::process_events () {
    while (this->running  &&  SDL_PollEvent(&this->event)) {
        try {
            switch (this->event.type) {
                case SDL_MOUSEMOTION:
                    this->handle.mouse_motion(this->event);
                    break;

                case SDL_MOUSEWHEEL:
                    this->handle.mouse_wheel(this->event);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    this->handle.mouse_buttons(this->event);
                    break;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    this->handle.keyboard(this->event);
                    break;

                case SDL_WINDOWEVENT:
                    this->handle.window(this->event);
                    break;

                case SDL_QUIT:
                    this->terminate();
                    break;
            }
        } catch (std::bad_function_call &e) {
            std::cout
                << "MainLoop::process_events: "
                << e.what()
                << std::endl;
        }
    }
}




/**
 *  Drawing.
 */
inline void MainLoop::draw () const {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
    this->camera.establish_projection();
    this->camera.establish_modelview();

    primitives::axes_grid(160.0f, 10.0f);

    primitives::point_cube(160.0f*64.0f, 640.0f, 0.6f);
    primitives::this_thing(160.0f*2.0f, 10.0f, 1.0f, 1.0f, 1.0f, GL_POINTS);
    primitives::this_thing(160.0f*16.0f, 80.0f, 0.7f, 1.0f, 0.1f, GL_LINES);
}




/**
 *  Main application loop.
 */
void MainLoop::run () {
    this->running = true;
    while (this->running) {
        this->process_events();
        this->draw();
        SDL_GL_SwapWindow(this->root->main_window);
    }
}




/**
 *  Gracefully finish.
 */
void MainLoop::terminate () {
    this->running = false;
}




} // namespace machina

#endif
