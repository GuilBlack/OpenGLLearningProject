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

	static void SetClearColor();
	static void Clear();
	void Render(const VertexArray& va, const Shader& shader) const;
private:
	static Renderer* s_Renderer;
	RenderCommandQueue m_CommandQueue;
};
