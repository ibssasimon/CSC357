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
tagBITMAPFILEHEADER biggerFHeader;

tagBITMAPINFOHEADER biggerIHeader;
tagBITMAPINFOHEADER smallerIHeader;

unsigned char* biggerData;
unsigned char* smallerData;

// function declarations
unsigned char getColor(unsigned char* imageData, int width, int x, int y, int color);
unsigned char getColorBilinear(unsigned char* imageData, int width, int height, float x, float y, int color);

int main(int argc, char *argv[]) {
  printf("%d\n", argc);
  FILE *file = fopen("lion.bmp","rb");
  FILE *file2 = fopen("jar.bmp", "rb");
  
  int x;
  int y;
  double ratio;
  ratio = 0.5;

  // null pointer check
  if(file == NULL) {
    return 0;
  }

  if(file2 == NULL) {
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
  fread(&fileHeader2.bfType, 1, sizeof(fileHeader2.bfType),  file2);
  fread(&fileHeader2.bfSize, 1, sizeof(fileHeader2.bfSize), file2);
  fread(&fileHeader2.bfReserved1, 1, sizeof(fileHeader2.bfReserved1), file2);
  fread(&fileHeader2.bfReserved2, 1, sizeof(fileHeader2.bfReserved2), file2);
  fread(&fileHeader2.bfOffBits, 1, sizeof(fileHeader2.bfOffBits), file2);


  // read image header data for both images
  fread(&infoHeader, 1, sizeof(tagBITMAPINFOHEADER), file);
  fread(&infoHeader2, 1, sizeof(tagBITMAPINFOHEADER), file2);


  //fseek(file, fileHeader.bfOffBits, SEEK_SET);
  unsigned char* imageData = (unsigned char*)malloc(infoHeader.biSizeImage);
  unsigned char* imageData2 = (unsigned char*)malloc(infoHeader2.biSizeImage);
  unsigned char* finalImageData;

  fread(imageData, 1, infoHeader.biSizeImage, file);
  fread(imageData2, 1, infoHeader2.biSizeImage, file2);

  fclose(file);
  fclose(file2);

  // check size of infoHeader's image size for both files before looping

  if(infoHeader.biWidth >= infoHeader2.biWidth) {
    biggerIHeader = infoHeader;
    smallerIHeader = infoHeader2;
    finalImageData = (unsigned char*)malloc(biggerIHeader.biSizeImage);
    biggerData = imageData;
    smallerData = imageData2;
  } else {
    biggerIHeader = infoHeader2;
    smallerIHeader = infoHeader;
    finalImageData = (unsigned char*)malloc(biggerIHeader.biSizeImage);
    biggerData = imageData2;
    smallerData = imageData;
  }



  unsigned int index = 0;
  for(y = 0; y < biggerIHeader.biHeight; y++) {

    int bytesPerLine = y * infoHeader.biWidth * 3;
    if(bytesPerLine % 4 != 0) {
      bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
    }

    for(x = 0; x < biggerIHeader.biWidth; x++) {


      // getting pixels of bigger image
      
      unsigned char b1 = getColor(biggerData, biggerIHeader.biWidth, x, y, 0);
      unsigned char g1 = getColor(biggerData, biggerIHeader.biWidth, x, y, 1);
      unsigned char r1 = getColor(biggerData, biggerIHeader.biWidth, x, y, 2);

      //UPDATE SECOND IMAGE
      // getting small image coordinates and pixels



      int small_x = x * ((float)smallerIHeader.biWidth / (float)biggerIHeader.biWidth);
      int small_y = y * ((float)smallerIHeader.biHeight / (float)biggerIHeader.biHeight);

     

      float x2 = small_x * (float)x;
      float y2 = small_y * (float)y;

      unsigned char b2 = getColorBilinear(smallerData, smallerIHeader.biWidth, smallerIHeader.biHeight,small_x, small_y, 0);
      unsigned char g2 = getColorBilinear(smallerData, smallerIHeader.biWidth, smallerIHeader.biHeight, small_x, small_y, 1);
      unsigned char r2 = getColorBilinear(smallerData, smallerIHeader.biWidth, smallerIHeader.biHeight, small_x, small_y, 2);



      // DO I MIX HERE FOR BILINEAR?

      // Blend image - use ratio to manipulate original pixels

      unsigned char blue_result = (b2 * ratio) + b1*(1 - ratio);
      unsigned char green_result = (g2 * ratio) + g1*(1 - ratio);
      unsigned char red_result = (r2 * ratio) + r1*(1 - ratio);


      // assign into final image data

      finalImageData[(x * 3)  +  y*biggerIHeader.biWidth*3 + 0] = blue_result;
      finalImageData[(x * 3)  + y*biggerIHeader.biWidth*3 + 1] = green_result;
      finalImageData[(x * 3)  + y*biggerIHeader.biWidth*3 + 2] = red_result;

    }
  }

  // allocate result image mem
  file = fopen("resultimage.bmp", "wb+");

  if(fileHeader.bfSize > fileHeader2.bfSize) {
    biggerFHeader = fileHeader;
    
  } else {
    biggerFHeader = fileHeader2;
  }


  fwrite(&biggerFHeader.bfType, 1, sizeof(biggerFHeader.bfType),  file);
  fwrite(&biggerFHeader.bfSize, 1, sizeof(biggerFHeader.bfSize), file);
  fwrite(&biggerFHeader.bfReserved1, 1, sizeof(biggerFHeader.bfReserved1), file);
  fwrite(&biggerFHeader.bfReserved2, 1, sizeof(biggerFHeader.bfReserved2), file);
  fwrite(&biggerFHeader.bfOffBits, 1, sizeof(biggerFHeader.bfOffBits), file);


  fwrite(&biggerIHeader, 1, sizeof(tagBITMAPINFOHEADER), file);

  fwrite(finalImageData, biggerIHeader.biSizeImage, 1, file);
  fclose(file);

  
  
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

unsigned char getColorBilinear(unsigned char* imageData, int width, int height, float x, float y, int color) {
  // width  check

  if(x >= width - 1 || y >= height - 1) {
    return 0;
  }

  // assign x and y ix, iy: ix = x; x = x - ix;
  int ix = x;
  int iy = y;

  x = x - ix;
  y = y - iy;


  // padding check (bytes per line stuff)
  int bytesPerLine = width * 3;
  if(bytesPerLine % 4 != 0) {
    bytesPerLine = bytesPerLine + (4 - (bytesPerLine % 4));
  }

  // access four pixels 
  unsigned char colorTopLeft = imageData[(ix * 3) + (iy * 3 * width) + color + bytesPerLine]; // top left
  unsigned char colorTopRight = imageData[((ix+ 1) * 3) + (iy * 3 * width) + color + bytesPerLine]; // top right
  unsigned char colorBottomLeft = imageData[(ix * 3) + ((iy+1) * 3 * width) + color + bytesPerLine]; // bottom left
  unsigned char colorBottomRight = imageData[((ix+ 1) * 3) + ((iy+1) * 3 * width) + color + bytesPerLine]; // bottom right corner


  // are my dx and dy ratios correct?
  //float dx = ix / (ix + 1);
  //float dy = (iy + 1) / iy;

  unsigned char colorLeft = colorTopLeft * (1 - y) + colorBottomLeft * y;
  unsigned char colorRight = colorTopRight * (1 - y) + colorBottomRight * y;
  unsigned char colorResult = colorLeft * (1 - x) + colorRight * x;

  return colorResult;
}
