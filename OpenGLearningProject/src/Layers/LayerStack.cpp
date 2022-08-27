#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack()
{
	m_LayerInsertor = m_Layers.begin();
}

LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
		delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
	m_LayerInsertor = m_Layers.emplace(m_LayerInsertor, layer);
	layer->OnAttach();
}

void LayerStack::PopLayer(Layer* layer)
{
	LayerCollection::iterator iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (iterator != m_Layers.end())
	{
		m_Layers.erase(iterator);
		layer->OnDetach();
		--m_LayerInsertor;
	}
}

void LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
	overlay->OnAttach();
}

void LayerStack::PopOverlay(Layer* overlay)
{
	LayerCollection::iterator iterator = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (iterator != m_Layers.end())
	{
		m_Layers.erase(iterator);
		overlay->OnDetach();
	}
}
