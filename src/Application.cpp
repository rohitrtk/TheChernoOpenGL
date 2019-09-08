#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing glew!";
	}

	std::cout << glGetString(GL_VERSION) << "\n";

	// Vertex positions of square
	float positions[] =
	{
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,  0.5f, // 2
		-0.5f,  0.5f, // 3
	};

	// Index buffer object specifying which vertex positions
	// to draw without duplication
	GLuint indicies[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	VertexArray* va				= new VertexArray();
	VertexBuffer* vb			= new VertexBuffer(positions, 4 * 2 * sizeof(float));	
	VertexBufferLayout* layout	= new VertexBufferLayout();

	layout->Push<float>(2);
	va->AddBuffer(*vb, *layout);

	IndexBuffer* ib				= new IndexBuffer(indicies, 6);
	Shader* shader				= new Shader("res/shaders/Basic.shader");
	shader->Bind();
	shader->SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.f);

	va->Unbind();
	shader->Unbind();
	va->Unbind();
	ib->Unbind();

	float r = 0.05f, g = 0.05f, b = 0.05f;
	float ri = 0.05f, gi = 0.1f, bi = 0.25f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Bind();
		shader->SetUniform4f("u_Colour", r, g, b, 1.f);

		va->Bind();
		ib->Bind();
		
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (r > 1.f || r < 0.f) ri *= -1;
		if (g > 1.f || g < 0.f) gi *= -1;
		if (b > 1.f || b < 0.f) bi *= -1;

		r += ri;
		g += gi;
		b += bi;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	delete va;
	delete vb;
	delete ib;
	delete layout;
	delete shader;

	glfwTerminate();
	return 0;
}
