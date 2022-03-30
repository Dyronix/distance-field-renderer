#include "regina_pch.h"

#include "mesh.h"
#include "model.h"
#include "model_importer.h"
#include "submesh.h"
#include "triangle.h"
#include "vertex.h"

#include "min_max.h"

#include "algorithms/max.h"
#include "algorithms/min.h"

#include "file_import.h"
#include "file_memory.h"

#include "containers/vector_util.h"

#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#define ENABLE_MESH_PROCESSOR_LOGGING 1
#define ENABLE_MESH_PROCESSOR_PROPERTIES_LOGGING 0 && ENABLE_MESH_PROCESSOR_LOGGING

#if ENABLE_MESH_PROCESSOR_LOGGING
#define MESH_PROCESSOR_INFO R_INFO
#define MESH_PROCESSOR_WARN R_WARN
#define MESH_PROCESSOR_ERROR R_ERROR
#else
#define MESH_PROCESSOR_INFO(...) UNUSED_PARAM(__VA_ARGS__)
#define MESH_PROCESSOR_WARN(...) UNUSED_PARAM(__VA_ARGS__)
#define MESH_PROCESSOR_ERROR(...) UNUSED_PARAM(__VA_ARGS__)
#endif

namespace regina
{
    namespace model_importer
    {
        using NodeMap = std::unordered_map<aiNode*, std::vector<uint32_t>>;

        struct MeshData
        {
            std::vector<rex::Vertex> vertices;
            std::vector<uint32> indices;
        };

        //-------------------------------------------------------------------------
        rex::matrix4 to_glm(const aiMatrix4x4& matrix)
        {
            rex::matrix4 result;
            // the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
            result[0][0] = matrix.a1;
            result[1][0] = matrix.a2;
            result[2][0] = matrix.a3;
            result[3][0] = matrix.a4;
            result[0][1] = matrix.b1;
            result[1][1] = matrix.b2;
            result[2][1] = matrix.b3;
            result[3][1] = matrix.b4;
            result[0][2] = matrix.c1;
            result[1][2] = matrix.c2;
            result[2][2] = matrix.c3;
            result[3][2] = matrix.c4;
            result[0][3] = matrix.d1;
            result[1][3] = matrix.d2;
            result[2][3] = matrix.d3;
            result[3][3] = matrix.d4;
            return result;
        }

        //-------------------------------------------------------------------------
        MeshData process_mesh(aiMesh* mesh)
        {
            // data to fill
            MeshData mesh_data;

            std::vector<rex::Vertex>& vertices = mesh_data.vertices;
            std::vector<uint32>& indices = mesh_data.indices;

            // walk through each of the mesh's vertices
            for (uint32 i = 0; i < mesh->mNumVertices; i++)
            {
                rex::Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;
                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.normal = vector;
                }
                // texture coordinates
                if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;

                    vertex.texcoord = vec;
                    if (mesh->HasTangentsAndBitangents())
                    {
                        // tangent
                        vector.x = mesh->mTangents[i].x;
                        vector.y = mesh->mTangents[i].y;
                        vector.z = mesh->mTangents[i].z;
                        vertex.tangent = vector;
                        // binormal
                        vector.x = mesh->mBitangents[i].x;
                        vector.y = mesh->mBitangents[i].y;
                        vector.z = mesh->mBitangents[i].z;
                        vertex.binormal = vector;
                    }
                }
                else
                {
                    vertex.texcoord = glm::vec2(0.0f, 0.0f);
                }

                vertices.push_back(vertex);
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for (uint32 i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for (uint32 j = 0; j < face.mNumIndices; j++)
                {
                    indices.push_back(face.mIndices[j]);
                }
            }

            // return a mesh object created from the extracted mesh data
            return mesh_data;
        }

        //-------------------------------------------------------------------------
        std::vector<MeshData> process_node(aiNode* node, const aiScene* scene)
        {
            std::vector<MeshData> meshes;

            // process each mesh located at the current node
            for (uint32 i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene.
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(process_mesh(mesh));
            }

            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (uint32 i = 0; i < node->mNumChildren; i++)
            {
                auto r = process_node(node->mChildren[i], scene);

                meshes.insert(std::end(meshes), std::begin(r), std::end(r));
            }

            return meshes;
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Model> import(const rex::StringID& path)
        {
            rex::FileMemory imported_mesh_blob = rex::read_from_disk(path.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);

            if (!imported_mesh_blob)
            {
                MESH_PROCESSOR_ERROR("[Model] Could not import mesh at path: {0}", path.to_string());
                return nullptr;
            }

            MESH_PROCESSOR_INFO("[Model] Importing mesh @ path: {0} - start", path.to_string());
            MESH_PROCESSOR_INFO("[Model] \tmesh size: {0} KB", imported_mesh_blob.get_size().kilo_bytes());

            Assimp::Importer importer;

            const aiScene* scene = importer.ReadFileFromMemory(imported_mesh_blob.get_data_as<const void>(), imported_mesh_blob.get_size(), aiProcessPreset_TargetRealtime_Quality);
            if (!scene || !scene->HasMeshes())
            {
                MESH_PROCESSOR_ERROR("Failed to process mesh file: {0}", path.to_string());
                return nullptr;
            }

            MESH_PROCESSOR_INFO("[Model] Importing mesh @ path: {0} - finish", path.to_string());
            MESH_PROCESSOR_INFO("[Model] Processing mesh: \"{0}\" - start", path.to_string());

            std::vector<MeshData> meshes = process_node(scene->mRootNode, scene);

            rex::ModelCreationInfo model_creation_info;

            model_creation_info.name = "No Name"_sid;
            model_creation_info.transform = rex::matrix4(1.0f);
            model_creation_info.inverse_transform = rex::inverse(rex::matrix4(1.0f));
            model_creation_info.bounding_box.minimum = {FLT_MAX, FLT_MAX, FLT_MAX};
            model_creation_info.bounding_box.maximum = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
            model_creation_info.diffuse_textures = {};
            model_creation_info.normal_textures = {};
            model_creation_info.roughness_textures = {};
            model_creation_info.materials = {};

            int32 vertex_count = 0;
            int32 index_count = 0;

            for (const MeshData& meshdata : meshes)
            {
                model_creation_info.vertices.insert(std::begin(model_creation_info.vertices), std::begin(meshdata.vertices), std::end(meshdata.vertices));

                for (int32 i = 0; i < gsl::narrow<int32>(meshdata.indices.size()); i += 3)
                {
                    rex::TriangleIndices triangle = 
                    {
                        meshdata.indices[i + 0], 
                        meshdata.indices[i + 1], 
                        meshdata.indices[i + 2]
                    };

                    model_creation_info.indices.push_back(triangle);
                }

                rex::Submesh submesh;

                submesh.base_index = index_count;
                submesh.base_index = vertex_count;
                submesh.transform = rex::identity<rex::matrix4>();
                submesh.material_index = 0;
                submesh.vertex_count = gsl::narrow<int32>(meshdata.vertices.size());
                submesh.index_count = gsl::narrow<int32>(meshdata.indices.size());
                submesh.mesh_name = "No Name"_sid;

                vertex_count += submesh.vertex_count;
                index_count += submesh.index_count;

                auto& aabb = submesh.bounding_box;

                aabb.minimum = {FLT_MAX, FLT_MAX, FLT_MAX};
                aabb.maximum = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

                model_creation_info.submeshes.push_back(submesh);
            }

            MESH_PROCESSOR_INFO("[Model] Processing mesh: \"{0}\" - finish", path.to_string());

            return rex::make_ref<rex::Model>(std::move(model_creation_info));
        }
    } // namespace model_importer
} // namespace regina