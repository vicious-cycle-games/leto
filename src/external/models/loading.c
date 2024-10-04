/**
 * @file loading.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A file to provide the implementation of the public model loading
 * interface defined in @file loading.h.
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

#include "loading.h" // Public interface parent

#include <cimport.h>
#include <postprocess.h>
#include <scene.h>

#include <external/files.h>            // Leto file utilities
#include <utilities/macros.h>          // Degrees to radians
#include <utilities/memory/allocate.h> // Leto memory allocators
#include <utilities/memory/free.h>     // Leto memory free-rs

#include <gl.h>     // OpenGL function macros
#include <stdio.h>  // Standard I/O functionality
#include <string.h> // Standard string manipulations

leto_mesh_t ProcessMesh_(const struct aiScene *scene, struct aiMesh *mesh)
{
    leto_mesh_t created_mesh = {
        {LetoAllocate(mesh->mNumVertices), mesh->mNumVertices},
        {NULL, 0},
        {NULL, 0}};

    // walk through each of the mesh's vertices
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        leto_vertex_t *current_vertex = &created_mesh.vertices._[i];
        current_vertex->position = *(leto_vec3_t *)(&mesh->mVertices[i]);
        current_vertex->normal = *(leto_vec3_t *)(&mesh->mNormals[i]);
        current_vertex->tangent = *(leto_vec3_t *)(&mesh->mTangents[i]);
        current_vertex->bitangent =
            *(leto_vec3_t *)(&mesh->mBitangents[i]);
        // a vertex can contain up to 8 different texture coordinates.
        // We thus make the assumption that we won't use models where a
        // vertex can have multiple texture coordinates so we always
        // take the first set (0).
        current_vertex->texture =
            *(leto_vec2_t *)(&mesh->mTextureCoords[0][i]);
    }

    // now wak through each of the mesh's faces (a face is a mesh its
    // triangle) and retrieve the corresponding vertex indices.
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        struct aiFace face = mesh->mFaces[i];
        created_mesh.indices._ = LetoReallocate(
            created_mesh.indices._,
            4 * (created_mesh.indices.count += face.mNumIndices));

        for (size_t j = 0; j < face.mNumIndices; j++)
            created_mesh.indices
                ._[created_mesh.indices.count - (face.mNumIndices - j)] =
                face.mIndices[j];
    }

    // process materials
    // struct aiMaterial *material =
    // scene->mMaterials[mesh->mMaterialIndex];
    // // we assume a convention for sampler names in the shaders. Each
    // // diffuse texture should be named as 'texture_diffuseN' where N is
    // a
    // // sequential number ranging from 1 to MAX_SAMPLER_NUMBER. Same
    // applies
    // // to other texture as the following list summarizes: diffuse:
    // // texture_diffuseN specular: texture_specularN normal:
    // texture_normalN

    // // 1. diffuse maps
    // vector<Texture> diffuseMaps = loadMaterialTextures(
    //     material, aiTextureType_DIFFUSE, "texture_diffuse");
    // textures.insert(textures.end(), diffuseMaps.begin(),
    //                 diffuseMaps.end());
    // // 2. specular maps
    // vector<Texture> specularMaps = loadMaterialTextures(
    //     material, aiTextureType_SPECULAR, "texture_specular");
    // textures.insert(textures.end(), specularMaps.begin(),
    //                 specularMaps.end());
    // // 3. normal maps
    // std::vector<Texture> normalMaps = loadMaterialTextures(
    //     material, aiTextureType_HEIGHT, "texture_normal");
    // textures.insert(textures.end(), normalMaps.begin(),
    // normalMaps.end());
    // // 4. height maps
    // std::vector<Texture> heightMaps = loadMaterialTextures(
    //     material, aiTextureType_AMBIENT, "texture_height");
    // textures.insert(textures.end(), heightMaps.begin(),
    // heightMaps.end());

    return created_mesh;
}

void ASSIMPProcessNode_(leto_model_t *model, const struct aiScene *scene,
                        struct aiNode *node)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual
        // objects in the scene. the scene contains all the data, node is
        // just to keep stuff organized (like relations between nodes).
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        model->meshes._ = LetoReallocate(model->meshes._,
                                         sizeof(leto_mesh_t) *
                                             (model->meshes.count += 1));
        if (model->meshes._ == NULL) return;
        model->meshes._[model->meshes.count - 1] =
            ProcessMesh_(scene, mesh);
    }
    // after we've processed all of the meshes (if any) we then recursively
    // process each of the children nodes
    for (size_t i = 0; i < node->mNumChildren; i++)
        ASSIMPProcessNode_(model, scene, node->mChildren[i]);
}

void ASSIMPLoad_(const char *path, leto_model_t *model)
{
    const struct aiScene *scene = aiImportFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                  aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (scene == NULL)
    {
        fprintf(stderr, "Failed to load model: %s\n", aiGetErrorString());
        return;
    }

    ASSIMPProcessNode_(model, scene, scene->mRootNode);
}

leto_model_t *LetoLoadModel(const char *path, leto_vec3_t position,
                            float rotation)
{
    if (path == NULL) return NULL;

    char *full_path = LetoAllocate(LETO_FILE_PATH_MAX);
    sprintf(full_path, ASSET_DIR "/models/%s", path);

    leto_model_t *created_model = LetoAllocate(sizeof(leto_model_t));
    ASSIMPLoad_(full_path, created_model);

    created_model->name = strdup(path);
    created_model->position = position;
    created_model->rotation = DEGREE_TO_RAD(rotation);

    return created_model;
}

void LetoUnloadModel(leto_model_t *model)
{
    if (model == NULL) return;

    LetoFree((void **)&model->name);
    model->position.x = 0;
    model->position.y = 0;
    model->position.z = 0;
    model->rotation = 0.0f;

    for (size_t i = 0; i < model->meshes.count; i++)
    {
        leto_mesh_t *current_mesh = &model->meshes._[i];

        // Delete the underlying OpenGL object, not just our ID of it.
        glDeleteVertexArrays(1, &current_mesh->VAO);
        current_mesh->VAO = 0;

        LetoFree((void **)&current_mesh->vertices._);
        LetoFree((void **)&current_mesh->indices._);
        current_mesh->vertices.count = 0;
        current_mesh->indices.count = 0;

        for (size_t j = 0; j < current_mesh->materials.count; j++)
            LetoFree((void **)&current_mesh->materials._[j].name);
        LetoFree((void **)&current_mesh->materials._);
        current_mesh->materials.count = 0;
    }
}
