#pragma once

namespace rex
{
    struct DirectionalLight
    {
        DirectionalLight();
        DirectionalLight(const DirectionalLight& other);
        DirectionalLight(const rex::vec3& inDirection, float inIntensity);

        bool operator==(const DirectionalLight& other) const;
        bool operator!=(const DirectionalLight& other) const;

        rex::vec3 direction;

        float intensity;
    };
}