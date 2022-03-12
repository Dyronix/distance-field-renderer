#pragma once

namespace rex
{
    class Layer;

    class LayerStack
    {
    public:
        using LayerStackIt = std::vector<std::unique_ptr<Layer>>::iterator;
        using ConstLayerStackIt = std::vector<std::unique_ptr<Layer>>::const_iterator;
        using ReverseLayerStackIt = std::vector<std::unique_ptr<Layer>>::reverse_iterator;
        using ConstReverseLayerStackIt = std::vector<std::unique_ptr<Layer>>::const_reverse_iterator;

        LayerStack(int32 initialSize = -1);
        ~LayerStack();

        //
        // Stack iterator
        //
        LayerStackIt begin();
        ConstLayerStackIt begin() const;
        ConstLayerStackIt cbegin() const;
        LayerStackIt end();
        ConstLayerStackIt end() const;
        ConstLayerStackIt cend() const;

        ReverseLayerStackIt rbegin();
        ConstReverseLayerStackIt rbegin() const;
        ConstReverseLayerStackIt crbegin() const;
        ReverseLayerStackIt rend();
        ConstReverseLayerStackIt rend() const;
        ConstReverseLayerStackIt crend() const;

        //
        // Stack functionality
        //
        void push(std::unique_ptr<Layer> layer);

        void remove_layer(const StringID& layerName);
        void remove_layer(Layer* layer);

        void clear();

    private:
        std::vector<std::unique_ptr<Layer>> m_layers;
    };
}