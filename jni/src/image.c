#include <png.h>
#include <string.h>
#include <stdlib.h>

#include "image.h"


typedef struct PngHandle {
    png_byte* imagedata;
    png_size_t imagesize;

    png_size_t offset;

    png_byte* rawdata;
    png_size_t rawsize;

    png_uint_32 width;
    png_uint_32 height;
    int color;
    int depth;
} PngHandle;


static void read_png_data_callback(png_structp png_ptr, png_byte* raw_data, png_size_t read_length)
{
    PngHandle* handle = png_get_io_ptr(png_ptr);
    const png_byte* png_src = handle->imagedata + handle->offset;

    memcpy(raw_data, png_src, read_length);
    handle->offset += read_length;
}


static void read_and_update_info(const png_structp png_ptr, const png_infop info_ptr, PngHandle * handle)
{
    png_uint_32 width, height;
    int bit_depth, color_type;

    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

    // Convert transparency to full alpha
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    // Convert grayscale, if needed.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    // Convert paletted images, if needed.
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    // Add alpha channel, if there is none (rationale: GL_RGBA is faster than GL_RGB on many GPUs)
    if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)
        png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

    // Ensure 8-bit packing
    if (bit_depth < 8)
        png_set_packing(png_ptr);
    else if (bit_depth == 16)
        png_set_scale_16(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    // Read the new color type after updates have been made.
    color_type = png_get_color_type(png_ptr, info_ptr);


    handle->width = width;
    handle->height = height;
    handle->color = color_type;
    handle->depth = bit_depth;
}


static void read_entire_png_image(const png_structp png_ptr, const png_infop info_ptr, PngHandle * handle)
{
    const png_uint_32 height = handle->height;
    const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    const int data_length = row_size * height;
    //assert(row_size > 0);

    png_byte* raw_image = malloc(data_length);
    //assert(raw_image != NULL);

    png_byte* row_ptrs[height];

    png_uint_32 i;
    for (i = 0; i < height; i++) {
        row_ptrs[i] = raw_image + i * row_size;
    }

    png_read_image(png_ptr, &row_ptrs[0]);


    handle->rawdata = raw_image;
    handle->rawsize = data_length;
}


static GLenum get_color_format(const int png_color_format)
{
    //assert(png_color_format == PNG_COLOR_TYPE_GRAY
    //        || png_color_format == PNG_COLOR_TYPE_RGB_ALPHA
    //        || png_color_format == PNG_COLOR_TYPE_GRAY_ALPHA);

    switch (png_color_format) {
        case PNG_COLOR_TYPE_GRAY:
            return GL_LUMINANCE;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            return GL_RGBA;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            return GL_LUMINANCE_ALPHA;
    }

    return 0;
}


RawImageData * load_raw_image_data_from_png(const void * png_data, int data_size)
{
    if ( !png_data || data_size <= 8 || !png_check_sig((void*)png_data, 8) )
        return NULL;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    PngHandle handle = {
        .imagedata= (png_byte*)png_data, .imagesize= data_size,
        .offset= 0,
        .rawdata= NULL, .rawsize= 0,
        .width= 0, .height= 0, .color= 0,
    };

    png_set_read_fn(png_ptr, &handle, read_png_data_callback);

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }


    read_and_update_info(png_ptr, info_ptr, &handle);
    read_entire_png_image(png_ptr, info_ptr, &handle);

    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);


    RawImageData * rawdata = (RawImageData*)malloc(sizeof(RawImageData));
    rawdata->width = handle.width;
    rawdata->height = handle.height;
    rawdata->format = get_color_format(handle.color);
    rawdata->size = handle.rawsize;
    rawdata->data = handle.rawdata;

    return rawdata;
}


void release_raw_image_data(RawImageData* rawdata)
{
    free(rawdata->data);
    free(rawdata);
}
