/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
  //YOUR CODE HERE
  ComplexNumber *z = newComplexNumber(0, 0);
  u_int64_t num_iters = 0;
  while (ComplexAbs(z) < threshold && num_iters < maxiters) {
    ComplexNumber *squared = ComplexProduct(z, z);
    freeComplexNumber(z);
    z = ComplexSum(squared, point);
    freeComplexNumber(squared);
    num_iters++;
  }
  freeComplexNumber(z);
  if (num_iters == maxiters) {
    return 0;
  }
  return num_iters;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){

  //YOUR CODE HERE
  //scale = distance from center pixel to top pixel
  //height and width = scale * 2c
  //bottom-left corner will evaluate the Mandelbrot function at C = (center - (scale + scale * i))
  double pixel_width = scale / resolution;
  double row_length = 2*resolution + 1;
  double total_pixels = row_length * row_length;
  double init_imaginary = Im(center) + scale;
  double init_real = Re(center) - scale;
  double new_imag = init_imaginary;
  int row_count = 0;

  //for each row
  for (int row=0; row < total_pixels; row+=row_length) {

    //for each column in a row
    for (int column=0; column < row_length; column+=1) {

      //make pixel in column
      ComplexNumber *newPixel = newComplexNumber(init_real + (column * pixel_width), new_imag);

      //calculate and set Mandelbrot iterations of newPixel in correct index of output
      output[row + column] = MandelbrotIterations(max_iterations, newPixel, threshold);

      //free pixel we just used
      freeComplexNumber(newPixel);
    }
    row_count+=1;
    //go down a row
    new_imag = init_imaginary - (pixel_width * row_count);
  }
}
