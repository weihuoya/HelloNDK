package com.zhangwei.app;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

public class MyRenderer implements GLSurfaceView.Renderer {

	private long savedTime;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    	savedTime = System.currentTimeMillis();
        JNIWrapper.on_surface_created();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
    	//savedTime = System.currentTimeMillis();
        JNIWrapper.on_surface_changed(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        // GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
    	long currentTime = System.currentTimeMillis();
        JNIWrapper.on_draw_frame(currentTime - savedTime);
        savedTime = currentTime;
    }
}