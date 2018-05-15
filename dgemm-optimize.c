#define OPT_2

#include <string.h>
#include <stdio.h>
#include "xmmintrin.h"

void dgemm( int m, int n, float *A, float *C)
{
// FILL-IN

/*references Streaming SIMD Extensions (Intel® SSE) intrinsics
https://software.intel.com/sites/landingpage/IntrinsicsGuide/#techs=SSE,SSE2&expand=1665
https://software.intel.com/en-us/node/683918*/

#ifdef OPT_0
/*Reordering loops to maximize spatial locality*/
//so in a matrix we used to look through j(columns) this would cause cache misses.
//we use i for checking through rows so there are less misses when searching through the matrix.
	for( int j = 0; j < m; j++){
	  for(int k = 0; k < n; k++){
	  	for(int i = 0; i < m;i++){ //j was originally here
	  		C[i+j*m]+=A[i+k*m] *A[j+k*m]; //index calculated based on matrix
	  	}
		}
	}
	/*by reordering i and j, with i being the inntermost,we will increase
	the likelihood of the value C[i+j*m] to be in cache, thus less cache misses
	leading to better performance*/
#endif

#ifdef OPT_1
/*Reordering Loops with Register Blocking.*/
//by resusing the value in temp you just look it up instead of calculating it multiple times in the loop below.
//This helps since we already know the values needed before entering the loop below.

	for(int j = 0; j < m; j++){
	  for(int k = 0; k < n; k++){
	  	float temp = A[j+k*m];

	  	for(int i = 0; i<m;i++){
	  		C[i+j*m]+=A[i+k*m] *temp;
	  	}
		}
		/*register blocking can be used to increase performance by splitting array
		into subregions that can be held in cache longer.*/
	}
#endif

#ifdef OPT_2
/*Reordering Loop, SSE_Unroll, Register Blocking*/
  int pad_m = m/8*8;
  int i;
	for( int j = 0; j < m; j++ ){
	  for( int k = 0; k < n; k++ ){

			//SSE_Unroll and Register Blocking
	    __m128 a2 = _mm_set1_ps(A[j+k*m]);
	    for(i=0; i<pad_m; i+=8){
	    		float *ikm_1, *ijm_1,*ikm_2, *ijm_2;
	    		ikm_1 = A + i+k*m;
	    		ijm_1 = C + i+j*m;
	        __m128 a1_1 = _mm_loadu_ps(ikm_1);
	        __m128 mm_1= _mm_mul_ps(a1_1, a2);
	        __m128 c_1  = _mm_loadu_ps(ijm_1);
	        __m128 sum_1 = _mm_add_ps(c_1, mm_1);
	        _mm_storeu_ps(ijm_1, sum_1);

	    		ikm_2 = A + (i+4)+k*m;
	    		ijm_2 = C + (i+4)+j*m;
	        __m128 a1_2 = _mm_loadu_ps(ikm_2);
	        __m128 mm_2= _mm_mul_ps(a1_2, a2);
	        __m128 c_2  = _mm_loadu_ps(ijm_2);
	        __m128 sum_2 = _mm_add_ps(c_2, mm_2);
	        _mm_storeu_ps(ijm_2, sum_2);
	    }
	  }
	}

//Loop_reordering
	for( int j = 0; j < m; j++ ){
	  for( int k = 0; k < n; k++ ){
	  	for(int i = pad_m; i<m;i++){
	  		C[i+j*m]+=A[i+k*m] *A[j+k*m];
	  	}
		}
	}
#endif

}
