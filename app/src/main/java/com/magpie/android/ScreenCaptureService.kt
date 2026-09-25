package com.magpie.android

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.content.pm.ServiceInfo
import android.graphics.PixelFormat
import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.Image
import android.media.ImageReader
import android.media.projection.MediaProjection
import android.media.projection.MediaProjectionManager
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.os.IBinder
import android.view.Gravity
import android.view.WindowManager
import java.nio.ByteBuffer

class ScreenCaptureService : Service() {

    companion object {
        private const val CHANNEL_ID = "ScreenCaptureChannel"
        private const val NOTIFICATION_ID = 1
        private const val VIRTUAL_DISPLAY_NAME = "MagpieDisplay"
    }

    private var mediaProjectionManager: MediaProjectionManager? = null
    private var mediaProjection: MediaProjection? = null
    private var virtualDisplay: VirtualDisplay? = null
    private var imageReader: ImageReader? = null
    private var windowManager: WindowManager? = null
    private var glView: MagpieGLView? = null
    private var handlerThread: HandlerThread? = null
    private var handler: Handler? = null
    private var selectedEffect = 0
    private var scaleFactor = 2.0f
    private var resultCode = 0
    private var resultData: Intent? = null

    private val projectionCallback = object : MediaProjection.Callback() {
        override fun onStop() {
            super.onStop()
            stopSelf()
        }
    }

    override fun onCreate() {
        super.onCreate()
        createNotificationChannel()
        
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            startForeground(
                NOTIFICATION_ID, 
                createNotification(), 
                ServiceInfo.FOREGROUND_SERVICE_TYPE_MEDIA_PROJECTION
            )
        } else {
            startForeground(NOTIFICATION_ID, createNotification())
        }
        
        windowManager = getSystemService(Context.WINDOW_SERVICE) as WindowManager
        mediaProjectionManager = getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
        
        handlerThread = HandlerThread("ScreenCaptureThread")
        handlerThread?.start()
        handler = Handler(handlerThread?.looper!!)
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        selectedEffect = intent?.getIntExtra("EFFECT", 0) ?: 0
        scaleFactor = intent?.getFloatExtra("SCALE", 2.0f) ?: 2.0f
        resultCode = intent?.getIntExtra("RESULT_CODE", 0) ?: 0
        @Suppress("DEPRECATION")
        resultData = intent?.getParcelableExtra("DATA")
        
        if (resultCode != 0 && resultData != null) {
            mediaProjection = mediaProjectionManager?.getMediaProjection(resultCode, resultData!!)
            setupMediaProjection()
            createOverlay()
        }
        
        return START_STICKY
    }

    private fun createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                CHANNEL_ID,
                "Screen Capture Service",
                NotificationManager.IMPORTANCE_LOW
            )
            val manager = getSystemService(NotificationManager::class.java)
            manager.createNotificationChannel(channel)
        }
    }

    private fun createNotification(): Notification {
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            Notification.Builder(this, CHANNEL_ID)
                .setContentTitle("Magpie Android")
                .setContentText("超分辨率运行中...")
                .setSmallIcon(android.R.drawable.ic_menu_gallery)
                .build()
        } else {
            @Suppress("DEPRECATION")
            Notification.Builder(this)
                .setContentTitle("Magpie Android")
                .setContentText("超分辨率运行中...")
                .setSmallIcon(android.R.drawable.ic_menu_gallery)
                .build()
        }
    }

    private fun setupMediaProjection() {
        val displayMetrics = resources.displayMetrics
        val width = displayMetrics.widthPixels
        val height = displayMetrics.heightPixels
        val density = displayMetrics.densityDpi

        imageReader = ImageReader.newInstance(width, height, PixelFormat.RGBA_8888, 2)
        
        imageReader?.setOnImageAvailableListener({ reader ->
            val image: Image? = try {
                reader.acquireLatestImage()
            } catch (e: Exception) {
                null
            }
            
            image?.let {
                processImage(it)
                it.close()
            }
        }, handler)
        
        mediaProjection?.registerCallback(projectionCallback, handler)
        
        virtualDisplay = mediaProjection?.createVirtualDisplay(
            VIRTUAL_DISPLAY_NAME,
            width,
            height,
            density,
            DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
            imageReader?.surface,
            null,
            null
        )
    }

    private fun processImage(image: Image) {
        val planes = image.planes
        val buffer: ByteBuffer = planes[0].buffer 
        val pixelStride = planes[0].pixelStride
        val rowStride = planes[0].rowStride
        
        val rowStridePixels = rowStride / pixelStride
        
        glView?.updateFrameDirect(buffer, image.width, image.height, rowStridePixels)
    }

    private fun createOverlay() {
        val params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.MATCH_PARENT,
            WindowManager.LayoutParams.MATCH_PARENT,
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
            else
                @Suppress("DEPRECATION")
                WindowManager.LayoutParams.TYPE_SYSTEM_ALERT,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or
                    WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE or
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN or
                    WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
            PixelFormat.TRANSLUCENT
        )
        params.gravity = Gravity.TOP or Gravity.START

        glView = MagpieGLView(this)
        glView?.setEffect(selectedEffect)
        glView?.setScaleFactor(scaleFactor)

        windowManager?.addView(glView, params)
    }

    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    override fun onDestroy() {
        super.onDestroy()
        
        glView?.let {
            windowManager?.removeView(it)
        }
        
        virtualDisplay?.release()
        imageReader?.close()
        mediaProjection?.unregisterCallback(projectionCallback)
        mediaProjection?.stop()
        handlerThread?.quitSafely()
    }
}
