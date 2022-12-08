#pragma once
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommandQueue.h"
#include "Camera.h"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "Entity/Mesh.h"

class Renderer
{
public:
	static void Init();
	static Renderer& GetRenderer()
	{
		static Renderer renderer;
		return renderer;
	}
	static void RendererShutdown() { Renderer::GetRenderer().Shutdown(); }

	static void BeginFrame();
	static void EndFrame();

	static void BeginScene(std::shared_ptr<Camera> cam);
	static void EndScene();

	static void Submit(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Shader>& shader, std::shared_ptr<Texture2D>& texture);

	static void SetClearColor(const glm::vec4& color);
	static void Clear();

	static inline RenderCommandQueue& GetRendererCommandQueue() { return Renderer::GetRenderer().GetCommandQueue(); }

private:
	Renderer()
		: m_SceneData( { glm::mat4(1.f) })
	{
		m_CommandQueue.Init();
	}

	void Shutdown();

	void Render(const VertexArray& va, const Shader& shader) const;

	//inline static Renderer& GetRenderer() { return *s_Renderer; }
	inline RenderCommandQueue& GetCommandQueue() { return m_CommandQueue; }

private:
	//static Renderer* s_Renderer;
	RenderCommandQueue m_CommandQueue;
	struct SceneData
	{
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_CamPosition;
	} m_SceneData;
};
