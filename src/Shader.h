#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

typedef unsigned int GLuint;

class Shader
{
private:
	std::string m_FilePath;
	GLuint m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader() = delete;
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int x);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	struct ShaderProgramSource ParseShader(const std::string& filePath);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	GLuint CompileShader(GLuint type, const std::string& source);
	
	int GetUniformLocation(const std::string& name);
};

#endif
