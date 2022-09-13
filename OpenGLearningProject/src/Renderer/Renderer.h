#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "RenderCommandQueue.h"

class Renderer
{
public:
	Renderer() {}
	static void Init();

	static void BeginFrame();
	static void EndFrame();

	static void Submit(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader);

	static void SetClearColor(const glm::vec4& color);
	static void Clear();
	void Render(const VertexArray& va, const Shader& shader) const;

	inline static Renderer& GetRenderer() { return *s_Renderer; }
	inline RenderCommandQueue& GetCommandQueue() { return m_CommandQueue; }
private:
	static Renderer* s_Renderer;
	RenderCommandQueue m_CommandQueue;
};
