#include "pch.h"
#include "RenderCommandQueue.h"

RenderCommandQueue::RenderCommandQueue()
{
}

void RenderCommandQueue::PushCommand(CommandFn fn)
{
	m_CommandQueue.push_back(fn);
}

void RenderCommandQueue::Execute()
{
	while (!m_CommandQueue.empty())
	{
		m_CommandQueue.front()();
		m_CommandQueue.pop_front();
	}
}
