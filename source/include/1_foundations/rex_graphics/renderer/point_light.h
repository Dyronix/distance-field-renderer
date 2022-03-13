#pragma once

namespace rex
{
    struct PointLight
    {
        PointLight();
        PointLight(const PointLight& other);
        PointLight(const rex::vec3& inPosition, float inIntensity, float inMinAttenuation, float inMaxAttenuation);
     
        bool operator==(const PointLight& other) const;
        bool operator!=(const PointLight& other) const;

        rex::vec3 position;

        float intensity;

        float min_attenuation;
        float max_attenuation;
    };
}