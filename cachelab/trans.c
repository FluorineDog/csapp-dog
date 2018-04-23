/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "cachelab.h"
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define swap(x, y) \
  do {             \
    tmp = (x);     \
    (x) = (y);     \
    (y) = tmp;     \
  } while (0)

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
  fprintf(stderr, "(%d, %d) %lx %lx\n", M, N, (intptr_t)A, (intptr_t)B);
  int i, j, tmp, j_base, i_base;
  if (M == 61 && N == 67) {
    for (j_base = 0; j_base < M; j_base += 8) {
      for (i = 0; i < N; i++) {
        for (j = j_base; j < min(M, j_base + 8); j++) {
          // fprintf(stderr, "(%d %d %d:: %d)", i, j, j_base, min(M, j_base +
          // 8)); fprintf(stderr, "%d", (int)(j < min(M, j_base + 8)));
          tmp = A[i][j];
          B[j][i] = tmp;
        }
      }
    }
    return;
  } else {
    // 32 * 8
    for (i_base = 0; i_base < N; i_base += 8) {
      for (j_base = 0; j_base < M; j_base += 8) {
        for (i = 0; i < 8; i++) {
          for (j = 0; j < 8; j++) {
            B[i_base + 7 - i][j_base + 7 - j] = A[j_base + i][i_base + j];
          }
        }
        for (i = 0; i < 8; i++) {
          for (j = 0; j < 7 - i; j++) {
            swap(B[i_base + i][j_base + j],  B[j_base + 7 - j][i_base + 7 - i]);
          }
        }
 
        // for (i = 0; i < 8; i++) {
        //   for (j = 0; j < i; j++) {
        //     swap(B[i_base + i][j_base + j],  B[i_base + 7 - j][j_base + 7 - i]);
        //   }
        // }
      }
    }
  }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, tmp;
  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      tmp = A[i][j];
      B[j][i] = tmp;
    }
  }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
  /* Register your solution function */
  registerTransFunction(transpose_submit, transpose_submit_desc);

  /* Register any additional transpose functions */
  registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < M; ++j) {
      if (A[i][j] != B[j][i]) {
        return 0;
      }
    }
  }
  return 1;
}
