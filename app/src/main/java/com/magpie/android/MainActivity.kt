package com.magpie.android

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.media.projection.MediaProjectionManager
import android.os.Bundle
import android.widget.Button
import android.widget.RadioGroup
import android.widget.SeekBar
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    private lateinit var effectGroup: RadioGroup
    private lateinit var scaleSeekBar: SeekBar
    private lateinit var scaleValue: TextView
    private lateinit var startButton: Button

    private var isCapturing = false
    private var selectedEffect = 0
    private var scaleFactor = 2.0f

    // C++の共有ライブラリをロード
    companion object {
        init {
            System.loadLibrary("magpie")
        }
    }

    private val projectionResultLauncher = registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()
    ) { result ->
        if (result.resultCode == Activity.RESULT_OK && result.data != null) {
            val intent = Intent(this, ScreenCaptureService::class.java).apply {
                putExtra("RESULT_CODE", result.resultCode)
                putExtra("DATA", result.data)
                putExtra("EFFECT", selectedEffect)
                putExtra("SCALE", scaleFactor)
            }
            startService(intent)
            
            isCapturing = true
            startButton.text = getString(R.string.stop_capture)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        initViews()
        setupListeners()
    }

    private fun initViews() {
        effectGroup = findViewById(R.id.effectGroup)
        scaleSeekBar = findViewById(R.id.scaleSeekBar)
        scaleValue = findViewById(R.id.scaleValue)
        startButton = findViewById(R.id.startButton)
    }

    private fun setupListeners() {
        effectGroup.setOnCheckedChangeListener { _, checkedId ->
            selectedEffect = when (checkedId) {
                R.id.radioAnime4K -> 0
                R.id.radioFSR -> 1
                R.id.radioBicubic -> 2
                R.id.radioLanczos -> 3
                else -> 0
            }
        }

        scaleSeekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                scaleFactor = when (progress) {
                    0 -> 1.5f
                    1 -> 2.0f
                    2 -> 2.5f
                    3 -> 3.0f
                    else -> 2.0f
                }
                scaleValue.text = "${scaleFactor}x"
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {}
            override fun onStopTrackingTouch(seekBar: SeekBar?) {}
        })

        startButton.setOnClickListener {
            if (isCapturing) {
                stopCapture()
            } else {
                requestPermission()
            }
        }
    }

    private fun requestPermission() {
        val mediaProjectionManager = getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
        projectionResultLauncher.launch(mediaProjectionManager.createScreenCaptureIntent())
    }

    private fun stopCapture() {
        val intent = Intent(this, ScreenCaptureService::class.java)
        stopService(intent)
        
        isCapturing = false
        startButton.text = getString(R.string.start_capture)
        Toast.makeText(this, "已停止", Toast.LENGTH_SHORT).show()
    }
}
