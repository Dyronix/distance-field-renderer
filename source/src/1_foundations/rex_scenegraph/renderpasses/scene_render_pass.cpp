#include "rex_scenegraph_pch.h"

#include "renderpasses/scene_render_pass.h"

#include "scene_renderer.h"

#include "renderer/pipeline.h"
#include "renderer/render_pass.h"
#include "renderer/renderer.h"

#include "resources/resource_factory.h"
#include "resources/frame_buffer.h"
#include "resources/material.h"

namespace rex
{
    //-------------------------------------------------------------------------
    SceneRenderPass::SceneRenderPass(const StringID& name)
        :m_name(name)
        ,m_is_enabled(true)
    {

    }

    //-------------------------------------------------------------------------
    SceneRenderPass::~SceneRenderPass()
    {
        m_material_map.clear();
        m_pipeline_map.clear();
    }

    //-------------------------------------------------------------------------
    void SceneRenderPass::resize(int32 width, int32 height)
    {
        auto& renderpass = get_pipeline()->get_render_pass();
        auto& framebuffer = renderpass->get_frame_buffer();

        if (framebuffer != nullptr)
        {
            R_ASSERT(width > 0);
            R_ASSERT(height > 0);

            int32 current_fb_width = framebuffer->get_width();
            int32 current_fb_height = framebuffer->get_height();

            if (current_fb_width != width || current_fb_height != height)
            {
                framebuffer->resize(width, height);
            }
        }
    }

    //-------------------------------------------------------------------------
    void SceneRenderPass::enable()
    {
        m_is_enabled = true;
    }

    //-------------------------------------------------------------------------
    void SceneRenderPass::disable()
    {
        m_is_enabled = false;
    }

    //-------------------------------------------------------------------------
    bool SceneRenderPass::is_enabled() const
    {
        return m_is_enabled;
    }

    //-------------------------------------------------------------------------
    const rex::StringID& SceneRenderPass::get_name() const
    {
        return m_name;
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::SceneRenderer>& SceneRenderPass::get_scene_renderer()
    {
        return m_scene_renderer;
    }

    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::SceneRenderer>& SceneRenderPass::get_scene_renderer() const
    {
        return m_scene_renderer;
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::ecs::Scene>& SceneRenderPass::get_scene()
    {
        return m_scene_renderer->get_scene();
    }

    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::ecs::Scene>& SceneRenderPass::get_scene() const
    {
        return m_scene_renderer->get_scene();
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::Pipeline> SceneRenderPass::get_pipeline()
    {
        return m_pipeline_map.begin()->second;
    }
    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::Pipeline> SceneRenderPass::get_pipeline() const
    {
        return m_pipeline_map.begin()->second;
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::Pipeline> SceneRenderPass::get_pipeline(const StringID& name)
    {
        if (name == ESID::SID_None)
        {
            return nullptr;
        }

        auto it = std::find_if(std::begin(m_pipeline_map), std::end(m_pipeline_map),
            [name](const auto& pair)
            {
                return name == pair.first;
            });

        if (it == std::end(m_pipeline_map))
        {
            return nullptr;
        }

        return it->second;
    }

    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::Pipeline> SceneRenderPass::get_pipeline(const StringID& name) const
    {
        if (name == ESID::SID_None)
        {
            return nullptr;
        }

        auto it = std::find_if(std::cbegin(m_pipeline_map), std::cend(m_pipeline_map),
            [name](const auto& pair)
            {
                return name == pair.first;
            });

        if (it == std::cend(m_pipeline_map))
        {
            return nullptr;
        }

        return it->second;
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::Material> SceneRenderPass::get_material()
    {
        return m_material_map.begin()->second;
    }

    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::Material> SceneRenderPass::get_material() const
    {
        return m_material_map.begin()->second;
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::Material> SceneRenderPass::get_material(const StringID& name)
    {
        if (name == ESID::SID_None)
        {
            return nullptr;
        }

        auto it = std::find_if(std::cbegin(m_material_map), std::cend(m_material_map),
            [name](const auto& pair)
            {
                return name == pair.first;
            });

        if (it == std::cend(m_material_map))
        {
            return nullptr;
        }

        return it->second;
    }

    //-------------------------------------------------------------------------
    const rex::ref_ptr<rex::Material> SceneRenderPass::get_material(const StringID& name) const
    {
        if (name == ESID::SID_None)
        {
            return nullptr;
        }

        auto it = std::find_if(std::cbegin(m_material_map), std::cend(m_material_map),
            [name](const auto& pair)
            {
                return name == pair.first;
            });

        if (it == std::cend(m_material_map))
        {
            return nullptr;
        }

        return it->second;
    }

    //-------------------------------------------------------------------------
    void SceneRenderPass::initialize(const ref_ptr<SceneRenderer>& renderer)
    {
        m_scene_renderer = renderer;

        on_initialize(renderer);
    }

    //-------------------------------------------------------------------------
    void SceneRenderPass::shutdown()
    {
        on_shutdown();

        m_material_map.clear();
        m_pipeline_map.clear();

        m_scene_renderer.reset();
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<rex::Pipeline> SceneRenderPass::create_pipeline(const PipelineDescription& desc)
    {
        auto p = Renderer::create_pipeline(desc);

        m_pipeline_map.insert(std::make_pair(desc.name, p));

        return p;
    }

    //-------------------------------------------------------------------------
    rex::ref_ptr<Material> SceneRenderPass::create_material(const ref_ptr<ShaderProgram>& shader, const StringID& materialName)
    {
        auto m = ResourceFactory::create_material(shader, materialName);

        m_material_map.insert(std::make_pair(materialName, m));
        
        return m;
    }

    //-------------------------------------------------------------------------
    void SceneRenderPass::set_render_target(const StringID& renderTargetName, const SceneRenderPassID& renderPass, const ref_ptr<Texture>& defaultTexture)
    {
        auto renderpass = m_scene_renderer->get_scene_render_pass(renderPass.pass_name);
        if (renderpass == nullptr)
        {
            R_WARN("No renderpass found with name: {0}", renderPass.pass_name.to_string());

            get_material()->set_texture2d(renderTargetName, defaultTexture);
        }
        else
        {
            auto framebuffer = renderpass->get_pipeline()->get_render_pass()->get_frame_buffer();

            get_material()->set_texture2d(renderTargetName, framebuffer->get_color_attachment(renderPass.attachement_index));
        }
    }

    //-------------------------------------------------------------------------
    void SceneRenderPass::on_initialize(const ref_ptr<SceneRenderer>& /*renderer*/)
    {
        // Nothing to implement
    }
    
    //-------------------------------------------------------------------------
    void SceneRenderPass::on_shutdown()
    {
        // Nothing to implement
    }
}