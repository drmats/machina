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
#include <numeric>
#include <algorithm>
#include <cstdint>
#include <cstdlib>

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using flat = std::vector<float>;
using multi_index = std::vector<ushort>;
using face = std::vector<multi_index>;




/**
 *  Mesh container.
 */
typedef struct {
    std::vector<vec3> verts;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<face> faces;
    std::vector<multi_index> multi_indices;
    std::vector<ushort> indices;
} mesh;




/**
 *  Equality test with allowed error.
 */
template <typename T>
inline bool close_to (T a, T b) {
    return std::fabs(a - b) <= static_cast<T>(1e-5);
}




/**
 *  Vector concatenator ( vector<T>  <-  vector<vector<T>> ).
 */
template <typename T>
void vector_flatten (
    std::vector<T> &out,
    const std::vector<std::vector<T>> &in
) {
    out.clear();
    out.reserve(std::accumulate(
        in.begin(), in.end(), 0,
        [] (
            const typename std::vector<T>::size_type &acc,
            const std::vector<T> &el
        ) -> typename std::vector<T>::size_type {
            return acc + el.size();
        }
    ));
    std::for_each(in.begin(), in.end(), [&] (const std::vector<T> &vec_el) {
        std::for_each(vec_el.begin(), vec_el.end(), [&] (const T &el) {
            out.push_back(el);
        });
    });
}




/**
 *  Create std::vector from any iterable.
 */
template <
    template <class, std::size_t...> class I,
    class T,
    std::size_t... Rest
>
std::vector<T> make_vector (const I<T, Rest...> &i) {
    return { i.begin(), i.end() };
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
    face current_face;

    std::for_each(
        std::sregex_iterator(strbuf.begin(), strbuf.end(), vec_regex),
        std::sregex_iterator(),
        [&] (const std::smatch &match) {
            multi_index mi;
            if (match.size() == 1+2*n) {
                for (std::size_t i = 1;  i < 1+2*n;  i += 2) {
                    mi.push_back(std::stod(match[i].str()));
                }
                current_face.push_back(mi);
            }
    });

    if (current_face.size() != 0) {
        faces.push_back(current_face);
        return 1;
    }

    return 0;
}




/**
 *  Parse input file line-by-line.
 */
void parse_mesh (mesh &m, std::ifstream &file_input) {
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
                    try_parse_vec(m.verts, match[2].str());
                } else if (match[1].str() == "vn") {
                    try_parse_vec(m.normals, match[2].str());
                } else if (match[1].str() == "vt") {
                    try_parse_vec(m.uvs, match[2].str());
                } else if (match[1].str() == "f") {
                    if (
                        // try to parse face with vertex/uv/normal structure
                        try_parse_face<'/'>(
                            m.faces, match[2].str()
                        ) == 0
                    ) {
                        // or try to parse face with vertex//normal structure
                        try_parse_face<'/', '/'>(
                            m.faces, match[2].str()
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
                << "Ignoring comment, empty or some garbage line."
                << std::endl;
        }
    }

    // "flatten" faces to indices vector.
    vector_flatten(m.multi_indices, m.faces);
}




/**
 *  Index <- multi-index.
 */
void reindex (mesh &out, const mesh &in) {
    std::map<flat, ushort> dict;
    ushort current_index { 0 };

    std::for_each (
        in.multi_indices.begin(),
        in.multi_indices.end(),
        [&] (const multi_index &mi) {
            vec3 const *current_vertex = nullptr;
            vec3 const *current_normal = nullptr;
            vec2 const *current_uv = nullptr;
            flat current_flat;

            current_vertex = &in.verts[mi[0] - 1];
            if (mi.size() == 2) {
                current_normal = &in.normals[mi[1] - 1];
                vector_flatten(current_flat, [&] () -> std::vector<flat> {
                    return {
                        make_vector(*current_vertex),
                        make_vector(*current_normal)
                    };
                }());
            } else {
                current_uv = &in.uvs[mi[1] - 1];
                current_normal = &in.normals[mi[2] - 1];
                vector_flatten(current_flat, [&] () -> std::vector<flat> {
                    return {
                        make_vector(*current_vertex),
                        make_vector(*current_uv),
                        make_vector(*current_normal)
                    };
                }());
            }

            auto current = dict.find(current_flat);

            if (current == dict.end()) {
                dict.insert({current_flat, current_index});
                out.verts.push_back(*current_vertex);
                if (current_uv != nullptr) { out.uvs.push_back(*current_uv); }
                out.normals.push_back(*current_normal);
                out.indices.push_back(current_index);
                current_index += 1;
            } else {
                out.indices.push_back(std::get<1>(*current));
            }
    });
}




/**
 *  Iterable to string serialization.
 */
template <typename T>
std::string iterable_to_string (
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
 *  Iterable of iterables to string serialization.
 */
template <typename T>
std::string higher_iterable_to_string (
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
 *  Mesh serializer.
 */
std::ostream& operator<< (std::ostream &os, const mesh &m) {
    auto vec3_to_string = [] (const vec3 &v) -> std::string {
        return iterable_to_string(v, "", "", " ", "");
    };
    auto vec2_to_string = [] (const vec2 &v) -> std::string {
        return iterable_to_string(v, "", "", " ", "");
    };
    auto multi_index_to_string = [] (const multi_index &mi) -> std::string {
        if (mi.size() == 2) {
            return iterable_to_string(mi, "", "", "//", "");
        }
        return iterable_to_string(mi, "", "", "/", "");
    };

    return os
        << higher_iterable_to_string<vec3>(
            m.verts, "", "v ", vec3_to_string, "\n", "\n"
        )
        << higher_iterable_to_string<vec2>(
            m.uvs, "", "vt ", vec2_to_string, "\n", "\n"
        )
        << higher_iterable_to_string<vec3>(
            m.normals, "", "vn ", vec3_to_string, "\n", "\n"
        )
        << higher_iterable_to_string<face>(
            m.faces, "", "f ",
            [&] (const face &f) -> std::string {
                return higher_iterable_to_string<multi_index>(
                    f, "", "", multi_index_to_string, " ", ""
                );
            }, "\n", "\n"
        )
        << higher_iterable_to_string<multi_index>(
            m.multi_indices, "mi ", "", multi_index_to_string, " ", "\n"
        )
        << iterable_to_string(m.indices, "i ", "", " ", "\n");
}




/**
 *  ...
 */
void write_bin_mesh (std::ofstream &file_output, mesh &m) {
    std::size_t
        uint32_s = sizeof(std::uint32_t),
        vec3_s = sizeof(vec3),
        vec2_s = sizeof(vec2),
        ushort_s = sizeof(ushort);
    std::uint32_t
        verts_length = static_cast<std::uint32_t>(m.verts.size()),
        uvs_length = static_cast<std::uint32_t>(m.uvs.size()),
        normals_length = static_cast<std::uint32_t>(m.normals.size()),
        indices_length = static_cast<std::uint32_t>(m.indices.size());

    file_output
        .write(reinterpret_cast<const char*>(&verts_length), uint32_s)
        .write(reinterpret_cast<const char*>(&uvs_length), uint32_s)
        .write(reinterpret_cast<const char*>(&normals_length), uint32_s)
        .write(reinterpret_cast<const char*>(&indices_length), uint32_s)
        .write(
            reinterpret_cast<const char*>(m.verts.data()),
            verts_length * vec3_s
        )
        .write(
            reinterpret_cast<const char*>(m.uvs.data()),
            uvs_length * vec2_s
        )
        .write(
            reinterpret_cast<const char*>(m.normals.data()),
            normals_length * vec3_s
        )
        .write(
            reinterpret_cast<const char*>(m.indices.data()),
            indices_length * ushort_s
        );
}




/**
 *  Reindex obj file -- multiple indexes to one index.
 */
int main (int argc, char *argv[]) {
    std::ifstream file_input;
    std::ofstream file_output;
    mesh input, output;

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
    parse_mesh(input, file_input);

    file_input.close();

    // ...
    reindex(output, input);

    // print-out this stuff
    std::cout
        << "# -------------- Parsed input: --------------\n"
        << input
        << "# -------------- Parsed output: -------------\n"
        << output;

    // check for output file ...
    if (argc == 3) {
        // ... and try to open it
        file_output.open(
            argv[2],
            std::ios::out | std::ios::binary | std::ios::trunc
        );
        if (!file_output) {
            std::cerr << "Cannot open output file: " << argv[2] << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // ...
        write_bin_mesh(file_output, output);

        file_output.close();
    }

    std::exit(EXIT_SUCCESS);
}




#endif
