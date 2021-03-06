#pragma once

namespace rex
{
    class ShaderResourceLocation
    {
    public:
        static ShaderResourceLocation EMPTY;

        ShaderResourceLocation();
        ShaderResourceLocation(int32 loc);

        operator bool() const;
        operator int32() const;

        int32 get() const;

    private:
        int32 m_location;
    };
}