/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __PRIMITIVES_HPP_
#define __PRIMITIVES_HPP_ 1

#include "batch.hpp"
#include <memory>

namespace machina {
    namespace primitives {




/**
 *  Axes.
 */
std::shared_ptr<VertexColorBatch> axes (GLfloat, GLfloat);


/**
 *  Grid.
 */
std::shared_ptr<VertexColorBatch> grid (
    GLfloat, GLfloat, m3d::GVector4<GLfloat>
);


/**
 *  Point-cube.
 */
std::shared_ptr<VertexColorBatch> point_cube (GLfloat, GLfloat, GLfloat);


/**
 *  This thing...
 */
std::shared_ptr<VertexColorBatch> this_thing (
    GLfloat, GLfloat,
    GLfloat, GLfloat, GLenum
);




    } // namespace primitives
} // namespace grid

#endif
