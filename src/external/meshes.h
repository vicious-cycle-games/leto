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
    const char *name;
    leto_mesh_format_t format;
    vec4 *vertices;
    vec3 *ps_vertices;
    vec3 *texture_coords;
    vec3 *normals;
    // Since faces aren't necessarily constrained to a specific count of
    // vertices, we use a two-dimension array of faces where any one face
    // value can be accessed via faces[i][j], where i is the face number
    // you're accessing and j is the index into the face array we're
    // looking for. The first number in the face is the count of values in
    // said face.
    // Ex. f 1/3/4 :: faces[i]{3, 1, 3, 4}
    uint32_t **faces;
    leto_material_t material;
} leto_mesh_t;

leto_mesh_t *LetoLoadMesh(const char *name, leto_mesh_format_t format);

#endif // LETO__MESHES__EXTERNAL
