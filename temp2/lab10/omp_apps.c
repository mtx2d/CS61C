#include "omp_apps.h"

/* -------------------------------Utilties, Do Not Modify------------------------------*/
double* gen_array(int n) {
  double* array = (double*)malloc(n * sizeof(double));
  for (int i = 0; i < n; i++) array[i] = drand48();
  return array;
}

int verify(double* x, double* y, void(*funct)(double *x, double *y, double *z)) {
  double *z_v_add = (double*) malloc(ARRAY_SIZE*sizeof(double));
  double *z_oracle = (double*) malloc(ARRAY_SIZE*sizeof(double));
  (*funct)(x, y, z_v_add);
  for(int i=0; i<ARRAY_SIZE; i++){
    z_oracle[i] = x[i] + y[i];
  }
  for(int i=0; i<ARRAY_SIZE; i++){
    if(z_oracle[i] != z_v_add[i])
      return 0; 
  }
  return 1;
}

/* -------------------------------Vector Addition------------------------------*/
/* 
Exercise 2 comments
- Why do we have #pragma omp parallel and #pragma omp for directives?
- Could we just have #pragma omp for directive alone?
  - When should we use each directive? Together or separate?
*/

// BEGIN PART 1 EX 2
void v_add_naive(double* x, double* y, double* z) {
  #pragma omp parallel
  {
    for(int i=0; i<ARRAY_SIZE; i++)
      z[i] = x[i] + y[i];
  }
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
  // TODO: Modify this function
  // Do NOT use the `for` directive here!

  #pragma omp parallel
  {
    int numThreads = omp_get_num_threads();
    int threadNum = omp_get_thread_num();
    for(int i = threadNum; i<ARRAY_SIZE; i += numThreads)
      z[i] = x[i] + y[i];
  }
}

// Chunks Method
void v_add_optimized_chunks1(double* x, double* y, double* z) {
  // TODO: Modify this function
  // Do NOT use the `for` directive here!
  #pragma omp parallel
  {
    int numThreads = omp_get_num_threads();
    int threadNum = omp_get_thread_num();
    int chunkSize = ARRAY_SIZE/numThreads;

    for(int i = threadNum * chunkSize; i < threadNum * chunkSize + chunkSize; i++)
      z[i] = x[i] + y[i];
    for (int i = numThreads * chunkSize; i < ARRAY_SIZE; i++) {
      z[i] = x[i] + y[i];
    }
  }
}

void v_add_optimized_chunks(double* x, double* y, double* z) {
  // TODO: Modify this function
  // Do NOT use the `for` directive here!
  #pragma omp parallel
  {
    int numThreads = omp_get_num_threads();
    int threadNum = omp_get_thread_num();
    int chunkSize = ARRAY_SIZE/numThreads;

    for(int i = 0; i < chunkSize; i++) {
      //int index = threadNum * ARRAY_SIZE/numThreads + i;--why is this version of index so much slower?
      //int index = threadNum * chunkSize + i;
      //z[index] = x[index] + y[index];
      z[threadNum * chunkSize + i] = x[threadNum * chunkSize + i] + y[threadNum * chunkSize + i];
      //z[index] = x[index] + y[index];
    }
    for (int i = ARRAY_SIZE/numThreads * numThreads; i < ARRAY_SIZE; i++) {
      z[i] = x[i] + y[i];
    }
  }
}

// END PART 1 EX 2

/* -------------------------------Dot Product------------------------------*/
// BEGIN PART 1 EX 3
double dotp_naive(double* x, double* y, int arr_size) {
  double global_sum = 0.0;
#pragma omp parallel
  {
    #pragma omp for
    for (int i = 0; i < arr_size; i++)
      #pragma omp critical
      global_sum += x[i] * y[i];
  }
  return global_sum;
}


/*
Is this understanding correct?
- We want to parallelly compute the sum of two arrays
- Threads can't all modify global sum at the same time (no guarantee that they're working w/ most updated version)
- Naive solution: each thread has to wait for thread currently w/ access to global_sum to finish adding to it
  - Is this analogous to a lock?
- Manual Optimized: does #pragma omp for automatically divide the tasks b/w the threads?
  - That is, if there are 8 threads --> the directive says you thread 1: add this chunk of the array, thread 2:
    add this (other) chunk of the array, etc.?
    - Then in the end, only need to aggregate each of the threads' sub-sums together serially
- Reduction optimized: what exactly does the reduction key word do?
  - Does it just prevent the "race condition?" What specifically is going on?
*/
// Manual Reduction
double dotp_manual_optimized(double* x, double* y, int arr_size) {
  // TODO: Modify this function
  // Do NOT use the `reduction` directive here!
  double global_sum = 0.0;
  #pragma omp parallel
  {
    double temp = 0;
    #pragma omp for
    for (int i = 0; i < arr_size; i++)
      temp += x[i] * y[i];
    #pragma omp critical
    global_sum += temp;
  }
  return global_sum;
}

// Reduction Keyword
double dotp_reduction_optimized(double* x, double* y, int arr_size) {
  // TODO: Modify this function
  // Please DO use the `reduction` directive here!
  // reduction([operation]: [shared variable])
  double global_sum = 0.0;
  #pragma omp parallel
  {
    #pragma omp for reduction(+ : global_sum)
    for (int i = 0; i < arr_size; i++)
      //#pragma omp critical
      global_sum += x[i] * y[i];
  }
  return global_sum;
}
// END PART 1 EX 3

char* compute_dotp(int arr_size) {
  // Generate input vectors
  char* report_buf = (char*)malloc(BUF_SIZE), *pos = report_buf;
  double start_time, run_time;

  double *x = gen_array(arr_size), *y = gen_array(arr_size);
  double serial_result = 0.0, result = 0.0;

  // calculate result serially
  for (int i = 0; i < arr_size; i++) {
    serial_result += x[i] * y[i];
  }

  int num_threads = omp_get_max_threads();
  for (int i = 1; i <= num_threads; i++) {
    omp_set_num_threads(i);
    start_time = omp_get_wtime();
    for (int j = 0; j < REPEAT; j++) result = dotp_manual_optimized(x, y, arr_size);
    run_time = omp_get_wtime() - start_time;
    pos += sprintf(pos, "Manual Optimized: %d thread(s) took %f seconds\n", i, run_time);

    // verify result is correct (within some threshold)
    if (fabs(serial_result - result) > 0.001) {
      pos += sprintf(pos, "Incorrect result!\n");
      *pos = '\0';
      return report_buf;
    }
  }

  for (int i = 1; i <= num_threads; i++) {
    omp_set_num_threads(i);
    start_time = omp_get_wtime();

    for (int j = 0; j < REPEAT; j++) {
      result = dotp_reduction_optimized(x, y, arr_size);
    }

    run_time = omp_get_wtime() - start_time;
    pos += sprintf(pos, "Reduction Optimized: %d thread(s) took %f seconds\n",
                   i, run_time);

    // verify result is correct (within some threshold)
    if (fabs(serial_result - result) > 0.001) {
      pos += sprintf(pos, "Incorrect result!\n");
      *pos = '\0';
      return report_buf;
    }
  }

  // Only run this once because it's too slow..
  omp_set_num_threads(1);
  start_time = omp_get_wtime();
  for (int j = 0; j < REPEAT; j++) result = dotp_naive(x, y, arr_size);
  run_time = omp_get_wtime() - start_time;

  pos += sprintf(pos, "Naive: %d thread(s) took %f seconds\n", 1, run_time);
  *pos = '\0';
  return report_buf;
}


/* ---------------------Image Processing: Sobel Edge Detector----------------------*/
int sobel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
void sobel_filter(bmp_pixel **src, bmp_pixel **dst, int row, int col) {
   int res = 0;
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         bmp_pixel pxl = src[row - 1 + i][col - 1 + j];
         res += ((int)pxl.blue + (int)pxl.green + (int)pxl.red) * sobel[i][j];
      }
   }
   res *= 2;    // scale a little bit so the result image is brighter.
   res = res < 0? 0 : (res > 255? 255 : res);
   bmp_pixel_init(&dst[row][col], res, res, res);
}

char *image_proc(const char* filename) {
   bmp_img img, img_copy;
   if (bmp_img_read(&img, filename) != 0)
      return 0;

   char *res= (char*)calloc(32, sizeof(char));
   strncat(res, filename, strlen(filename) - 4);
   strcat(res, "_sobel.bmp");

   bmp_img_read(&img_copy, filename);

   unsigned int wid = img.img_header.biWidth;
   unsigned int hgt = img.img_header.biHeight;
   bmp_img_init_df(&img_copy, wid, hgt);

   // To parallelize these for loops, check out scheduling policy: http://jakascorner.com/blog/2016/06/omp-for-scheduling.html
   // and omp collapse directive https://software.intel.com/en-us/articles/openmp-loop-collapse-directive
   for (int i = 1; i < hgt-1; i++) {
      for (int j = 1; j < wid-1; j++) {
         sobel_filter(img.img_pixels, img_copy.img_pixels, i, j);
      }
   }
   bmp_img_write(&img_copy, res);
   bmp_img_free(&img_copy);
   bmp_img_free(&img);
   return res;
}