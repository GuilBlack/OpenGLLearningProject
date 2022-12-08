#pragma once
#include "Renderer/Texture.h"
#include "glm/glm.hpp"
#include "Renderer/Shader.h"

class Material
{
	Material(const std::shared_ptr<Shader>& shader) 
		: m_Shader(shader) {}
	
private:
	using UniformType = std::variant<float, glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4>;

	std::vector<Texture2D> m_Textures;
	std::unordered_map<std::string, UniformType> m_Uniforms;
	std::shared_ptr<Shader> m_Shader;
};

