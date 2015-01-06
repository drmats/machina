/**
 *  machina
 *
 *  Copyright (c) 2015, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __GFRAME_CPP_
#define __GFRAME_CPP_ 1

#include "gframe.hpp"

namespace machina {




/**
 *  GFrame initialization.
 */
template <typename T>
GFrame<T>::GFrame ():
    forward { static_cast<T>(0.0f), static_cast<T>(0.0f), static_cast<T>(-1.0f) },
    up { static_cast<T>(0.0f), static_cast<T>(1.0f), static_cast<T>(0.0f) }
    {}




/**
 *  Instantiation of allowed types.
 */
template class GFrame<GLfloat>;
template class GFrame<GLdouble>;




} // namespace machina

#endif
