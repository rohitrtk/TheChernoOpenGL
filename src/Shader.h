#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <unordered_map>

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
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);

private:
	struct ShaderProgramSource ParseShader(const std::string& filePath);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	GLuint CompileShader(GLuint type, const std::string& source);
	GLuint GetUniformLocation(const std::string& name);
};

#endif
