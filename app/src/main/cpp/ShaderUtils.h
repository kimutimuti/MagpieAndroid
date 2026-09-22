#pragma once

#include <string>
#include <GLES3/gl3.h>

namespace ShaderUtils {
    GLuint createShader(GLenum type, const std::string& source);
    GLuint createProgram(const std::string& vertexSource, const std::string& fragmentSource);
    std::string loadShaderFromAssets(const char* name);
}
