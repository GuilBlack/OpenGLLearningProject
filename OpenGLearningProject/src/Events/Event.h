#pragma once
#include "../pch.h"

#define BIT(x) (1 << x)

enum class EventType
{
	None = 0,
	WindowResize, WindowClose,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	None = 0,
	WindowEvent = BIT(0),
	InputEvent = BIT(1),
	KeyboardEvent = BIT(2),
	MouseEvent = BIT(3),
	MouseButtonEvent = BIT(4)
};

#define EVENT_TYPE_CLASS_DEFINE(type)	static EventType GetStaticEventType() { return EventType::##type; }\
										virtual EventType GetEventType() const override { return GetStaticEventType(); }\
										virtual const char* GetName() const override { return #type; }

#define EVENT_CATEGORY_CLASS_DEFINE(category)	virtual int GetCategory() const override { return category; }

class Event
{
	friend inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
public:
	bool Handler = false;

public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategory() const = 0;
	virtual std::string ToString() const { return GetName(); }

	virtual bool IsCategory(EventCategory category)
	{
		return GetCategory() & (int)category;
	}
};

/// <summary>
/// Used to dispatch events.
/// </summary>
class EventDispatcher
{
public:
	template<typename T>
	using EventFn = std::function<bool(T&)>;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="e">It's an instance of an inheritor of the Event class and not an instance of the class directly.</param>
	EventDispatcher(Event& ev)
		: m_Event(ev) {}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T">An inheritor of the Event class</typeparam>
	/// <param name="fn">A callback function that takes in an inheritor of the Event class and returns a bool to know if the event has been handled.</param>
	/// <returns>Bool signifying if the dispatched event has been handled.</returns>
	template<typename T>
	bool Dispatch(EventFn<T> fn)
	{
		if (m_Event.GetEventType() == T::GetStaticEventType())
		{
			m_Event.Handler = fn(*(T*)&m_Event);
			return true;
		}
		return false;
	}

private:
	Event& m_Event;
};

using EventCallbackFn = std::function<void(Event&)>;
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)