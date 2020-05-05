# Mandelbrot Fractal Zoomer

CS61C: Computer Architecture & Machine Structures, Fall 2019

Spec Link: https://cs61c.org/fa19/projects/proj1/

## Background

In this project, you will be implementing the Mandelbrot function and translating the result to an image! As your first major foray into the world of C programming, we hope to give you practice working with structs, pointers, memory, and more. The project will be split into 2 parts: 1) evaluating the Mandelbrot function given some inputs, and 2) creating images from the Mandelbrot output.

The core idea behind this project is the following function:

### M(Z,C)=Z^2+C

… and how this behaves if you start with Z as zero (i.e., 0 + 0i) and iterate on itself. (Z and C are assumed to be complex numbers.) That means after you get a value out, you stick it back in as Z and do it again, and again, etc. C always stays the same across iterations, though we are interested in how different C’s affect how the function behaves. If the output stays bounded (the absolute value of the complex number stays less than an arbitrary threshold, which we choose to be 2), then it’s in the Mandelbrot Set! If it isn’t, then what’s interesting to calculate is how many times does it needs to iterate until it surpasses the threshold.

Let’s start with an example. Let’s say C=1. Now let’s see how it behaves with Z=0+0i and iterate.

    - M(0,C=1)=02+1=1 Now, let’s set that to be our new Z and keep going:

    - M(1,C=1)=12+1=2 Now, let’s set that to be our new Z and keep going:

    - M(2,C=1)=22+1=5 At this point, the result is bigger than 2, and if we keep going it becomes 26 and even bigger!

So that means C=1 is NOT in the set, since it blows up. If we were to write this more succinctly, we would say that the Z values for iterations for C=1 starting with Z=0 are the following: 0−>1−>2−>5. So it went through 3 iterations (there were three -> arrows) until we got farther than 2 away from the origin.


## Testing

Inputs are provided on command line; read through the provided function printUsage for the expected sequence of inputs. Most inputs correspond to the ones in void MandelMovie. The ones which don’t are:

    - outputfolder: This is a folder in which you will store the output images.

    - colorfile: This is a file containing information on what colors to use for the output.

The goal of this main function is to run MandelMovie with the given command line arguments, convert each iteration image into a color image using the mapping provided in colorfile, and output each color image into files in outputfolder.

We suggest using the following general steps for this function:

    1. Check inputs to confirm that they are valid, then convert them into variables.

    2. Run MandelMovie.

    3. Convert from iteration count to colors, and output the results into output files.

Your code should return early with return value 1 in the event that the input is incorrect. This includes when, but is not limited to:

    1. The number of arguments provided does not match the needed number.

    2. threshold, maxiterations, or scale is 0 or less.

    3. Insufficient memory for output. (Because of this, you should allocate enough memory for output in your main function).

    4. The number of frames exceeds 10,000, or is 0 or less.

    5. resolution is less than 0.

    6. colorfile is not properly formatted, as specified in the above description of colorfile.

    7. The number of frames is 1 AND initialscale != finalscale.

Note that the following are allowed:

    1. initialscale ≤ finalscale. In this case, the image will zoom out or stay constant.
    2. The number of frames is 1, and initialscale == finalscale

Outputs should be made in .ppm P6 (NOT P3) file format, with image names frame00000.ppm for the first frame, frame00001.ppm for the second frame, and so on. The function sprintf may come in handy here; sprintf works the same as printf and fprintf, but instead outputs into a string.

### It is recommended to use the main function found in MandelFrame.c as a starting point, since it follows a similar procedure to run Mandelbrot and store the output.

Pixels with iteration value 0 will be colored with black (RGB value 0,0,0). Pixels with iteration value 1 through len(colormap) will be colored with their corresponding colormap color. For pixels with greater values, the colors used will loop; thus, if colormap = ((255,0,0), (255,255,0), (0,255,0), (0,255,255), (0,0,255), (255,0,255)), pixels with value 1, 7, 13, … will be colored red (RGB value 255,0,0), pixels with value 2,8,14, … will be colored yellow (RGB value 255,255,0), etc.

### Lastly, we provided a method convert from your sequence of ppm files into an animation

In order to generate the video, run ffmpeg -framerate 24 -i student_output/partB/frame%05d.ppm output.mp4 in your project folder. In order to actually view the movie file, we recommend you git push and download the video to your own computer so you can open the video with whatever video software you have. Windows computers may have difficulties opening .mp4 files; in this case, you can view your video by uploading a private or unlisted video to Youtube.

## Testing & Running Your Code
### Part A
Run make testA to run a small case for part A, and run make mem-checkA to check for memory leaks.

make testASimple runs a small test (threshold 2, center 5+3i, scale 5, resolution 2, similar to the example table above), which is useful for debugging.

Unless you have Valgrind installed locally, you need to run the memory check on the Hive machines.

For debugging you can use CGDB on Mandelframe. You will need to set the initial args. Use set <args>

### Part B.1
make testB1Small will run a small case for part B.1, and make testB1Big will run a normal case for part B.1. We also provide a memory check by running make memcheckB1.

### Part B.3
Run make testB2Small to run a small case for part B, and run make mem-checkB2Small to check for memory leaks.

Run make testB2 to run an average case for part B, and run make mem-checkB2 to check for memory leaks.

After you’ve unzipped the tests (if you pulled PartB after 9pm on 9/19), make BigTest will run a giant test of part B. It is suggested to run this on the hive computer, as this test may take up to an hour to run.
