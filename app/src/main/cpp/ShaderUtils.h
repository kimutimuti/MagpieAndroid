#pragma once

#include &lt;string&gt;
#include &lt;GLES3/gl3.h&gt;

namespace ShaderUtils {
    GLuint createShader(GLenum type, const std::string&amp; source);
    GLuint createProgram(const std::string&amp; vertexSource, const std::string&amp; fragmentSource);
    std::string loadShaderFromAssets(const char* name);
}
