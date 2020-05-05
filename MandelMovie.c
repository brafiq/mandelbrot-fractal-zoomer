/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    //YOUR CODE HERE
    for (u_int64_t count=0; count < framecount; count++) {
      double new_scale = initialscale * pow((finalscale / initialscale), (double) count/(framecount - 1));
      Mandelbrot(threshold, max_iterations, center, new_scale, resolution, output[count]);
    }
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function:
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order.

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	//YOUR CODE HERE

  if (argc != 11) { return 1; }

  //store variables
  double threshold = atof(argv[1]);
  u_int64_t maxiterations = (u_int64_t)atoi(argv[2]);
  ComplexNumber* center = newComplexNumber(atof(argv[3]), atof(argv[4]));
  double initialscale = atof(argv[5]);
  double finalscale = atof(argv[6]);
  int framecount = atoi(argv[7]);
  u_int64_t resolution = (u_int64_t)atoi(argv[8]);
  char* output_folder = argv[9];
  char* colorfile = argv[10];
  u_int64_t size = 2 * resolution + 1;
  u_int64_t num_pixels_per_frame = size * size;

  if (threshold <= 0 || maxiterations <= 0 || initialscale <= 0 || finalscale <= 0) { return 1; }
  if (framecount > 10000 || framecount <= 0) { return 1; }
  if (resolution <= 0) { return 1; }
  //TODO: check if color file not specified properly
  if (framecount == 1 && (initialscale != finalscale)) { return 1; }

	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space.
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	//YOUR CODE HERE
  //get our colorcount and colormap using colorfile
  int* colorcount = (int*) malloc(sizeof(int));
  uint8_t** colormap = FileToColorMap(colorfile, colorcount);
  if (colormap == NULL) {
    free(colorcount);
    return 1;
  }

  int colors = *colorcount;

  //allocate memory for an an array containing arrays for iterations of each frame
  u_int64_t** output =(u_int64_t**) malloc(framecount * sizeof(u_int64_t*));
  if (output == NULL) {
    for (int i=0; i<colors; i++) {
      free(colormap[i]);
    }
    free(colormap);
    free(colorcount);
    freeComplexNumber(center);
    return 1;
  }

  //allocate memory for num of pixels in each frame
  for (int i=0; i<framecount; i++) {
    output[i] = (u_int64_t*) malloc(num_pixels_per_frame * sizeof(u_int64_t));
    if (output[i] == NULL) {
      for (int k=0; k<i; k++) {
        free(output[k]);
      }
      free(output);
      for (int j=0; j<colors; j++) {
        free(colormap[j]);
      }
      free(colorcount);
      free(colormap);
      freeComplexNumber(center);
      return 1;
    }
  }

  //generate thresholds of each frame
  MandelMovie(threshold, maxiterations, center, initialscale, finalscale, framecount, resolution, output);

	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/

	//YOUR CODE HERE

  // for each frame in output, generate ppm6 file by matching mandel iters of each pixel to a color, store file in output_folder
  for (u_int64_t frame=0; frame<framecount; frame++) {

      //make empty .ppm P6 file in output_folder
      char newFileName[strlen(output_folder) + 50];
      sprintf(newFileName, "%s/frame%05d.ppm", output_folder, frame);

    	//store initial file pointer
    	FILE* newFile = fopen(newFileName, "w+");

      //print metadata
      fprintf(newFile, "P6 %u %u 255\n", size, size);

      //print the color corresponding to each iteration color

    	// for each iter val in a frame
    	for (u_int64_t iter_index = 0; iter_index < (2 * resolution + 1) * (2 * resolution + 1); iter_index++) {
        //get iterVal
        u_int64_t iterVal = output[frame][iter_index];
        if (iterVal == 0) {
          fprintf(newFile, "%c%c%c", (uint8_t)0, (uint8_t)0, (uint8_t)0);
        } else {
          //get color index
          int colorIndex = (iterVal-1) % colors;
          uint8_t r = colormap[colorIndex][0];
      		uint8_t g = colormap[colorIndex][1];
      		uint8_t b = colormap[colorIndex][2];
          fprintf(newFile, "%c%c%c", r, g, b);
        }
    	}
      fclose(newFile);
  }

	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	//YOUR CODE HERE
  // free memory, close file
  for (int i=0; i<colors; i++) {
    free(colormap[i]);
  }
  for (int j=0; j<framecount; j++) {
    free(output[j]);
  }
  free(colormap);
  free(colorcount);
  free(output);
  freeComplexNumber(center);
	return 0;
}
