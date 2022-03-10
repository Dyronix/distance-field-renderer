#pragma once

#include "mesh.h"
#include "model.h"

#include <memory>

namespace rex
{
    namespace mesh_factory
    {
        void load();
        void clear();

        std::unique_ptr<IMesh> createBox(const rex::vec3& size);
        std::unique_ptr<IMesh> createSphere(float radius);
        std::unique_ptr<IMesh> createCapsule(float radius, float height);
        std::unique_ptr<IMesh> createQuad(const rex::vec3& size);

        ref_ptr<Model> getUnitCube();
        ref_ptr<Model> getUnitSphere();
        ref_ptr<Model> getUnitCapsule();
        ref_ptr<Model> getUnitQuad();
    }
}