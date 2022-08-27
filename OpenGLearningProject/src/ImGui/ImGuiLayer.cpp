#include "pch.h"
#include "ImGuiLayer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <../Engine3D.h>

ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") 
{
	m_ImGuiKeyMap.insert(
		{
			{GLFW_KEY_TAB, ImGuiKey_Tab},
			{GLFW_KEY_LEFT, ImGuiKey_LeftArrow},
			{GLFW_KEY_RIGHT, ImGuiKey_RightArrow},
			{GLFW_KEY_UP, ImGuiKey_UpArrow},
			{GLFW_KEY_DOWN, ImGuiKey_DownArrow},
			{GLFW_KEY_PAGE_UP, ImGuiKey_PageUp},
			{GLFW_KEY_PAGE_DOWN, ImGuiKey_PageDown},
			{GLFW_KEY_HOME, ImGuiKey_Home},
			{GLFW_KEY_END, ImGuiKey_End},
			{GLFW_KEY_INSERT, ImGuiKey_Insert},
			{GLFW_KEY_DELETE, ImGuiKey_Delete},
			{GLFW_KEY_BACKSPACE, ImGuiKey_Backspace},
			{GLFW_KEY_SPACE, ImGuiKey_Space},
			{GLFW_KEY_ENTER, ImGuiKey_Enter},
			{GLFW_KEY_ESCAPE, ImGuiKey_Escape},
			{GLFW_KEY_APOSTROPHE, ImGuiKey_Apostrophe},
			{GLFW_KEY_COMMA, ImGuiKey_Comma},
			{GLFW_KEY_MINUS, ImGuiKey_Minus},
			{GLFW_KEY_PERIOD, ImGuiKey_Period},
			{GLFW_KEY_SLASH, ImGuiKey_Slash},
			{GLFW_KEY_SEMICOLON, ImGuiKey_Semicolon},
			{GLFW_KEY_EQUAL, ImGuiKey_Equal},
			{GLFW_KEY_LEFT_BRACKET, ImGuiKey_LeftBracket},
			{GLFW_KEY_BACKSLASH, ImGuiKey_Backslash},
			{GLFW_KEY_RIGHT_BRACKET, ImGuiKey_RightBracket},
			{GLFW_KEY_GRAVE_ACCENT, ImGuiKey_GraveAccent},
			{GLFW_KEY_CAPS_LOCK, ImGuiKey_CapsLock},
			{GLFW_KEY_SCROLL_LOCK, ImGuiKey_ScrollLock},
			{GLFW_KEY_NUM_LOCK, ImGuiKey_NumLock},
			{GLFW_KEY_PRINT_SCREEN, ImGuiKey_PrintScreen},
			{GLFW_KEY_PAUSE, ImGuiKey_Pause},
			{GLFW_KEY_KP_0, ImGuiKey_Keypad0},
			{GLFW_KEY_KP_1, ImGuiKey_Keypad1},
			{GLFW_KEY_KP_2, ImGuiKey_Keypad2},
			{GLFW_KEY_KP_3, ImGuiKey_Keypad3},
			{GLFW_KEY_KP_4, ImGuiKey_Keypad4},
			{GLFW_KEY_KP_5, ImGuiKey_Keypad5},
			{GLFW_KEY_KP_6, ImGuiKey_Keypad6},
			{GLFW_KEY_KP_7, ImGuiKey_Keypad7},
			{GLFW_KEY_KP_8, ImGuiKey_Keypad8},
			{GLFW_KEY_KP_9, ImGuiKey_Keypad9},
			{GLFW_KEY_KP_DECIMAL, ImGuiKey_KeypadDecimal},
			{GLFW_KEY_KP_DIVIDE, ImGuiKey_KeypadDivide},
			{GLFW_KEY_KP_MULTIPLY, ImGuiKey_KeypadMultiply},
			{GLFW_KEY_KP_SUBTRACT, ImGuiKey_KeypadSubtract},
			{GLFW_KEY_KP_ADD, ImGuiKey_KeypadAdd},
			{GLFW_KEY_KP_ENTER, ImGuiKey_KeypadEnter},
			{GLFW_KEY_KP_EQUAL, ImGuiKey_KeypadEqual},
			{GLFW_KEY_LEFT_SHIFT, ImGuiKey_LeftShift},
			{GLFW_KEY_LEFT_CONTROL, ImGuiKey_LeftCtrl},
			{GLFW_KEY_LEFT_ALT, ImGuiKey_LeftAlt},
			{GLFW_KEY_LEFT_SUPER, ImGuiKey_LeftSuper},
			{GLFW_KEY_RIGHT_SHIFT, ImGuiKey_RightShift},
			{GLFW_KEY_RIGHT_CONTROL, ImGuiKey_RightCtrl},
			{GLFW_KEY_RIGHT_ALT, ImGuiKey_RightAlt},
			{GLFW_KEY_RIGHT_SUPER, ImGuiKey_RightSuper},
			{GLFW_KEY_MENU, ImGuiKey_Menu},
			{GLFW_KEY_0, ImGuiKey_0},
			{GLFW_KEY_1, ImGuiKey_1},
			{GLFW_KEY_2, ImGuiKey_2},
			{GLFW_KEY_3, ImGuiKey_3},
			{GLFW_KEY_4, ImGuiKey_4},
			{GLFW_KEY_5, ImGuiKey_5},
			{GLFW_KEY_6, ImGuiKey_6},
			{GLFW_KEY_7, ImGuiKey_7},
			{GLFW_KEY_8, ImGuiKey_8},
			{GLFW_KEY_9, ImGuiKey_9},
			{GLFW_KEY_A, ImGuiKey_A},
			{GLFW_KEY_B, ImGuiKey_B},
			{GLFW_KEY_C, ImGuiKey_C},
			{GLFW_KEY_D, ImGuiKey_D},
			{GLFW_KEY_E, ImGuiKey_E},
			{GLFW_KEY_F, ImGuiKey_F},
			{GLFW_KEY_G, ImGuiKey_G},
			{GLFW_KEY_H, ImGuiKey_H},
			{GLFW_KEY_I, ImGuiKey_I},
			{GLFW_KEY_J, ImGuiKey_J},
			{GLFW_KEY_K, ImGuiKey_K},
			{GLFW_KEY_L, ImGuiKey_L},
			{GLFW_KEY_M, ImGuiKey_M},
			{GLFW_KEY_N, ImGuiKey_N},
			{GLFW_KEY_O, ImGuiKey_O},
			{GLFW_KEY_P, ImGuiKey_P},
			{GLFW_KEY_Q, ImGuiKey_Q},
			{GLFW_KEY_R, ImGuiKey_R},
			{GLFW_KEY_S, ImGuiKey_S},
			{GLFW_KEY_T, ImGuiKey_T},
			{GLFW_KEY_U, ImGuiKey_U},
			{GLFW_KEY_V, ImGuiKey_V},
			{GLFW_KEY_W, ImGuiKey_W},
			{GLFW_KEY_X, ImGuiKey_X},
			{GLFW_KEY_Y, ImGuiKey_Y},
			{GLFW_KEY_Z, ImGuiKey_Z},
			{GLFW_KEY_F1, ImGuiKey_F1},
			{GLFW_KEY_F2, ImGuiKey_F2},
			{GLFW_KEY_F3, ImGuiKey_F3},
			{GLFW_KEY_F4, ImGuiKey_F4},
			{GLFW_KEY_F5, ImGuiKey_F5},
			{GLFW_KEY_F6, ImGuiKey_F6},
			{GLFW_KEY_F7, ImGuiKey_F7},
			{GLFW_KEY_F8, ImGuiKey_F8},
			{GLFW_KEY_F9, ImGuiKey_F9},
			{GLFW_KEY_F10, ImGuiKey_F10},
			{GLFW_KEY_F11, ImGuiKey_F11},
			{GLFW_KEY_F12, ImGuiKey_F12}
		}
		);
}

void ImGuiLayer::OnAttach()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	Engine3D& engine =  Engine3D::GetEngine();
	ImGui_ImplGlfw_InitForOpenGL(&engine.GetWindow(), false);
	ImGui_ImplOpenGL3_Init("#version 420");
}

void ImGuiLayer::OnDetach()
{}

void ImGuiLayer::OnUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool showDemo = true;

	if (showDemo)
		ImGui::ShowDemoWindow(&showDemo);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event & ev)
{
	EventDispatcher dispatcher(ev);
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
	dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
}

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(ev.GetMouseButton(), true);
	return false;
}

bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(ev.GetMouseButton(), false);
	return false;
}

bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent((float)ev.GetX(), (float)ev.GetY());
	return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent((float)ev.GetOffsetX(), (float)ev.GetOffsetY());
	return false;
}

bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(KeyToImGuiKey(ev.GetKeyCode()), true);

	io.KeyAlt = (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt));
	io.KeyCtrl = (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl));
	io.KeyShift = (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift));
	io.KeySuper = (ImGui::IsKeyDown(ImGuiKey_LeftSuper) || ImGui::IsKeyDown(ImGuiKey_RightSuper));

	ENGINE_TRACE("{0}, {1}", KeyToImGuiKey(ev.GetKeyCode()), ev);
	return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(KeyToImGuiKey(ev.GetKeyCode()), false);

	io.KeyAlt = (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt));
	io.KeyCtrl = (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl));
	io.KeyShift = (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift));
	io.KeySuper = (ImGui::IsKeyDown(ImGuiKey_LeftSuper) || ImGui::IsKeyDown(ImGuiKey_RightSuper));

	ENGINE_TRACE("{0}, {1}", KeyToImGuiKey(ev.GetKeyCode()), ev);
	return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& ev)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter(ev.GetKeyCode());

	return false;
}

int ImGuiLayer::KeyToImGuiKey(int key)
{
	try
	{
		int newKey = m_ImGuiKeyMap.at(key);
		return newKey;
	}
	catch (const std::out_of_range& oor)
	{
		return ImGuiKey_None;
	}
	return ImGuiKey_None;
}
