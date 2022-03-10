#pragma once

#include "datatype.h"

namespace sbt
{
    struct BufferElement
    {
        std::string name;
        DataType type;
        uint32 size;
        uint32 offset;
        bool normalized;
        uint8 component_count;

        BufferElement();
        BufferElement(DataType type, const std::string& elementName, bool shouldNormalize = false);
    };

    class BufferLayout
    {
    public:
        using BufferElements = std::vector<BufferElement>;

        BufferLayout();
        BufferLayout(const BufferLayout& other);
        BufferLayout(BufferLayout&& other);
        BufferLayout(const std::initializer_list<BufferElement>& elements);

        BufferLayout& operator=(const BufferLayout& other);
        BufferLayout& operator=(BufferLayout&& other);

        uint32 get_stride();
        const uint32 get_stride() const;
        uint32 get_size();
        const uint32 get_size() const;

        BufferElements& get_elements();
        const BufferElements& get_elements() const;

        BufferElements::iterator begin();
        BufferElements::const_iterator begin() const;
        BufferElements::iterator end();
        BufferElements::const_iterator end() const;
        BufferElements::const_iterator cbegin() const;
        BufferElements::const_iterator cend() const;

        BufferElement& operator[](int index);
        const BufferElement& operator[](int index) const;

    private:
        void calculate_offsets_and_stride();

        BufferElements m_elements;
        uint32 m_stride;
    };
}