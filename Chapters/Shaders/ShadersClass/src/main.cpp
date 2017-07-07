#include <opengl_loader.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <shader_loader.h>

#define WIDTH 800
#define HEIGHT 600

const bool enableWireframeMode = false;

GLFWwindow* initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Fixing compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello OpenGL", NULL, NULL);
    if (window != NULL)
        glfwMakeContextCurrent(window);
    return window;
}

//Callbacks:

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void setGLFWcallbacks(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void setupVAO(GLuint& vao, GLuint& vbo)
{
    struct Vertex
    {
        GLfloat position[3];
        GLfloat color[3];
    };


    Vertex vertices[3] =
	{
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},     //left
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},      //right
        {{0.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}		//top
	};

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered 
	//VBO as the vertex attribute's bound vertex buffer object so afterwards 
	//we can safely unbind:
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally 
	//modify this VAO, but this rarely happens. Modifying other VAOs requires 
	//a call to glBindVertexArray anyways so we generally don't unbind VAOs 
	//(nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void renderFrame(ShaderLoader& shader, GLuint vao)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
    auto window = initGLFW();
	if (window == NULL)
	{
        std::cerr << "Cannot create a GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

    setGLFWcallbacks(window);

    if (!loadOpenGL())
    {
        std::cout << "Failed to load OpenGL" << std::endl;
        glfwTerminate();
        return 1;
    }

    ShaderLoader shader("shaders/shader.vs", "shaders/shader.fs");
    try
    {
        shader.linkShaders();
    }
    catch (std::string str)
    {
        std::cerr << str << std::endl;
        return 1;
    }

	GLuint vao, vbo;
	setupVAO(vao, vbo);

	if (enableWireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        renderFrame(shader, vao);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
    glfwTerminate();
}
