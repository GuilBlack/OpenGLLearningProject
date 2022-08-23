#include "pch.h"
#include "Shader.h"

#include <GL/glew.h>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath)
{
	ShaderSourceProgram source = ParseShader(m_FilePath);
	m_RendererID = CreateShaderProgram(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GlCall(glUseProgram(0));
}

void Shader::SetUniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix)
{
	GlCall(glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, &matrix[0][0]));
}

ShaderSourceProgram Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	ShaderType type = ShaderType::NONE;
	std::stringstream ss[2];

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.rfind("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.rfind("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line.c_str() << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

void Shader::CompileShader(uint32_t& program, uint32_t type, const std::string& source)
{
	uint32_t id;
	const char* src = source.c_str();

	GlCallReturns(glCreateShader(type), id);
	GlCall(glShaderSource(id, 1, &src, NULL));
	GlCall(glCompileShader(id));

	int32_t compiledShader;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &compiledShader));
	if (compiledShader != GL_TRUE)
	{
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
		std::cout << message << std::endl;

		GlCall(glDeleteShader(id));
	}

	GlCall(glAttachShader(program, id));
}

uint32_t Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	uint32_t program;
	GlCallReturns(glCreateProgram(), program);
	CompileShader(program, GL_VERTEX_SHADER, vertexShader);
	CompileShader(program, GL_FRAGMENT_SHADER, fragmentShader);

	GlCall(glLinkProgram(program));

	int32_t linkedProgram;
	GlCall(glGetProgramiv(program, GL_LINK_STATUS, &linkedProgram));
	if (linkedProgram != GL_TRUE)
	{
		int32_t length;
		GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));

		std::cout << "Failed to compile program:" << std::endl;
		std::cout << message << std::endl;
	}

	return program;
}

int32_t Shader::GetUniformLocation(const std::string& uniformName)
{
	if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end())
		return m_UniformLocationCache[uniformName];
	
	GlCall(int32_t location = glGetUniformLocation(m_RendererID, uniformName.c_str()));
	if (location == -1)
		std::cout << "[Warning] Uniform '" << uniformName << "' doesn't exist." << std::endl;

	m_UniformLocationCache[uniformName] = location;
	return location;
}
