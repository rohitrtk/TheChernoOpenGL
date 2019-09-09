#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Renderer.h"
#include "GLFW/glfw3.h"

#include <string>

class Texture
{
private:
	GLuint m_RendererID;
	std::string m_FilePath;
	GLubyte* m_LocalBuffer;
	int m_Width;
	int m_Height;
	int m_BPP;

public:
	Texture() = delete;
	Texture(const std::string& path);
	~Texture();

	void Bind(GLuint slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return this->m_Width; }
	inline int GetHeight() const { return this->m_Height; }
};

#endif