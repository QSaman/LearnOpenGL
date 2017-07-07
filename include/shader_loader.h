#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <opengl_loader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include <exception>

class ShaderLoader
{
public:
    explicit ShaderLoader(const char* vertexPath, const char* fragmentPath) :
        vertexPath{vertexPath}, fragmentPath{fragmentPath}
    {
    }

    void linkShaders()
    {
        std::string vertexCode = loadShader(vertexPath);
        std::string fragmentCode = loadShader(fragmentPath);

        auto vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexCodeC = vertexCode.c_str();
        glShaderSource(vertexShaderId, 1, &vertexCodeC, NULL);
        if (!compileShader(vertexShaderId))
        {
            std::string message = getCompileErrorMessage(vertexShaderId);
            throw("Vertex Shader Compilation Error: " + message);
        }

        auto fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentCodeC = fragmentCode.c_str();
        glShaderSource(fragmentShaderId, 1, &fragmentCodeC, NULL);
        if (!compileShader(fragmentShaderId))
        {
            std::string message = getCompileErrorMessage(fragmentShaderId);
            throw("Fragment Shader Compilation Error: " + message);
        }

        programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);
        if (!linkShadersHelper())
            throw ("Linking shaders error: " + getLinkErrorMessage());
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    void use()
    {
        glUseProgram(programId);
    }
    
    GLuint getProgramId()
    {
        return programId;
    }

private:
    std::string loadShader(const char* shaderPath)
    {
        std::ifstream shaderFile;

        shaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        shaderFile.open(shaderPath);

        std::ostringstream os;
        os << shaderFile.rdbuf();
        return os.str();
    }

    GLint compileShader(GLuint shader)
    {
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        return success;
    }

    std::string getCompileErrorMessage(GLuint shader)
    {
        GLint size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        if (size == 0)
            return "";
        std::unique_ptr<GLchar[]> message(new GLchar[size]);
        glGetShaderInfoLog(shader, size, NULL, message.get());
        return std::string(message.get());
    }

    GLint linkShadersHelper()
    {
        glLinkProgram(programId);
        GLint success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        return success;
    }

    std::string getLinkErrorMessage()
    {
        GLint size;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &size);
        if (size == 0)
            return "";
        std::unique_ptr<GLchar[]> message(new GLchar[size]);
        glGetProgramInfoLog(programId, size, NULL, message.get());
        return std::string(message.get());
    }

private:
    GLuint programId;
    const char* vertexPath, * fragmentPath;
};

#endif
