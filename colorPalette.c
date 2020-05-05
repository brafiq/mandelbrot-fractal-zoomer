/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{

	//store initial file pointer
	FILE* output = fopen(outputfile, "w");

  //malformed file check, return null if it is
  if (output == NULL) {
		fclose(output);
    return 1;
  }
	//set up colorcount, colorarray, and vertical
	int* colorcount = (int*) malloc(sizeof(int));
	uint8_t** colorarray = FileToColorMap(colorfile, colorcount);
	int colors = *colorcount;
	free(colorcount);
	int vert_height = heightpercolor * colors;

	//bounds check
	if (width < 1 || colors < 1 || heightpercolor < 1) { return 1; }

	//print metadata
	fprintf(output, "P3 %u %u 255\n", width, vert_height);

	// for each color
	for (int num_color = 0; num_color < colors; num_color++) {

		int r = colorarray[num_color][0];
		int g = colorarray[num_color][1];
		int b = colorarray[num_color][2];
		free(colorarray[num_color]);

		//for each row in a specific color
		for (int color_height = 0; color_height < heightpercolor; color_height++) {

			//for each column of a specific color
			for (int width_index = 0; width_index < width - 1; width_index++) {
				fprintf(output, "%u %u %u ", r, g, b);
			}
			fprintf(output, "%u %u %u\n", r, g, b);
		}
	}
	// free memory close file
	//for (int i=0; i<colorcount; i++) {
	//	free(colorarray[i]);
	//}
	free(colorarray);
	fclose(output);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	//store initial file pointer
	FILE* output = fopen(outputfile, "w");

  //malformed file check, return null if it is
  if (output == NULL) {
		fclose(output);
    return 1;
  }
	//set up colorcount, colorarray, and vertical
	int* colorcount = (int*) malloc(sizeof(int));
	uint8_t** colorarray = FileToColorMap(colorfile, colorcount);
	int colors = *colorcount;
	free(colorcount);
	int vert_height = heightpercolor * colors;

	//bounds check
	if (width < 1 || colors < 1 || heightpercolor < 1) { return 1; }

	//print metadata
	fprintf(output, "P6 %u %u 255\n", width, vert_height);

	// for each color
	for (int num_color = 0; num_color < colors; num_color++) {

		int r = colorarray[num_color][0];
		int g = colorarray[num_color][1];
		int b = colorarray[num_color][2];
		free(colorarray[num_color]);

		//for each row in a specific color
		for (int color_height = 0; color_height < heightpercolor; color_height++) {

			//for each column of a specific color
			for (int width_index = 0; width_index < width; width_index++) {
				fprintf(output, "%c%c%c", r, g, b);
			}
		}
	}
	// // free memory close file
	// for (int i=0; i<colors; i++) {
	// 	free(colorarray[i]);
	// }
	free(colorarray);
	fclose(output);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)+1];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}
