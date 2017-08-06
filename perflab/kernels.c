/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <assert.h>

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
void debugger(int dim, pixel* fuck){
	// printf("dim = %d\n", dim);
	// for(int i = 0; i < dim; ++i){
		// for(int j = 0; j < dim; ++j){
			// pixel f = fuck[i*dim + j];
			// printf("%03d ", f.red);
		// }
		// printf("\n");
	// }
	// printf("---------------\n");
}

static const int STRIP_I = 32;
static const int STRIP_J = 34;
static char rotate_old_descr[] = "rotate_old: past working version";
void rotate_old(int dim, pixel* restrict src, pixel* restrict dst) 
{
	int Ni, j;
	int dimnn = dim - 1;
	
	for (j = 0; j < dim - STRIP_J; j+= STRIP_J) {
		for (Ni = 0; Ni < dim; Ni++ ){
			int stop_j = j + STRIP_J;
			int loc = dimnn - Ni;
			for(int Nj = j; Nj < stop_j; ++Nj){
				// dst[RIDX(dimnn-Nj, Ni, dim)] = src[RIDX(Ni, Nj, dim)];
				dst[RIDX(Ni, Nj, dim)] = src[RIDX(Nj, loc, dim)];
			}
		}
	}
	for (Ni = 0; Ni < dim; Ni++ ){
		int stop_j = dim;
		int loc = dimnn - Ni;
		for(int Nj = j; Nj < stop_j; ++Nj){
			// dst[RIDX(dimnn-Nj, Ni, dim)] = src[RIDX(Ni, Nj, dim)];
			dst[RIDX(Ni, Nj, dim)] = src[RIDX(Nj, loc, dim)];
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
	// add_rotate_function(&naive_rotate, naive_rotate_descr);   
	// add_rotate_function(&rotate_old, rotate_old_descr);   
	// add_rotate_function(&rotate, rotate_descr);   
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
 const int STRIP_I_2 = 32;
 const int STRIP_J_2 = 32;


char smooth_old_descr[] = "smooth: Past version";
void smooth_old(int dim, pixel *src, pixel *dst) 
{	
	int i, j;

	for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
		dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

inline pixel pixel_add(pixel a, pixel b){
	register pixel tmp;
	tmp.red = a.red + b.red;
	tmp.green = a.green + b.green;
	tmp.blue = a.blue + b.blue;
	return tmp;
}

const int STRIP_HORIZONTAL = 32;
char smooth_descr[] = "smooth: Current working version";
void line_proc(int beg, int end, const pixel* restrict const src, pixel* restrict const dst) {
	short* src_line = (short*)src;
	short* dst_line = (short*)dst;
	for(int i = beg; i < end; ++i){
		dst_line[i] = src_line[i] + src_line[i - 3] + src_line[i + 3];
	}
}


const int VSTRIP = 32;
const int HSTRIP = 32;
void line_cross_fucking(const int dim, char* restrict src_raw, char* restrict dst_raw){	
	typedef long long T;
	const int cacheline = 64;
	T* src = (T*) src_raw;
	T* dst = (T*) dst_raw;
	int dim_trans = dim * sizeof(pixel) / sizeof(T);
	int strip_trans = 3*cacheline/sizeof(T);
	for(int bigj = 0; bigj < dim_trans; bigj+= strip_trans){
		// fucking i = 0
		{
			int i = 0;
			for(int j = bigj; j < bigj + strip_trans; ++j){
				int base = i * dim_trans + j;
				dst[base] = src[base] + src[base + dim_trans];
			}
		}
		for(int i = 1; i < dim-1; ++i){
			for(int j = bigj; j < bigj + strip_trans; ++j){
				int base = i * dim_trans + j;
				dst[base] = src[base - dim_trans] + src[base] + src[base + dim_trans];
			}
		}
		// fucking i = dim - 1
		{
			int i = dim - 1;
			for(int j = bigj; j < bigj + strip_trans; ++j){
				int base = i * dim_trans + j;
				dst[base] = src[base] + src[base - dim_trans];
			}
		}
	}
}

void smooth(const int dim, pixel* restrict const src_raw, pixel* restrict const dst_raw) 
{	
	// split into trucks VERTICAL_SIZE * steam processing 
	// calculate 
	// calculate 32*6 = 64 byte * 3 (512bits)
	// task 1: for each line, find the best method way calc
	// or alternatively, do it line by line;
	// then fuck it with hard method
	// the head has make the desicion
	debugger(dim, src_raw);
	short* tmp = (short*)malloc(dim * dim * sizeof(pixel));
	line_cross_fucking(dim, (char*)src_raw, (char*)tmp);
	// printf("fucking\n");
	debugger(dim, (pixel*)tmp);
	short* src = (short*) tmp;	
	short* dst = (short*) dst_raw;	
	int dim_trans = dim*sizeof(pixel)/sizeof(short);
	{
		int i = 0;
		{
			int j = 0;
			int base = i * dim_trans + j;	
			dst[base + 0] = (src[base + 0] + src[base + 3]) / 4;
			dst[base + 1] = (src[base + 1] + src[base + 4]) / 4;
			dst[base + 2] = (src[base + 2] + src[base + 5]) / 4;
		}
		for(int j = 3; j < dim_trans - 3; j++){
			int base = i * dim_trans + j;	
			dst[base] = (src[base - 3] + src[base] + src[base + 3]) / 6;
		}
		{
			int j = dim_trans - 3;
			int base = i * dim_trans + j;	
			dst[base + 0] = (src[base + 0] + src[base - 3]) / 4;
			dst[base + 1] = (src[base + 1] + src[base - 2]) / 4;
			dst[base + 2] = (src[base + 2] + src[base - 1]) / 4;
		}
	}

	for(int i = 1; i < dim - 1; ++i){
		{
			int j = 0;
			int base = i * dim_trans + j;	
			dst[base + 0] = (src[base + 0] + src[base + 3]) / 6;
			dst[base + 1] = (src[base + 1] + src[base + 4]) / 6;
			dst[base + 2] = (src[base + 2] + src[base + 5]) / 6;
		}
		for(int j = 3; j < dim_trans - 3; j++){
			int base = i * dim_trans + j;	
			dst[base] = (src[base - 3] + src[base] + src[base + 3]) / 9;
		}
		{
			int j = dim_trans - 3;
			int base = i * dim_trans + j;	
			dst[base + 0] = (src[base + 0] + src[base - 3]) / 6;
			dst[base + 1] = (src[base + 1] + src[base - 2]) / 6;
			dst[base + 2] = (src[base + 2] + src[base - 1]) / 6;
		}
	}
	{
		int i = dim - 1;
		{
			int j = 0;
			int base = i * dim_trans + j;	
			dst[base + 0] = (src[base + 0] + src[base + 3]) / 4;
			dst[base + 1] = (src[base + 1] + src[base + 4]) / 4;
			dst[base + 2] = (src[base + 2] + src[base + 5]) / 4;
		}
		for(int j = 3; j < dim_trans - 3; j++){
			int base = i * dim_trans + j;	
			dst[base] = (src[base - 3] + src[base] + src[base + 3]) / 6;
		}
		{
			int j = dim_trans - 3;
			int base = i * dim_trans + j;	
			dst[base + 0] = (src[base + 0] + src[base - 3]) / 4;
			dst[base + 1] = (src[base + 1] + src[base - 2]) / 4;
			dst[base + 2] = (src[base + 2] + src[base - 1]) / 4;
		}
	}


	//fucking last
	debugger(dim, (pixel*)dst);
	free(tmp);
}

/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/
void register_smooth_functions() {
	add_smooth_function(&naive_smooth, naive_smooth_descr);
	// add_smooth_function(&smooth_old, smooth_old_descr);
	
	add_smooth_function(&smooth, smooth_descr);
	/* ... Register additional test functions here */
}

