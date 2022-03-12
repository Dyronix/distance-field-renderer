#include "rex_graphics_pch.h"

#include "resources/buffer_layout.h"
#include "data_type.h"

namespace rex
{
    //-------------------------------------------------------------------------
    uint8 get_component_count(DataType type)
    {
        switch (type)
        {
            case DataType::FLOAT32: return 1;
            case DataType::VEC2: return 2;
            case DataType::VEC3: return 3;
            case DataType::VEC4: return 4;

            case DataType::MAT3: return 3 * 3;
            case DataType::MAT4: return 4 * 4;

            case DataType::INT8: return 1;
            case DataType::INT16: return 1;
            case DataType::INT32: return 1;
            case DataType::INT64: return 1;
            case DataType::IVEC2: return 2;
            case DataType::IVEC3: return 3;
            case DataType::IVEC4: return 4;

            case DataType::UNSIGNED_INT8: return 1;
            case DataType::UNSIGNED_INT16: return 1;
            case DataType::UNSIGNED_INT32: return 1;
            case DataType::UNSIGNED_INT64: return 1;
            case DataType::UIVEC2: return 2;
            case DataType::UIVEC3: return 3;
            case DataType::UIVEC4: return 4;

            case DataType::BOOL: return 1;

            default: R_ASSERT_X(false, "Invalid DataType"); return 0;
        }
    }

    //-------------------------------------------------------------------------
    BufferElement::BufferElement()
        : name("")
        , type(DataType::NONE)
        , size(0)
        , offset(0)
        , normalized(false)
        , component_count(0)
    {
    }
    //-------------------------------------------------------------------------
    BufferElement::BufferElement(DataType type, const std::string& elementName, bool shouldNormalize)
        : name(elementName)
        , type(type)
        , size((uint32)type.to_size())
        , offset(0)
        , normalized(shouldNormalize)
        , component_count(get_component_count(type))
    {
    }

    //-------------------------------------------------------------------------
    BufferLayout::BufferLayout()
        : m_stride(0)
    {
    }
    //-------------------------------------------------------------------------
    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_elements(elements)
        , m_stride(0)
    {
        calculate_offsets_and_stride();
    }

    //-------------------------------------------------------------------------
    uint32 BufferLayout::get_stride()
    {
        return m_stride;
    }
    //-------------------------------------------------------------------------
    const uint32 BufferLayout::get_stride() const
    {
        return m_stride;
    }

    //-------------------------------------------------------------------------
    uint32 BufferLayout::get_size()
    {
        return static_cast<uint32>(m_elements.size());
    }
    //-------------------------------------------------------------------------
    const uint32 BufferLayout::get_size() const
    {
        return static_cast<uint32>(m_elements.size());
    }

    //-------------------------------------------------------------------------
    std::vector<BufferElement>& BufferLayout::get_elements()
    {
        return m_elements;
    }
    //-------------------------------------------------------------------------
    const std::vector<BufferElement>& BufferLayout::get_elements() const
    {
        return m_elements;
    }

    //-------------------------------------------------------------------------
    std::vector<BufferElement>::iterator BufferLayout::begin()
    {
        return m_elements.begin();
    }
    //-------------------------------------------------------------------------
    std::vector<BufferElement>::iterator BufferLayout::end()
    {
        return m_elements.end();
    }
    //-------------------------------------------------------------------------
    std::vector<BufferElement>::const_iterator BufferLayout::cbegin() const
    {
        return m_elements.cbegin();
    }
    //-------------------------------------------------------------------------
    std::vector<BufferElement>::const_iterator BufferLayout::cend() const
    {
        return m_elements.cend();
    }

    //-------------------------------------------------------------------------
    BufferElement& BufferLayout::operator[](int index)
    {
        R_VERIFY(index < static_cast<int>(m_elements.size()));

        return m_elements[index];
    }
    //-------------------------------------------------------------------------
    const BufferElement& BufferLayout::operator[](int index) const
    {
        R_VERIFY(index < static_cast<int>(m_elements.size()));

        return m_elements[index];
    }

    //-------------------------------------------------------------------------
    void BufferLayout::calculate_offsets_and_stride()
    {
        uint32 offset = 0;
        m_stride = 0;

        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }
}
