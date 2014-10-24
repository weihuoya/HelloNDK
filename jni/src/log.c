#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <android/log.h>

#include "log.h"


#define MAX_LOG_MESSAGE 2048


static const char * category_prefixes[] = {
    "APP",
    "ERROR",
    "SYSTEM",
    "AUDIO",
    "VIDEO",
    "RENDER",
    "INPUT"
};


static int android_priority[] = {
    ANDROID_LOG_UNKNOWN,
    ANDROID_LOG_VERBOSE,
    ANDROID_LOG_DEBUG,
    ANDROID_LOG_INFO,
    ANDROID_LOG_WARN,
    ANDROID_LOG_ERROR,
    ANDROID_LOG_FATAL
};


void Log(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    char tag[32];
    char * message = (char*) malloc(MAX_LOG_MESSAGE);

    snprintf(tag, arraysize(tag), "NDK/%s", category_prefixes[0]);
    vsnprintf(message, MAX_LOG_MESSAGE, fmt, ap);

    __android_log_write(android_priority[3], tag, message);

    va_end(ap);
    free(message);
}
