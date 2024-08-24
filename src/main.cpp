// main.cpp
// Ethan Rudy


// Graphics Header(s)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Header(s)
#include <iostream>

// Custom cOpenGL header(s)
#include "../include/gl/shader.h"
#include "../include/gl/texture.h"

// Ray Tracing header(s)
#include "../include/rtw/ray_tracer.h"

/**
* Directory Notes:
* 
*	The include and source are both seperated into a'gl' and 'rtw' folder. 
*	They contain the above Custom OpenGL and
*	Ray Tracing headers respectively. "RTW" meaning "Ray Tracing in One Weekend"
*	as I followed those books.
* 
*	You might also notice that there are TWO texture headers
*	One is for the OpenGL stuff, and the other for the Raytracing
*	Keeps those two nice and isolated :P
* 
*	I also have a mix of (.h, .cpp) pairs and (.hpp) standalones
*	For a couple headers that I wanted seperate, but didn't seem
*	hearty enough to warant an entire second file, I left as an
*	.hpp
*/

/**
* Program Flow:
* 
*	Frankly this is one of the largest cohesive projects I've worked on lately
*	And it can be a little indimidating to tweak and refactor because its just
*	me sitting alone at a desk on an unhealth mg count of caffiene :D
* 
*	When the program is run, the following happends
* 
*	Main Execution thread:
*		Initializes the glad, glfw, the window, textures, etc.
*		It manages ALL opengl render calls, not any raytracing
*		It calls program thread 2, which I call this second
*		thread the render manager thread
* 
*	Render Manager thread:
*		This thread is immediately detached from the main "parent"
*		thread. Meaning these two begin running independently. Shared
*		memory, shared space, but different processes. This new 
*		thread's main job is to MANAGE the render threads. It exists
*		to call the threads actually doing the rendering/heavy lifting.
*		We'll call these sub-render threads span threads, as each one
*		renders a span of the resulting image.
* 
*	Span Thread:
*		Manages (1 / N_THREADS) of the image, each threads pixels are
*		randomly assigned to try and mitigate one thread ending drastically
*		before its neighbor. One other way of resolving this issue would
*		be by using a threadpool, which I originally attempted. Results
*		were suboptimal (non functional lol).
* 
* 
*	Once the span threads are complete, the main exec thread will stop
*	refreshing and writing, leaving the window open until forcefully
*	closed. Ex: Exited by 'x' button or escape key pressed
*/



// Render Width and Height
const unsigned WIDTH = 1920 / 2, HEIGHT = 1080 / 2;

/**
* Window Resize Callback
* Resizes the viewport scale to match the new window size
* 
* @param window	Window object
* @param width	New width
* @param height	New height
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
* Input Processor
* 
* @param window Window object
*/
void processInput(GLFWwindow* window);


int main() {
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "RayTracer", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Window settings
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Init glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	
	// Shader program
	gl::Shader shaderProgram("./shaders/shader.vert", "./shaders/shader.frag");
	// Texture
	gl::Texture texture("./textures/output.jpg");


	// This program displays our raytraced result as a texture on a rectangle (two triangles)
	// This is the respective code for those vertices
	// Triangle Vertices
	float vertices[] = {
		// Positions		// Texture Coords
		 1.0f,  1.0f, 0.0f,	1.0f, 1.0f,		// Top Right
		 1.0f, -1.0f, 0.0f,	1.0f, 0.0f,		// Bottom Right
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,		// Bottom Left
		-1.0f,  1.0f, 0.0f,	0.0f, 1.0f,		// Top Left
	};
	unsigned indices[] = {
		0, 1, 3,	// First triangle (top right)
		1, 2, 3		// Second Triangle (bottom left)
	};

	// OpenGL Buffers
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Wireframe mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// Ray Tracer
	rtw::RayTracer ray_tracer(WIDTH, HEIGHT);
	ray_tracer.write();

	// Begin rendering & detach manager thread
	std::thread render_thread(&rtw::RayTracer::render, &ray_tracer);
	render_thread.detach();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind texture
		texture.bind();

		// Use Shader
		shaderProgram.use();

		// Bind VAO and draw Elements
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		

		// If the raytracer, isn't done, write the in-progress
		// image and reload the texture
		// Lastly output the progress percentage
		if (!ray_tracer.done()) {

			// "Realtime" reload
			// Using a PBO would probably be better
			// But I think it would create more 
			// work with memory management, and
			// its a little out of scope right now
			// UPDATE a couple days later: skill out of scope :P
			ray_tracer.write();
			
			texture.reload();

			ray_tracer.get_progress();
		}
		
	}

	// Free up buffer memory
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}


// Window Resize Callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Input Processor
void processInput(GLFWwindow* window) {
	// Close the window if escape is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}