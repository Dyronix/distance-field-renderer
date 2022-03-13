#pragma once

#include "yes_no_enum.h"
#include "transform.h"

namespace rex
{
    struct PipelineDescription;

    class ShaderProgram;
    class Material;
    class Texture;
    class Pipeline;
    class SceneRenderer;

    namespace ecs
    {
        class Scene;
        class SceneCamera;
    }

    struct SceneRenderPassID
    {
        //-------------------------------------------------------------------------
        SceneRenderPassID(const StringID& name)
            :pass_name(name)
            ,attachement_index(0u)
        {}
        //-------------------------------------------------------------------------
        SceneRenderPassID(const StringID& name, uint32 attachmentIndex)
            :pass_name(name)
            ,attachement_index(attachmentIndex)
        {}

        StringID    pass_name;
        uint32      attachement_index;
    };

    using CreateFrameBuffer = YesNoEnum;

    class SceneRenderPass
    {
    public:
        virtual ~SceneRenderPass();

        /// <summary>
        /// Resize the contained framebuffer if available to a new width and/or height
        /// </summary>
        /// <param name="width">New width to resize to</param>
        /// <param name="height">New height to resize to</param>
        void                                resize(int32 width, int32 height);

        /// <summary>
        /// Enable the render pass
        /// </summary>
        void                                enable();
        /// <summary>
        /// Disable the render pass
        /// </summary>
        void                                disable();

        /// <summary>
        /// Is this render pass enabled
        /// </summary>
        /// <returns>True if it is enabled, otherwise false</returns>
        bool                                is_enabled() const;

        /// <summary>
        /// Retrieve the name of this render pass
        /// </summary>
        /// <returns>Name of the render pass</returns>
        const StringID&                     get_name() const;
        
        /// <summary>
        /// Retrieve the scene renderer of this render pass
        /// </summary>
        /// <returns>The scene renderer</returns>
        ref_ptr<rex::SceneRenderer>&        get_scene_renderer();
        /// <summary>
        /// Retrieve the scene renderer of this render pass
        /// </summary>
        /// <returns>The scene renderer</returns>
        const ref_ptr<rex::SceneRenderer>&  get_scene_renderer() const;

        /// <summary>
        /// Retrieve the scene renderer of this render pass
        /// </summary>
        /// <returns>The scene renderer</returns>
        ref_ptr<rex::ecs::Scene>&           get_scene();
        /// <summary>
        /// Retrieve the scene renderer of this render pass
        /// </summary>
        /// <returns>The scene renderer</returns>
        const ref_ptr<rex::ecs::Scene>&     get_scene() const;

        /// <summary>
        /// Pipeline state object of the render pass
        /// </summary>
        /// <returns>Return the pipeline state object</returns>
        ref_ptr<rex::Pipeline>              get_pipeline();
        /// <summary>
        /// Pipeline state object of the render pass
        /// </summary>
        /// <returns>Return the pipeline state object</returns>
        const ref_ptr<rex::Pipeline>        get_pipeline() const;

        /// <summary>
        /// Pipeline state object of the render pass
        /// </summary>
        /// <returns>Return the pipeline state object</returns>
        ref_ptr<rex::Pipeline>              get_pipeline(const StringID& name);
        /// <summary>
        /// Pipeline state object of the render pass
        /// </summary>
        /// <returns>Return the pipeline state object</returns>
        const ref_ptr<rex::Pipeline>        get_pipeline(const StringID& name) const;

        /// <summary>
        /// Retrieve create material for the scene render pass
        /// This material will override any provided material given to meshes
        /// This material can be null if it is not created
        /// </summary>
        /// <returns>Retrieve the material</returns>
        ref_ptr<rex::Material>              get_material();
        /// <summary>
        /// Retrieve create material for the scene render pass
        /// This material will override any provided material given to meshes
        /// This material can be null if it is not created
        /// </summary>
        /// <returns>Retrieve the material</returns>
        const ref_ptr<rex::Material>        get_material() const;

        /// <summary>
        /// Retrieve create material for the scene render pass
        /// This material will override any provided material given to meshes
        /// This material can be null if it is not created
        /// </summary>
        /// <returns>Retrieve the material</returns>
        ref_ptr<rex::Material>              get_material(const StringID& name);
        /// <summary>
        /// Retrieve create material for the scene render pass
        /// This material will override any provided material given to meshes
        /// This material can be null if it is not created
        /// </summary>
        /// <returns>Retrieve the material</returns>
        const ref_ptr<rex::Material>        get_material(const StringID& name) const;

        /// <summary>
        /// Initialize scene render pass
        /// </summary>
        /// <param name="renderer">Scene renderer for this render pass</param>
        void                                initialize(const ref_ptr<SceneRenderer>& renderer);
        /// <summary>
        /// Prepare destruction scene render pass
        /// </summary>
        void                                shutdown();

        /// <summary>
        /// Begin the scene render pass
        /// </summary>
        /// <param name="camera">Camera provided to render the scene</param>
        virtual void                        begin(const ecs::SceneCamera& camera, const Transform& cameraTransform) = 0;
        /// <summary>
        /// Render the scene render pass
        /// </summary>
        virtual void                        render() = 0;
        /// <summary>
        /// Ending the scene render pass
        /// </summary>
        virtual void                        end() = 0;
                
    protected:
        /// <summary>
        /// Ctor of the scene render pass
        /// </summary>
        /// <param name="name">Name of the scene render pass</param>
        SceneRenderPass(const StringID& name);

        /// <summary>
        /// Create scene render pass pipeline
        /// </summary>
        /// <param name="desc">Pipeline description</param>
        /// <returns>Refrence to the created pipeline</returns>
        ref_ptr<Pipeline>                   create_pipeline(const PipelineDescription& desc);
        /// <summary>
        /// Create scene render pass material
        /// </summary>
        /// <param name="shader">Shader to create the material</param>
        /// <param name="materialName">Name of the material</param>
        /// <returns>Reference to the create material</returns>
        ref_ptr<Material>                   create_material(const ref_ptr<ShaderProgram>& shader, const StringID& materialName);

        /// <summary>
        /// Assign sampler id from a specific render target to the created material
        /// </summary>
        /// <param name="renderTargetName">Name of the render target we would like to assign our sampler id to</param>
        /// <param name="renderPass">Render pass info containing the color attachment index and scene render pass name</param>
        void                                set_render_target(const StringID& renderTargetName, const SceneRenderPassID& renderPass, const ref_ptr<Texture>& defaultTexture);

        /// <summary>
        /// Initialize scene render pass
        /// </summary>
        /// <param name="renderer">Scene renderer for this render pass</param>
        virtual void                        on_initialize(const ref_ptr<SceneRenderer>& renderer);
        /// <summary>
        /// Prepare destruction scene render pass
        /// </summary>
        virtual void                        on_shutdown();

    private:
        using PipelineMap = std::unordered_map<StringID, ref_ptr<Pipeline>>;
        using MaterialMap = std::unordered_map<StringID, ref_ptr<Material>>;

        StringID                            m_name;

        bool                                m_is_enabled;

        ref_ptr<SceneRenderer>              m_scene_renderer;

        PipelineMap                         m_pipeline_map;
        MaterialMap                         m_material_map;
    };
}