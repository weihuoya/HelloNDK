#ifndef _ASSET_H_
#define _ASSET_H_

#include <jni.h>

void asset_init(JNIEnv* env, jobject assetManager);

void load_asset(const char * filename, void ** outbuf, int * size);

#endif /* _ASSET_H_ */
