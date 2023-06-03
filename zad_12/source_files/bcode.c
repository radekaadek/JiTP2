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

void set_pixel_black(ImageInfo *pImg, unsigned int x, unsigned int y)
{
    if (x >= pImg->width || y >= pImg->height)
        return;
    unsigned pixels_per_byte = 8 / pImg->bitsperpel;
    unsigned int byte_idx = y * pImg->line_bytes + x / pixels_per_byte;
    unsigned int bit_idx = x % pixels_per_byte;
    unsigned char *pByte = pImg->pImg + byte_idx;
    *pByte &= ~(0x80 >> bit_idx);
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

enum BarType *char_to_bar(char c, enum BarType* bar_dest)
{
    switch(c)
    {
        case 'J':
            bar_dest[0] = Ascender;
            bar_dest[1] = Descender;
            bar_dest[2] = Tracker;
            bar_dest[3] = Full_Height;
            break;
        case 'I':
            bar_dest[0] = Ascender;
            bar_dest[1] = Tracker;
            bar_dest[2] = Descender;
            bar_dest[3] = Full_Height;
            break;
        case 'T':
            bar_dest[0] = Full_Height;
            bar_dest[1] = Descender;
            bar_dest[2] = Ascender;
            bar_dest[3] = Tracker;
            break;
        case 'P':
            bar_dest[0] = Ascender;
            bar_dest[1] = Descender;
            bar_dest[2] = Ascender;
            bar_dest[3] = Descender;
            break;
        case '2':
            bar_dest[0] = Tracker;
            bar_dest[1] = Descender;
            bar_dest[2] = Full_Height;
            bar_dest[3] = Ascender;
            break;
        case 'B':
            bar_dest[0] = Descender;
            bar_dest[1] = Full_Height;
            bar_dest[2] = Tracker;
            bar_dest[3] = Ascender;
            break;
        case 'X':
            bar_dest[0] = Full_Height;
            bar_dest[1] = Ascender;
            bar_dest[2] = Tracker;
            bar_dest[3] = Descender;
            break;
        case '1':
            bar_dest[0] = Tracker;
            bar_dest[1] = Descender;
            bar_dest[2] = Ascender;
            bar_dest[3] = Full_Height;
            break;
        case 'L':
            bar_dest[0] = Full_Height;
            bar_dest[1] = Tracker;
            bar_dest[2] = Tracker;
            bar_dest[3] = Full_Height;
            break;
        case 'A':
            bar_dest[0] = Descender;
            bar_dest[1] = Ascender;
            bar_dest[2] = Descender;
            bar_dest[3] = Ascender;
            break;
        case '9':
            bar_dest[0] = Descender;
            bar_dest[1] = Ascender;
            bar_dest[2] = Tracker;
            bar_dest[3] = Full_Height;
            break;
        case 'U':
            bar_dest[0] = Ascender;
            bar_dest[1] = Ascender;
            bar_dest[2] = Descender;
            bar_dest[3] = Descender;
            break;
        default:
            bar_dest = NULL;
            return bar_dest;
    }
    return bar_dest;
}

// This function gives 4 bars for each character
enum BarType *get_bars(const char *text)
{
    const size_t len = strlen(text);
    enum BarType *bars = malloc(4 * len * sizeof(enum BarType));
    for (size_t i = 0; i < len; i++)
    {
        enum BarType *check = char_to_bar(text[i], bars + 4 * i);
        if (check == NULL)
        {
            free(bars);
            return NULL;
        }
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

static const uint32_t weights[] = { 4, 2, 1, 0 };

// takes an array of 4 bars and returns the top checksum
uint32_t top_bar_checksum(enum BarType *bars)
{
    uint32_t checksum = 0;
    for (size_t i = 0; i < 4; i++)
    {
        if (bars[i] == Full_Height || bars[i] == Ascender)
            checksum += weights[i];
    }
    printf("Top half value: %d\n", checksum);
    return checksum;
}

// takes an array of 4 bars and returns the bottom checksum
uint32_t bottom_bar_checksum(enum BarType *bars)
{
    uint32_t checksum = 0;
    for (size_t i = 0; i < 4; i++)
    {
        if (bars[i] == Full_Height || bars[i] == Descender)
            checksum += weights[i];
    }
    printf("Bottom half value: %d\n", checksum);
    return checksum;
}

// takes an array of 4 * n bars and returns the top checksum
uint32_t top_bar_summed(enum BarType *bars, unsigned long bars_len)
{
    uint32_t checksum = 0;
    for (size_t i = 0; i < bars_len; i+=4)
    {
        checksum += top_bar_checksum(bars + i);
    }
    return checksum % 6;
}

// takes an array of 4 * n bars and returns the bottom checksum
uint32_t bottom_bar_summed(enum BarType *bars, unsigned long bars_len)
{
    uint32_t checksum = 0;
    for (size_t i = 0; i < bars_len; i+=4)
    {
        checksum += bottom_bar_checksum(bars + i);
    }
    return checksum % 6;
}

static const uint32_t extensions[] = { 0xC, 0x3, 0x5, 0x6, 0x9, 0xA };

uint32_t code_to_ext(uint32_t code)
{
    return extensions[code];
}


void draw_msg(ImageInfo *imageinfo, enum BarType *bars, unsigned long bars_len)
{
    const uint32_t col_padding = 3 + 8 + 2;
    const uint32_t bar_width = imageinfo->width / (bars_len * 2 + col_padding);
    const uint32_t margin_size = imageinfo->height / 8;
    const uint32_t max_h = imageinfo->height - 2 * margin_size;

    // draw start bar
    uint32_t x = bar_width;
    draw_bar(imageinfo, x, margin_size, bar_width, max_h, Ascender);
    x += 2 * bar_width;

    for (uint32_t i = 0; i < bars_len; ++i)
    {
        draw_bar(imageinfo, x, margin_size, bar_width, max_h, bars[i]);
        x += 2 * bar_width;
    }

    // draw top checksum
    uint32_t top_ext = code_to_ext(top_bar_summed(bars, bars_len));
    uint32_t bot_ext = code_to_ext(bottom_bar_summed(bars, bars_len));

    // iterate over bits of top checksum
    for (uint32_t i = 0; i < 4; ++i)
    {
        if (top_ext & (1 << (3 - i)))
        {
            if(bot_ext & (1 << (3 - i)))
                draw_bar(imageinfo, x, margin_size, bar_width, max_h, Full_Height);
            else
                draw_bar(imageinfo, x, margin_size, bar_width, max_h, Ascender);
        }
        else
        {
            if(bot_ext & (1 << (3 - i)))
                draw_bar(imageinfo, x, margin_size, bar_width, max_h, Descender);
            else
                draw_bar(imageinfo, x, margin_size, bar_width, max_h, Tracker);
        }
        x += 2 * bar_width;
    }

    // draw stop bar
    draw_bar(imageinfo, x, margin_size, bar_width, max_h, Full_Height);
}

char *validated_rm4scc(const char *text)
{
    const size_t len = strlen(text) + 2;
    char *text_copy = malloc(len * sizeof(char) + 1);
    // validate input and append '1A' to the end
    strcpy(text_copy, text);
    strcat(text_copy, "9U");
    
    for (size_t i = 0; i < len; ++i)
    {
        if (isalpha(text_copy[i]))
        {
            text_copy[i] = toupper(text_copy[i]);
        }
        else if (!isdigit(text_copy[i]))
        {
            printf("Invalid character in input string: %c\n", text_copy[i]);
            free(text_copy);
            return NULL;
        }
    }
    return text_copy;
}

ImageInfo *rm4scc_gen(unsigned int width, unsigned int height, const char *text)
{
    /*
        Przy pisaniu kodu na początku nie wiedziałem, zę jest
        specjalny pattern tych symboli tak jak jest to w
        tabelce poniżej więc implementowałem znaki po kolei ręcznie :(

                RM4SCC bar code symbols
        Top	Bottom (1=long bar, 0=short)
        0011	0101	0110	1001	1010	1100
                    1	2	3	4	5	0
        0011	1	0	1	2	3	4	5
        0101	2	6	7	8	9	A	B
        0110	3	C	D	E	F	G	H
        1001	4	I	J	K	L	M	N
        1010	5	O	P	Q	R	S	T
        1100	0	U	V	W	X	Y	Z
    */
    char *text_copy = validated_rm4scc(text);
    if (text_copy == NULL)
        return NULL;

    enum BarType *bars = get_bars(text_copy);
    if (bars == NULL) {
        free(text_copy);
        return NULL;
    }

    ImageInfo *imageinfo = createImage(width, height, 1);
    const size_t bars_len = strlen(text_copy) * 4;
    free(text_copy);

    draw_msg(imageinfo, bars, bars_len);

    free(bars);
    return imageinfo;
}
