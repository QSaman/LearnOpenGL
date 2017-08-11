#include <opengl_loader.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define WIDTH 800
#define HEIGHT 600
#define ENABLE_DEPTH_TEST

static glm::mat4 view, projection;

static float deltaTime = 0.0f, lastTime = 0.0f;
static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

struct MyImage
{
    const char* path;
    GLenum pixelFormat;
};

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
	float deltaMove = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraFront * deltaMove;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraFront * deltaMove;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaMove;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaMove;
}

void setupVAO(GLuint& vao, GLuint& vbo, GLuint& ebo)
{
    struct Vertex
    {
        GLfloat position[3];
        GLfloat color[3];
        GLfloat texture[2];
    };


    //There are some duplications for vertices. Since each face of cube has it's own texture,
    //we need to duplicate some vertices
    //
    //If you want to re-use vertices (as you did by specifying only 8 vertices) you need to
    //make sure each vertex uniquely describes the vertex data for that point, this includes
    //all vertex attributes of a vertex and not just positions. Because the texture coordinates
    //are different for each face (even though they share some of the same positions as other faces)
    //they do not share the same texture coordinates and these points can thus not be shared by
    //multiple triangles which is what you're trying to do.
    //
    //In the case of our cube, most vertices are unique on its own (they have a set of point
    //and texture coordinate data that is unique to that vertex) so for those it is not
    //possible to share vertices among the cube's triangles. The cube hosts a total of
    //36 vertices and some of them can indeed be shared (shared edge of the two triangles per face)
    //as they have the same vertex attributes (24 are unique so 12 vertices can be shared).
    //
    //Note that a cube is a bit of a worst-case example as most complex models usually do share quite
    //a few vertices among triangles.
    Vertex vertices[6][4] =
	{
        //Front face
        {
            {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},     //top right
            {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},    //bottom right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   //bottom left
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}     //top left
        },
        //back face
        {
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},     //top right
            {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},    //bottom right
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   //bottom left
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}     //top left
        },
        //right face
        {
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},     //top right
            {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},    //bottom right
            {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   //bottom left
            {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}     //top left
        },
        //Left face
        {
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},     //top right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},    //bottom right
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   //bottom left
            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}     //top left
        },
        //Top face
        {
            {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},     //top right
            {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},    //bottom right
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   //bottom left
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}     //top left
        },
        //Bottom face
        {
            {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},     //top right
            {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},    //bottom right
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   //bottom left
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}     //top left
        }
	};    

    //Don't forget to tell glDrawElements we have 36 vertices
    //The indices are clockwise. For each face we have:
    //(top right, bottom right, top left), (bottom right, bottom left, top left)
    GLuint indices[6][2][3] =
    {
        //Front face
        {
            {0, 1, 3},
            {1, 2, 3}
        },
        //Back face
        {
            {4, 5, 7},
            {5, 6, 7}
        },
        //Right face
        {
            {8, 9, 11},
            {9, 10, 11}
        },
        //Left face
        {
            {12, 13, 15},
            {13, 14, 15}
        },
        //Top face
        {
            {16, 17, 19},
            {17, 18, 19}
        },
        //Bottom face
        {
            {20, 21, 23},
            {21, 22, 23}
        }
    };

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

    glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered 
	//VBO as the vertex attribute's bound vertex buffer object so afterwards 
	//we can safely unbind:
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally 
	//modify this VAO, but this rarely happens. Modifying other VAOs requires 
	//a call to glBindVertexArray anyways so we generally don't unbind VAOs 
	//(nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool setupTexture(GLuint* texture, int texNum, MyImage* image)
{
    for (int i = 0; i < texNum; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, channelNumber;
        // tell stb_image.h to flip loaded texture's on the y-axis.
        stbi_set_flip_vertically_on_load(true);
        auto data = stbi_load(image[i].path, &width, &height, &channelNumber, 0);
        if (data == NULL)
        {
            std::cerr << "Cannot load " << image[i].path << std::endl;
            return false;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, image[i].pixelFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    return true;
}

void renderFrame(ShaderLoader& shader, GLuint vao, GLuint* texture, int texNum)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
#ifdef ENABLE_DEPTH_TEST
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
	glClear(GL_COLOR_BUFFER_BIT);
#endif // ENABLE_DEPTH_TEST    
    shader.use();
    for (int i = 0; i < texNum; ++i)
    {
        // bind textures on corresponding texture units
        glActiveTexture(static_cast<GLenum>(static_cast<int>(GL_TEXTURE0) + i));
        glBindTexture(GL_TEXTURE_2D, texture[i]);
    }

	glBindVertexArray(vao);
	view = glm::mat4();
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(glGetUniformLocation(shader.getProgramId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	for (int i = 0; i < 10; ++i)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgramId(), "model"), 1, GL_FALSE, glm::value_ptr(model));		
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	}
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

    GLuint texture[2];
    //Note that the png file has alpha channel!
    MyImage image[2] =
    {
      {"shaders/container.jpg", GL_RGB},
      {"shaders/awesomeface.png", GL_RGBA}
    };
    glGenTextures(2, texture);
    if (!setupTexture(texture, 2, image))
        return false;

    GLuint vao, vbo, ebo;
    setupVAO(vao, vbo, ebo);

	if (enableWireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.use();
    //tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    //We can use glUniform1i after shader.use
    glUniform1i(glGetUniformLocation(shader.getProgramId(), "ourTexture1"), 0);
    glUniform1i(glGetUniformLocation(shader.getProgramId(), "ourTexture2"), 1);

    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 100.0f);
    //In model matrix we translate objects by (0, 0, -3), so we have:
    //projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -3.5f, 2.5f);
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgramId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
#ifdef ENABLE_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
#endif // ENABLE_DEPTH_TEST

    while (!glfwWindowShouldClose(window))
    {
		auto currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
        processInput(window);
        renderFrame(shader, vao, texture, 2);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(2, texture);
    glfwTerminate();
}
