#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <emmintrin.h>

/* Your function must have the following signature: */

void dgemm( int m, int n, float *A, float *C );

/* The reference code */
void dgemm_reference( int m, int n, float *A, float *C )
{
  for( int i = 0; i < m; i++ )
    for( int k = 0; k < n; k++ ) 
      for( int j = 0; j < m; j++ ) 
        C[i+j*m] += A[i+k*m] * A[j+k*m];
}

/* The benchmarking program */

int main( int argc, char **argv )
{
  srand(time(NULL));

  	for( int n = 32; n < 100; n = n+1+n/3 )
//	for( int n = 40; n <  41; n = n+1+n/3 )
	{
  /* Try different m */
  for( int m = 32; m <  100; m = m+1+m/3 )
//for( int m = 48; m <  49; m = m+1+m/3 )
  {
    /* Allocate and fill 2 random matrices A, C */
    float *A = (float*) malloc( m * n * sizeof(float) );
    float *C = (float*) malloc( m * m * sizeof(float) );
    float *C_ref = (float*) malloc( m * m * sizeof(float) );
    
    for( int i = 0; i < m*n; i++ ) A[i] = 2 * drand48() - 1;
    for( int i = 0; i < m*m; i++ ) C[i] = 2 * drand48() - 1;
 
    /* measure Gflop/s rate; time a sufficiently long sequence of calls to eliminate noise */
    double Gflop_s, seconds = -1.0;
    for( int n_iterations = 1; seconds < 0.1; n_iterations *= 2 ) 
    {
      /* warm-up */
      dgemm( m, n, A, C );
      
      /* measure time */
      struct timeval start, end;
      gettimeofday( &start, NULL );
      for( int i = 0; i < n_iterations; i++ )
	dgemm( m,n,A, C );
      gettimeofday( &end, NULL );
      seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
      
      /* compute Gflop/s rate */
      Gflop_s = 2e-9 * n_iterations * m * m * n / seconds;
    }
    
    printf( "%d by %d matrix with strip size %d \t %g Gflop/s\n", n, n,m, Gflop_s );
    
    /* Ensure that error does not exceed the theoretical error bound */
		
    /* Set initial C to 0 and do matrix multiply of A*B */
    memset( C, 0, sizeof( float ) * m * m );
    dgemm( m,n, A, C );

    /* Subtract A*B from C using standard dgemm and reference (note that this should be 0 to within machine roundoff) */
    memset( C_ref, 0, sizeof( float ) * m * m );
    dgemm_reference( m,n,A,C_ref );

    /* Subtract the maximum allowed roundoff from each element of C */
    for( int i = 0; i < m*m; i++ ) C[i] -= C_ref[i] ; 

    /* After this test if any element in C is still positive something went wrong in square_dgemm */
    for( int i = 0; i < m * m; i++ )
      if( C[i] > 0 ) {
  	printf( "FAILURE: error in matrix multiply exceeds an acceptable margin\n" );
  	printf( "Off by: %f, from the reference: %f, at n = %d, m = %d\n",C[i], C_ref[i], n, m ); 
  	return -1;
      }

    /* release memory */
    free( C_ref );
    free( C );
    free( A );
  }
	}  
  return 0;
}
