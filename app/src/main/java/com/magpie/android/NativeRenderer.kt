package com.magpie.android

class NativeRenderer {
    companion object {
        init {
            System.loadLibrary("magpie")
        }
    }

    external fun createRenderer(): Long
    external fun initialize(rendererPtr: Long, width: Int, height: Int): Boolean
    external fun destroy(rendererPtr: Long)
    external fun render(rendererPtr: Long, inputTexture: Int)
    external fun setEffect(rendererPtr: Long, effectType: Int)
    external fun setScaleFactor(rendererPtr: Long, scale: Float)
    external fun createTexture(rendererPtr: Long, width: Int, height: Int): Int
    external fun updateTexture(rendererPtr: Long, texture: Int, buffer: java.nio.ByteBuffer, width: Int, height: Int)
}
