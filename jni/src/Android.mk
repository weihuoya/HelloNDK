LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := main

# LOCAL_CFLAGS    := -Wall -Wextra

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../libpng \
    $(LOCAL_PATH)/../zlib

LOCAL_SRC_FILES := \
    jniwrapper.c \
    glcontext.c \
    glcube.c \
    image.c \
    touch.c \
    log.c

LOCAL_SHARED_LIBRARIES := png z

LOCAL_LDLIBS := -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
