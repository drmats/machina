/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#include "camera.h"




/**
 * Instantiation for used types.
 */
template class Camera< GLfloat >;
template class Camera< GLdouble >;




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
void Camera< GLfloat >::establish () {
    glLoadMatrixf(*this->rotation);
    glMultMatrixf(*this->translation);
}




/**
 *  Establish camera projection in a viewport (GLdouble specialization).
 */
template <>
void Camera< GLdouble >::establish () {
    glLoadMatrixd(*this->rotation);
    glMultMatrixd(*this->translation);
}
