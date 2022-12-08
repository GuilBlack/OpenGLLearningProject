#pragma once

using CommandFn = std::function<void()>;
struct GLFWwindow;

class RenderCommandQueue
{
	friend class Renderer;
public:

	RenderCommandQueue();
	~RenderCommandQueue();

	void PushCommand(CommandFn fn);
private:
	std::deque<CommandFn> m_CommandQueue;
	std::thread m_RenderThread;
	std::mutex m_CommandQueueLock;
	bool m_Running;
	bool m_SafeExit;
	std::atomic_char16_t m_Loop;

private:
	void Init();
	void Shutdown();
	void Run(GLFWwindow* window);
	void Execute();

	void NotifyBeginLoop();
	void WaitForLoop();

	inline void SetSafeExit(const bool& safeExit) { m_SafeExit = safeExit; }
};
