package com.zhangwei.app;


import android.content.Context;
import android.content.DialogInterface;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.Log;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.View;
import android.app.Activity;
import android.app.AlertDialog;


public class MySurfaceView extends GLSurfaceView {
	
	public MySurfaceView(Context context) {
		super(context);
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.v("NDK", "[key] donw:"+keyCode);
		if(keyCode == KeyEvent.KEYCODE_BACK) {
			showQuitAlert();
			return true;
		} else {
			return super.onKeyDown(keyCode, event);
		}
	}
	
	@Override
	public boolean onKeyMultiple(int keyCode, int count, KeyEvent event) {
		Log.v("NDK", "[key] multiple:"+keyCode);
		if(keyCode == KeyEvent.KEYCODE_BACK) {
			((Activity)super.getParent()).finish();
			//((Activity)super.getContext()).finish();
			return true;
		} else {
			return super.onKeyMultiple(keyCode, count, event);
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if(event != null)
		{
	        /* Ref: http://developer.android.com/training/gestures/multi.html */
	        final int touchDevId = event.getDeviceId();
	        final int pointerCount = event.getPointerCount();
	        int action = event.getActionMasked();
	        int pointerFingerId;
	        int i = -1;
	        float x,y,p;

	        switch(action) {
	            case MotionEvent.ACTION_MOVE:
	          	  	//Log.v("SDL", "event: ACTION_MOVE");
	                for (i = 0; i < pointerCount; i++) {
	                    pointerFingerId = event.getPointerId(i);
	                    x = event.getX(i);
	                    y = event.getY(i);
	                    p = event.getPressure(i);
	                    JNIWrapper.on_touch_event(touchDevId, pointerFingerId, action, x, y, p);
	                }
	                JNIWrapper.on_touch_event(touchDevId, -1, action, 0, 0, 0);
	                break;

	            case MotionEvent.ACTION_UP:
	            case MotionEvent.ACTION_DOWN:
	                // Primary pointer up/down, the index is always zero
	                i = 0;
	            case MotionEvent.ACTION_POINTER_UP:
	            case MotionEvent.ACTION_POINTER_DOWN:
	                // Non primary pointer up/down
	                if (i == -1) {
	                    i = event.getActionIndex();
	                }

	                pointerFingerId = event.getPointerId(i);
	                x = event.getX(i);
	                y = event.getY(i);
	                p = event.getPressure(i);
	                JNIWrapper.on_touch_event(touchDevId, pointerFingerId, action, x, y, p);
	                break;

	            default:
	                break;
	        }
			
			return true;
		}
		else
		{
			return super.onTouchEvent(event);
		}
	}
	
	protected void showQuitAlert() {
		AlertDialog.Builder builder = new AlertDialog.Builder(getContext());
		builder.setTitle(R.string.app_name);
		builder.setMessage(R.string.alert_quit);
		builder.setPositiveButton(R.string.btn_confirm, new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
				// TODO Auto-generated method stub
				android.os.Process.killProcess(android.os.Process.myPid());
			}
		});
		builder.setNegativeButton(R.string.btn_cancel, null);
		builder.show();
	}
}
