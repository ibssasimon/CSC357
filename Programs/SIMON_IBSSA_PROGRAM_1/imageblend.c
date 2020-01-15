#include <stdio.h>

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
typedef struct tagBITMAPFILEHEADER {
  WORD bfType; //specifies the file type
  DWORD bfSize; //specifies the size in bytes of the bitmap file
  WORD bfReserved1; //reserved; must be 0
  WORD bfReserved2; //reserved; must be 0
  DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits
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
} tagBITMAPINFOHEADER;


int main(int argc, char *argv[]) {
  printf("%d\n", argc);
  FILE *file = fopen("lion.bmp","rb");
  tagBITMAPFILEHEADER bmFileHeader;
  unsigned char* bmImageData;

  // null pointer check
  if(file == NULL) {
    return NULL;
  }

  // read file header, each are size 1 byte
  fread(&bmFileHeader, sizeof(tagBITMAPFILEHEADER), 1, file);

  printf("%d\n", bmFileHeader.bfSize);

  // TODO: check to see if BMP file by checking first two bytes
  
  fclose(file);

  // first two bits are 'BM', indicating it is a bitmap file indicated by 
  // offset for start of BMP file data is 36. So we can find start of data at 0x36
  return 0;
}