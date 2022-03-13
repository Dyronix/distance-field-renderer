#pragma once

#include "event.h"

namespace rex
{
    struct FrameInfo;

    class Layer
    {
    public:
        using EnableImGUIRendering = YesNoEnum;
        using StackOrder = int32;

        Layer(const StringID name = "Layer"_sid, StackOrder order = -1, EnableImGUIRendering EnableImGUIRendering = EnableImGUIRendering::YES);
        virtual ~Layer();

        void enable();
        void disable();

        void attach();
        void detach();

        void update(const FrameInfo& info);
        void imgui_render();
        void handle_event(events::Event& evt);

        const StringID& get_name() const;
        const StackOrder get_order() const;

        const bool is_enabled() const;
        const bool is_im_gui_enabled() const;

    protected:
        virtual void on_attach(){/* Nothing to implement */};
        virtual void on_detach(){/* Nothing to implement */};

        virtual void on_update(const FrameInfo& /* info */);
        virtual void on_im_gui_render();
        virtual void on_event(events::Event& /* event */);

    private:
        StringID m_name;
        StackOrder m_order;

        bool m_enabled;
        bool m_imgui_enabled;
    };
}