/**
 *  reindexer
 *
 *  Copyright (c) 2015, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __REINDEXER_CPP_
#define __REINDEXER_CPP_ 1

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <regex>
#include <vector>
#include <map>
#include <cstdlib>

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec3i = std::array<ushort, 3>;
using face = std::vector<vec3i>;
using vun = std::array<float, 8>;




/**
 *  Geometry container.
 */
typedef struct {
    std::vector<vec3> verts;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<face> faces;
    std::vector<vec3i> multi_indices;
    std::vector<ushort> indices;
} geometry;




/**
 *  Equality test with allowed error.
 */
template <typename T>
inline bool close_to (T a, T b) {
    return std::fabs(a - b) <= static_cast<T>(1e-5);
}




/**
 *  Construct regex matching "n" numbers (floats or ints).
 */
std::string construct_number_regex_string (
    std::size_t n,
    const std::string &sep_regex,
    const std::string &end_regex
) {
    static const std::string number_regex { "(-?[0-9]+(\\.?[0-9]+)?)" };
    std::string full_regex;

    for (std::size_t i = 0;  i < n;  i++) {
        full_regex += number_regex;
        if (i < n-1) { full_regex += sep_regex; }
    }
    full_regex += end_regex;

    return full_regex;
}




/**
 *  Try to parse N values of T in a line and push it back to "vecs" vector.
 */
template <typename T, std::size_t N>
std::size_t try_parse_vec (
    std::vector<std::array<T, N>> &vecs,
    const std::string &strbuf
) {
    std::regex vec_regex { construct_number_regex_string(N, "\\s*", ".*") };
    std::smatch match;
    std::array<T, N> out;

    if (std::regex_match(strbuf, match, vec_regex)) {
        if (match.size() == N*2+1) {
            try {
                for (std::size_t i = 1;  i < match.size();  i += 2) {
                    T val = std::stof(match[i].str());
                    // convert e.g. "0.999999" to "1.0"
                    if (close_to(val, std::round(val))) {
                        val = std::round(val);
                    }
                    // get rid of "-0.0"
                    if (val == 0) { val = std::fabs(val); }
                    out[(i-1)/2] = val;
                }
            } catch (...) { return 0; }
            vecs.push_back(out);
            return 1;
        }
    }

    return 0;
}




/**
 *  Try to parse line with any number of index-vectors (a/b/c a/b/c ...).
 */
std::size_t try_parse_face (
    std::vector<face> &faces,
    const std::string &strbuf
) {
    std::regex vec_regex { construct_number_regex_string(3, "/", "") };
    auto
        vecs_begin = std::sregex_iterator(
            strbuf.begin(), strbuf.end(), vec_regex
        ),
        vecs_end = std::sregex_iterator();
    vec3i iv;
    face current_face;

    for (auto it = vecs_begin;  it != vecs_end;  it++) {
        if (it->size() == 7) {
            try {
                for (std::size_t i = 1;  i < 7;  i += 2) {
                    iv[(i-1)/2] = std::stod((*it)[i].str());
                }
            } catch (...) { continue; }
            current_face.push_back(iv);
        }
    }
    if (current_face.size() != 0) {
        faces.push_back(current_face);
        return 1;
    }

    return 0;
}




/**
 *  "Flatten" faces to indices vector.
 */
void faces_to_multi_indices (
    const std::vector<face> &faces,
    std::vector<vec3i> &multi_indices
) {
    for (auto fit = faces.begin();  fit != faces.end();  fit++) {
        for (auto iit = fit->begin();  iit != fit->end();  iit++) {
            multi_indices.push_back(*iit);
        }
    }
}




/**
 *  (vertex, uv, normal) -> vun
 */
inline vun make_vun (const vec3 &v, const vec2 &u, const vec3 &n) {
    return {
        v[0], v[1], v[2],
        u[0], u[1],
        n[0], n[1], n[2]
    };
}




/**
 *  ...
 */
void reindex (const geometry &in, geometry &out) {
    std::map<vun, ushort> dict;
    ushort current_index { 0 };
    vun current_vun;
    std::map<vun, ushort>::iterator current;
    for (
        auto multi_index = in.multi_indices.begin();
        multi_index != in.multi_indices.end();
        multi_index++
    ) {
        current_vun = make_vun(
            in.verts[(*multi_index)[0] - 1],
            in.uvs[(*multi_index)[1] - 1],
            in.normals[(*multi_index)[2] - 1]
        );
        current = dict.find(current_vun);
        if (current == dict.end()) {
            dict.insert({current_vun, current_index});
            out.verts.push_back(in.verts[(*multi_index)[0] - 1]);
            out.uvs.push_back(in.uvs[(*multi_index)[1] - 1]);
            out.normals.push_back(in.normals[(*multi_index)[2] - 1]);
            out.indices.push_back(current_index);
            current_index += 1;
        } else {
            out.indices.push_back(std::get<1>(*current));
        }
    }
}




/**
 *  Array to string serialization.
 */
template <typename T, std::size_t N>
std::string array_to_string (
    const std::string &sep,
    const std::array<T, N> &a
) {
    std::stringstream ss;
    ss << std::setprecision(6) << std::fixed;
    for (std::size_t i = 0;  i < N;  i++) {
        ss << a[i];
        if (i < N-1) { ss << sep; }
    }
    return ss.str();
}




/**
 *  Vector of arrays to string serialization.
 */
template <typename T, std::size_t N>
std::string vector_arrays_to_string (
    const std::string &prefix,
    const std::vector<std::array<T, N>> &v,
    const std::string &array_sep,
    const std::string &suffix
) {
    std::stringstream ss;
    ss << std::setprecision(6) << std::fixed;
    for (auto it = v.begin();  it != v.end();  it++) {
        ss << prefix << array_to_string(array_sep, *it) << suffix;
    }
    return ss.str();
}




/**
 *  Vector of faces to string serialization.
 */
std::string vector_faces_to_string (
    const std::string &prefix,
    const std::vector<face> &v
) {
    std::stringstream ss;
    ss << std::setprecision(6) << std::fixed;
    for (auto it = v.begin();  it != v.end();  it++) {
        ss << prefix << vector_arrays_to_string(" ", *it, "/", "") << "\n";
    }
    return ss.str();
}




/**
 *  Vector of multi_indices (vec3i) to string serialization.
 */
std::string vector_multi_indices_to_string (
    const std::string &prefix,
    const std::vector<vec3i> &mi
) {
    std::stringstream ss;
    ss << prefix;
    for (std::size_t i = 0;  i < mi.size();  i++) {
        ss << array_to_string("/", mi[i]);
        if (i < mi.size() - 1) { ss << " "; }
    }
    ss << "\n";
    return ss.str();
}




/**
 *  Vector of indices (T) to string serialization.
 */
template <typename T>
std::string vector_indices_to_string (
    const std::string &prefix,
    const std::vector<T> &ind
) {
    std::stringstream ss;
    ss << prefix;
    for (std::size_t i = 0;  i < ind.size();  i++) {
        ss << ind[i];
        if (i < ind.size() - 1) { ss << " "; }
    }
    ss << "\n";
    return ss.str();
}




/**
 *  Reindex obj file -- multiple indexes to one index.
 */
int main (int argc, char *argv[]) {
    const unsigned int max_line_size { 8192 };
    std::ifstream file_input;
    std::array<char, max_line_size> buffer;
    std::string strbuf;
    std::regex line_regex { "^\\s*(\\w+)\\s*(.*)$" };
    std::smatch match;
    geometry input, output;

    // check for input file ...
    if (argc < 2) {
        std::cerr << "No input file." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // ... and try to open it
    file_input.open(argv[1]);
    if (!file_input) {
        std::cerr << "Cannot open input file: " << argv[1] << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // parse input file line-by-line
    while (!file_input.eof()) {
        file_input.getline(buffer.data(), max_line_size);
        strbuf = buffer.data();
        if (std::regex_match(strbuf, match, line_regex)) {
            if (match.size() == 3) {
                if (match[1].str() == "v") {
                    try_parse_vec(input.verts, match[2].str());
                } else if (match[1].str() == "vn") {
                    try_parse_vec(input.normals, match[2].str());
                } else if (match[1].str() == "vt") {
                    try_parse_vec(input.uvs, match[2].str());
                } else if (match[1].str() == "f") {
                    try_parse_face(input.faces, match[2].str());
                } else {
                    std::cerr
                        << "Ignoring line starting with "
                        << "\"" << match[1].str() << "\"."
                        << std::endl;
                }
            }
        } else {
            std::cerr
                << "Ignoring comment (or some garbage) line."
                << std::endl;
        }
    }

    // ...
    faces_to_multi_indices(input.faces, input.multi_indices);

    // ...
    reindex(input, output);

    // print-out converted stuff
    std::cout
        << vector_arrays_to_string("v ", output.verts, " ", "\n")
        << vector_arrays_to_string("vt ", output.uvs, " ", "\n")
        << vector_arrays_to_string("vn ", output.normals, " ", "\n")
        << vector_indices_to_string("indices ", output.indices);
        // << vector_faces_to_string("f", input.faces)
        // << vector_multi_indices_to_string("mi ", input.multi_indices);

    std::exit(EXIT_SUCCESS);
}




#endif
