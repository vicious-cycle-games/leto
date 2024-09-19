/**
 * @file meshes.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for loading and managing
 * mesh objects of various different types.
 * @date 2024-09-15
 *
 * @copyright (c) 2024 - the Leto Team
 * This file is under the GNU Affero General Public License v3.0. That
 * means it is fully legal to distribute this source code and/or any
 * modified copies of this source code in both private and commercial
 * contexts. However, you must note copyright and state any changes you've
 * made. Source disclosure is also necessary. For more information, see the
 * @file LICENSE.md file that should have been distributed alongside this
 * source code.
 */

#ifndef LETO__MESHES__EXTERNAL
#define LETO__MESHES__EXTERNAL

#include <vec3.h>
#include <vec4.h>

typedef enum
{
    wavefront
} leto_mesh_format_t;

typedef enum
{
    color_no_ambient,
    color_w_ambient,
    highlight,
    reflection_w_raytrace,
    glass_w_raytrace,
    fresnel_w_raytrace,
    refraction_no_fresnel_w_raytrace,
    refraction_w_fresnel_w_raytrace,
    reflection_no_raytrace,
    glass,
    no_raytrace,
    shadows_on_invis,
} leto_material_illumination_t;

typedef struct
{
    const char *name;
    vec3 ambient;
    vec3 diffuse;
    vec4 specular; // se is specular[3]
    float dissolve;
    vec3 tfilter_color;
    float optical_density;
    leto_material_illumination_t illumination;
} leto_material_t;

typedef struct
{
    ivec3 *elements; // v/vt/vn
    size_t element_count;
} leto_face_t;

typedef struct
{
    const char *name;
    leto_mesh_format_t format;
    vec3 *vertices;
    size_t vertex_count;
    vec3 *ps_vertices;
    size_t ps_vertex_count;
    vec3 *texture_coords;
    size_t texture_coord_count;
    vec3 *normals;
    size_t normal_count;
    leto_face_t *faces;
    size_t face_count;
    leto_material_t material;
} leto_mesh_t;

#define LETO_MAX_FACE_ELEMENTS 32

leto_mesh_t *LetoLoadMesh(const char *name, leto_mesh_format_t format);
void LetoUnloadMesh(leto_mesh_t *mesh);

#endif // LETO__MESHES__EXTERNAL
