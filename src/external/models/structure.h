/**
 * @file structure.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the public interface for the structure of
 * model, mesh, and material objects. These objects provide a standard
 * blueprint for storing models within the program.
 * @date 2024-09-24
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

#ifndef LETO__MODELS__STRUCTURE
#define LETO__MODELS__STRUCTURE

// Standard macro definitions, like size_t.
#include <stddef.h>
// Leto-defined vector types.
#include <utilities/vectors.h>

/**
 * @brief A list of supported model formats. Only those models specified in
 * this list can be loaded by the program, anything else will throw errors.
 */
typedef enum leto_model_format_t
{
    /**
     * @brief Wavefront Object file, a text-based list-style 3D model
     * format that we use for prototyping within Leto. They're simple to
     * import and work with, although not as optimized for our use case as
     * some of the other formats, like FBX.
     */
    wavefront,
    /**
     * @brief Filmbox format, a format created originally for filmmaking in
     * 2005 and nowadays maintained by Autodesk. It is very useful for 3D
     * rendering, and is the format we use for final-edition models.
     * @note Not yet implemented.
     */
    fbx
} leto_model_format_t;

/**
 * @brief A standardized material object, containing all the details we
 * need from the material formats of both OBJ and FBX files.
 */
typedef struct leto_material_t
{
    /**
     * @brief A material name provided for identification purposes.
     */
    const char *name;
    /**
     * @brief The ambient lighting color of the object, defined as an RGB
     * value with components between 0 and 255.
     */
    leto_rgb_t ambient;
    /**
     * @brief The diffuse, or dispersed reflection color, of the object.
     * This is defined as an RGB value with components between 0 and 255.
     */
    leto_rgb_t diffuse;
    /**
     * @brief The specular color of the object. Similar to the diffuse
     * color, this is a reflection value, but instead of dispersed it's
     * concentrated in a single spot.
     */
    struct
    {
        /**
         * @brief The RGB array defining the specular color, with
         * components between 0 and 255.
         */
        leto_rgb_t _;
        /**
         * @brief The specular exponent of the color, or the glossiness of
         * it. This value can be from 0.000 to 1000.000.
         */
        float exponent;
    } specular;
    /**
     * @brief The transparency values of the material.
     */
    struct
    {
        /**
         * @brief The dissolve of the material. This is the global opacity
         * of the object, stored as a normalized value with 1.0
         * corresponding to fully opaque, and 0.0 fully transparent.
         */
        float dissolve;
        /**
         * @brief The optical density of the material is how hard it bends
         * the light that passes through it. This value is a normalized
         * number between 0.0 and 1.0.
         */
        float optical_density;
        /**
         * @brief The filter color of the light passing through this
         * object. This value is stored as an RGB color, with components
         * between 0 and 255.
         */
        leto_rgb_t filter_color;
    } transparency;
} leto_material_t;

/**
 * @brief A structure that defines a single, basic vertex; a position,
 * normal value, and texture coordinates.
 */
typedef struct leto_vertex_t
{
    /**
     * @brief The position of the vertex, with the X value of the vector
     * matching with the X coordinate, and so on.
     */
    leto_vec3_t position;
    /**
     * @brief A directional vector representation of the vertex's normal.
     */
    leto_vec3_t normal;
    /**
     * @brief The UV coordinates of the vector, stored as (X,Y) because we
     * assume no textures are being used that exceed two dimensions.
     */
    leto_vec2_t texture;
} leto_vertex_t;

/**
 * @brief A single mesh object. This is a piece of a model, typically a
 * single component or something that would generally benefit being
 * rendered separately from other objects, be it because it has other
 * materials added to it or something else.
 */
typedef struct leto_mesh_t
{
    /**
     * @brief This is a storage structure of all the unique sets of
     * vertices that we'll need to render the mesh. If two vertices are the
     * same, we don't bother storing their information twice, as that'd be
     * a waste of memory.
     */
    struct leto_vertex_list_t
    {
        /**
         * @brief The array of vertices.
         */
        leto_vertex_t *_;
        /**
         * @brief The count of vertices within the array. The last value
         * within the array is this value minus one.
         */
        size_t count;
    } vertices;
    /**
     * @brief This is an array of vertex indices, indexes into the the @ref
     * vertices array within this mesh that correspond to the vertex we
     * want to draw next. These are stored in sequential order, meaning
     * they are rendered in the same way they're loaded.
     */
    struct leto_index_list_t
    {
        /**
         * @brief The array of indices. These can never be zero, so we
         * store them as unsigned integers.
         */
        uint32_t *_;
        /**
         * @brief The count of indices in the array. The last value within
         * the array is this value minus one.
         */
        size_t count;
    } indices;
    /**
     * @brief This is a list of materials to use when rendering the mesh.
     * This includes things like ambient occlusion, transparency, color
     * filtering, and more. These values may be defined in a multitude of
     * ways, but are always expressed here in a standardized format.
     */
    struct leto_material_list_t
    {
        /**
         * @brief The list of materials.
         */
        leto_material_t *_;
        /**
         * @brief The count of materials within the array. The last value
         * in the array is this value minus one.
         */
        size_t count;
    } materials;
    /**
     * @brief The vertex array object of the mesh. This object stores all
     * necessary flags and vertex data that must be supplied before
     * rendering a mesh. This is assigned by the @ref LetoLoadModel
     * function. This value should rarely (if ever) change during a mesh's
     * lifetime.
     */
    unsigned int VAO;
} leto_mesh_t;

/**
 * @brief A model object. This is a collection of meshes and the universal
 * position they build off of. A name is also included for identification
 * purposes.
 */
typedef struct leto_model_t
{
    /**
     * @brief A UTF-8 encoded string representing the name of the model.
     * This is typically either a filename or expressly defined in the
     * model file(s).
     */
    const char *name;
    /**
     * @brief A vector representing the model's position in universal 3D
     * space. This value provides the baseline for all the meshes'
     * individual positions.
     */
    leto_vec3_t position;
    /**
     * @brief The rotation, in radians, of the full model.
     */
    float rotation;
    /**
     * @brief The list of meshes that make up the model. Some incredibly
     * simple models only have a single mesh, but most will have far more.
     */
    struct leto_mesh_list_t
    {
        /**
         * @brief The actual array of meshes.
         */
        leto_mesh_t *_;
        /**
         * @brief The count of meshes within the array. The last value in
         * the array can be accessed using this value minus one.
         */
        size_t count;
    } meshes;
} leto_model_t;

#endif // LETO__MODELS__STRUCTURE
