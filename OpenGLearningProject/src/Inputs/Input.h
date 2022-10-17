#pragma once
#include "InputCodes.h"
#include <glm/glm.hpp>

class Input
{
public:
	static bool IsKeyPressed(KeyCode key);
	static bool IsMouseButtonPressed(MouseCode button);
	static glm::dvec2 GetMousePos();
	static double GetMousePosX();
	static double GetMousePosY();
};

