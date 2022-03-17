#include "regina_pch.h"

#include "model_importer.h"
#include "model.h"
#include "mesh.h"
#include "submesh.h"
#include "vertex.h"
#include "triangle.h"

#include "min_max.h"

#include "algorithms/min.h"
#include "algorithms/max.h"

#include "file_import.h"
#include "file_memory.h"

#include "containers/vector_util.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

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
        void traverse_nodes(NodeMap& nodeMap, std::vector<rex::Submesh>& subMeshes, aiNode* node, const rex::matrix4& parentTransform = rex::identity<rex::matrix4>(), uint32_t level = 0)
        {
            rex::matrix4 transform = parentTransform * to_glm(node->mTransformation);

            nodeMap[node].resize(node->mNumMeshes);
            for (uint32_t i = 0; i < node->mNumMeshes; i++)
            {
                uint32_t mesh = node->mMeshes[i];

                rex::Submesh& submesh = subMeshes[mesh];

                submesh.node_name = rex::create_sid(node->mName.C_Str());
                submesh.transform = transform;

                nodeMap[node][i] = mesh;
            }

            for (uint32_t i = 0; i < node->mNumChildren; i++)
            {
                traverse_nodes(nodeMap, subMeshes, node->mChildren[i], transform, level + 1);
            }
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Model> import(const rex::StringID& path)
        {
            rex::FileMemory imported_mesh_blob = rex::read_from_disk(path.to_string(), rex::ReadBinary::YES, rex::NullTerminate::YES);

            if (!imported_mesh_blob)
            {
                MESH_PROCESSOR_ERROR("Could not import mesh at path: {0}", path.to_string());
                return nullptr;
            }

            Assimp::Importer importer;

            const aiScene* scene = importer.ReadFileFromMemory(imported_mesh_blob.get_data_as<const void>(), imported_mesh_blob.get_size(), aiProcessPreset_TargetRealtime_Quality);
            if (!scene || !scene->HasMeshes())
            {
                MESH_PROCESSOR_ERROR("Failed to process mesh file: {0}", path.to_string());
                return nullptr;
            }

            MESH_PROCESSOR_INFO("Processing mesh: \"{0}\" - start", path.to_string());

            rex::ModelCreationInfo model_creation_info;

            model_creation_info.name = scene->mName.length == 0 ? rex::ESID::SID_None : rex::create_sid(scene->mName.C_Str());
            model_creation_info.vertices = {};
            model_creation_info.indices = {};
            model_creation_info.transform = to_glm(scene->mRootNode->mTransformation);
            model_creation_info.inverse_transform = rex::inverse(to_glm(scene->mRootNode->mTransformation));
            model_creation_info.bounding_box = {};
            model_creation_info.submeshes = rex::create_vector_with_capacity<rex::Submesh>(scene->mNumMeshes);
            model_creation_info.diffuse_textures = {};
            model_creation_info.normal_textures = {};
            model_creation_info.roughness_textures = {};

            R_ASSERT_X(!scene->mAnimations, "Animations are not supported.");

            int32 vertex_count = 0;
            int32 index_count = 0;

            rex::AABB& bounding_box = model_creation_info.bounding_box;

            bounding_box.minimum = {FLT_MAX, FLT_MAX, FLT_MAX};
            bounding_box.maximum = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

            std::vector<rex::Submesh>& submeshes = model_creation_info.submeshes;
            for (uint32 submesh_index = 0; submesh_index < scene->mNumMeshes; ++submesh_index)
            {
                aiMesh* mesh = scene->mMeshes[submesh_index];

                rex::Submesh new_submesh;
                new_submesh.base_vertex = vertex_count;
                new_submesh.base_index = index_count;
                new_submesh.transform = rex::identity<rex::matrix4>();
                new_submesh.material_index = mesh->mMaterialIndex;
                new_submesh.vertex_count = mesh->mNumVertices;
                new_submesh.index_count = mesh->mNumFaces * 3;
                new_submesh.mesh_name = mesh->mName.length == 0 ? rex::ESID::SID_None : rex::create_sid(mesh->mName.C_Str());

                // Assign first submesh name to the model name if no name was given to the scene
                if (model_creation_info.name.is_none() && !new_submesh.mesh_name.is_none())
                {
                    model_creation_info.name = new_submesh.mesh_name;
                }

                vertex_count += mesh->mNumVertices;
                index_count += new_submesh.index_count;

                R_ASSERT_X(mesh->HasPositions(), "Meshes require positions.");
                R_ASSERT_X(mesh->HasNormals(), "Meshes require normals.");

                // Vertices
                //
                auto& aabb = new_submesh.bounding_box;

                aabb.minimum = {FLT_MAX, FLT_MAX, FLT_MAX};
                aabb.maximum = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

                submeshes.push_back(new_submesh);

                for (size_t i = 0; i < mesh->mNumVertices; ++i)
                {
                    rex::Vertex vertex;
                    vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                    vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

                    aabb.minimum.x = rex::min(vertex.position.x, aabb.minimum.x);
                    aabb.minimum.y = rex::min(vertex.position.y, aabb.minimum.y);
                    aabb.minimum.z = rex::min(vertex.position.z, aabb.minimum.z);
                    aabb.maximum.x = rex::max(vertex.position.x, aabb.maximum.x);
                    aabb.maximum.y = rex::max(vertex.position.y, aabb.maximum.y);
                    aabb.maximum.z = rex::max(vertex.position.z, aabb.maximum.z);

                    if (mesh->HasTangentsAndBitangents())
                    {
                        vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                        vertex.binormal = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
                    }

                    if (mesh->HasTextureCoords(0))
                    {
                        vertex.texcoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
                    }

                    model_creation_info.vertices.push_back(vertex);
                }

                // Indices
                //
                for (size_t i = 0; i < mesh->mNumFaces; i++)
                {
                    if (mesh->mFaces[i].mNumIndices != 3)
                    {
                        MESH_PROCESSOR_ERROR("Mesh is not triangulated properly ({0}), Must have 3 indices per face.", mesh->mName.C_Str());
                        MESH_PROCESSOR_INFO("Processing mesh: \"{0}\" - failed", path.to_string());
                        return nullptr;
                    }

                    rex::TriangleIndices indices = {mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]};
                    model_creation_info.indices.push_back(indices);
                }

                for (const auto& submesh : submeshes)
                {
                    rex::AABB transformed_submesh_AABB = submesh.bounding_box;
                    rex::vec3 min = rex::vec3(submesh.transform * rex::vec4(transformed_submesh_AABB.minimum, 1.0f));
                    rex::vec3 max = rex::vec3(submesh.transform * rex::vec4(transformed_submesh_AABB.maximum, 1.0f));

                    bounding_box.minimum.x = rex::min(bounding_box.minimum.x, min.x);
                    bounding_box.minimum.y = rex::min(bounding_box.minimum.y, min.y);
                    bounding_box.minimum.z = rex::min(bounding_box.minimum.z, min.z);
                    bounding_box.maximum.x = rex::max(bounding_box.maximum.x, max.x);
                    bounding_box.maximum.y = rex::max(bounding_box.maximum.y, max.y);
                    bounding_box.maximum.z = rex::max(bounding_box.maximum.z, max.z);
                }               
            }

            NodeMap node_map;
            traverse_nodes(node_map, submeshes, scene->mRootNode);

            // Assign a default name when no name was found.
            if (model_creation_info.name.is_none())
            {
                model_creation_info.name = "No Name"_sid;
            }

            MESH_PROCESSOR_INFO("Processing mesh: \"{0}\" - success!", model_creation_info.name.to_string());

            return rex::make_ref<rex::Model>(model_creation_info);
        }
    }
}