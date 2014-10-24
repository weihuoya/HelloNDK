#include "jniwrapper.h"
#include "glcontext.h"
#include "touch.h"
#include "asset.h"
#include "log.h"


void JNICALL Java_com_zhangwei_app_JNIWrapper_load_1assets(JNIEnv * env, jclass clazz, jobject object)
{
    asset_init(env, object);
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1surface_1created(JNIEnv * env, jclass clazz)
{
    glcontext_surface_created();
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1surface_1changed(JNIEnv * env, jclass clazz, jint width, jint height)
{
    glcontext_surface_changed(width, height);
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1draw_1frame(JNIEnv * env, jclass clazz)
{
    glcontext_draw_frame();
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1touch_1event(
        JNIEnv * env, jclass clazz,
        jint touch_device_id, jint pointer_finger_id, jint action,
        jfloat x, jfloat y, jfloat p)
{
    touch_event((TouchID)touch_device_id, (FingerID)pointer_finger_id, action, x, y, p);
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1pause(JNIEnv * env, jclass clazz)
{
    Log("[pause]");
    reset_touch();
}


void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1resume(JNIEnv * env, jclass clazz)
{
    Log("[resume]");
}

