#ifndef _ASSET_H_
#define _ASSET_H_

#include <jni.h>
#include <stdio.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "glwrapper.h"


class Asset
{
public:
    ~Asset();

    static Asset * instance();

    void initialize(JNIEnv* env, jobject assetManager);

    void getBuffer(const char * filename, void ** outbuf, int * size);

    void travelDir();

    void readData(const char * filename);

private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Asset);

    AAssetManager *manager_;
};


#endif /* _ASSET_H_ */
