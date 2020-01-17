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
tagBITMAPFILEHEADER fileHeader2;
tagBITMAPINFOHEADER infoHeader;
tagBITMAPINFOHEADER infoHeader2;

// function declarations
unsigned char getColor(unsigned char* imageData, int width, int x, int y, int color);

int main(int argc, char *argv[]) {
  printf("%d\n", argc);
  FILE *file = fopen("lion.bmp","rb");
  //FILE *file2 = fopen("flowers.bmp", "rb");
  
  int x;
  int y;

  // null pointer check
  if(file == NULL) {
    return 0;
  }

  // TODO: check to see if BMP file by checking first two bytes


  // read file header data for image 1
  fread(&fileHeader.bfType, 1, sizeof(fileHeader.bfType),  file);
  fread(&fileHeader.bfSize, 1, sizeof(fileHeader.bfSize), file);
  fread(&fileHeader.bfReserved1, 1, sizeof(fileHeader.bfReserved1), file);
  fread(&fileHeader.bfReserved2, 1, sizeof(fileHeader.bfReserved2), file);
  fread(&fileHeader.bfOffBits, 1, sizeof(fileHeader.bfOffBits), file);

  // read file header data for image 2
  //fread(&fileHeader2.bfType, 2, sizeof(fileHeader2.bfType),  file2);
  //fread(&fileHeader2.bfSize, 1, sizeof(fileHeader2.bfSize), file2);
  //fread(&fileHeader2.bfReserved1, 1, sizeof(fileHeader2.bfReserved1), file2);
  //fread(&fileHeader2.bfReserved2, 1, sizeof(fileHeader2.bfReserved2), file2);
  //fread(&fileHeader2.bfOffBits, 1, sizeof(fileHeader2.bfOffBits), file2);


  // read image header data for both images
  fread(&infoHeader, 1, sizeof(tagBITMAPINFOHEADER), file);
  //fread(&infoHeader2, 1, sizeof(infoHeader), file2);


  //fseek(file, fileHeader.bfOffBits, SEEK_SET);
  unsigned char* imageData = (unsigned char*)malloc(infoHeader.biSizeImage);
  unsigned char* finalImageData = (unsigned char*)malloc(infoHeader.biSizeImage);
  fread(imageData, 1, infoHeader.biSizeImage, file);
  fclose(file);
  //fclose(file2);

  // check size of infoHeader's image size for both files before looping

  if(infoHeader.biSizeImage >= infoHeader2.biSizeImage) {
    tagBITMAPINFOHEADER bigger = infoHeader;
    tagBITMAPINFOHEADER smaller = infoHeader2;
  } else {
    tagBITMAPINFOHEADER bigger = infoHeader2;
    tagBITMAPINFOHEADER smaller = infoHeader;
  }



  unsigned int index = 0;
  for(y = 0; y < infoHeader.biHeight; y++) {

    int bytesPerLine = y * infoHeader.biWidth * 3;
    if(bytesPerLine % 4 != 0) {
      bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
    }

    for(x = 0; x < infoHeader.biWidth; x++) {

      
      // TODO: insert correct getColumn invocation here
      unsigned char b1 = getColor(imageData, infoHeader.biWidth, x, y, 0);

      finalImageData[(x * 3)  + bytesPerLine + 0] = b1;

      unsigned char g1 = getColor(imageData, infoHeader.biWidth, x, y, 1);
      finalImageData[(x * 3)  + bytesPerLine + 1] = 0;

      unsigned char r1 = getColor(imageData, infoHeader.biWidth, x, y, 2);
      finalImageData[(x * 3)  + bytesPerLine + 2] = r1;

      /* UPDATE SECOND IMAGEunsigned char b2 = getColor(imageData, infoHeader.biWidth, x, y, 0);

      finalImageData[(x * 3)  + bytesPerLine + 0] = b2;

      unsigned char g2 = getColor(imageData, infoHeader.biWidth, x, y, 1);
      finalImageData[(x * 3)  + bytesPerLine + 1] = 0;

      unsigned char r2 = getColor(imageData, infoHeader.biWidth, x, y, 2);
      finalImageData[(x * 3)  + bytesPerLine + 2] = r2;*/

      // Blend image

      // assign into final image data


    }

  }

  // allocate result image mem
  file = fopen("resultimage.bmp", "wb+");
  fwrite(&fileHeader.bfType, 1, sizeof(fileHeader.bfType),  file);
  fwrite(&fileHeader.bfSize, 1, sizeof(fileHeader.bfSize), file);
  fwrite(&fileHeader.bfReserved1, 1, sizeof(fileHeader.bfReserved1), file);
  fwrite(&fileHeader.bfReserved2, 1, sizeof(fileHeader.bfReserved2), file);
  fwrite(&fileHeader.bfOffBits, 1, sizeof(fileHeader.bfOffBits), file);


  fwrite(&infoHeader, 1, sizeof(tagBITMAPINFOHEADER), file);

  


  fwrite(finalImageData, infoHeader.biSizeImage, 1, file);
  fclose(file);

  
  
  return 0;
}



// function to manipulate image data
unsigned char getColor(unsigned char* imageData, int width, int x, int y, int color) {
  int bytesPerLine = y * infoHeader.biWidth * 3;
  if(bytesPerLine % 4 != 0) {
    bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
  }

 // return imageData[(x * 3)  + bytesPerLine*y + (width* 3 * y) + color] = color;
  //finalImageData[(x*3) + bytesPerLine] = imageData[(x * 3)  + bytesPerLine + color];
  //finalImageData[(x*3) + bytesPerLine + 1] = imageData[(x * 3)  + bytesPerLine + 1];
  return imageData[(x * 3)  + bytesPerLine + color];

}
