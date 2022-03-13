#include "regina_pch.h"

#include "regina_layer.h"

namespace regina
{
    //-------------------------------------------------------------------------
    ReginaLayer::ReginaLayer()
    {}
    //-------------------------------------------------------------------------
    ReginaLayer::~ReginaLayer()
    {}

    //-------------------------------------------------------------------------
    void ReginaLayer::on_attach()
    {
        // load shader, deferred shader: g-buffer
        // load shader, deferred shader: light
        // load shader, deferred shader: light-box

        // load model, lattice

        
    }
    //-------------------------------------------------------------------------
    void ReginaLayer::on_detach()
    {

    }
}