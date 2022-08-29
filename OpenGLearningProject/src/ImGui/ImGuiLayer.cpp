#include "pch.h"
#include "ImGuiLayer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Inputs/InputCodes.h"

#include <../Engine3D.h>

ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") 
{
	m_ImGuiKeyMap.insert(
		{
			{KTab,			ImGuiKey_Tab},
			{KLeft,			ImGuiKey_LeftArrow},
			{KRight,		ImGuiKey_RightArrow},
			{KUp,			ImGuiKey_UpArrow},
			{KDown,			ImGuiKey_DownArrow},
			{KPageUp,		ImGuiKey_PageUp},
			{KPageDown,		ImGuiKey_PageDown},
			{KHome,			ImGuiKey_Home},
			{KEnd,			ImGuiKey_End},
			{KInsert,		ImGuiKey_Insert},
			{KDelete,		ImGuiKey_Delete},
			{KBackspace,	ImGuiKey_Backspace},
			{KSpace,		ImGuiKey_Space},
			{KEnter,		ImGuiKey_Enter},
			{KSpace,		ImGuiKey_Escape},
			{KApostrophe,	ImGuiKey_Apostrophe},
			{KComma,		ImGuiKey_Comma},
			{KMinus,		ImGuiKey_Minus},
			{KPeriod,		ImGuiKey_Period},
			{KSlash,		ImGuiKey_Slash},
			{KSemicolon,	ImGuiKey_Semicolon},
			{KEqual,		ImGuiKey_Equal},
			{KLeftBracket,	ImGuiKey_LeftBracket},
			{KBackslash,	ImGuiKey_Backslash},
			{KRightBracket,	ImGuiKey_RightBracket},
			{KGraveAccent,	ImGuiKey_GraveAccent},
			{KCapsLock,		ImGuiKey_CapsLock},
			{KScrollLock,	ImGuiKey_ScrollLock},
			{KNumLock,		ImGuiKey_NumLock},
			{KPrintScreen,	ImGuiKey_PrintScreen},
			{KPause,		ImGuiKey_Pause},
			{KP0,			ImGuiKey_Keypad0},
			{KP1,			ImGuiKey_Keypad1},
			{KP2,			ImGuiKey_Keypad2},
			{KP3,			ImGuiKey_Keypad3},
			{KP4,			ImGuiKey_Keypad4},
			{KP5,			ImGuiKey_Keypad5},
			{KP6,			ImGuiKey_Keypad6},
			{KP7,			ImGuiKey_Keypad7},
			{KP8,			ImGuiKey_Keypad8},
			{KP9,			ImGuiKey_Keypad9},
			{KPDecimal,		ImGuiKey_KeypadDecimal},
			{KPDivide,		ImGuiKey_KeypadDivide},
			{KPMultiply,	ImGuiKey_KeypadMultiply},
			{KPSubtract,	ImGuiKey_KeypadSubtract},
			{KPAdd,			ImGuiKey_KeypadAdd},
			{KPEnter,		ImGuiKey_KeypadEnter},
			{KPEqual,		ImGuiKey_KeypadEqual},
			{KLeftShift,	ImGuiKey_LeftShift},
			{KLeftControl,	ImGuiKey_LeftCtrl},
			{KLeftAlt,		ImGuiKey_LeftAlt},
			{KLeftSuper,	ImGuiKey_LeftSuper},
			{KRightShift,	ImGuiKey_RightShift},
			{KRightControl,	ImGuiKey_RightCtrl},
			{KRightAlt,		ImGuiKey_RightAlt},
			{KRightSuper,	ImGuiKey_RightSuper},
			{KMenu,			ImGuiKey_Menu},
			{K0,			ImGuiKey_0},
			{K1,			ImGuiKey_1},
			{K2,			ImGuiKey_2},
			{K3,			ImGuiKey_3},
			{K4,			ImGuiKey_4},
			{K5,			ImGuiKey_5},
			{K6,			ImGuiKey_6},
			{K7,			ImGuiKey_7},
			{K8,			ImGuiKey_8},
			{K9,			ImGuiKey_9},
			{KA,			ImGuiKey_A},
			{KB,			ImGuiKey_B},
			{KC,			ImGuiKey_C},
			{KD,			ImGuiKey_D},
			{KE,			ImGuiKey_E},
			{KF,			ImGuiKey_F},
			{KG,			ImGuiKey_G},
			{KH,			ImGuiKey_H},
			{KI,			ImGuiKey_I},
			{KJ,			ImGuiKey_J},
			{KK,			ImGuiKey_K},
			{KL,			ImGuiKey_L},
			{KM,			ImGuiKey_M},
			{KN,			ImGuiKey_N},
			{KO,			ImGuiKey_O},
			{KP,			ImGuiKey_P},
			{KQ,			ImGuiKey_Q},
			{KR,			ImGuiKey_R},
			{KS,			ImGuiKey_S},
			{KT,			ImGuiKey_T},
			{KU,			ImGuiKey_U},
			{KV,			ImGuiKey_V},
			{KW,			ImGuiKey_W},
			{KX,			ImGuiKey_X},
			{KY,			ImGuiKey_Y},
			{KZ,			ImGuiKey_Z},
			{KF1,			ImGuiKey_F1},
			{KF2,			ImGuiKey_F2},
			{KF3,			ImGuiKey_F3},
			{KF4,			ImGuiKey_F4},
			{KF5,			ImGuiKey_F5},
			{KF6,			ImGuiKey_F6},
			{KF7,			ImGuiKey_F7},
			{KF8,			ImGuiKey_F8},
			{KF9,			ImGuiKey_F9},
			{KF10,			ImGuiKey_F10},
			{KF11,			ImGuiKey_F11},
			{KF12,			ImGuiKey_F12}
		});
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
		ENGINE_ERROR("{0}", oor.what());
		return ImGuiKey_None;
	}
	return ImGuiKey_None;
}
