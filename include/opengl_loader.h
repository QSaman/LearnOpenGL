#ifndef OPENGL_LOADER_H
#define #ifndef OPENGL_LOADER_H

#ifdef USE_GLBINDING

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

#define GLFW_INCLUDE_NONE 

#endif

#ifdef USE_GLAD

#include <glad/glad.h>

#endif

bool loadOpenGL()
{
#ifdef USE_GLBINDING
    
    glbinding::Binding::initialize();
    return true;
    
#endif

#ifdef USE_GLAD
    
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
#endif
}

#endif
