#pragma once

#include "Event.h"

class MouseButtonEvent : public Event
{
public:
	inline int GetMouseButton() const { return m_Button; }

	EVENT_CATEGORY_CLASS_DEFINE(MouseEvent | InputEvent)

protected:
	MouseButtonEvent(int button)
		: m_Button(button) {}
	int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button)
		: MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button << std::endl;
		return ss.str();
	}

	EVENT_TYPE_CLASS_DEFINE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button)
		: MouseButtonEvent(button)
	{}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button << std::endl;
		return ss.str();
	}

	EVENT_TYPE_CLASS_DEFINE(MouseButtonReleased)
};

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(double x, double y) 
		: m_MouseX(x), m_MouseY(y) {}

	inline double GetX() const { return m_MouseX; }
	inline double GetY() const { return m_MouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: x = " << m_MouseX << ", y = " << m_MouseY;
		return ss.str();
	}

	EVENT_TYPE_CLASS_DEFINE(MouseMoved)
	EVENT_CATEGORY_CLASS_DEFINE(MouseEvent | InputEvent)

private:
	double m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(double offsetX, double offsetY)
		: m_OffsetX(offsetX), m_OffsetY(offsetY) {}

	inline double GetOffsetX() const { return m_OffsetX; }
	inline double GetOffsetY() const { return m_OffsetY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: offsetX = " << m_OffsetX << ", offsetY = " << m_OffsetY << std::endl;
		return ss.str();
	}

	EVENT_TYPE_CLASS_DEFINE(MouseScrolled)
	EVENT_CATEGORY_CLASS_DEFINE(MouseEvent | InputEvent)

private:
	double m_OffsetX, m_OffsetY;
};