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
#include <cstring>
#include <array>
#include <regex>
#include <vector>
#include <map>
#include <functional>
#include <cstdlib>

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using flat = std::vector<float>;
using multi_index = std::vector<ushort>;
using face = std::vector<multi_index>;
using vun = std::array<float, 8>;




/**
 *  Geometry container.
 */
typedef struct {
    std::vector<vec3> verts;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<face> faces;
    std::vector<multi_index> multi_indices;
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
    static const std::regex vec_regex {
        construct_number_regex_string(N, "\\s*", ".*")
    };
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
 *  Try to parse line with any number of index-vectors
 *  (a/b/c a/b/c ... or a//c a//c ...).
 */
template <const char sep1, const char sep2 = '\0'>
std::size_t try_parse_face (
    std::vector<face> &faces,
    const std::string &strbuf
) {
    static const char sep[] { sep1, sep2 };
    static const size_t n { 4 - std::strlen(sep) };
    static const std::regex vec_regex {
        construct_number_regex_string(n, sep, "")
    };
    auto
        vecs_begin = std::sregex_iterator(
            strbuf.begin(), strbuf.end(), vec_regex
        ),
        vecs_end = std::sregex_iterator();
    multi_index mi;
    face current_face;

    for (auto it = vecs_begin;  it != vecs_end;  it++) {
        if (it->size() == 1+2*n) {
            try {
                mi.clear();
                for (std::size_t i = 1;  i < 1+2*n;  i += 2) {
                    mi.push_back(std::stod((*it)[i].str()));
                }
            } catch (...) { continue; }
            current_face.push_back(mi);
        }
    }
    if (current_face.size() != 0) {
        faces.push_back(current_face);
        return 1;
    }

    return 0;
}




/**
 *  Parse input file line-by-line.
 */
void parse (geometry &g, std::ifstream &file_input) {
    const unsigned int max_line_size { 8192 };
    std::array<char, max_line_size> buffer;
    std::string strbuf;
    std::regex line_regex { "^\\s*(\\w+)\\s*(.*)$" };
    std::smatch match;

    while (!file_input.eof()) {
        file_input.getline(buffer.data(), max_line_size);
        strbuf = buffer.data();
        if (std::regex_match(strbuf, match, line_regex)) {
            if (match.size() == 3) {
                if (match[1].str() == "v") {
                    try_parse_vec(g.verts, match[2].str());
                } else if (match[1].str() == "vn") {
                    try_parse_vec(g.normals, match[2].str());
                } else if (match[1].str() == "vt") {
                    try_parse_vec(g.uvs, match[2].str());
                } else if (match[1].str() == "f") {
                    if (
                        // try to parse face with vertex/uv/normal structure
                        try_parse_face<'/'>(
                            g.faces, match[2].str()
                        ) == 0
                    ) {
                        // or try to parse face with vertex//normal structure
                        try_parse_face<'/', '/'>(
                            g.faces, match[2].str()
                        );
                    }
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

    // "Flatten" faces to indices vector.
    for (auto fit = g.faces.begin();  fit != g.faces.end();  fit++) {
        for (auto iit = fit->begin();  iit != fit->end();  iit++) {
            g.multi_indices.push_back(*iit);
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
void reindex (geometry &out, const geometry &in) {
    std::map<vun, ushort> dict;
    ushort current_index { 0 };
    vun current_vun;
    std::map<vun, ushort>::iterator current;
    static const vec2 empty_uv { 0.0, 0.0 };
    vec3 const *current_vertex;
    vec3 const *current_normal;
    vec2 const *current_uv;
    for (
        auto multi_index = in.multi_indices.begin();
        multi_index != in.multi_indices.end();
        multi_index++
    ) {
        current_vertex = &in.verts[(*multi_index)[0] - 1];
        if (multi_index->size() == 3) {
            current_uv = &in.uvs[(*multi_index)[1] - 1];
            current_normal = &in.normals[(*multi_index)[2] - 1];
        } else {
            current_uv = &empty_uv;
            current_normal = &in.normals[(*multi_index)[1] - 1];
        }
        current_vun = make_vun(*current_vertex, *current_uv, *current_normal);
        current = dict.find(current_vun);
        if (current == dict.end()) {
            dict.insert({current_vun, current_index});
            out.verts.push_back(*current_vertex);
            out.uvs.push_back(*current_uv);
            out.normals.push_back(*current_normal);
            out.indices.push_back(current_index);
            current_index += 1;
        } else {
            out.indices.push_back(std::get<1>(*current));
        }
    }
}




/**
 *  Enumerable to string serialization.
 */
template <typename T>
std::string enumerable_to_string (
    const T &e,
    const std::string &px,
    const std::string &p,
    const std::string &s,
    const std::string &sx
) {
    std::stringstream ss;
    if (e.size() > 0) {
        ss << std::setprecision(6) << std::fixed << px;
        for (std::size_t i = 0;  i < e.size();  i++) {
            ss << p << e[i];
            if (i < e.size() - 1) { ss << s; }
        }
        ss << sx;
        return ss.str();
    }
    return "";
}




/**
 *  Enumerable of enumerables to string serialization.
 */
template <typename T>
std::string higher_enumerable_to_string (
    const std::vector<T> &ee,
    const std::string &px,
    const std::string &p,
    std::function<std::string (const T &)> to_str,
    const std::string &s,
    const std::string &sx
) {
    std::stringstream ss;
    if (ee.size() > 0) {
        ss << std::setprecision(6) << std::fixed << px;
        for (std::size_t i = 0;  i < ee.size();  i++) {
            ss << p << to_str(ee[i]);
            if (i < ee.size() - 1) { ss << s; }
        }
        ss << sx;
        return ss.str();
    }
    return "";
}




/**
 *  Geometry serializer.
 */
std::ostream& operator<< (std::ostream &os, const geometry &g) {
    auto vec3_to_string = [] (const vec3 &v) -> std::string {
        return enumerable_to_string(v, "", "", " ", "");
    };
    auto vec2_to_string = [] (const vec2 &v) -> std::string {
        return enumerable_to_string(v, "", "", " ", "");
    };
    auto multi_index_to_string = [] (const multi_index &mi) -> std::string {
        if (mi.size() == 2) {
            return enumerable_to_string(mi, "", "", "//", "");
        }
        return enumerable_to_string(mi, "", "", "/", "");
    };

    return os
        << higher_enumerable_to_string<vec3>(
            g.verts, "", "v ", vec3_to_string, "\n", "\n"
        )
        << higher_enumerable_to_string<vec2>(
            g.uvs, "", "vt ", vec2_to_string, "\n", "\n"
        )
        << higher_enumerable_to_string<vec3>(
            g.normals, "", "vn ", vec3_to_string, "\n", "\n"
        )
        << higher_enumerable_to_string<face>(
            g.faces, "", "f ",
            [&] (const face &f) -> std::string {
                return higher_enumerable_to_string<multi_index>(
                    f, "", "", multi_index_to_string, " ", ""
                );
            }, "\n", "\n"
        )
        << higher_enumerable_to_string<multi_index>(
            g.multi_indices, "mi ", "", multi_index_to_string, " ", "\n"
        )
        << enumerable_to_string(g.indices, "i ", "", " ", "\n");
}




/**
 *  Reindex obj file -- multiple indexes to one index.
 */
int main (int argc, char *argv[]) {
    std::ifstream file_input;
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

    // parse input file
    parse(input, file_input);

    file_input.close();

    // ...
    reindex(output, input);

    // print-out this stuff
    std::cout
        << "# -------------- Parsed input: --------------\n"
        << input
        << "# -------------- Parsed output: -------------\n"
        << output;

    std::exit(EXIT_SUCCESS);
}




#endif
