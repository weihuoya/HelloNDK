LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := main

# LOCAL_CFLAGS    := -Wall -Wextra

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../libpng \
    $(LOCAL_PATH)/../zlib

LOCAL_SRC_FILES := \
    gltransform.cpp \
    jniwrapper.cpp \
    quaternion.cpp \
    glcontext.cpp \
    glwrapper.cpp \
    glcube.cpp \
    matrix.cpp \
    touch.cpp \
    asset.cpp \
    image.c \

LOCAL_SHARED_LIBRARIES := pngn zn

LOCAL_LDLIBS := -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
