#include <stdio.h>
#include <stdlib.h>
#include "bcode.h"

ImageInfo* inv_pixel1bpp(ImageInfo* pImg, unsigned int x, unsigned int y)
{
    unsigned char *pPix = pImg->pImg + y * pImg->line_bytes + x / 8;
    unsigned char mask = 1 << (7 - x % 8);
    *pPix ^= mask;
    return pImg;
}

ImageInfo* inv_pixel4bpp(ImageInfo* pImg, unsigned int x, unsigned int y)
{
    unsigned char *pPix = pImg->pImg + y * pImg->line_bytes + x / 2;
    unsigned char mask = x & 0x1 ? 0x0F : 0xF0;
    *pPix ^= mask;
    return pImg;
}

void diagonal1bpp(ImageInfo *pImg)
{
    unsigned int cnt = pImg->width < pImg->height ? pImg->width : pImg->height;
    for (unsigned int xy = 0; xy < cnt; ++xy)
        inv_pixel1bpp(pImg, xy, xy);
}

void diagonal4bpp(ImageInfo *pImg)
{
    unsigned int cnt = pImg->width < pImg->height ? pImg->width : pImg->height;
    for (unsigned int xy = 0; xy < cnt; ++xy)
        inv_pixel4bpp(pImg, xy, xy);
}

int main(int argc, char *argv[])
{
    if (sizeof(bmpHdr) != 54)
    {
        printf("Size of the bitmap header is invalid (%lu). Please, check compiler options.\n", sizeof(bmpHdr));
        return 1;
    }

    ImageInfo *pImg = rm4scc_gen(1024, 128, "JiTaP2");
    if (pImg == NULL)
    {
        printf("Invalid string for RM4SCC encoding\n");
        return 1;
    }
    saveBmp("jitp2.bmp", pImg);
    freeImage(pImg);
    return 0;
}
