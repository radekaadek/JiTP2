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

enum BarType* char_to_bar(char c)
{
    enum BarType* bar = malloc(4 * sizeof(enum BarType));
    switch(c)
    {
        case 'J':
            bar[0] = Ascender;
            bar[1] = Descender;
            bar[2] = Tracker;
            bar[3] = Full_Height;
            break;
        case 'I':
            bar[0] = Ascender;
            bar[1] = Tracker;
            bar[2] = Descender;
            bar[3] = Full_Height;
            break;
        case 'T':
            bar[0] = Full_Height;
            bar[1] = Descender;
            bar[2] = Ascender;
            bar[3] = Tracker;
            break;
        case 'P':
            bar[0] = Ascender;
            bar[1] = Descender;
            bar[2] = Ascender;
            bar[3] = Descender;
            break;
        case '2':
            bar[0] = Tracker;
            bar[1] = Descender;
            bar[2] = Full_Height;
            bar[3] = Ascender;
            break;
        default:
            free(bar);
            return NULL;
    }
    return bar;
}

// This function gives 4 bars for each character
enum BarType *get_bars(const char *text)
{
    size_t len = strlen(text);
    enum BarType *bars = malloc(4 * len * sizeof(enum BarType));
    for (size_t i = 0; i < len; i++)
    {
        enum BarType *bar = char_to_bar(text[i]);
        if (bar == NULL)
        {
            free(bars);
            free(bar);
            return NULL;
        }
        memcpy(bars + 4 * i, bar, 4 * sizeof(enum BarType));
        free(bar);
    }
    return bars;
}

void draw_bar(ImageInfo *pImg, uint32_t x, uint32_t y, uint32_t width, uint32_t max_height, enum BarType bar_type)
{
    uint32_t bar_height = 0;
    switch (bar_type)
    {
    case Tracker:
        bar_height = max_height / 3;
        y += max_height / 3;
        break;
    case Ascender:
        bar_height = 2 * max_height / 3;
        y += max_height / 3;
        break;
    case Descender:
        bar_height = 2 * max_height / 3;
        break;
    case Full_Height:
        bar_height = max_height;
        break;
    }
    black_rect(pImg, x, y, width, bar_height);
}

void draw_msg(ImageInfo *imageinfo, enum BarType *bars, unsigned long long bars_len)
{
    const uint32_t bar_width = imageinfo->width / (bars_len * 2 + 5);
    const uint32_t margin_bottom = imageinfo->height / 8;
    const uint32_t max_h = imageinfo->height - 2 * margin_bottom;

    // draw start bar
    uint32_t x = bar_width;
    draw_bar(imageinfo, x, margin_bottom, bar_width, max_h, Ascender);
    x += 2 * bar_width;

    for (uint32_t i = 0; i < bars_len; ++i)
    {
        draw_bar(imageinfo, x, margin_bottom, bar_width, max_h, bars[i]);
        x += 2 * bar_width;
    }

    // draw stop bar
    draw_bar(imageinfo, x, margin_bottom, bar_width, max_h, Full_Height);
}

char *validated_rm4scc(const char *text)
{
    size_t len = strlen(text);
    char *text_copy = malloc(len * sizeof(char) + 1);
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
            free(text_copy);
            return NULL;
        }
    }
    return text_copy;
}

ImageInfo *rm4scc_gen(unsigned int width, unsigned int height, const char *text)
{
    char *text_copy = validated_rm4scc(text);
    if (text_copy == NULL)
        return NULL;

    // nie wiem jak to zrobić przy użyciu stosu
    enum BarType *bars = get_bars(text_copy);
    free(text_copy);
    if (bars == NULL)
        return NULL;

    ImageInfo *imageinfo = createImage(width, height, 1);
    size_t bars_len = strlen(text) * 4;

    draw_msg(imageinfo, bars, bars_len);

    free(bars);
    return imageinfo;
}
