/* Simon Ibssa, Cal Poly SLO*/
#include <stdio.h>
#include <stdlib.h>

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


// global vars
tagBITMAPFILEHEADER fileHeader;
tagBITMAPINFOHEADER infoHeader;

int main(int argc, char *argv[]) {
  printf("%d\n", argc);
  unsigned char* bmImageData;
  FILE *file = fopen("lion.bmp","rb");
  
  int x;
  int y;

  // null pointer check
  if(file == NULL) {
    return 0;
  }

  // TODO: check to see if BMP file by checking first two bytes


  // read file header data
  fread(&fileHeader.bfType,1, 2,  file);
  fread(&fileHeader.bfSize, 1, 4, file);
  fread(&fileHeader.bfReserved1, 1, 2, file);
  fread(&fileHeader.bfReserved2, 1, 2, file);
  fread(&fileHeader.bfOffBits, 1, 4, file);


  fread(&infoHeader, 1, sizeof(infoHeader), file);

  unsigned char* imageData = (unsigned char*)malloc(infoHeader.biSizeImage);
  fread(&imageData, 1, infoHeader.biSizeImage, file);
  fclose(file);

  file = fopen("resultimage.bmp", "wb+");
  fwrite(&fileHeader.bfType,1, 2,  file);
  fwrite(&fileHeader.bfSize, 1, 4, file);
  fwrite(&fileHeader.bfReserved1, 1, 2, file);
  fwrite(&fileHeader.bfReserved2, 1, 2, file);
  fwrite(&fileHeader.bfOffBits, 1, 4, file);

  
  for(y = 0; y < infoHeader.biHeight; y++) {

    for(x = 0; x < infoHeader.biWidth; x++) {
      // TODO: insert correct getColumn invocation here


    }

  }

  return 0;
}



// function to manipulate image data
unsigned char * getColumn(unsigned char* imageData, int width, int x, int y, int color) {
  int bytesPerLine = y * infoHeader.biWidth * 3;
  if(bytesPerLine % 4 != 0) {
    bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
  }

  imageData[(x * 3)  + bytesPerLine + color] = 0;
  return imageData;

}