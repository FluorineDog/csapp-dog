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
  if (M == 61 && N == 67) {
    for (int j_base = 0; j_base < M; j_base += 13) {
      for (int i = 0; i < N; i++) {
        for (int j = j_base; j < min(M, j_base + 13); j++) {
          // fprintf(stderr, "(%d %d %d:: %d)", i, j, j_base, min(M, j_base +
          // 8)); fprintf(stderr, "%d", (int)(j < min(M, j_base + 8)));
          int tmp = A[i][j];
          B[j][i] = tmp;
        }
      }
    }
    return;
  } else if (M == 32 && N == 32) {
    // 32 * 8
    for (int i_base = 0; i_base < N; i_base += 8) {
      for (int j_base = 0; j_base < M; j_base += 8) {
        for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
            B[i_base + i][j_base + j] = A[j_base + 7 - i][i_base + 7 - j];
          }
        }
        for (int i = 0; i < 8; ++i) {
          for (int j = 0; j < 7 - i; j++) {
            int tmp;
            swap(B[i_base + i][j_base + j], B[i_base + 7 - j][j_base + 7 - i]);
          }
        }
      }
    }

  } else {
    int anchor = 0;
    // define buffer to "proxy" the bottom half of an 8*8 block
    int(*buf)[N] = (int(*)[N])(&B[M - 8][N - 8]);
    for (int i_base = 0; i_base < M; i_base += 8) {
      for (int j_base = 0; j_base < N; j_base += 8) {
        // find the best location of buffer
        // require: i_base != anchor and j_base != anchor
        if (i_base == anchor || j_base == anchor) {
          // if last_line
          if (i_base == M - 8) {
            if(j_base >= N - 16){
              anchor = N - 8;
            }
            else{
              anchor = N - 16;
            }
          } else {
            // lazy way to avoid frequent eviction of buffer
            anchor = j_base - 8;
            if (anchor < 0) anchor += N;
            if (anchor == i_base) anchor -= 8;
            if (anchor < 0) anchor += N;
          }
          // anchor = N - 8;
          buf = (int(*)[N])(&B[M - 8][anchor]);
        }

        for (int i = 0; i < 8; ++i) {
          for (int j = 0; j < 8; ++j) {
            // if i < 4, use normal block(i_base, j_base)
            // if i >= 4, use the proxy buffer
            if (i < 4) {
              B[i_base + i][j_base + j] = A[j_base + 7 - i][i_base + 7 - j];
            } else {
              buf[i][j] = A[j_base + 7 - i][i_base + 7 - j];
            }
          }
        }
        for (int i = 0; i < 8; ++i) {
          for (int j = 0; j < 7 - i; j++) {
            int tmp, tmp2;
            if (i < 4) {
              tmp2 = B[i_base + i][j_base + j];
            } else {
              tmp2 = buf[i][j];
            }

            if (j >= 4) {
              swap(B[i_base + 7 - j][j_base + 7 - i], tmp2);
            } else {
              swap(buf[7 - j][7 - i], tmp2);
            }

            if (i < 4) {
              B[i_base + i][j_base + j] = tmp2;
            } else {
              buf[i][j] = tmp2;
            }
          }
        }
        for (int i = 4; i < 8; ++i) {
          for (int j = 0; j < 8; ++j) {
            B[i_base + i][j_base + j] = buf[i][j];
          }
        }
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
