/**
 *  machina
 *
 *  Copyright (c) 2015, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MESH_LOADER_CPP_
#define __MESH_LOADER_CPP_ 1

#include "mesh_loader.hpp"
#include <vector>
#include <fstream>
#include <cstdint>
#include <cstring>

namespace machina {

using vec2 = m3d::GVector2<GLfloat>;
using vec3 = m3d::GVector3<GLfloat>;




/**
 *  Mesh container.
 */
typedef struct {
    std::vector<vec3> verts;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<GLushort> indices;
} mesh;




/**
 *  Allocate space in a given vector.
 */
template <typename V>
V& vector_allocate (V &v, std::size_t len) {
    v.clear(); v.reserve(len);
    v.insert(v.end(), len, typename V::value_type {});
    v.shrink_to_fit();
    return v;
}




/**
 *  Read mesh from binary format.
 */
void read_bin_mesh (
    mesh &m,
    std::ifstream &file_input
) throw (std::runtime_error) {
    char magic_string[4] { 0 };
    std::uint32_t
        uint32_s = sizeof(std::uint32_t),
        vec3_s, vec2_s, ushort_s,
        verts_length,
        uvs_length,
        normals_length,
        indices_length;

    file_input.read(magic_string, 4);

    if (std::strncmp("OoOo", magic_string, 4) != 0) {
        throw std::runtime_error("Not a .ooo format.");
    }

    file_input
        .read(reinterpret_cast<char*>(&vec3_s), uint32_s)
        .read(reinterpret_cast<char*>(&vec2_s), uint32_s)
        .read(reinterpret_cast<char*>(&ushort_s),uint32_s)
        .read(reinterpret_cast<char*>(&verts_length), uint32_s)
        .read(reinterpret_cast<char*>(&uvs_length), uint32_s)
        .read(reinterpret_cast<char*>(&normals_length), uint32_s)
        .read(reinterpret_cast<char*>(&indices_length), uint32_s);

    vector_allocate(m.verts, verts_length);
    vector_allocate(m.uvs, uvs_length);
    vector_allocate(m.normals, normals_length);
    vector_allocate(m.indices, indices_length);

    file_input
        .read(
            reinterpret_cast<char*>(m.verts.data()),
            verts_length * vec3_s
        )
        .read(
            reinterpret_cast<char*>(m.uvs.data()),
            uvs_length * vec2_s
        )
        .read(
            reinterpret_cast<char*>(m.normals.data()),
            normals_length * vec3_s
        )
        .read(
            reinterpret_cast<char*>(m.indices.data()),
            indices_length * ushort_s
        );
}




/**
 *  *.ooo file loader.
 */
std::shared_ptr<TriangleBatch> load_mesh (
    const std::string &path
) throw (std::runtime_error) {
    std::ifstream file_input;
    mesh geometry;
    auto batch = std::make_shared<TriangleBatch>();

    file_input.open(
        path,
        std::ios::in | std::ios::binary
    );
    if (!file_input) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    read_bin_mesh(geometry, file_input);
    file_input.close();

    batch->prepare(
        geometry.verts,
        geometry.normals,
        geometry.uvs,
        geometry.indices
    );

    return batch;
}




} // namespace grid

#endif
