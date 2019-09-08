#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Renderer.h"

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

Shader::Shader(const std::string& filePath) 
	: m_FilePath(filePath), m_RendererID(NULL)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}


void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(NULL));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w)
{
	GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}
	
	m_UniformLocationCache[name] = location;

	return location;
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);	// Attaching vertex shader to program
	glAttachShader(program, fs);	// Attaching fragment shader to program
	glLinkProgram(program);			// Links the program
	glValidateProgram(program);		// Checks if the program executable can execute given current OpenGL status

	glDeleteShader(vs);				// Deleting shaders since they've already been linked to program
	glDeleteShader(fs);

	return program;
}

GLuint Shader::CompileShader(GLuint type, const std::string& source)
{
	GLuint id = glCreateShader(type);		// The shader
	const char* src = source.c_str();		// Shader code

	glShaderSource(id, 1, &src, nullptr);	// Sets the source code of the shader to the string specified
	glCompileShader(id);					// Compiles the source code strings in the shader

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)	// Print error if shader failed to compile
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!";
		std::cout << message << "\n";

		glDeleteShader(id);
		return 0;
	}

	return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& path)
{
	std::ifstream stream(path);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;
	std::string line;
	std::stringstream ss[2];
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}