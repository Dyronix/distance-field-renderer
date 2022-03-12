#pragma once

namespace rex
{
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT
    };

    namespace opengl
    {
        class Shader
        {
        public:
            ~Shader();

            //implicit conversion
            operator uint32() const { return m_shader_id; }

        protected:
            Shader(ShaderType type, const memory::byte* data, size_t dataSize);

        private:
            uint32 m_shader_id;
        };

        class VertexShader : public Shader
        {
        public:
            VertexShader(const memory::byte* data, size_t dataSize)
                :Shader(ShaderType::VERTEX, data, dataSize)
            {}
        };
        class FragmentShader : public Shader
        {
        public:
            FragmentShader(const memory::byte* data, size_t dataSize)
                :Shader(ShaderType::FRAGMENT, data, dataSize)
            {}
        };
    }
}