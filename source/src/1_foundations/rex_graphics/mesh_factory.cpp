#include "rex_graphics_pch.h"

#include "mesh_factory.h"

#include "vertex.h"
#include "triangle.h"
#include "submesh.h"
#include "mesh.h"
#include "model.h"

#include "resources/vertex_buffer.h"
#include "resources/index_buffer.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace rex
{
    namespace mesh_factory
    {
        ref_ptr<Model> g_unit_cube = nullptr;
        ref_ptr<Model> g_unit_sphere = nullptr;
        ref_ptr<Model> g_unit_capsule = nullptr;
        ref_ptr<Model> g_unit_quad = nullptr;

        struct GeometryPackage
        {
            std::vector<Vertex> vertices;
            std::vector<TriangleIndices> indices;
        };

        //-------------------------------------------------------------------------
        GeometryPackage get_cube_geometry_package(const rex::vec3& size)
        {
            std::vector<Vertex> vertices;
            vertices.resize(8);
            vertices[0].position = {-size.x / 2.0F, -size.y / 2.0F, size.z / 2.0F};
            vertices[1].position = {size.x / 2.0F, -size.y / 2.0F, size.z / 2.0F};
            vertices[2].position = {size.x / 2.0F, size.y / 2.0F, size.z / 2.0F};
            vertices[3].position = {-size.x / 2.0F, size.y / 2.0F, size.z / 2.0F};
            vertices[4].position = {-size.x / 2.0F, -size.y / 2.0F, -size.z / 2.0F};
            vertices[5].position = {size.x / 2.0F, -size.y / 2.0F, -size.z / 2.0F};
            vertices[6].position = {size.x / 2.0F, size.y / 2.0F, -size.z / 2.0F};
            vertices[7].position = {-size.x / 2.0F, size.y / 2.0F, -size.z / 2.0F};

            vertices[0].normal = {-1.0F, -1.0F, 1.0F};
            vertices[1].normal = {1.0F, -1.0F, 1.0F};
            vertices[2].normal = {1.0F, 1.0F, 1.0F};
            vertices[3].normal = {-1.0F, 1.0F, 1.0F};
            vertices[4].normal = {-1.0F, -1.0F, -1.0F};
            vertices[5].normal = {1.0F, -1.0F, -1.0F};
            vertices[6].normal = {1.0F, 1.0F, -1.0F};
            vertices[7].normal = {-1.0F, 1.0F, -1.0F};

            std::vector<TriangleIndices> indices;
            indices.resize(12);
            indices[0] = {0, 1, 2};
            indices[1] = {2, 3, 0};
            indices[2] = {1, 5, 6};
            indices[3] = {6, 2, 1};
            indices[4] = {7, 6, 5};
            indices[5] = {5, 4, 7};
            indices[6] = {4, 0, 3};
            indices[7] = {3, 7, 4};
            indices[8] = {4, 5, 1};
            indices[9] = {1, 0, 4};
            indices[10] = {3, 2, 6};
            indices[11] = {6, 7, 3};

            return {vertices, indices};
        }
        //-------------------------------------------------------------------------
        GeometryPackage get_sphere_geometry_package(float radius)
        {
            std::vector<Vertex> vertices;
            std::vector<TriangleIndices> indices;

            constexpr float latitudeBands = 30;
            constexpr float longitudeBands = 30;

            for (float latitude = 0.0f; latitude <= latitudeBands; latitude++)
            {
                const float theta = latitude * (float)M_PI / latitudeBands;
                const float sinTheta = std::sin(theta);
                const float cosTheta = std::cos(theta);

                for (float longitude = 0.0f; longitude <= longitudeBands; longitude++)
                {
                    const float phi = longitude * 2.f * (float)M_PI / longitudeBands;
                    const float sinPhi = std::sin(phi);
                    const float cosPhi = std::cos(phi);

                    Vertex vertex;
                    vertex.normal = {cosPhi * sinTheta, cosTheta, sinPhi * sinTheta};
                    vertex.position = {radius * vertex.normal.x, radius * vertex.normal.y, radius * vertex.normal.z};
                    vertices.push_back(vertex);
                }
            }

            for (uint32_t latitude = 0; latitude < (uint32_t)latitudeBands; latitude++)
            {
                for (uint32_t longitude = 0; longitude < (uint32_t)longitudeBands; longitude++)
                {
                    const uint32_t first = (latitude * ((uint32_t)longitudeBands + 1)) + longitude;
                    const uint32_t second = first + (uint32_t)longitudeBands + 1;

                    indices.push_back({first, second, first + 1});
                    indices.push_back({second, second + 1, first + 1});
                }
            }

            return {vertices, indices};
        }
        //-------------------------------------------------------------------------
        GeometryPackage get_capsule_geometry_package(float radius, float height)
        {
            std::vector<Vertex> vertices;
            std::vector<TriangleIndices> indices;

            constexpr int segments = 30;
            constexpr int pointCount = segments + 1;

            float pointsX[pointCount];
            float pointsY[pointCount];
            float pointsZ[pointCount];
            float pointsR[pointCount];

            float calcH = 0.0f;
            float calcV = 0.0f;

            for (int i = 0; i < pointCount; i++)
            {
                float calcHRadians = rex::deg2rad(calcH);
                float calcVRadians = rex::deg2rad(calcV);

                pointsX[i] = std::sin(calcHRadians);
                pointsZ[i] = std::cos(calcHRadians);
                pointsY[i] = std::cos(calcVRadians);
                pointsR[i] = std::sin(calcVRadians);

                calcH += 360.f / (float)segments;
                calcV += 180.f / (float)segments;
            }

            float yOffset = (height - (radius * 2.0f)) * 0.5f;
            if (yOffset < 0.f)
                yOffset = 0.f;

            int top = (int)std::ceil(pointCount * 0.5f);

            for (int y = 0; y < top; y++)
            {
                for (int x = 0; x < pointCount; x++)
                {
                    Vertex vertex;
                    vertex.position = rex::vec3(pointsX[x] * pointsR[y], pointsY[y] + yOffset, pointsZ[x] * pointsR[y]) * radius;
                    vertices.push_back(vertex);
                }
            }

            int bottom = (int)std::floor(pointCount * 0.5f);

            for (int y = bottom; y < pointCount; y++)
            {
                for (int x = 0; x < pointCount; x++)
                {
                    Vertex vertex;
                    vertex.position = rex::vec3(pointsX[x] * pointsR[y], -yOffset + pointsY[y], pointsZ[x] * pointsR[y]) * radius;
                    vertices.push_back(vertex);
                }
            }

            for (int y = 0; y < segments + 1; y++)
            {
                for (int x = 0; x < segments; x++)
                {
                    TriangleIndices triangle_indices_0;
                    triangle_indices_0.v1 = ((y + 0) * (segments + 1)) + x + 0;
                    triangle_indices_0.v2 = ((y + 1) * (segments + 1)) + x + 0;
                    triangle_indices_0.v3 = ((y + 1) * (segments + 1)) + x + 1;
                    indices.push_back(triangle_indices_0);

                    TriangleIndices triangle_indices_1;
                    triangle_indices_1.v1 = ((y + 0) * (segments + 1)) + x + 1;
                    triangle_indices_1.v2 = ((y + 0) * (segments + 1)) + x + 0;
                    triangle_indices_1.v3 = ((y + 1) * (segments + 1)) + x + 1;
                    indices.push_back(triangle_indices_1);
                }
            }

            return {vertices, indices};
        }
        //-------------------------------------------------------------------------
        GeometryPackage get_quad_geometry_package(const rex::vec3& size)
        {
            float x = -size.x * 0.5f;
            float y = -size.y * 0.5f;
            float width = size.x;
            float height = size.y;

            std::vector<Vertex> vertices;
            vertices.resize(4);
            vertices[0].position = rex::vec3(x, y, 0.0f);  // BL
            vertices[0].texcoord = rex::vec2(0, 0);
            vertices[1].position = rex::vec3(x + width, y, 0.0f);  // BR
            vertices[1].texcoord = rex::vec2(1, 0);
            vertices[2].position = rex::vec3(x + width, y + height, 0.0f);  // TR
            vertices[2].texcoord = rex::vec2(1, 1);
            vertices[3].position = rex::vec3(x, y + height, 0.0f);  // TL
            vertices[3].texcoord = rex::vec2(0, 1);

            std::vector<TriangleIndices> indices;
            indices.resize(2);
            indices[0] = {0, 1, 2};
            indices[1] = {2, 3, 0};

            return {vertices, indices};
        }

        //-------------------------------------------------------------------------
        void load_unit_cube()
        {
            const rex::vec3 size = {2.0f, 2.0f, 2.0f};

            GeometryPackage package = get_cube_geometry_package(size);

            ModelCreationInfo info;

            AABB aabb;
            aabb.minimum = (-size) * 0.5f;
            aabb.maximum = (size)*0.5f;

            info.name = "Unit Cube"_sid;
            info.vertices = package.vertices;
            info.indices = package.indices;
            info.transform = rex::matrix4(1.0f);
            info.inverse_transform = rex::inverse(info.transform);
            info.bounding_box = aabb;

            Submesh submesh;
            submesh.base_index = 0;
            submesh.base_vertex = 0;
            submesh.bounding_box = aabb;
            submesh.index_count = (uint32)package.indices.size() * TriangleIndices::INDICES_PER_TRIANGLE;
            submesh.material_index = 0;
            submesh.mesh_name = "Unit Cube"_sid;
            submesh.node_name = "";
            submesh.transform = rex::matrix4(1.0f);
            submesh.vertex_count = (uint32)package.vertices.size();

            info.submeshes = {submesh};

            info.diffuse_textures = {};
            info.normal_textures = {};
            info.roughness_textures = {};
            info.materials = {};

            g_unit_cube = make_ref<Model>(info);
        }
        //-------------------------------------------------------------------------
        void load_unit_sphere()
        {
            const float radius = 1.0f;

            GeometryPackage package = get_sphere_geometry_package(radius);

            ModelCreationInfo info;

            AABB aabb;
            aabb.minimum = (rex::vec3{-radius}) * 0.5f;
            aabb.maximum = (rex::vec3{radius}) * 0.5f;

            info.name = "Unit Sphere"_sid;
            info.vertices = package.vertices;
            info.indices = package.indices;
            info.transform = rex::matrix4(1.0f);
            info.inverse_transform = rex::inverse(info.transform);
            info.bounding_box = aabb;

            Submesh submesh;
            submesh.base_index = 0;
            submesh.base_vertex = 0;
            submesh.bounding_box = aabb;
            submesh.index_count = (uint32)package.indices.size() * TriangleIndices::INDICES_PER_TRIANGLE;
            submesh.material_index = 0;
            submesh.mesh_name = "Unit Sphere"_sid;
            submesh.node_name = "";
            submesh.transform = rex::matrix4(1.0f);
            submesh.vertex_count = (uint32)package.vertices.size();

            info.submeshes = {submesh};

            info.diffuse_textures = {};
            info.normal_textures = {};
            info.roughness_textures = {};
            info.materials = {};

            g_unit_sphere = make_ref<Model>(info);
        }
        //-------------------------------------------------------------------------
        void load_unit_capsule()
        {
            const float radius = 1.0f;
            const float height = 2.0f;

            GeometryPackage package = get_capsule_geometry_package(radius, height);

            ModelCreationInfo info;

            AABB aabb;
            aabb.minimum = (rex::vec3{-radius, -height, -radius}) * 0.5f;
            aabb.maximum = (rex::vec3{radius, height, radius}) * 0.5f;

            info.name = "Unit Capsule"_sid;
            info.vertices = package.vertices;
            info.indices = package.indices;
            info.transform = rex::matrix4(1.0f);
            info.inverse_transform = rex::inverse(info.transform);
            info.bounding_box = aabb;

            Submesh submesh;
            submesh.base_index = 0;
            submesh.base_vertex = 0;
            submesh.bounding_box = aabb;
            submesh.index_count = (uint32)package.indices.size() * TriangleIndices::INDICES_PER_TRIANGLE;
            submesh.material_index = 0;
            submesh.mesh_name = "Unit Capsule"_sid;
            submesh.node_name = "";
            submesh.transform = rex::matrix4(1.0f);
            submesh.vertex_count = (uint32)package.vertices.size();

            info.submeshes = {submesh};

            info.diffuse_textures = {};
            info.normal_textures = {};
            info.roughness_textures = {};
            info.materials = {};

            g_unit_capsule = make_ref<Model>(info);
        }
        //-------------------------------------------------------------------------
        void load_unit_quad()
        {
            const rex::vec3 size(2.0f, 2.0f, 2.0f);

            GeometryPackage package = get_quad_geometry_package(size);

            ModelCreationInfo info;

            AABB aabb;
            aabb.minimum = (-size) * 0.5f;
            aabb.maximum = (size)*0.5f;

            info.name = "Unit Quad"_sid;
            info.vertices = package.vertices;
            info.indices = package.indices;
            info.transform = rex::matrix4(1.0f);
            info.inverse_transform = rex::inverse(info.transform);
            info.bounding_box = aabb;

            Submesh submesh;
            submesh.base_index = 0;
            submesh.base_vertex = 0;
            submesh.bounding_box = aabb;
            submesh.index_count = (uint32)package.indices.size() * TriangleIndices::INDICES_PER_TRIANGLE;
            submesh.material_index = 0;
            submesh.mesh_name = "Unit Quad"_sid;
            submesh.node_name = "";
            submesh.transform = rex::matrix4(1.0f);
            submesh.vertex_count = (uint32)package.vertices.size();

            info.submeshes = {submesh};

            info.diffuse_textures = {};
            info.normal_textures = {};
            info.roughness_textures = {};
            info.materials = {};

            g_unit_quad = make_ref<Model>(info);
        }

        //-------------------------------------------------------------------------
        void load()
        {
            load_unit_cube();
            load_unit_sphere();
            load_unit_capsule();
            load_unit_quad();
        }
        //-------------------------------------------------------------------------
        void clear()
        {
            g_unit_cube.reset();
            g_unit_sphere.reset();
            g_unit_capsule.reset();
            g_unit_quad.reset();
        }

        //-------------------------------------------------------------------------
        std::unique_ptr<IMesh> create_box(const rex::vec3& size)
        {
            GeometryPackage package = get_cube_geometry_package(size);

            return std::make_unique<StaticMesh>(package.vertices, package.indices);
        }
        //-------------------------------------------------------------------------
        std::unique_ptr<IMesh> create_sphere(float radius)
        {
            GeometryPackage package = get_sphere_geometry_package(radius);

            return std::make_unique<StaticMesh>(package.vertices, package.indices);
        }
        //-------------------------------------------------------------------------
        std::unique_ptr<IMesh> create_capsule(float radius, float height)
        {
            GeometryPackage package = get_capsule_geometry_package(radius, height);

            return std::make_unique<StaticMesh>(package.vertices, package.indices);
        }
        //-------------------------------------------------------------------------
        std::unique_ptr<rex::IMesh> create_quad(const rex::vec3& size)
        {
            GeometryPackage package = get_quad_geometry_package(size);

            return std::make_unique<StaticMesh>(package.vertices, package.indices);
        }

        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Model> get_unit_cube()
        {
            R_ASSERT(g_unit_cube);

            return g_unit_cube;
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Model> get_unit_sphere()
        {
            R_ASSERT(g_unit_sphere);

            return g_unit_sphere;
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Model> get_unit_capsule()
        {
            R_ASSERT(g_unit_capsule);

            return g_unit_capsule;
        }
        //-------------------------------------------------------------------------
        rex::ref_ptr<rex::Model> get_unit_quad()
        {
            R_ASSERT(g_unit_quad);

            return g_unit_quad;
        }
    }
}
