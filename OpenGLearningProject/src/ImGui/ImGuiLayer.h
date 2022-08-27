#pragma once
#include "../Layers/Layer.h"

#include "../Events/WindowEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();

	void OnAttach();
	void OnDetach();
	void OnUpdate();
	void OnEvent(Event& ev);

	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& ev);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& ev);
	bool OnMouseMovedEvent(MouseMovedEvent& ev);
	bool OnMouseScrolledEvent(MouseScrolledEvent& ev);
	bool OnKeyPressedEvent(KeyPressedEvent& ev);
	bool OnKeyReleasedEvent(KeyReleasedEvent& ev);
	bool OnKeyTypedEvent(KeyTypedEvent& ev);

private:
	std::unordered_map<int,int> m_ImGuiKeyMap;

private:
	int KeyToImGuiKey(int key);
};

