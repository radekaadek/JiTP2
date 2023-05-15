#include "image.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int min(unsigned int a, unsigned int b)
{
    return a < b ? a : b;
}

ImageInfo* readBmp(const char* fileName)
{
    FILE* file = fopen(fileName,"rb");
    if(file == NULL)
    {
        return NULL;
    }

    RGBbmpHdr btmhdr;
    if (fread((void*) &btmhdr, sizeof(btmhdr), 1, file) != 1)
    {
        fclose(file);
        return NULL;
    }

    if (btmhdr.bfType != 0x4D42 ||
        btmhdr.biPlanes != 1 ||
        btmhdr.biBitCount != 24 ||
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
    imginfo->line_bytes = imginfo->width * 3;
    if (imginfo->line_bytes % 4 != 0)
        imginfo->line_bytes += 4 - (imginfo->line_bytes % 4);
    unsigned imgsize = imginfo->height * imginfo->line_bytes;

    if (fseek(file, btmhdr.bfOffBits, SEEK_SET) != 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    imginfo->pImg = (unsigned char*) malloc(imgsize);
    if(imginfo->pImg == 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    if (fread(imginfo->pImg, 1, imgsize, file) != imgsize)
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

    unsigned int lineBytes = (int) imageinfo->line_bytes;
	unsigned int imgSize = lineBytes * imageinfo->height;
    RGBbmpHdr bmpHead =
	{
	0x4D42,				// unsigned short bfType;
	sizeof(RGBbmpHdr)+imgSize,		// unsigned long  bfSize;
	0, 0,				// unsigned short bfReserved1, bfReserved2;
	sizeof(RGBbmpHdr),	// unsigned long  bfOffBits;
	40,					// unsigned long  biSize;
	(int32_t)imageinfo->width,		// long  biWidth;
	(int32_t)imageinfo->height,		// long  biHeight;
	3,					// short biPlanes;
	24,					// short biBitCount;
	0,					// unsigned long  biCompression;
	(uint32_t) imgSize, // unsigned long  biSizeImage;
	11811,				// long biXPelsPerMeter; = 300 dpi
	11811,				// long biYPelsPerMeter;
	0,					// unsigned long  biClrUsed;
	0,					// unsigned long  biClrImportant;
	};

	if (fwrite(&bmpHead, sizeof(RGBbmpHdr), 1, file) != 1)
    {
        fclose(file);
        return 2;
    }
    if (fwrite(imageinfo->pImg, 1, bmpHead.biSizeImage, file) != bmpHead.biSizeImage)
    {
        fclose(file);
        return 3;
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

ImageInfo* filter(ImageInfo *imageinfo)
{

    unsigned imgsize = imageinfo->height * imageinfo->line_bytes;
    for (unsigned i = 0; i < imgsize; i += 3)
    {
        unsigned char b = imageinfo->pImg[i];
        unsigned char g = imageinfo->pImg[i + 1];
        unsigned char r = imageinfo->pImg[i + 2];
        unsigned int new_red = min(r * 0.393 + g * 0.769 + b * 0.189, 255);
        unsigned int new_green = min(r * 0.349 + g * 0.686 + b * 0.168, 255);
        unsigned int new_blue = min(r * 0.272 + g * 0.534 + b * 0.131, 255);
        imageinfo->pImg[i] = new_blue;
        imageinfo->pImg[i + 1] = new_green;
        imageinfo->pImg[i + 2] = new_red;
    }
    return imageinfo;
}

/*

3
JTP2 lato 2022/2023 Ćwiczenia 11 1/5 
Przetwarzanie obrazów 
Na chwilę porzucimy elementy interfejsu użytkownika i zajmiemy się 
innymi, może nawet bardziej interesującymi zastosowaniami 
programowania. Jednym z nich jest przetwarzanie obrazów.  
Kod przetwarzania obrazu napiszemy w języku C – będzie to jedna 
prosta funkcja wykonująca filtrację obrazu. Żeby nie komplikować 
niepotrzebnie sprawy, ograniczymy się (w tym zadaniu) do obrazów 
RGB: takich, w których jeden piksel jest reprezentowany przez trzy 
bajty odpowiadające składowym czerwonej, zielonej i niebieskiej 
koloru.  
Prócz operowania na obrazach w realizacji tego zadania pojawi się 
więcej nowości:  
• operowanie na plikach binarnych,  
• wykorzystanie kodu napisanego w C, 
• przekazywanie parametrów do programu wykonywalnego. 
JTP2 lato 2022/2023 Ćwiczenia 11 2/5 
Parametry programu 
W linii komend możemy podać parametry do uruchomienia 
programu: 
C:\....\zad_7\bin\Debug>zad_7 tiger.bmp tiger_out.bmp 
Żeby mieć dostęp do tych parametrów deklarujemy funkcję main 
w następujący sposób: 
int main(int argc, char* argv[]) 
argc oznacza liczbę argumentów (zawsze >= 1, bo pierwszym 
argumentem jest nazwa uruchamianego programu). 
Dostęp do kolejnych argumentów (w postaci ciągów znaków 
zakończonych zerem) dają kolejne elementy tablicy argv. 
Problem w tym, że uruchamiając program w Code::Blocks nie 
widzę miejsca na argumenty (można je ustawić w Project > Set 
programs’ arguments).  
JTP2 lato 2022/2023 Ćwiczenia 11 3/5 
Operacje na obrazie 
Szczęśliwie czytanie i zapis plików obrazów RGB w formacie BMP 
jest już zaimplementowane.  
Do zaimplementowania są funkcje get_pixel i put_pixel i trzeba 
tutaj przeliczyć współrzędne (x, y) piksela na offset (indeks) 
względem początku obrazu: 
offset = y * linebytes + x * 3 
Drugą operacją, wykonywaną niezależnie dla każdego piksela, jest 
filtr sepii. Nowe wartości składowych wyznaczamy na postawie 
składowych aktualnych w następujący sposób: 
new_red = red * 0.393 + green * 0.769 + blue * 0.189 
new_green = red * 0.349 + green * 0.686 + blue * 0.168 
new_blue = red * 0.272 + green * 0.534 + blue * 0.131 
Jest tu jeszcze haczyk: wartość może wyjść większa niż 255 – 
trzeba ją wtedy ustawić na maksymalne 255. 
JTP2 lato 2022/2023 Ćwiczenia 11 4/5 
Wynik filtrowania 
 
JTP2 lato 2022/2023 Ćwiczenia 11 5/5 
Oddawanie 
Rozwiązanie zadania składa się z 3 plików: 
image.h  – zawiera definicje struktur oraz deklaracje 
funkcji  
image.c  – implementacje funkcji  
zad_7.c – funkcja main w postaci z ćwiczeń. 
Rozwiązanie proszę załadować do Moodle do: 
21 maja 23:59 
 
*/
