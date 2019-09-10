#include "Renderer.h"

#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetClearColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a) const
{
	glClearColor(r, g, b, a);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const 
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(VertexArray* va, IndexBuffer* ib, Shader* shader) const
{
	shader->Bind();
	va->Bind();
	ib->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}
