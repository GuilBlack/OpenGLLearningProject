#pragma once
#include "../pch.h"
#include "../Events/Event.h"

class Layer
{
public:
	Layer(const std::string& debugName = "Layer")
		: m_DebugName(debugName) {}
	~Layer() {}

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnEvent(Event& ev) {}

protected:
	std::string m_DebugName;
};