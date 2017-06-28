#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

const bool enableWireframeMode = false;

const char* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}";

const char* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

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

bool loadOpenGL()
{
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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

GLuint createVertexShader()
{
	auto shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &vertexShaderSrc, NULL);
	return shader;
}

GLuint createFragmentShader()
{
	auto shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &fragmentShaderSrc, NULL);
	return shader;
}

GLint compileShader(GLuint shader, const char* shaderName)
{
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar message[512];
		glGetShaderInfoLog(shader, 512, NULL, message);
		std::cerr << "Shader compilation error (" << shaderName << "): " << message << std::endl;
	}
	return success;
}

bool setupShaders(GLuint& shaderProgram)
{
	auto vertexShader = createVertexShader();
	if (!compileShader(vertexShader, "vertex shader"))
		return false;
	auto fragmentShader = createFragmentShader();
	if (!compileShader(fragmentShader, "fragment shader"))
		return false;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar message[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, message);
		std::cerr << "Shader link error: " << message << std::endl;
		return false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}

//In C when we pass an array to a function, sizeof(vertices) is equal to the size of a pointer.
void setupVAO(GLuint vao, GLuint vbo, GLfloat vertices[][3], int size)
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)NULL);
	glEnableVertexAttribArray(0);

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

void renderFrame(GLuint shaderProgram, GLuint vao)
{
	glUseProgram(shaderProgram);
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

	GLuint shaderProgram;
	if (!setupShaders(shaderProgram))
		return 1;

	GLuint vao[2], vbo[2];
	GLfloat vertices[2][3][3] =
	{
		{
			{ -1.0f, -0.5f, 0.0f },	//left
			{ 0.0f, -0.5f, 0.0f },	//right
			{ -0.5f, 0.5f, 0.0f }		//top
		},
		{
			{ 0.0f, -0.5f, 0.0f },	//left
			{ 1.0f, -0.5f, 0.0f },	//right
			{ 0.5f, 0.5f, 0.0f }		//top
		}
	};

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);
	for (int i = 0; i < 2; ++i)
		setupVAO(vao[i], vbo[i], vertices[i], sizeof(vertices[i]));

	if (enableWireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		for (int i = 0; i < 2; ++i)
			renderFrame(shaderProgram, vao[i]);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
    glfwTerminate();
}
