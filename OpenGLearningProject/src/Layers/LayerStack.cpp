#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack()
{}

LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
		delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertorIndex, layer);
	++m_LayerInsertorIndex;
	layer->OnAttach();
}

void LayerStack::PopLayer(Layer* layer)
{
	LayerCollection::iterator iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (iterator != m_Layers.end())
	{
		m_Layers.erase(iterator);
		layer->OnDetach();
		--m_LayerInsertorIndex;
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
