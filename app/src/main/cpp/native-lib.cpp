#include <jni.h>
#include <string>
#include "Renderer.h"

extern "C" {

static Renderer* g_renderer = nullptr;

JNIEXPORT jlong JNICALL
Java_com_magpie_android_NativeRenderer_createRenderer(JNIEnv* env, jobject thiz) {
    g_renderer = new Renderer();
    return reinterpret_cast<jlong>(g_renderer);
}

JNIEXPORT jboolean JNICALL
Java_com_magpie_android_NativeRenderer_initialize(JNIEnv* env, jobject thiz, jlong rendererPtr, jint width, jint height) {
    Renderer* renderer = reinterpret_cast<Renderer*>(rendererPtr);
    if (!renderer) return JNI_FALSE;
    return renderer->initialize(width, height) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_destroy(JNIEnv* env, jobject thiz, jlong rendererPtr) {
    Renderer* renderer = reinterpret_cast<Renderer*>(rendererPtr);
    if (renderer) {
        renderer->destroy();
        delete renderer;
        g_renderer = nullptr;
    }
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_render(JNIEnv* env, jobject thiz, jlong rendererPtr, jint inputTexture) {
    Renderer* renderer = reinterpret_cast<Renderer*>(rendererPtr);
    if (renderer) {
        renderer->render(static_cast<GLuint>(inputTexture));
    }
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_setEffect(JNIEnv* env, jobject thiz, jlong rendererPtr, jint effectType) {
    Renderer* renderer = reinterpret_cast<Renderer*>(rendererPtr);
    if (renderer) {
        renderer->setEffect(effectType);
    }
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_setScaleFactor(JNIEnv* env, jobject thiz, jlong rendererPtr, jfloat scale) {
    Renderer* renderer = reinterpret_cast<Renderer*>(rendererPtr);
    if (renderer) {
        renderer->setScaleFactor(scale);
    }
}

JNIEXPORT jint JNICALL
Java_com_magpie_android_NativeRenderer_createTexture(JNIEnv* env, jobject thiz, jlong rendererPtr, jint width, jint height) {
    Renderer* renderer = reinterpret_cast<Renderer*>(rendererPtr);
    if (renderer) {
        return static_cast<jint>(renderer->createTexture(width, height));
    }
    return 0;
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_updateTexture(JNIEnv* env, jobject thiz, jlong rendererPtr, jint texture, jobject buffer, jint width, jint height) {
    Renderer* renderer = reinterpret_cast<Renderer*>(rendererPtr);
    if (renderer) {
        void* data = env->GetDirectBufferAddress(buffer);
        renderer->updateTexture(static_cast<GLuint>(texture), data, width, height);
    }
}

}
