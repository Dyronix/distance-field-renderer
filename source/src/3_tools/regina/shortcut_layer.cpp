#include "regina_pch.h"
#include "shortcut_layer.h"

#include "core_application.h"

#include "event_dispatcher.h"

#include "input/key_pressed.h"
#include "input/win32_input.h"
#include "input/keyboard/win32_key_codes.h"

namespace regina
{
    //-------------------------------------------------------------------------
    ShortcutLayer::ShortcutLayer()
        : rex::Layer("shortcut_layer"_sid, -1, rex::Layer::EnableImGUIRendering::NO)
    {
    }

    //-------------------------------------------------------------------------
    ShortcutLayer::~ShortcutLayer()
    {
    }

    //-------------------------------------------------------------------------
    void ShortcutLayer::on_event(rex::events::Event& event)
    {
        rex::events::EventDispatcher dispatcher(event);

        dispatcher.dispatch<rex::events::KeyPressed>([&](rex::events::KeyPressed& pressedEvt) { return on_key_pressed(pressedEvt); });
    }

    //-------------------------------------------------------------------------
    bool ShortcutLayer::on_key_pressed(const rex::events::KeyPressed& keyPressEvent)
    {
        switch (keyPressEvent.get_key())
        {
            case R_KEY_ESCAPE:
                R_INFO("Escape key pressed, closing application");
                rex::CoreApplication::get_instance()->quit();
                return true;

            default: return false;
        }
    }

}