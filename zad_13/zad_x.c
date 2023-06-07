#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main()
{
    ImageInfo *pImg = createImage(512, 512, 4);
    Point start, stop;
    start.x = start.y = 11;
    stop.x = stop.y = 255;
    unsigned char color = 3;
    while (start.y < stop.y)
    {
        bline(pImg, start, stop, color);
        start.y += 4;
        stop.y -= 32;
        color++;
    }
    saveBmp("diagonal.bmp", pImg);
    freeImage(pImg);
    return 0;
}
