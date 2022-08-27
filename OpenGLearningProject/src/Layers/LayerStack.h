#pragma once
#include "Layer.h"

using LayerCollection = std::vector<Layer*>;

/// <summary>
/// A container for layers and overlays. 
/// The container will contain the layers in the front and the overlays in the back.
/// It's made like this so that when we iterate to render, we will draw the layers first then the overlays on top of them.
/// For events, it will be handled in a backwards manner. We will iterate from the end to the beginning of the stack so that the last layer/overlay in the stack will be the first to try and handle the event.
/// </summary>
class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopOverlay(Layer* overlay);

	LayerCollection::iterator begin() { return m_Layers.begin(); }
	LayerCollection::iterator end() { return m_Layers.end(); }

private:
	LayerCollection m_Layers;
	LayerCollection::iterator m_LayerInsertor;
};