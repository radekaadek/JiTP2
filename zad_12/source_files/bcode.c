#include "bcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


ImageInfo* readBmp(const char* fileName)
{
    FILE* file = fopen(fileName,"rb");
    if(file == NULL)
    {
        return NULL;
    }

    bmpHdr btmhdr;
    if (fread((void*) &btmhdr, sizeof(btmhdr), 1, file) != 1)
    {
        fclose(file);
        return NULL;
    }

    if (btmhdr.bfType != 0x4D42 ||
        btmhdr.biPlanes != 1 ||
        (btmhdr.biBitCount != 1 &&
        btmhdr.biBitCount != 4) ||
        btmhdr.biCompression != 0)
    {
        printf("Invalid bitmap header.\n");
        fclose(file);
        return NULL;
    }

    ImageInfo* imginfo = (ImageInfo *) malloc(sizeof(ImageInfo));
    if (imginfo == NULL)
    {
        fclose(file);
        return NULL;
    }
    imginfo->pImg = NULL;
    imginfo->height = abs(btmhdr.biHeight);
    imginfo->width = btmhdr.biWidth;
    imginfo->bitsperpel = btmhdr.biBitCount;

    unsigned pixels_per_byte = 8 / imginfo->bitsperpel;
    imginfo->line_bytes = (imginfo->width + pixels_per_byte - 1) / pixels_per_byte;
    if (imginfo->line_bytes % 4 != 0)
        imginfo->line_bytes += 4 - (imginfo->line_bytes % 4);

    // reading lookup table
    unsigned lut_entries = 1 << imginfo->bitsperpel;
    if (fread((void*) &(imginfo->LUT), 4, lut_entries, file) != lut_entries)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }

    if (fseek(file, btmhdr.bfOffBits, SEEK_SET) != 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }

    imginfo->pImg = (unsigned char*) malloc(btmhdr.biSizeImage);
    if(imginfo->pImg == 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    if (fread(imginfo->pImg, 1, btmhdr.biSizeImage, file) != btmhdr.biSizeImage)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    fclose(file);
    return imginfo;
}

int saveBmp(const char* fileName, ImageInfo *imageinfo)
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        return 1;
    }

    unsigned int lineBytes = imageinfo->line_bytes;
	unsigned int imgSize = lineBytes * imageinfo->height;

    unsigned int lut_entries = 1 << imageinfo->bitsperpel;
    unsigned int headerSize = sizeof(bmpHdr) + 4 * lut_entries;

    bmpHdr bmpHead =
	{
	0x4D42,				// unsigned short bfType;
	headerSize+imgSize,		// unsigned long  bfSize;
	0, 0,				// unsigned short bfReserved1, bfReserved2;
	headerSize,	        // unsigned long  bfOffBits;
	40,					// unsigned long  biSize;
	(int32_t)imageinfo->width,		// long  biWidth;
	(int32_t)imageinfo->height,		// long  biHeight;
	1,					// short biPlanes;
	imageinfo->bitsperpel,		// short biBitCount;
	0,					// unsigned long  biCompression;
	(uint32_t) imgSize, // unsigned long  biSizeImage;
	11811,				// long biXPelsPerMeter; = 300 dpi
	11811,				// long biYPelsPerMeter;
	lut_entries,		// unsigned long  biClrUsed;
	lut_entries,		// unsigned long  biClrImportant;
	};

	if (fwrite(&bmpHead, sizeof(bmpHdr), 1, file) != 1)
    {
        fclose(file);
        return 2;
    }
    if (fwrite(&imageinfo->LUT, 4, lut_entries, file) != lut_entries)
    {
        fclose(file);
        return 3;
    }
    if (fwrite(imageinfo->pImg, 1, bmpHead.biSizeImage, file) != bmpHead.biSizeImage)
    {
        fclose(file);
        return 4;
    }
    fclose(file);
	return 0;
}

void freeImage(ImageInfo *imageinfo)
{
    if (imageinfo != NULL)
    {
        if(imageinfo->pImg != NULL)
            free(imageinfo->pImg);
        free(imageinfo);
    }
}

static unsigned char bw_lut[] = {0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0};
static unsigned char sixteen_lut[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00,
    0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00,
    0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
    0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
    };

ImageInfo* createImage(unsigned int width, unsigned int height, unsigned int bpp)
{
    if (width == 0 || height == 0 || (bpp != 1 && bpp != 4))
        return NULL;
    ImageInfo *pImg = malloc(sizeof(ImageInfo));
    if (pImg == NULL)
        return NULL;

    pImg->width = width;
    pImg->height = height;
    pImg->bitsperpel = bpp;
    unsigned pixels_per_byte = 8 / pImg->bitsperpel;
    pImg->line_bytes = (pImg->width + pixels_per_byte - 1) / pixels_per_byte;
    if (pImg->line_bytes % 4 != 0)
        pImg->line_bytes += 4 - (pImg->line_bytes % 4);

    // LUT filling
    if (pImg->bitsperpel == 1)
        memcpy(pImg->LUT, bw_lut, sizeof(bw_lut));
    else
        memcpy(pImg->LUT, sixteen_lut, sizeof(sixteen_lut));

    pImg->pImg = malloc(pImg->line_bytes * pImg->height);
    if (pImg->pImg == NULL)
    {
        free(pImg);
        return NULL;
    }
    memset(pImg->pImg, 0xFF, pImg->line_bytes * pImg->height);
    return pImg;
}

void showLookupTable(ImageInfo *imageinfo)
{
    unsigned int lut_entries = 1 << imageinfo->bitsperpel;
    for (unsigned int idx = 0; idx < lut_entries; ++idx)
    {
        unsigned char *plut = imageinfo->LUT + 4 * idx;
        printf("%2d:  %02X %02X %02X %02X\n", idx, plut[0], plut[1], plut[2], plut[3]);
    }
}

void set_pixel_black(ImageInfo* pImg, unsigned int x, unsigned int y)
{
    unsigned char *pPix = pImg->pImg + y * pImg->line_bytes + x / 8;
    unsigned char mask = 0x80 << (x % 8);
    *pPix &= -mask;
}

void black_rect(ImageInfo* pImg, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    for (unsigned int row = y; row < y + height; ++row)
    {
        for (unsigned int col = x; col < x + width; ++col)
        {
            set_pixel_black(pImg, col, row);
        }
    }
}

// enum BarType {Tracker, Ascender, Descender, Full_Height};

void draw_bar(ImageInfo* pImg, unsigned int x, unsigned int y, unsigned int width, unsigned int max_height, enum BarType bar_type)
{
    unsigned int bar_height = 0;
    switch (bar_type)
    {
        case Tracker:
            bar_height = max_height / 4;
            y += max_height / 4;
            break;
        case Ascender:
            bar_height = max_height / 2;
            break;
        case Descender:
            bar_height = max_height / 2;
            y += max_height / 4;
            break;
        case Full_Height:
            bar_height = max_height;
            break;
    }
    black_rect(pImg, x, y, width, bar_height);
}

// This function gives 4 bars for each character
enum BarType* get_bar_types(const char* text)
{
    size_t len = strlen(text);
    enum BarType* bars = malloc(4 * len * sizeof(enum BarType));
    for(size_t i = 0; i < len; i++)
    {
        switch (text[i])
        {
            case 'J':
                bars[4*i] = Ascender;
                bars[4*i+1] = Descender;
                bars[4*i+2] = Tracker;
                bars[4*i+3] = Full_Height;
                break;
            case 'I':
                bars[4*i] = Ascender;
                bars[4*i+1] = Tracker;
                bars[4*i+2] = Descender;
                bars[4*i+3] = Full_Height;
                break;
            case 'T':
                bars[4*i] = Full_Height;
                bars[4*i+1] = Descender;
                bars[4*i+2] = Ascender;
                bars[4*i+3] = Tracker;
                break;
            case 'P':
                bars[4*i] = Ascender;
                bars[4*i+1] = Descender;
                bars[4*i+2] = Ascender;
                bars[4*i+3] = Descender;
                break;
            case '2':
                bars[4*i] = Tracker;
                bars[4*i+1] = Descender;
                bars[4*i+2] = Full_Height;
                bars[4*i+3] = Ascender;
                break;
            default:
                return NULL;
        }
    }
    return bars;
}


ImageInfo *rm4scc_gen(uint32_t width, uint32_t height, const char *text)
{
    size_t len = strlen(text);
    ImageInfo *imageinfo = createImage(width, height, 1);
    char *text_copy = malloc(len*sizeof(char));
    strcpy(text_copy, text);
    // go through the string and until its null
    for (size_t i = 0; i < len; i++)
    {
        if (isalpha(text_copy[i]))
        {
            text_copy[i] = toupper(text_copy[i]);
        }
        else if (!isdigit(text_copy[i]))
        {
            // print invalid character
            printf("Invalid character in input string: %c\n", text_copy[i]);
            return NULL;
        }
    }
    // print text copy
    printf("Text copy: %s\n", text_copy);
    unsigned int bar_width = width / 64;
    unsigned int margin_bottom = height / 8;

    // a list of bars to draw
    // enum BarType bars[8] = {Tracker, Ascender, Descender, Full_Height, Tracker, Ascender, Descender, Full_Height};
    // allocate memory for the bars
    enum BarType *bars;
    bars = get_bar_types(text_copy);
    if (bars == NULL)
        return NULL;

    // draw_bar(imageinfo, 0, margin_bottom, bar_width, height - 2 * margin_bottom, Full_Height);

    for (unsigned int i = 0; i < len * 4; ++i)
    {
        draw_bar(imageinfo, bar_width + 2 * i * bar_width, margin_bottom, bar_width, height - 2 * margin_bottom, bars[i]);
    }
    free(bars);
    return imageinfo;
}
