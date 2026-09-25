#include "Renderer.h"
#include "ShaderUtils.h"
#include "effects/Effects.h"

const char* vertexShaderSource = R"(
    #version 300 es
    in vec2 aPosition;
    in vec2 aTexCoord;
    out vec2 vTexCoord;
    
    void main() {
        gl_Position = vec4(aPosition, 0.0, 1.0);
        vTexCoord = aTexCoord;
    }
)";

const char* passThroughFragmentShaderSource = R"(
    #version 300 es
    precision mediump float;
    in vec2 vTexCoord;
    out vec4 fragColor;
    uniform sampler2D uTexture;
    
    void main() {
        fragColor = texture(uTexture, vTexCoord);
    }
)";

Renderer::Renderer()
    : m_width(0), m_height(0), m_outputWidth(0), m_outputHeight(0),
      m_scale(2.0f), m_effectType(0), m_program(0), m_passThroughProgram(0),
      m_vbo(0), m_vao(0), m_fbo(0), m_outputTexture(0) {
}

Renderer::~Renderer() {
    destroy();
}

bool Renderer::initialize(int width, int height) {
    m_width = width;
    m_height = height;
    m_outputWidth = static_cast<int>(width * m_scale);
    m_outputHeight = static_cast<int>(height * m_scale);
    
    std::string fragmentShader;
    switch (m_effectType) {
        case 0:
            fragmentShader = Effects::getAnime4KShader();
            break;
        case 1:
            fragmentShader = Effects::getFSRShader();
            break;
        case 2:
            fragmentShader = Effects::getBicubicShader();
            break;
        case 3:
            fragmentShader = Effects::getLanczosShader();
            break;
        default:
            fragmentShader = Effects::getAnime4KShader();
            break;
    }
    
    m_program = ShaderUtils::createProgram(vertexShaderSource, fragmentShader);
    if (!m_program) {
        LOGE("Failed to create shader program");
        return false;
    }
    
    m_passThroughProgram = ShaderUtils::createProgram(vertexShaderSource, passThroughFragmentShaderSource);
    if (!m_passThroughProgram) {
        LOGE("Failed to create pass-through shader program");
        return false;
    }
    
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLint posLoc = glGetAttribLocation(m_program, "aPosition");
    GLint texLoc = glGetAttribLocation(m_program, "aTexCoord");
    
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(texLoc);
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    glBindVertexArray(0);
    
    if (!createFramebuffer()) {
        LOGE("Failed to create framebuffer");
        return false;
    }
    
    LOGI("Renderer initialized: %dx%d -> %dx%d", width, height, m_outputWidth, m_outputHeight);
    return true;
}

void Renderer::destroy() {
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
    if (m_passThroughProgram) {
        glDeleteProgram(m_passThroughProgram);
        m_passThroughProgram = 0;
    }
    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_fbo) {
        glDeleteFramebuffers(1, &m_fbo);
        m_fbo = 0;
    }
    if (m_outputTexture) {
        glDeleteTextures(1, &m_outputTexture);
        m_outputTexture = 0;
    }
}

bool Renderer::createFramebuffer() {
    glGenFramebuffers(1, &m_fbo);
    glGenTextures(1, &m_outputTexture);
    
    glBindTexture(GL_TEXTURE_2D, m_outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_outputWidth, m_outputHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_outputTexture, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Framebuffer not complete");
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void Renderer::render(GLuint inputTexture) {
    if (!m_program || !m_passThroughProgram) return;
    
    // --- 1. FBO (m_outputTexture) への描画（拡大・超解像処理） ---
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_outputWidth, m_outputHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 背景を透明黒でクリア
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(m_program);
    
    GLint texLoc = glGetUniformLocation(m_program, "uTexture");
    GLint resLoc = glGetUniformLocation(m_program, "uResolution");
    GLint scaleLoc = glGetUniformLocation(m_program, "uScale");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    glUniform1i(texLoc, 0);
    
    if (resLoc != -1) {
        glUniform2f(resLoc, static_cast<float>(m_width), static_cast<float>(m_height));
    }
    if (scaleLoc != -1) {
        glUniform1f(scaleLoc, m_scale);
    }
    
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // --- 2. 実際の画面への描画（全画面表示） ---
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 実際の画面も透明黒でクリア
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(m_passThroughProgram);
    
    GLint passTexLoc = glGetUniformLocation(m_passThroughProgram, "uTexture");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_outputTexture);
    glUniform1i(passTexLoc, 0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::setEffect(int effectType) {
    if (m_effectType == effectType) return;
    
    m_effectType = effectType;
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
    
    std::string fragmentShader;
    switch (m_effectType) {
        case 0:
            fragmentShader = Effects::getAnime4KShader();
            break;
        case 1:
            fragmentShader = Effects::getFSRShader();
            break;
        case 2:
            fragmentShader = Effects::getBicubicShader();
            break;
        case 3:
            fragmentShader = Effects::getLanczosShader();
            break;
        default:
            fragmentShader = Effects::getAnime4KShader();
            break;
    }
    
    m_program = ShaderUtils::createProgram(vertexShaderSource, fragmentShader);
}

void Renderer::setScaleFactor(float scale) {
    m_scale = scale;
    m_outputWidth = static_cast<int>(m_width * m_scale);
    m_outputHeight = static_cast<int>(m_height * m_scale);
    
    if (m_outputTexture) {
        glBindTexture(GL_TEXTURE_2D, m_outputTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_outputWidth, m_outputHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }
}

GLuint Renderer::createTexture(int width, int height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture;
}

void Renderer::updateTexture(GLuint texture, void* data, int width, int height, int stride) {
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // 行のパディング(Stride)をOpenGLに教えることで画像崩れを防ぐ
    glPixelStorei(GL_UNPACK_ROW_LENGTH, stride);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // 他のテクスチャ操作に影響を与えないよう元に戻す
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
}

void Renderer::renderQuad() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
