#ifndef LETO__MODELS__STRUCTURE
#define LETO__MODELS__STRUCTURE

#include <stddef.h>
#include <stdint.h>
#include <utilities/attributes.h>
#include <utilities/vectors.h>

typedef enum
{
    wavefront,
    stl // unimplemented
} leto_model_format_t;

typedef struct
{
    leto_vec3_t position;
    leto_vec3_t normal;
    leto_vec2_t texture;
} leto_vertex_t;

typedef struct
{
    const char *name;
    leto_vec3_t ambient;
    leto_vec3_t diffuse;
    leto_vec4_t specular; // se is specular[3]
    float dissolve;
    leto_vec3_t dfilter_color;
    float optical_density;
    enum
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
    } LETO_PACKED illumination;
} leto_material_t;

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
    unsigned int VBO, EBO;
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
