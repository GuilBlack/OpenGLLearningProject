#include "pch.h"
#include "Shader.h"

#include <GL/glew.h>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath)
{
	CreateShaderProgram();
}

Shader::~Shader()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glDeleteProgram(m_RendererID);
		});
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::BindCommand() const
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			glUseProgram(m_RendererID);
		});
}

void Shader::UnbindCommand() const
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([]()
		{
			glUseProgram(0);
		});
}

void Shader::SetUniformMatrix4fv(const std::string& uniformName, const glm::mat4& matrix)
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this, uniformName, matrix]()
		{
			glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, &matrix[0][0]);
		});
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
	uint32_t id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int32_t compiledShader;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiledShader);
	if (compiledShader != GL_TRUE)
	{
		ENGINE_ERROR("Couldn't compile shaders.");
		glDeleteShader(id);
	}

	glAttachShader(program, id);
}

void Shader::CreateShaderProgram()
{
	Renderer::GetRenderer().GetCommandQueue().PushCommand([this]()
		{
			auto[vertexShader, fragmentShader] = ParseShader(m_FilePath);
			uint32_t program = glCreateProgram();
			CompileShader(program, GL_VERTEX_SHADER, vertexShader);
			CompileShader(program, GL_FRAGMENT_SHADER, fragmentShader);

			glLinkProgram(program);

			int32_t linkedProgram;
			glGetProgramiv(program, GL_LINK_STATUS, &linkedProgram);
			if (linkedProgram != GL_TRUE)
			{
				ENGINE_ERROR("Couldn't link shaders.");
			}

			m_RendererID = program;
		});
}

int32_t Shader::GetUniformLocation(const std::string& uniformName)
{
	if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end())
		return m_UniformLocationCache[uniformName];
	
	int32_t location = glGetUniformLocation(m_RendererID, uniformName.c_str());
	if (location == -1)
		std::cout << "[Warning] Uniform '" << uniformName << "' doesn't exist." << std::endl;

	m_UniformLocationCache[uniformName] = location;
	return location;
}
