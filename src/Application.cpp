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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

constexpr auto WindowTitle = "OpenGL Tutorials";
constexpr auto WindowWidth  = 960.f;
constexpr auto WindowHeight = 540.f;

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
	window = glfwCreateWindow(WindowWidth, WindowHeight, "Hello World", NULL, NULL);
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
		std::cout << "Error initializing glew!" << std::endl;
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << "\n";

	// Vertex positions
	// Vertex coordinates & Texture coordinates
	float positions[] =
	{
		100.f, 100.f, 0.0f, 0.0f, // 0
		200.f, 100.f, 1.0f, 0.0f, // 1
		200.f, 200.f, 1.0f, 1.0f, // 2
		100.f, 200.f, 0.0f, 1.0f  // 3
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

	constexpr auto view = glm::mat4(1.f);

	glm::mat4 projectionMatrix	= glm::ortho(0.f, WindowWidth, 0.f, WindowHeight);
	glm::mat4 viewMatrix		= glm::translate(view, glm::vec3(-100.f, 0.f, 0.f));
	glm::mat4 modelMatrix		= glm::translate(view, glm::vec3(200, 200, 0));

	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

	Shader* shader				= new Shader("res/shaders/Basic.shader");
	shader->Bind();
	shader->SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.f);
	shader->SetUniformMat4f("u_MVP", mvp);

	Texture* texture			= new Texture("res/textures/vine.png");
	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	va->Unbind();
	va->Unbind();
	ib->Unbind();
	shader->Unbind();

	Renderer* renderer			= new Renderer();
	renderer->SetClearColour(0, 0, 1);

	while (!glfwWindowShouldClose(window))
	{
		renderer->Clear();
		
		renderer->Draw(va, ib, shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete va;
	delete vb;
	delete ib;
	delete layout;
	delete shader;
	delete texture;
	delete renderer;

	glfwTerminate();
	return 0;
}
