#include "jniwrapper.h"
#include "glcontext.h"
#include "touch.h"
#include "asset.h"


void JNICALL Java_com_zhangwei_app_JNIWrapper_load_1assets(JNIEnv * env, jclass clazz, jobject assetManager)
{
    Asset::instance()->initialize(env, assetManager);
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1surface_1created(JNIEnv * env, jclass clazz)
{
    GLContext::instance()->surfaceCreated();
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1surface_1changed(JNIEnv * env, jclass clazz, jint width, jint height)
{
    GLContext::instance()->surfaceChanged(width, height);
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1draw_1frame(JNIEnv * env, jclass clazz)
{
    GLContext::instance()->drawFrame();
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1touch_1event(
        JNIEnv * env, jclass clazz,
        jint touch_device_id, jint pointer_finger_id, jint action,
        jfloat x, jfloat y, jfloat p)
{
    Touch::instance()->touchEvent((TouchID)touch_device_id, (FingerID)pointer_finger_id, action, x, y, p);
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1pause(JNIEnv * env, jclass clazz)
{
    logWrite("[pause]");
    Touch::instance()->reset();
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1resume(JNIEnv * env, jclass clazz)
{
    logWrite("[resume]");
}

