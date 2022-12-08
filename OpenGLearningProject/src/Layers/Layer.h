#pragma once
#include "../pch.h"
#include "../Events/Event.h"
#include "Timestep.h"

class Layer
{
public:
	Layer(const std::string& debugName = "Layer")
		: m_DebugName(debugName) {}
	~Layer() {}

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(Timestep deltaTime) {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(Event& ev) {}

protected:
	std::string m_DebugName;
};