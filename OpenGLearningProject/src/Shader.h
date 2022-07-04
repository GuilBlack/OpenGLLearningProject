#pragma once
#include <string>
#include <unordered_map>
#include <stdint.h>
#include "vendor/glm/glm.hpp"

struct ShaderSourceProgram
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	uint32_t m_RendererID;
	std::unordered_map<std::string, int32_t> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix);

private:
	ShaderSourceProgram ParseShader(const std::string& filepath);
	void CompileShader(uint32_t& program, uint32_t type, const std::string& source);
	uint32_t CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	int32_t GetUniformLocation(const std::string& uniformName);
};

