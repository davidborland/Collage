///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        GLSLShader.cpp
//
// Author:      David Borland
//
// Description: Loads a GLSL shader.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "GLSLShader.h"

#include <iostream>
#include <fstream>

   
GLSLShader::GLSLShader() {
}

GLSLShader::~GLSLShader() {
}


bool GLSLShader::LoadVertexShader(const std::string& fileName, GLuint& program) {
    std::vector<std::string> dummy;
    return LoadVertexShader(fileName, program, dummy);
}

bool GLSLShader::LoadVertexShader(const std::string& fileName, GLuint& program,
                                  const std::vector<std::string>& preprocessorDefinitions) {
    glDeleteProgram(program);

    program = glCreateProgram();

    GLuint shader;
    if (!LoadShader(fileName, GL_VERTEX_SHADER, shader, preprocessorDefinitions)) {
        std::cout << "GLSLShader::LoadVertexShader() : Couldn't open " << fileName << std::endl;
        return false;
    }

    glAttachShader(program, shader);
    glLinkProgram(program);

    return true;
}


bool GLSLShader::LoadFragmentShader(const std::string& fileName, GLuint& program) {
    std::vector<std::string> dummy;
    return LoadFragmentShader(fileName, program, dummy);
}

bool GLSLShader::LoadFragmentShader(const std::string& fileName, GLuint& program,
                                    const std::vector<std::string>& preprocessorDefinitions) {
    glDeleteProgram(program);

    program = glCreateProgram();

    GLuint shader;
    if (!LoadShader(fileName, GL_FRAGMENT_SHADER, shader, preprocessorDefinitions)) {
        std::cout << "GLSLShader::LoadFragmentShader() : Couldn't open " << fileName << std::endl;
        return false;
    }

    glAttachShader(program, shader);
    glLinkProgram(program);

    return true;
}


bool GLSLShader::LoadShaders(const std::string& vertexFileName, const std::string& fragmentFileName, 
                             GLuint& program) {
    std::vector<std::string> dummy1, dummy2;
    return LoadShaders(vertexFileName, fragmentFileName, program, dummy1, dummy2);
}

bool GLSLShader::LoadShaders(const std::string& vertexFileName, const std::string& fragmentFileName, 
                             GLuint& program,
                             const std::vector<std::string>& vertexPreprocessorDefinitions,
                             const std::vector<std::string>& fragmentPreprocessorDefinitions) {
    glDeleteProgram(program);

    program = glCreateProgram();

    // Vertex shader
    GLuint vertexShader;
    if (!LoadShader(vertexFileName, GL_VERTEX_SHADER, vertexShader, vertexPreprocessorDefinitions)) {
        std::cout << "GLSLShader::LoadShaders() : Couldn't open " << vertexFileName << std::endl;
        return false;
    }

    // Fragment shader
    GLuint fragmentShader;
    if (!LoadShader(fragmentFileName, GL_FRAGMENT_SHADER, fragmentShader, fragmentPreprocessorDefinitions)) {
        std::cout << "GLSLShader::LoadShaders() : Couldn't open " << fragmentFileName << std::endl;
        return false;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return true;
}


bool GLSLShader::LoadShader(const std::string& fileName, GLenum shaderType, GLuint& shader, 
                            const std::vector<std::string>& preprocessorDefinitions) {
    // Open the shader file
    std::ifstream shaderFile(fileName.c_str());
    if (shaderFile.fail()) {
        std::cout << "GLSLShader::LoadShader() : Couldn't open shader." << std::endl;
        return false;
    }


    // Prepend preprocessor definitions
    std::string shaderString;
    for (int i = 0; i < (int)preprocessorDefinitions.size(); i++) {
        shaderString += "#define " + preprocessorDefinitions[i] + "\n";
    }


    // Load the file
    while (!shaderFile.eof()) {
        std::string s;
        getline(shaderFile, s);
        shaderString += s + "\n";
    }
    shaderFile.close();


    // Create the shader
    glDeleteShader(shader);
    shader = glCreateShader(shaderType);
    GLchar* shaderStringGL = new GLchar[shaderString.length() + 1];
    strcpy(shaderStringGL, shaderString.c_str());
    glShaderSource(shader, 1, (const GLchar**)&shaderStringGL, NULL);
    glCompileShader(shader);


    // Test compilation
    int compiled, logLength, dummy = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH  , &logLength);
    GLchar* logString = new GLchar[logLength];
    glGetShaderInfoLog(shader, logLength, &dummy, logString);

    if (!compiled) {
        std::cout << "GLSLShader::LoadShader() : Couldn't compile shader." << std::endl << std::endl;
        std::cout << logString << std::endl;
        return false;
    }

    return true;
}


