/* Simon Ibssa, CPE 357, Cal Poly SLO*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

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

FILE* file;

// function declarations
unsigned char getColor(unsigned char* imageData, int width, int x, int y, int color);



int main(int argc, char *argv[]) {

  // clock start
  clock_t start = clock();
  char* err;


  if(argc == 5) {
    file = fopen(argv[1],"rb");
  } else {
    printf("enter correct args: ./yourprogram [IMAGEFILE] [BRIGHTNESS] [PARALLEL] [OUTPUTFILE]\n");
    return 0;
  }
  
  
  int x;
  int y;
  double brightness = strtod(argv[2], &err);

  int parallel = strtol(argv[3], &err, 10);

  file = fopen(argv[1], "rb");

  // null pointer check
  if(file == NULL) {
    printf("File does not exist!\n");
    return 0;
  }

  // read file header data for image 1
  fread(&fileHeader.bfType, 1, sizeof(fileHeader.bfType),  file);
  fread(&fileHeader.bfSize, 1, sizeof(fileHeader.bfSize), file);
  fread(&fileHeader.bfReserved1, 1, sizeof(fileHeader.bfReserved1), file);
  fread(&fileHeader.bfReserved2, 1, sizeof(fileHeader.bfReserved2), file);
  fread(&fileHeader.bfOffBits, 1, sizeof(fileHeader.bfOffBits), file);

  // read image header data for both images
  fread(&infoHeader, 1, sizeof(tagBITMAPINFOHEADER), file);

  //fseek(file, fileHeader.bfOffBits, SEEK_SET);
  unsigned char* imageData = (unsigned char*)mmap(NULL, infoHeader.biSizeImage, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  unsigned char* finalImageData = (unsigned char*)mmap(NULL, infoHeader.biSizeImage, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  fread(imageData, 1, infoHeader.biSizeImage, file);
  fclose(file);

  // check here for even or odd number of rows for height. Child should be passed in bottom half of the picture


  if(parallel == 1) {
    // mechanism to link child and parent
    int *g;

    // determine bottom half height
    unsigned int bottomHalfLimit = infoHeader.biHeight / 2;
    
    if(fork() == 0) {
      // child case. brighten the top half of the image
      unsigned int index = 0;
      for(y = 0; y < bottomHalfLimit; y++) {

      int bytesPerLine = infoHeader.biWidth * 3;
      if(bytesPerLine % 4 != 0) {
        bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
      }

      for(x = 0; x < infoHeader.biWidth; x++) {
        // getting pixels of bigger image
        unsigned char b1 = getColor(imageData, infoHeader.biWidth, x, y, 0);
        unsigned char g1 = getColor(imageData, infoHeader.biWidth, x, y, 1);
        unsigned char r1 = getColor(imageData, infoHeader.biWidth, x, y, 2);

        int brightB1 = (int)b1;
        brightB1 = brightB1 + (brightness) * 255;

        int brightG1 = (int)g1;
        brightG1 = brightG1 + (brightness) * 255;

        int brightR1 = (int)r1;
        brightR1 = brightR1 + (brightness) * 255;

        if(brightB1 > 255) {
          brightB1 = 255;
        }

        if(brightG1 > 255) {
          brightG1 = 255;
        }

        if(brightR1 > 255) {
          brightR1 = 255;
        }

        brightB1 = (unsigned char)brightB1;
        brightG1 = (unsigned char)brightG1;
        brightR1 = (unsigned char)brightR1;
        // assign into final image data
        finalImageData[(x * 3)  + y * bytesPerLine + 0] = brightB1;
        finalImageData[(x * 3)  + y * bytesPerLine + 1] = brightG1;
        finalImageData[(x * 3)  + y * bytesPerLine + 2] = brightR1;
      }

    }
      return 1;
    } else {
      // parent case. 
      wait(&g);
      unsigned int index = 0;
      for(y = bottomHalfLimit; y < infoHeader.biHeight; y++) {

      int bytesPerLine = infoHeader.biWidth * 3;
      if(bytesPerLine % 4 != 0) {
        bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
      }

      for(x = 0; x < infoHeader.biWidth; x++) {
        // getting pixels of bigger image
        unsigned char b1 = getColor(imageData, infoHeader.biWidth, x, y, 0);
        unsigned char g1 = getColor(imageData, infoHeader.biWidth, x, y, 1);
        unsigned char r1 = getColor(imageData, infoHeader.biWidth, x, y, 2);

        int brightB1 = (int)b1;
        brightB1 = brightB1 + (brightness) * 255;

        int brightG1 = (int)g1;
        brightG1 = brightG1 + (brightness) * 255;

        int brightR1 = (int)r1;
        brightR1 = brightR1 + (brightness) * 255;

        if(brightB1 > 255) {
          brightB1 = 255;
        }

        if(brightG1 > 255) {
          brightG1 = 255;
        }

        if(brightR1 > 255) {
          brightR1 = 255;
        }

        brightB1 = (unsigned char)brightB1;
        brightG1 = (unsigned char)brightG1;
        brightR1 = (unsigned char)brightR1;
        // assign into final image data
        finalImageData[(x * 3)  + y * bytesPerLine + 0] = brightB1;
        finalImageData[(x * 3)  + y * bytesPerLine + 1] = brightG1;
        finalImageData[(x * 3)  + y * bytesPerLine + 2] = brightR1;
      }

    }

    }

  } else {
    unsigned int index = 0;
    for(y = 0; y < infoHeader.biHeight; y++) {

    int bytesPerLine = infoHeader.biWidth * 3;
    if(bytesPerLine % 4 != 0) {
      bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
    }

    for(x = 0; x < infoHeader.biWidth; x++) {
      // getting pixels of bigger image
      unsigned char b1 = getColor(imageData, infoHeader.biWidth, x, y, 0);
      unsigned char g1 = getColor(imageData, infoHeader.biWidth, x, y, 1);
      unsigned char r1 = getColor(imageData, infoHeader.biWidth, x, y, 2);

      int brightB1 = (int)b1;
      brightB1 = brightB1 + (brightness) * 255;

      int brightG1 = (int)g1;
      brightG1 = brightG1 + (brightness) * 255;

      int brightR1 = (int)r1;
      brightR1 = brightR1 + (brightness) * 255;

      if(brightB1 > 255) {
        brightB1 = 255;
      }

      if(brightG1 > 255) {
        brightG1 = 255;
      }

      if(brightR1 > 255) {
        brightR1 = 255;
      }

      brightB1 = (unsigned char)brightB1;
      brightG1 = (unsigned char)brightG1;
      brightR1 = (unsigned char)brightR1;
      // assign into final image data
      finalImageData[(x * 3)  + (y * bytesPerLine) + 0] = brightB1;
      finalImageData[(x * 3)  + (y * bytesPerLine) + 1] = brightG1;
      finalImageData[(x * 3)  + (y * bytesPerLine) + 2] = brightR1;
    }
  }
  }
  
  

  // allocate result image mem
  file = fopen(argv[4], "wb+");

  fwrite(&fileHeader.bfType, 1, sizeof(fileHeader.bfType), file);
  fwrite(&fileHeader.bfSize, 1, sizeof(fileHeader.bfSize), file);
  fwrite(&fileHeader.bfReserved1, 1, sizeof(fileHeader.bfReserved1), file);
  fwrite(&fileHeader.bfReserved2, 1, sizeof(fileHeader.bfReserved2), file);
  fwrite(&fileHeader.bfOffBits, 1, sizeof(fileHeader.bfOffBits), file);


  fwrite(&infoHeader, 1, sizeof(tagBITMAPINFOHEADER), file);

  fwrite(finalImageData, infoHeader.biSizeImage, 1, file);
  fclose(file);

  
  clock_t end = clock();

  double cpu_time_used = (double)(end-start);
  printf("program took: %f nanoseconds to run\n", cpu_time_used);
  return 0;
}


// function to manipulate image data
unsigned char getColor(unsigned char* imageData, int width, int x, int y, int color) {
  int bytesPerLine = width * 3;
  if(bytesPerLine % 4 != 0) {
    bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
  }

 // return imageData[(x * 3)  + bytesPerLine*y + (width* 3 * y) + color] = color;
  //finalImageData[(x*3) + bytesPerLine] = imageData[(x * 3)  + bytesPerLine + color];
  //finalImageData[(x*3) + bytesPerLine + 1] = imageData[(x * 3)  + bytesPerLine + 1];
  return imageData[(x * 3)  + y * bytesPerLine + color];

}