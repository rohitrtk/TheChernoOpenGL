#ifndef VERTEXARRAY_H_
#define VERTEXARRAY_H_

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	GLuint m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};

#endif
