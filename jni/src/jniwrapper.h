/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_zhangwei_app_JNIWrapper */

#ifndef _Included_com_zhangwei_app_JNIWrapper
#define _Included_com_zhangwei_app_JNIWrapper
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_zhangwei_app_JNIWrapper
 * Method:    load_assets
 * Signature: (Landroid/content/res/AssetManager;)V
 */
JNIEXPORT void JNICALL Java_com_zhangwei_app_JNIWrapper_load_1assets
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_zhangwei_app_JNIWrapper
 * Method:    on_surface_created
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1surface_1created
  (JNIEnv *, jclass);

/*
 * Class:     com_zhangwei_app_JNIWrapper
 * Method:    on_surface_changed
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1surface_1changed
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_zhangwei_app_JNIWrapper
 * Method:    on_draw_frame
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1draw_1frame
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_zhangwei_app_JNIWrapper
 * Method:    on_touch_event
 * Signature: (IIIFFF)V
 */
JNIEXPORT void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1touch_1event
  (JNIEnv *, jclass, jint, jint, jint, jfloat, jfloat, jfloat);

/*
 * Class:     com_zhangwei_app_JNIWrapper
 * Method:    on_pause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1pause
  (JNIEnv *, jclass);

/*
 * Class:     com_zhangwei_app_JNIWrapper
 * Method:    on_resume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_zhangwei_app_JNIWrapper_on_1resume
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
