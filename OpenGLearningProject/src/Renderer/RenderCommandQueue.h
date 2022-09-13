#pragma once

using CommandFn = std::function<void()>;

class RenderCommandQueue
{
public:

	RenderCommandQueue();

	void PushCommand(CommandFn fn);
	void Execute();

private:
	std::deque<CommandFn> m_CommandQueue;
	std::thread m_RenderThread;
};

