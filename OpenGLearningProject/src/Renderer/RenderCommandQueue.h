#pragma once

using CommandFn = std::function<void()>;
struct GLFWwindow;

class RenderCommandQueue
{
	friend class Renderer;
public:

	RenderCommandQueue();
	~RenderCommandQueue();

	void Init();

	void PushCommand(CommandFn fn);
	void Execute();

	void NotifyBeginLoop();
	void WaitForLoop();

	inline void SetSafeExit(const bool& safeExit) { m_SafeExit = safeExit; }
private:
	std::deque<CommandFn> m_CommandQueue;
	std::thread m_RenderThread;
	std::mutex m_CommandQueueLock;
	bool m_Running;
	bool m_SafeExit;
	std::atomic_char16_t m_Loop;

private:
	void Shutdown();
	void Run(GLFWwindow* window);
};
