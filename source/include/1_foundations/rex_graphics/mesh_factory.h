#pragma once

namespace rex
{
    class IMesh;
    class Model;

    namespace mesh_factory
    {
        void load();
        void clear();

        std::unique_ptr<IMesh> create_box(const rex::vec3& size);
        std::unique_ptr<IMesh> create_sphere(float radius);
        std::unique_ptr<IMesh> create_capsule(float radius, float height);
        std::unique_ptr<IMesh> create_quad(const rex::vec3& size);

        ref_ptr<Model> get_unit_cube();
        ref_ptr<Model> get_unit_sphere();
        ref_ptr<Model> get_unit_capsule();
        ref_ptr<Model> get_unit_quad();
    }
}