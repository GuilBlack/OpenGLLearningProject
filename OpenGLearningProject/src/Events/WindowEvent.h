#pragma once
#include "Event.h"

class WindowResizedEvent : public Event
{
public:
	WindowResizedEvent(int32_t width, int32_t height)
		: m_Width(width), m_Height(height) {}

	EVENT_TYPE_CLASS_DEFINE(WindowResize)
	EVENT_CATEGORY_CLASS_DEFINE(WindowEvent)

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << GetName() << ": " << m_Width << ", " << m_Height;
		return ss.str();
	}

	std::pair<int32_t, int32_t> GetWidthAndHeight() { return { m_Width, m_Height }; }

private:
	int32_t m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_TYPE_CLASS_DEFINE(WindowClose)
	EVENT_CATEGORY_CLASS_DEFINE(WindowEvent)
};