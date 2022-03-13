#pragma once

#include "layer.h"

namespace regina
{
    class ReginaLayer : public rex::Layer
    {
    public:
        ReginaLayer();
        ~ReginaLayer() override;

        void on_attach() override;
        void on_detach() override;
    };
}