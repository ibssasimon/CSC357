#include <stdio.h>

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
struct tagBITMAPFILEHEADER {
  WORD bfType; //specifies the file type
  DWORD bfSize; //specifies the size in bytes of the bitmap file
  WORD bfReserved1; //reserved; must be 0
  WORD bfReserved2; //reserved; must be 0
  DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits
};

struct tagBITMAPINFOHEADER {
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
};


int main(int argc, char *argv[]) {
  printf("%d\n", argc);
  FILE *file = fopen("jar.bmp","rb");
  if(file!=NULL)printf("success!\n");
  fclose(file);

  // first two bits are 'BM', indicating it is a bitmap file
  // offset for start of BMP file data is 36. So we can find start of data at 0x36
  return 0;
}