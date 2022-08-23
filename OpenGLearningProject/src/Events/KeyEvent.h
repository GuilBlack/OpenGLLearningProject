#pragma once
#include "Event.h"

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() { return m_KeyCode; }
	EVENT_CATEGORY_CLASS_DEFINE(KeyboardEvent | InputEvent)

protected:
	KeyEvent(int keyCode) : m_KeyCode(keyCode) {}
	int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keyCode, int repeatCount)
		: KeyEvent(keyCode), m_RepeatCount(repeatCount)
	{}

	inline int GetRepeatCount() { return m_RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << ", repeated: " << m_RepeatCount << std::endl;
		return ss.str();
	}

	EVENT_TYPE_CLASS_DEFINE(KeyPressed)
private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keyCode)
		: KeyEvent(keyCode)
	{}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode << std::endl;
		return ss.str();
	}

	EVENT_TYPE_CLASS_DEFINE(KeyReleased)
};