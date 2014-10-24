package com.zhangwei.app;

import android.content.res.AssetManager;

public class JNIWrapper {

    static {
        System.loadLibrary("main");
    }

    public static native void load_assets(AssetManager manager);
    
    public static native void on_surface_created();

    public static native void on_surface_changed(int width, int height);

    public static native void on_draw_frame();

    public static native void on_touch_event(int touch_device_id_in, int pointer_finger_id_in, int action, float x, float y, float p);
    
    public static native void on_pause();
    
    public static native void on_resume();
}
