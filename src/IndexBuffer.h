#ifndef INDEXBUFFER_H_
#define INDEXBUFFER_H_

#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const GLuint* data, GLuint count);
	~IndexBuffer();

	void Bind()		const;
	void Unbind()	const;

	inline const GLuint& GetCount() const { return this->m_Count; }
private:
	GLuint m_RendererID;
	GLuint m_Count;
};

#endif