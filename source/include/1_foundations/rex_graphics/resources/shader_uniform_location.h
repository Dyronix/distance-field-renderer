#pragma once

namespace rex
{
    class ShaderUniformLocation
    {
    public:
        static ShaderUniformLocation EMPTY;

        ShaderUniformLocation();
        ShaderUniformLocation(int32 loc);

        operator bool() const;
        operator int32() const;
        
        int32 get() const;

    private:
        int32 m_location;
    };
}