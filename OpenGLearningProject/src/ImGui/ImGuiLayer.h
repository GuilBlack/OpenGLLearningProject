#pragma once
#include "../Layers/Layer.h"

#include "../Events/WindowEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void OnAttach() override;
	void OnDetach() override;

	void Begin();
	void End();
	void OnImGuiRender() override;
};

