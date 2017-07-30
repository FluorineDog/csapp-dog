#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "defs.h"
const int SIZE = 512;
const int SMALLSIZE = 96;
const int chunk_size = SMALLSIZE * SMALLSIZE * 3;
int main(){
	
	short src[chunk_size];
	short dst1[chunk_size];
	short dst2[chunk_size];
	for(int i = 0; i < chunk_size; ++i) {
		src[i] = rand() & 0xFF;
	}

	naive_smooth(SMALLSIZE, (pixel*)src, (pixel*)dst1);	
	naive_smooth(SMALLSIZE, (pixel*)src, (pixel*)dst2);
	for(int i = 0; i < chunk_size; ++i) {
		if(dst1[i] != dst2[i]){
			fprintf(stderr, "fuck");
			exit(0);
		}
	}
	fprintf(stderr, "done");
	return 0;
}