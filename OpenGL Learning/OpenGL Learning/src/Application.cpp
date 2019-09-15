#include <glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "shader.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	//This sets the OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Glew Error!" << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	//Scope to fix the application not terminating
	{ 
		float Positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};
		unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

		//This code generates and binds a vertex array object
    	VertexArray va;
    	
		VertexBuffer vb(Positions, 4 * 2 * sizeof(float));
    	VertexBufferLayout layout;
    	layout.Push<float>(2);
    	va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
    	shader.Bind();
    	shader.SetUniform4f("u_Colour", 0.7f, 0.3f, 0.5f, 1.0f);

		//These unbinds the buffers
		va.UnBind();
    	shader.UnBind();
		vb.UnBind();
		ib.UnBind();
		
		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
		    /* Render here */
		    GLCall(glClear(GL_COLOR_BUFFER_BIT));

			shader.Bind();
			shader.SetUniform4f("u_Colour", r, 0.3f, 0.5f, 1.0f);
			
			va.Bind();
			ib.Bind();
			
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));	

			if (r > 1.0f)
				increment = -0.01f;
			else if (r < 0.0f)
				increment = 0.01f;

			r+=increment;
			
		    /* Swap front and back buffers */
		    GLCall(glfwSwapBuffers(window));

		    /* Poll for and process events */
		    GLCall(glfwPollEvents());
			
		}
	}
	
    glfwTerminate();
	return 0;
}