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

#include "main_loop.hpp"
#include "machina.hpp"
#include "primitives.hpp"
#include "test_mesh.hpp"

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
 *  Mouse-motion event handler (look-around-target).
 */
void MainLoop::default_handler_t::look_around_target_camera (
    MainLoop *ml, const SDL_Event &e, const GLfloat yaw_direction
) {
    ml->camera.pitch = m3d::positive_fmod(
        ml->camera.pitch - e.motion.yrel * 0.3f, 360.0f
    );
    ml->camera.yaw = m3d::positive_fmod(
        ml->camera.yaw + yaw_direction * e.motion.xrel * 0.3f, 360.0f
    );
    ml->camera.recompute_transform();
}




/**
 *  Mouse-motion event handler (look-around-target).
 */
void MainLoop::default_handler_t::look_around_camera (
    MainLoop *ml, const SDL_Event &e
) {
    ml->camera.relative_rotate_y(e.motion.xrel * -0.2f);
    ml->camera.relative_rotate_x(e.motion.yrel * 0.2f);
}




/**
 *  Mouse-motion event handler (move-around).
 */
void MainLoop::default_handler_t::move_around_camera (
    MainLoop *ml, const SDL_Event &e
) {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_LCTRL] == 1) {
        ml->camera.move_on_xy(
            e.motion.xrel * ml->camera.dist * 0.00215f,
            e.motion.yrel * ml->camera.dist * 0.00215f
        );
    } else {
        ml->camera.move_on_xz(
            e.motion.xrel * ml->camera.dist * 0.00215f,
            e.motion.yrel * ml->camera.dist * 0.00215f
        );
    }
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

    } else if (keystate[SDL_SCANCODE_LSHIFT] == 1) {

        // move camera on a x-z plane (forward/backward)
        vec2 dir(vec2(
            ml->camera.transform.forward[0],
            ml->camera.transform.forward[2]
        ).normalize());
        auto delta = e.wheel.y * 0.1 * ml->camera.dist;
        ml->camera.target[0] += delta*dir[0];
        ml->camera.target[2] += delta*dir[1];
        ml->camera.recompute_transform();

    } else {

        // set camera distance-to-target
        ml->camera.dist =
            std::exp2(std::fabs(
                std::log2(ml->camera.dist) - e.wheel.y*0.2f
            ));
        ml->camera.recompute_transform();

    }
}




/**
 *  Default mouse-buttons event handler.
 */
void MainLoop::default_handler_t::mouse_buttons (
    MainLoop *ml, const SDL_Event &e
) {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    switch (e.button.button) {

        // look-around camera
        case SDL_BUTTON_MIDDLE:
        case SDL_BUTTON_RIGHT:
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (keystate[SDL_SCANCODE_LCTRL] == 1) {
                    ml->handle.mouse_motion =
                        [ml] (const SDL_Event &e) -> void {
                            return ml->default_handler.look_around_camera(
                                ml, e
                            );
                        };
                } else {
                    if (
                        ml->camera.pitch > 90.0f  &&
                        ml->camera.pitch <= 270.0f
                    ) {
                        ml->handle.mouse_motion =
                            [ml] (const SDL_Event &e) -> void {
                                return ml->default_handler.look_around_target_camera(
                                    ml, e, 1.0f
                                );
                            };
                    } else {
                        ml->handle.mouse_motion =
                            [ml] (const SDL_Event &e) -> void {
                                return ml->default_handler.look_around_target_camera(
                                    ml, e, -1.0f
                                );
                            };
                    }
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

    // quit program on ESC
    if (keystate[SDL_SCANCODE_ESCAPE] == 1) {
        ml->terminate();
        return;
    }
    switch (e.key.keysym.sym) {

        // quit program
        case SDLK_q:
            if (e.key.state == SDL_PRESSED) {
                ml->terminate();
            }
            break;

        // reset camera position
        case SDLK_c:
            if (e.key.state == SDL_PRESSED) {
                // reset position
                ml->camera.target.reset();
                ml->camera.recompute_transform();
                // reset camera.projection.fovy
                ml->camera.projection.set_all([ml] () {
                    auto all = ml->camera.projection.get_all();
                    all[0] = 60.0f;
                    return all;
                }());
            }
            break;

        // list OpenGL extenstions
        case SDLK_l:
            if (e.key.state == SDL_PRESSED) {
                ml->root->list_gl_extensions();
            }
            break;

        // display camera position
        case SDLK_SPACE:
            if (e.key.state == SDL_PRESSED) {
                std::cout
                    << "---------------- cam props. ----------------" << std::endl
                    << "    origin: " << ml->camera.transform.origin << std::endl
                    << "    target: " << ml->camera.target << std::endl
                    << "   forward: " << ml->camera.transform.forward << std::endl
                    << "projection: "
                    << vec4(ml->camera.projection.get_all().data()) << std::endl
                    << "      dist: " << ml->camera.dist << " "
                    << "yaw: " << ml->camera.yaw << " "
                    << "pitch: " << ml->camera.pitch
                    << std::endl;
            }
            break;

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
                << e.window.data1 << "x" << e.window.data2
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
    root{root},
    vertex_color_attrib_shader{shader::vs_basic_attribute_color, shader::fs_basic_color_in},
    vertex_color_uniform_shader{shader::vs_basic, shader::fs_basic_color_uniform}
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
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
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
    mat4 vp_matrix(this->camera.get_vp_matrix());

    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );

    // load first shader and ...
    this->vertex_color_attrib_shader.use(vp_matrix);

    // ... draw things with it
    glDisable(GL_CULL_FACE);
    glEnable(GL_LINE_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glLineWidth(2.2f);
    this->scene[0]->draw();
    glLineWidth(1.4f);
    this->scene[1]->draw();
    glPointSize(1.0f);
    this->scene[2]->draw();
    this->scene[3]->draw();
    glLineWidth(1.0f);
    this->scene[4]->draw();
    glLineWidth(1.4f);
    this->scene[2]->draw();

    glEnable(GL_CULL_FACE);

    // load second shader and ...
    this->vertex_color_uniform_shader.use_with_uniform_color(
        vp_matrix, vec4(0.3f, 0.3f, 0.3f, 1.0f)
    );

    // ... draw things with it
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    this->scene[6]->draw();

    glDisable(GL_CULL_FACE);

    // wireframe ...
    this->vertex_color_uniform_shader.use_with_uniform_color(
        vp_matrix, vec4(0.8f, 0.6f, 0.0f, 1.0f)
    );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1.0f);
    glPolygonOffset(-1.0f, -1.0f);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_LINE_SMOOTH);
    this->scene[6]->draw();
    glDisable(GL_POLYGON_OFFSET_LINE);
}




/**
 *  Main application loop.
 */
void MainLoop::run () {
    this->running = true;

    // prepare "scene"
    this->scene.push_back(primitives::axes(160.0f, 10.0f));
    this->scene.push_back(primitives::grid(160.0f, 10.0f, vec4(0.15f, 0.15f, 0.25f, 1)));
    this->scene.push_back(primitives::point_cube(160.0f*64.0f, 640.0f, 0.6f));
    this->scene.push_back(primitives::this_thing(160.0f*2.0f, 10.0f, 1.0f, 1.0f, GL_POINTS));
    this->scene.push_back(primitives::this_thing(160.0f*16.0f, 80.0f, 0.7f, 0.1f, GL_LINES));
    this->scene.push_back(primitives::grid(1100.0f, 5.0f, vec4(0.35f, 0.35f, 0.45f, 0.05)));
    this->scene.push_back(primitives::test_mesh());

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
