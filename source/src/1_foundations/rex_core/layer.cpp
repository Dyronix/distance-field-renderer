#include "rex_core_pch.h"

#include "layer.h"

namespace rex
{
    //-------------------------------------------------------------------------
    Layer::Layer(const StringID name, StackOrder order, EnableImGUIRendering EnableImGUIRendering)
        : m_name(name)
        , m_order(order)
        , m_enabled(true)
        , m_imgui_enabled(EnableImGUIRendering == EnableImGUIRendering::YES)
    {
    }
    //-------------------------------------------------------------------------
    Layer::~Layer() = default;

    //-------------------------------------------------------------------------
    void Layer::enable()
    {
        m_enabled = true;
    }
    //-------------------------------------------------------------------------
    void Layer::disable()
    {
        m_enabled = false;
    }

    //-------------------------------------------------------------------------
    void Layer::attach()
    {
        on_attach();
    }

    //-------------------------------------------------------------------------
    void Layer::detach()
    {
        on_detach();
    }

    //-------------------------------------------------------------------------
    void Layer::update(const FrameInfo& info)
    {
        on_update(info);
    }

    //-------------------------------------------------------------------------
    void Layer::imgui_render()
    {
        if (m_imgui_enabled == false)
        {
            return;
        }

        on_im_gui_render();
    }

    //-------------------------------------------------------------------------
    void Layer::handle_event(events::Event& evt)
    {
        on_event(evt);
    }

    //-------------------------------------------------------------------------
    const StringID& Layer::get_name() const
    {
        return m_name;
    }
    //-------------------------------------------------------------------------
    const Layer::StackOrder Layer::get_order() const
    {
        return m_order;
    }
    //-------------------------------------------------------------------------
    const bool Layer::is_enabled() const
    {
        return m_enabled;
    }
    //-------------------------------------------------------------------------
    const bool Layer::is_im_gui_enabled() const
    {
        return m_imgui_enabled;
    }

    //-------------------------------------------------------------------------
    void Layer::on_update(const FrameInfo& info)
    {
        UNUSED_PARAM(info);
    }

    //-------------------------------------------------------------------------
    void Layer::on_im_gui_render()
    {
        // Noting to implement
    }

    //-------------------------------------------------------------------------
    void Layer::on_event(events::Event& event)
    {
        UNUSED_PARAM(event);
    }
}