/**
 *  machina
 *
 *  Copyright (c) 2015, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MESH_LOADER_HPP_
#define __MESH_LOADER_HPP_ 1

#include "batch.hpp"
#include <memory>
#include <string>
#include <stdexcept>

namespace machina {




/**
 *  *.ooo file loader.
 */
std::shared_ptr<TriangleBatch> load_mesh (
    const std::string &path
) noexcept(false);




} // namespace grid

#endif
