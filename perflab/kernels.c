/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
	"dog",              /* Team name */

	"fluorinedog",     /* First member full name */
	"fluorinedog@gmail.com",  /* First member email address */

	"guilingou",                   /* Second member full name (leave blank if none) */
	"guilingou@hustunique.com"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
static char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
	int i, j;

	for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
		dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */

static const int STRIP_I = 32;
static const int STRIP_J = 32;
static char rotate_old_descr[] = "rotate_old: past working version";
void rotate_old(int dim, pixel* restrict src, pixel* restrict dst) 
{
	int Ni, j;
	int dimnn = dim - 1;
	
	for (j = 0; j < dim; j+= STRIP_J) {
		for (Ni = 0; Ni < dim; Ni++ ){
			int stop_j = j + STRIP_J;
			int loc = dimnn - Ni;
			for(int Nj = j; Nj < stop_j; ++Nj){
				// dst[RIDX(dimnn-Nj, Ni, dim)] = src[RIDX(Ni, Nj, dim)];
				dst[RIDX(Ni, Nj, dim)] = src[RIDX(Nj, loc, dim)];
			}
		}
	}
}

static char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel* restrict src, pixel* restrict dst) 
{
	int Ni, j;
	int dimnn = dim - 1;
	
	for (j = 0; j < dim; j+= STRIP_J) {
		for (Ni = 0; Ni < dim; Ni++ ){
			int stop_j = j + STRIP_J;
			int loc = dimnn - Ni;
			for(int Nj = j; Nj < stop_j; ++Nj){
				// dst[RIDX(dimnn-Nj, Ni, dim)] = src[RIDX(Ni, Nj, dim)];
				dst[RIDX(Ni, Nj, dim)] = src[RIDX(Nj, loc, dim)];
			}
		}
	}
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
	add_rotate_function(&naive_rotate, naive_rotate_descr);   
	add_rotate_function(&rotate_old, rotate_old_descr);   
	add_rotate_function(&rotate, rotate_descr);   
	/* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
	int red;
	int green;
	int blue;
	int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
	sum->red = sum->green = sum->blue = 0;
	sum->num = 0;
	return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
	sum->red += (int) p.red;
	sum->green += (int) p.green;
	sum->blue += (int) p.blue;
	sum->num++;
	return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
	current_pixel->red = (unsigned short) (sum.red/sum.num);
	current_pixel->green = (unsigned short) (sum.green/sum.num);
	current_pixel->blue = (unsigned short) (sum.blue/sum.num);
	return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
	int ii, jj;
	pixel_sum sum;
	pixel current_pixel;

	initialize_pixel_sum(&sum);
	for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
		accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

	assign_sum_to_pixel(&current_pixel, sum);
	return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
	int i, j;

	for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
		dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{	
	int i, j;

	for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
		dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
	// add_smooth_function(&smooth, smooth_descr);
	// add_smooth_function(&naive_smooth, naive_smooth_descr);
	/* ... Register additional test functions here */
}

