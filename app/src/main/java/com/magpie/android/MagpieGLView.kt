package com.magpie.android

import android.content.Context
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MagpieGLView(context: Context) : GLSurfaceView(context) {

    private val renderer: MagpieRenderer

    init {
        setEGLContextClientVersion(3)
        renderer = MagpieRenderer()
        setRenderer(renderer)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    fun setEffect(effect: Int) {
        renderer.setEffect(effect)
    }

    fun setScaleFactor(scale: Float) {
        renderer.setScaleFactor(scale)
    }

    fun updateFrame(data: ByteArray, width: Int, height: Int) {
        renderer.updateFrame(data, width, height)
        requestRender()
    }

    private inner class MagpieRenderer : Renderer {
        private var nativeRendererPtr: Long = 0
        private var inputTexture: Int = 0
        private var currentEffect = 0
        private var currentScale = 2.0f
        private var frameWidth = 0
        private var frameHeight = 0
        private var frameData: ByteArray? = null

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            val nativeRenderer = NativeRenderer()
            nativeRendererPtr = nativeRenderer.createRenderer()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            if (frameWidth > 0 && frameHeight > 0) {
                val nativeRenderer = NativeRenderer()
                nativeRenderer.initialize(nativeRendererPtr, frameWidth, frameHeight)
                nativeRenderer.setEffect(nativeRendererPtr, currentEffect)
                nativeRenderer.setScaleFactor(nativeRendererPtr, currentScale)
                inputTexture = nativeRenderer.createTexture(nativeRendererPtr, frameWidth, frameHeight)
            }
        }

        override fun onDrawFrame(gl: GL10?) {
            val nativeRenderer = NativeRenderer()
            frameData?.let { data ->
                val buffer = java.nio.ByteBuffer.wrap(data)
                nativeRenderer.updateTexture(nativeRendererPtr, inputTexture, buffer, frameWidth, frameHeight)
            }
            if (inputTexture != 0) {
                nativeRenderer.render(nativeRendererPtr, inputTexture)
            }
        }

        fun setEffect(effect: Int) {
            currentEffect = effect
            if (nativeRendererPtr != 0L) {
                NativeRenderer().setEffect(nativeRendererPtr, effect)
            }
        }

        fun setScaleFactor(scale: Float) {
            currentScale = scale
            if (nativeRendererPtr != 0L) {
                NativeRenderer().setScaleFactor(nativeRendererPtr, scale)
            }
        }

        fun updateFrame(data: ByteArray, width: Int, height: Int) {
            frameData = data
            if (frameWidth != width || frameHeight != height) {
                frameWidth = width
                frameHeight = height
                if (nativeRendererPtr != 0L) {
                    val nativeRenderer = NativeRenderer()
                    nativeRenderer.initialize(nativeRendererPtr, width, height)
                    nativeRenderer.setEffect(nativeRendererPtr, currentEffect)
                    nativeRenderer.setScaleFactor(nativeRendererPtr, currentScale)
                    if (inputTexture != 0) {
                    }
                    inputTexture = nativeRenderer.createTexture(nativeRendererPtr, width, height)
                }
            }
        }
    }
}
