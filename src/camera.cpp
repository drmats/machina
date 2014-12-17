/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __CAMERA_CPP_
#define __CAMERA_CPP_ 1

#include "camera.h"

namespace machina {




/**
 *  Camera initialization.
 */
template < typename T >
Camera< T >::Camera () {
    this->rotation.load_identity();
    this->translation.load_identity();
}




/**
 *  Camera destruction.
 */
template < typename T >
Camera< T >::~Camera () {}




/**
 *  Establish camera projection in a viewport (GLfloat specialization).
 */
template <>
void Camera< GLfloat >::establish_projection () const {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(*this->projection.get_matrix());
}




/**
 *  Establish camera projection in a viewport (GLdouble specialization).
 */
template <>
void Camera< GLdouble >::establish_projection () const {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(*this->projection.get_matrix());
}




/**
 *  Establish camera modelview in a viewport (GLfloat specialization).
 */
template <>
void Camera< GLfloat >::establish_modelview () const {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(*this->rotation);
    glMultMatrixf(*this->translation);
}




/**
 *  Establish camera modelview in a viewport (GLdouble specialization).
 */
template <>
void Camera< GLdouble >::establish_modelview () const {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(*this->rotation);
    glMultMatrixd(*this->translation);
}




/**
 *  Instantiation for allowed types.
 */
template class Camera< GLfloat >;
template class Camera< GLdouble >;




} // namespace machina

#endif
