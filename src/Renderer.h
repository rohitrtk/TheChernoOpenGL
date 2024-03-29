#ifndef RENDERER_H_
#define RENDERER_H_

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
	void Clear()																	const;
	void SetClearColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f)			const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)	const;
	void Draw(VertexArray* va, IndexBuffer* ib, Shader* shader)						const;
};

#endif 