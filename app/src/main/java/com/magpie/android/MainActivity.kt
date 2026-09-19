package com.magpie.android

import android.app.Activity
import android.content.Context
import android.content.Intent
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

    private val projectionResultLauncher = registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()
    ) { result -&gt;
        if (result.resultCode == Activity.RESULT_OK) {
            Toast.makeText(this, "权限已授予（演示版本）", Toast.LENGTH_SHORT).show()
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
        effectGroup.setOnCheckedChangeListener { _, checkedId -&gt;
            selectedEffect = when (checkedId) {
                R.id.radioAnime4K -&gt; 0
                R.id.radioFSR -&gt; 1
                R.id.radioBicubic -&gt; 2
                R.id.radioLanczos -&gt; 3
                else -&gt; 0
            }
        }

        scaleSeekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                scaleFactor = when (progress) {
                    0 -&gt; 1.5f
                    1 -&gt; 2.0f
                    2 -&gt; 2.5f
                    3 -&gt; 3.0f
                    else -&gt; 2.0f
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
        Toast.makeText(this, "这是演示版本，实际功能需要 NDK 构建", Toast.LENGTH_LONG).show()
        isCapturing = true
        startButton.text = getString(R.string.stop_capture)
    }

    private fun stopCapture() {
        isCapturing = false
        startButton.text = getString(R.string.start_capture)
        Toast.makeText(this, "已停止", Toast.LENGTH_SHORT).show()
    }
}
