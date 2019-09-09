#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
		-0.5f, -0.5f, 0.0f, 0.0f, // 0
		 0.5f, -0.5f, 1.0f, 0.0f, // 1
		 0.5f,  0.5f, 1.0f, 1.0f, // 2
		-0.5f,  0.5f, 0.0f, 1.0f  // 3
	};

	// Index buffer object specifying which vertex positions
	// to draw without duplication
	GLuint indicies[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	VertexArray* va				= new VertexArray();
	VertexBuffer* vb			= new VertexBuffer(positions, 4 * 4 * sizeof(float));	
	VertexBufferLayout* layout	= new VertexBufferLayout();

	layout->Push<float>(2);
	layout->Push<float>(2);
	va->AddBuffer(*vb, *layout);

	IndexBuffer* ib				= new IndexBuffer(indicies, 6);
	Shader* shader				= new Shader("res/shaders/Basic.shader");
	shader->Bind();
	shader->SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.f);

	Texture* texture			= new Texture("res/textures/vine.png");
	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	va->Unbind();
	va->Unbind();
	ib->Unbind();
	shader->Unbind();

	Renderer renderer;

	float r	 = 0.05f, g  = 0.05f, b = 0.05f;
	float ri = 0.05f, gi = 0.1f, bi = 0.25f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();

		shader->Bind();
		shader->SetUniform4f("u_Colour", r, g, b, 1.f);

		renderer.Draw(va, ib, shader);

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
	delete texture;

	glfwTerminate();
	return 0;
}
