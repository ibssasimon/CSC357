#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

typedef struct tagBITMAPFILEHEADER{
    char bfType[2];  //specifies the file type
    int bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
} tagBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize; //specifies the number of bytes required by the struct
    LONG biWidth; //specifies width in pixels
    LONG biHeight; //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage; //size of image in bytes
    LONG biXPelsPerMeter; //number of pixels per meter in x axis
    LONG biYPelsPerMeter; //number of pixels per meter in y axis
    DWORD biClrUsed; //number of colors used by th ebitmap
    DWORD biClrImportant; //number of colors that are important
}tagBITMAPINFOHEADER;


FILE* file;
tagBITMAPFILEHEADER fHeader;
tagBITMAPINFOHEADER iHeader;

// function declaration
unsigned char getColor(unsigned char* imageData, int x, int y, int width, int color);

int main() {
  
  // First name is S. K-Z program
  int x;
  int y;
  file = fopen("nopadding.bmp", "rb");

  if(file == NULL) {
    printf("Cannot open file!\n");
    return 0;
  }

  fread(&fHeader.bfType, 1, sizeof(fHeader.bfType), file);
  fread(&fHeader.bfSize, 1, sizeof(fHeader.bfSize), file);
  fread(&fHeader.bfReserved1, 1, sizeof(fHeader.bfReserved1), file);
  fread(&fHeader.bfReserved2, 1, sizeof(fHeader.bfReserved2), file);
  fread(&fHeader.bfOffBits, 1, sizeof(fHeader.bfOffBits), file);

  fread(&iHeader, 1, sizeof(tagBITMAPINFOHEADER), file);

  unsigned char* imageData = (unsigned char*)malloc(iHeader.biSizeImage);
  unsigned char* finalImageData = (unsigned char*)malloc(iHeader.biSizeImage);

  fread(imageData, 1, iHeader.biSizeImage, file);

  fclose(file);


  for(y = 0; y < iHeader.biHeight; y++) {
    int bytesPerLine = iHeader.biWidth * 3;

    if(bytesPerLine % 4 != 0) {
      bytesPerLine = bytesPerLine +  4 - (bytesPerLine%4);
    }

    for(x = 0; x < iHeader.biWidth; x++) {

      unsigned char b = getColor(imageData, x, y, bytesPerLine, 0);
      unsigned char g = getColor(imageData, x, y, bytesPerLine, 1);
      unsigned char r = getColor(imageData, x, y, bytesPerLine, 2);


      int halfB = (int)b;
      halfB = halfB / 2;
      unsigned char newHalfB = (unsigned char)halfB;

      int halfG = (int)g;
      halfG = halfG / 2;
      unsigned char newHalfG = (unsigned char)halfG;



      finalImageData[(x*3) + y*bytesPerLine + 0] = newHalfG;
      finalImageData[(x*3) + y*bytesPerLine + 1] = halfB + halfG;
      finalImageData[(x*3) + y*bytesPerLine + 2] = newHalfB;

    }

  }


  // write data
  file = fopen("result.bmp", "wb+");

  fwrite(&fHeader.bfType, 1, sizeof(fHeader.bfType), file);
  fwrite(&fHeader.bfSize, 1, sizeof(fHeader.bfSize), file);
  fwrite(&fHeader.bfReserved1, 1, sizeof(fHeader.bfReserved1), file);
  fwrite(&fHeader.bfReserved2, 1, sizeof(fHeader.bfReserved2), file);
  fwrite(&fHeader.bfOffBits, 1, sizeof(fHeader.bfOffBits), file);

  fwrite(&iHeader, 1, sizeof(tagBITMAPINFOHEADER), file);

  fwrite(finalImageData, iHeader.biSizeImage, 1, file);
  fclose(file);

  return 0;

}

unsigned char getColor(unsigned char* imageData, int x, int y, int width, int color) {

    return imageData[(x*3) + width*y + color];

}