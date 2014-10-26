#include <stdio.h>
#include <stdlib.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "asset.h"


Asset::Asset() : manager_(NULL)
{
}

Asset::~Asset()
{
    //if(manager_) delete manager_;
}

Asset * Asset::instance()
{
    static Asset asset;
    return &asset;
}

void Asset::initialize(JNIEnv* env, jobject assetManager)
{
    manager_ = AAssetManager_fromJava(env, assetManager);
}

void Asset::travelDir()
{
    const char * filename = NULL;
    AAssetDir* assetDir = AAssetManager_openDir(manager_, "");

    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
    {
        logWrite(filename);
    }

    AAssetDir_close(assetDir);
}

void Asset::readData(const char * filename)
{
    char buffer[1024];
    int read_size = 0;
    AAsset* asset = AAssetManager_open(manager_, filename, AASSET_MODE_STREAMING);
    //off_t size = AAsset_getLength(asset);

    while ( (read_size = AAsset_read(asset, buffer, ARRAYSIZE(buffer)-1)) > 0 )
    {
        buffer[read_size] = 0;
        logWrite(buffer);
    }

    AAsset_close(asset);
}

void Asset::getBuffer(const char * filename, void ** outbuf, int * size)
{
    AAsset * asset = AAssetManager_open(manager_, filename, AASSET_MODE_UNKNOWN);
    const void * buffer = AAsset_getBuffer(asset);
    off_t length = AAsset_getLength(asset);

    char * temp = new char [length+1];
    memcpy(temp, buffer, length);
    temp[length] = 0;

    *outbuf = temp;
    *size = length;

    AAsset_close(asset);
}
