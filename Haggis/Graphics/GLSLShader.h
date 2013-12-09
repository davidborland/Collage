///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        GLSLShader.h
//
// Author:      David Borland
//
// Description: Loads a GLSL shader.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef GLSLSHADER_H
#define GLSLSHADER_H


#include <GL/glew.h>

#include <string>
#include <vector>


class GLSLShader {
public:    
    GLSLShader();
    ~GLSLShader();

    // Use for only a vertex shader
    static bool LoadVertexShader(const std::string& fileName, GLuint& program);
    static bool LoadVertexShader(const std::string& fileName, GLuint& program,
                                 const std::vector<std::string>& preprocessorDefinitions);

    // Use for only a fragment shader
    static bool LoadFragmentShader(const std::string& fileName, GLuint& program);
    static bool LoadFragmentShader(const std::string& fileName, GLuint& program,
                                   const std::vector<std::string>& preprocessorDefinitions);

    // Use for vertex and fragment shaders
    static bool LoadShaders(const std::string& vertexFileName, const std::string& fragmentFileName,
                            GLuint& program);
    static bool LoadShaders(const std::string& vertexFileName, const std::string& fragmentFileName,
                            GLuint& program, 
                            const std::vector<std::string>& vertexPreprocessorDefinitions,
                            const std::vector<std::string>& fragmentPreprocessorDefinitions);

private:
    static bool LoadShader(const std::string& fileName, GLenum shaderType, GLuint& shader,
                           const std::vector<std::string>& preprocessorDefinitions);
};


#endif