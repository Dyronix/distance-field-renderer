#pragma once

#include "layer.h"
#include "event.h"

namespace rex
{
    namespace events
    {
        class KeyPressed;
    }
}

namespace regina
{
    class ShortcutLayer : public rex::Layer
    {
    public:
        ShortcutLayer();
        ~ShortcutLayer() override;

        void on_event(rex::events::Event& event) override;

    private:
        bool on_key_pressed(const rex::events::KeyPressed& keyPressEvent);
    };
}