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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
		-50.f,	50.f, 0.0f, 0.0f,	// 0
		 50.f, -50.f, 1.0f, 0.0f,	// 1
		 50.f,  50.f, 1.0f, 1.0f,	// 2
		-50.f,	50.f, 0.0f, 1.0f	// 3
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
	glm::mat4 viewMatrix		= glm::translate(view, glm::vec3(0.f, 0.f, 0.f));
	glm::vec3 translationA		= glm::vec3(200, 200, 0);
	glm::vec3 translationB		= glm::vec3(400, 300, 0);

	Shader* shader = new Shader("res/shaders/Basic.shader");
	shader->Bind();
	shader->SetUniform4f("u_Colour", 0.8f, 0.3f, 0.8f, 1.f);

	Texture* texture			= new Texture("res/textures/vine.png");
	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	va->Unbind();
	va->Unbind();
	ib->Unbind();
	shader->Unbind();

	Renderer* renderer			= new Renderer();
	renderer->SetClearColour(0, 0, 1);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glm::mat4 modelMatrix;
	glm::mat4 mvp;

	while (!glfwWindowShouldClose(window))
	{
		renderer->Clear();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		shader->Bind();
		
		// Translation A
		modelMatrix = glm::translate(view, translationA);
		mvp = projectionMatrix * viewMatrix * modelMatrix;
		shader->SetUniformMat4f("u_MVP", mvp);
		renderer->Draw(va, ib, shader);

		// Translation B
		modelMatrix = glm::translate(view, translationB);
		mvp = projectionMatrix * viewMatrix * modelMatrix;
		shader->SetUniformMat4f("u_MVP", mvp);
		renderer->Draw(va, ib, shader);

		ImGui::Begin("ImGui");

		ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, WindowWidth);
		ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, WindowWidth);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
