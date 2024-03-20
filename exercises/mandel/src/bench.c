#include <stdlib.h>
#include <stdio.h>

// log() 
#include <math.h>

// printf()
#include <stdio.h>

// file operations: open() etc
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// write()
#include <unistd.h>

// strcp() 
#include <string.h>

// gettimeofday()
#include <sys/time.h>

int save_to_file(double x0, double y0, double incr, int, int, int, char*);

int probe(double r, double i, int max);

void color(int depth, int max, char *out);

int ppm_write(char*,char*, int, int, int, char*);

int main(int argc, const char * argv[]) {

  int width = 1920;
  int height = 1080;
  int depth = 1024;
  double x0 = -0.14;    
  double y0 =  0.8422;
  double incr = 0.0000035;

  struct timeval t0,t1,t2,t3,t4;
  
  // The arguments to the program, specifies the image.
  if(argc < 7) {
    printf("usage: mandelbrot <width> <height> <depth> <x-upper> <y-left> <k-incr>\n");
    printf("will use deafult values: 1920 1080 1024 -0.14 0.8422 0.0000035\n");
  } else {
    width = atoi(argv[1]);
    height = atoi(argv[2]);    
    depth = atoi(argv[3]);
    x0 = atof(argv[4]);        
    y0 = atof(argv[5]);
    incr = atof(argv[6]);
  }

  printf("generating mandelbrot: width %d, height %d, depth %d, upper left corner (%f,%f), step: %f\n", width, height, depth, x0 , y0, incr);  
  
  // We're building a rgb image and need three bytes per pixel.

  gettimeofday(&t0, NULL);

  // The host buffer to where we generate the image
  char image[height][width][3];
  
  gettimeofday(&t1, NULL);

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      double xi = x0+incr*x;
      double yi = y0-incr*y;
      color(probe(xi, yi, depth), depth, &image[y][x][0]);
    }
  }

  gettimeofday(&t2, NULL);

  // printf("Saving image to file .\n");
  
  ppm_write("image.ppm", "mandelbrot", width, height, 255, &image[0][0][0]);
  
  gettimeofday(&t3, NULL);

  printf("Platform setup in %ld ms\n", (t1.tv_sec - t0.tv_sec)*1000 + (t1.tv_usec - t0.tv_usec)/1000);  
  printf("Image rendered in %ld ms\n", (t2.tv_sec - t1.tv_sec)*1000 + (t2.tv_usec - t1.tv_usec)/1000);
  printf("Saving file    in %ld ms\n", (t3.tv_sec - t2.tv_sec)*1000 + (t3.tv_usec - t2.tv_usec)/1000);
  printf("Total time        %ld ms\n", (t3.tv_sec - t0.tv_sec)*1000 + (t3.tv_usec - t0.tv_usec)/1000);    

  return 0;
}

int probe(double r, double i, int max) {

  double zr = 0.0;   
  double zi = 0.0;    

  int depth = 0;

  while(depth < max) {
    double zr2 = zr*zr;
    double zi2 = zi*zi;
    double a2 = zr2 + zi2;
    if(a2 < 4.0) {
      depth++;
      zi = 2*zr*zi + i;
      zr = zr2 - zi2 + r;
    } else {
      return depth;
    }
  }
  return 0;
}


void set(char *out, int r, int g, int b) {
	 out[0] = r;
	 out[1] = g;
	 out[2] = b;
}

void color(int depth, int max, char *out) {

  // depth is in the span [0 - (max-1)]

  double f = ((double)depth)/max;

  double a = f * 5;
  
  int x = trunc(a);	          // x is [0,1,2,3,4]
  
  int y = trunc(255 * (a - x));    // y is [0-255]

  
  switch(x) {
  case 0 :
    set(out, y, 0, 0);    
    break;
  case 1 :
    set(out, 255, y, 0);  
    break;
  case 2 :
    set(out, 255-y, 255, 0);  
    break;
  case 3 :
    set(out, 0, 255, y);  
    break;
  case 4 :
    set(out, 0, 255-y, 255);  
    break;
  } 
}


int ppm_write(char *filepath, char *descr, int width, int height, int colors, char *image) {


  // Open the output file for reading and writing.
  int fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    
  if (fd== -1) {
    printf("erro ppm: failed to open file %s\n", filepath);
    return -1;
  }

  dprintf(fd, "P6\n");
  dprintf(fd, "#%s\n", descr);
  dprintf(fd, "%d %d\n", width, height);
  dprintf(fd, "%d\n", colors);  
  for(int y = 0; y < height; y++) {
      for(int x = 0; x < width; x++) {
	ssize_t res = write(fd, (image + (y*width*3)+(x*3)), 3);
      }
  }
  close(fd);
}




  


