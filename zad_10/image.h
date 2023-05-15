#ifndef __IMAGE_H_INCLUDE__
#define __IMAGE_H_INCLUDE__

#include <stdint.h>

#pragma pack(push, 1)

typedef struct
{
	uint16_t  bfType;	// 0x4D42
	uint32_t  bfSize;	// file size in bytes
	uint16_t  bfReserved1;
	uint16_t  bfReserved2;
	uint32_t  bfOffBits;	// offset of pixel data
	uint32_t  biSize;		// header size (bitmap info size)
	int32_t   biWidth;			// image width
	int32_t   biHeight;			// image height
	int16_t   biPlanes;			// bitmap planes (== 3)
	int16_t   biBitCount;		// bit count of a pixel (== 24)
	uint32_t  biCompression;	// should be 0 (no compression)
	uint32_t  biSizeImage;		// image size (not file size!)
	int32_t   biXPelsPerMeter;			// horizontal resolution
	int32_t   biYPelsPerMeter;			// vertical resolution
	uint32_t  biClrUsed;		// not important for RGB images
	uint32_t  biClrImportant;	// not important for RGB images
} RGBbmpHdr;

#pragma pack(pop)

typedef struct
{
    unsigned int width,height;
    unsigned int line_bytes;
    unsigned char* pImg;
} ImageInfo;


void freeImage(ImageInfo *imageinfo);
ImageInfo* readBmp(const char* fileName);
int saveBmp(const char* fileName, ImageInfo *imageinfo);
ImageInfo* filter(ImageInfo *imageinfo);
unsigned int min(unsigned int a, unsigned int b);


#endif // __IMAGE_H_INCLUDE__
