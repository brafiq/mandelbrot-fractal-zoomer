/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
  //store initial file pointer
  FILE* fp = fopen(colorfile, "r");

  // //malformed file check, return null if it is
  if (fp == NULL) {
    return NULL;
  }

  //set colorcount to first integer in colorfile
  if (fscanf(fp, "%u", colorcount) != 1) {
    return NULL;
  }

  int colors = *colorcount;
  if (colors == 0) { return NULL; }

  //malloc 2D array
  uint8_t** colorarray = (uint8_t**) malloc(*colorcount * sizeof(uint8_t*));

  //prepare rgb per colorval
  uint8_t* r = (uint8_t*) malloc(sizeof(uint8_t));
  uint8_t* g = (uint8_t*) malloc(sizeof(uint8_t));
  uint8_t* b = (uint8_t*) malloc(sizeof(uint8_t));

  // store color values in each index
  for(int count=0; count < colors; count++) {

    // create and store space for a colorval containing three color ints
    colorarray[count] = (uint8_t*) malloc(3 * sizeof(uint8_t));

    //get three rgb vals
    if ((fscanf(fp, "%u %u %u", (unsigned int*)r, (unsigned int*)g, (unsigned int*)b) == 3)){

      if (*r > 255 || *r < 0 || *g > 255 || *g < 0 || *b > 255 || *b < 0) {
        free(r);
        free(g);
        free(b);
        for (int i=0; i<count; i++) {
          free(colorarray[i]);
        }
        free(colorarray);
        fclose(fp);
        return NULL;
      }
      colorarray[count][0] = *r;
      colorarray[count][1] = *g;
      colorarray[count][2] = *b;

    } else {
      // free memory close file
      for (int i=0; i<count; i++) {
        free(colorarray[i]);
      }
      free(r);
      free(g);
      free(b);
      free(colorarray);
      fclose(fp);
      return NULL;
    }
  }
  // close file
  free(r);
  free(g);
  free(b);
  fclose(fp);
  return colorarray;
}

// int main(int argc, char** argv) {
//   int colorcount;
//   uint8_t** colors = FileToColorMap("minicolormap.txt", &colorcount);
//   for(int i=0; i < colorcount; i++) {
//     printf("\n%d\n", colors[i][0]);
//   }
//   return 0;
// }
