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
	int16_t   biPlanes;			// bitmap planes (== 1)
	int16_t   biBitCount;		// bit count of a pixel (== 1 or 4)
	uint32_t  biCompression;	// should be 0 (no compression)
	uint32_t  biSizeImage;		// image size (not file size!)
	int32_t   biXPelsPerMeter;			// horizontal resolution
	int32_t   biYPelsPerMeter;			// vertical resolution
	uint32_t  biClrUsed;		// not important for RGB images
	uint32_t  biClrImportant;	// not important for RGB images
} bmpHdr;

#pragma pack(pop)

typedef struct
{
    unsigned int width,height;
    unsigned int line_bytes;
	unsigned int bitsperpel;
    unsigned char* pImg;
	unsigned char LUT[16 * 4]; // 'large', i.e. 4bpp color look-up table
} ImageInfo;

enum BarType {Tracker, Ascender, Descender, Full_Height};

void freeImage(ImageInfo *imageinfo);
ImageInfo* createImage(unsigned int width, unsigned int height, unsigned int bpp);

ImageInfo* readBmp(const char* fileName);
int saveBmp(const char* fileName, ImageInfo *imageinfo);

void showLookupTable(ImageInfo *imageinfo);

void set_pixel_black(ImageInfo* pImg, unsigned int x, unsigned int y);
void black_rect(ImageInfo* pImg, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
enum BarType* char_to_bar(char c);
enum BarType* get_bar_types(const char* text);
void draw_bar(ImageInfo* pImg, uint32_t x, uint32_t y, uint32_t width, uint32_t max_height, enum BarType bar_type);
void draw_msg(ImageInfo* imageinfo, uint32_t margin_bottom, uint32_t bar_width, uint32_t max_h, enum BarType* bars, unsigned long long bars_len);
char* validated_rm4scc(const char* text);
ImageInfo* rm4scc_gen(uint32_t width, uint32_t height, const char* text);

#endif // __IMAGE_H_INCLUDE__
