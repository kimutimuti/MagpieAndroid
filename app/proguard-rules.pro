# Add project specific ProGuard rules here.
-keep class com.magpie.android.** { *; }
-keepclasseswithmembernames class * {
    native &lt;methods&gt;;
}

# Keep OpenGL related classes
-keep class android.opengl.** { *; }
