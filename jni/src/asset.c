#include <stdio.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "asset.h"


#define ARRAYSIZE(a) ( (sizeof(a) / sizeof(*(a)) )


static AAssetManager *manager = NULL;


void asset_init(JNIEnv* env, jobject assetManager)
{
    manager = AAssetManager_fromJava(env, object);
}


/*
static void travelDir()
{
    AAssetDir* assetDir = AAssetManager_openDir(manager, "");

    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
    {
        Log(filename);
    }

    AAssetDir_close(assetDir);
}


static void readData(const char * filename)
{
    char buffer[1024];
    int read_size = 0;
    AAsset* asset = AAssetManager_open(manager, filename, AASSET_MODE_STREAMING);
    //off_t size = AAsset_getLength(asset);

    while ( (read_size = AAsset_read(asset, buffer, ARRAYSIZE(buffer)-1)) > 0 )
    {
        buffer[read_size] = 0;
        Log(buffer);
    }

    AAsset_close(asset);
}
*/


void load_asset(const char * filename, void ** outbuf, int * size)
{
    AAsset * asset = AAssetManager_open(manager, filename, AASSET_MODE_UNKNOWN);
    void * buffer = AAsset_getBuffer(asset);
    off_t length = AAsset_getLength(asset);

    char * temp = (char *)malloc(length+1);
    memcpy(temp, buffer, length);
    temp[length] = 0;

    *outbuf = temp;
    *size = length;

    AAsset_close(asset);
}
