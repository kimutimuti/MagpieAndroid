#pragma once

#include <android/log.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <string>

#define LOG_TAG "MagpieNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool initialize(int width, int height);
    void destroy();
    void render(GLuint inputTexture);
    void setEffect(int effectType);
    void setScaleFactor(float scale);
    
    GLuint createTexture(int width, int height);
    void updateTexture(GLuint texture, void* data, int width, int height, int stride); // stride追加
    
private:
    bool createFramebuffer();
    void renderQuad();
    
    int m_width;
    int m_height;
    int m_outputWidth;
    int m_outputHeight;
    float m_scale;
    int m_effectType;
    
    GLuint m_program;
    GLuint m_passThroughProgram;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_fbo;
    GLuint m_outputTexture;
};
