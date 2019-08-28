#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <GL/glew.h>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void Bind()		const;
	void Unbind()	const;
private:
	GLuint m_RendererID;
};

#endif