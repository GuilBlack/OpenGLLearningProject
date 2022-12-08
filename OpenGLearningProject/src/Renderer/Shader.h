#pragma once
#include "vendor/glm/glm.hpp"

struct ShaderSourceProgram
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();
	static Shader* Create(const std::string& filepath);

	void Bind() const;
	void Unbind() const;
	void BindCommand() const;
	void UnbindCommand() const;

	//Set uniforms
	void SetUniformVec3f(const std::string& uniformName, const glm::vec3& vec);
	void SetUniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix);

private:
	std::string m_FilePath;
	uint32_t m_RendererID;
	std::unordered_map<std::string, int32_t> m_UniformLocationCache;

private:
	ShaderSourceProgram ParseShader(const std::string& filepath);
	void CompileShader(uint32_t& program, uint32_t type, const std::string& source);
	void CreateShaderProgram();
	int32_t GetUniformLocation(const std::string& uniformName);
};

