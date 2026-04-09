#include &lt;jni.h&gt;
#include &lt;string&gt;
#include "Renderer.h"

extern "C" {

static Renderer* g_renderer = nullptr;

JNIEXPORT jlong JNICALL
Java_com_magpie_android_NativeRenderer_createRenderer(JNIEnv* env, jobject thiz) {
    g_renderer = new Renderer();
    return reinterpret_cast&lt;jlong&gt;(g_renderer);
}

JNIEXPORT jboolean JNICALL
Java_com_magpie_android_NativeRenderer_initialize(JNIEnv* env, jobject thiz, jlong rendererPtr, jint width, jint height) {
    Renderer* renderer = reinterpret_cast&lt;Renderer*&gt;(rendererPtr);
    if (!renderer) return JNI_FALSE;
    return renderer-&gt;initialize(width, height) ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_destroy(JNIEnv* env, jobject thiz, jlong rendererPtr) {
    Renderer* renderer = reinterpret_cast&lt;Renderer*&gt;(rendererPtr);
    if (renderer) {
        renderer-&gt;destroy();
        delete renderer;
        g_renderer = nullptr;
    }
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_render(JNIEnv* env, jobject thiz, jlong rendererPtr, jint inputTexture) {
    Renderer* renderer = reinterpret_cast&lt;Renderer*&gt;(rendererPtr);
    if (renderer) {
        renderer-&gt;render(static_cast&lt;GLuint&gt;(inputTexture));
    }
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_setEffect(JNIEnv* env, jobject thiz, jlong rendererPtr, jint effectType) {
    Renderer* renderer = reinterpret_cast&lt;Renderer*&gt;(rendererPtr);
    if (renderer) {
        renderer-&gt;setEffect(effectType);
    }
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_setScaleFactor(JNIEnv* env, jobject thiz, jlong rendererPtr, jfloat scale) {
    Renderer* renderer = reinterpret_cast&lt;Renderer*&gt;(rendererPtr);
    if (renderer) {
        renderer-&gt;setScaleFactor(scale);
    }
}

JNIEXPORT jint JNICALL
Java_com_magpie_android_NativeRenderer_createTexture(JNIEnv* env, jobject thiz, jlong rendererPtr, jint width, jint height) {
    Renderer* renderer = reinterpret_cast&lt;Renderer*&gt;(rendererPtr);
    if (renderer) {
        return static_cast&lt;jint&gt;(renderer-&gt;createTexture(width, height));
    }
    return 0;
}

JNIEXPORT void JNICALL
Java_com_magpie_android_NativeRenderer_updateTexture(JNIEnv* env, jobject thiz, jlong rendererPtr, jint texture, jobject buffer, jint width, jint height) {
    Renderer* renderer = reinterpret_cast&lt;Renderer*&gt;(rendererPtr);
    if (renderer) {
        void* data = env-&gt;GetDirectBufferAddress(buffer);
        renderer-&gt;updateTexture(static_cast&lt;GLuint&gt;(texture), data, width, height);
    }
}

}
