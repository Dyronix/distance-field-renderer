#include "rex_graphics_pch.h"

#include "resources/uniform_buffer_set.h"

#include "resources/resource_factory.h"

namespace rex
{
    //-------------------------------------------------------------------------
    rex::UniformBufferSet*  UniformBufferSet::s_instance        = nullptr;
    std::vector<int32>      UniformBufferSet::s_binding_points  = {};

    //-------------------------------------------------------------------------
    int32 generate_unique_binding_point(int32 bindingPoint)
    {
        auto it = std::find(std::cbegin(UniformBufferSet::get_binding_points()), std::cend(UniformBufferSet::get_binding_points()), bindingPoint);

        if (it != std::cend(UniformBufferSet::get_binding_points()))
        {
            return generate_unique_binding_point(bindingPoint + 1);
        }

        return bindingPoint;
    }

    //-------------------------------------------------------------------------
    rex::UniformBufferSet* UniformBufferSet::instance()
    {
        return s_instance;
    }

    //-------------------------------------------------------------------------
    const int32 UniformBufferSet::get_new_binding_point()
    {
        const int32 in_binding_point = 0;
        const int32 out_binding_point = generate_unique_binding_point(in_binding_point);

        s_binding_points.push_back(out_binding_point);

        return out_binding_point;
    }
    //-------------------------------------------------------------------------
    const int32 UniformBufferSet::get_binding_point_count()
    {
        return gsl::narrow<int32>(s_binding_points.size());
    }

    //-------------------------------------------------------------------------
    std::vector<int32>& UniformBufferSet::get_binding_points()
    {
        return s_binding_points;
    }

    //-------------------------------------------------------------------------
    UniformBufferSet::UniformBufferSet(uint32 maxFrames)
    {
        if (s_instance != nullptr)
        {
            R_ERROR("Second instance of uniform buffer set created!");
            return;
        }

        s_instance = this;

        for (uint32 i = 0; i < maxFrames; ++i)
        {
            m_uniform_buffers[i] = {};
        }
    }
    //-------------------------------------------------------------------------
    UniformBufferSet::~UniformBufferSet()
    {
        clear();
    }

    //-------------------------------------------------------------------------
    void UniformBufferSet::clear()
    {
        for (auto& pair : m_uniform_buffers)
        {
            for (auto& child_pair : pair.second)
            {
                child_pair.second->release();
            }
        }

        m_uniform_buffers.clear();
    }

    //-------------------------------------------------------------------------
    ref_ptr<UniformBuffer> UniformBufferSet::get(uint32 binding, uint32 frame) const
    {
        R_ASSERT_X(m_uniform_buffers.find(frame) != std::cend(m_uniform_buffers), "Frame index not found");

        auto& buffers = m_uniform_buffers.at(frame);

        auto it = std::find_if(std::cbegin(buffers), std::cend(buffers),
            [binding](const auto& pair)
            {
                return pair.second->get_binding() == binding;
            });

        return it != std::cend(buffers)
            ? it->second
            : nullptr;
    }
    //-------------------------------------------------------------------------
    ref_ptr<UniformBuffer> UniformBufferSet::get(const StringID& name, uint32 frame) const
    {
        R_ASSERT_X(m_uniform_buffers.find(frame) != std::cend(m_uniform_buffers), "Frame index not found");

        auto& buffers = m_uniform_buffers.at(frame);

        auto it = std::find_if(std::cbegin(buffers), std::cend(buffers),
            [name](const auto& pair)
            {
                return pair.second->get_name() == name;
            });

        return it != std::cend(buffers)
            ? it->second
            : nullptr;
    }

    //-------------------------------------------------------------------------
    StringID UniformBufferSet::create(const ShaderUniformBlock& block)
    {
        for (auto& pair : m_uniform_buffers)
        {
            if (pair.second.find(block.get_name()) != std::cend(pair.second))
            {
                R_WARN("Uniform buffer already present for this frame, skipping. frame index: \"{0}\", uniform buffer name: \"{1}\"", pair.first, block.get_name().to_string());
                continue;
            }

            auto it = std::find_if(std::cbegin(pair.second), std::cend(pair.second),
                [binding = block.get_binding()](const auto& child_pair)
                {
                    return child_pair.second->get_binding() == binding;
                });

            if (it != std::cend(pair.second))
            {
                R_ERROR("Duplicate binding found! \"{0}\" has a binding of \"{1}\", but \"{2}\" is already bound to this binding index", block.get_name().to_string(), block.get_binding(), it->second->get_name().to_string());
                continue;
            }

            s_binding_points.push_back(block.get_binding());

            pair.second.insert(std::make_pair(block.get_name(), create_uniform_buffer(block)));
        }

        return block.get_name();
    }
}