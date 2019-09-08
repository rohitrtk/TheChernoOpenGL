#ifndef VERTEXBUFFERLAYOUT_H_
#define VERTEXBUFFERLAYOUT_H_

#include <vector>

#include "GLFW/glfw3.h"

#include "Renderer.h"

struct VertexBufferElement
{
	GLuint type;
	GLuint count;
	GLubyte normalized;

	static GLuint GetSizeOfType(GLuint type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:
			return sizeof(GLubyte);
		}

		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	GLuint m_Stride;

public:
	VertexBufferLayout() : m_Stride(0) {}

	template<typename T>
	void Push(GLuint count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(GLuint count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<GLuint>(GLuint count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<GLubyte>(GLuint count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return this->m_Elements; }
	inline GLuint GetStride() const { return this->m_Stride; }
};

#endif