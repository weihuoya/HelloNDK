#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <GLES2/gl2.h>


typedef struct {
    int width;
    int height;
    GLenum format;
    int size;
    char* data;
} RawImageData;


RawImageData * load_raw_image_data_from_png(const void * png_data, int data_size);

void release_raw_image_data(RawImageData* data);


#endif /* _IMAGE_H_ */
