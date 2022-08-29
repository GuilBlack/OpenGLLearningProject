#include "pch.h"
#include "Engine.h"
#include "Input.h"

bool Input::IsKeyPressed(KeyCode key)
{
    GLFWwindow* window = &Engine::GetEngine().GetWindow();
    int state = glfwGetKey(window, static_cast<int32_t>(key));
    return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
    GLFWwindow* window = &Engine::GetEngine().GetWindow();
    int state = glfwGetMouseButton(window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
}

glm::dvec2 Input::GetMousePos()
{
    GLFWwindow* window = &Engine::GetEngine().GetWindow();

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { xpos, ypos };
}

double Input::GetMousePosX()
{
    return GetMousePos().x;
}

double Input::GetMousePosY()
{
    return GetMousePos().y;
}
