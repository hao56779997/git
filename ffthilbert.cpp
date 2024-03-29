//use FFT => Phase Shift => IFFT method to do the hilbert transform 
#include<mex.h>
#include <complex>
#include "fftw3.h"


/* The gateway routine. */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
	mwSize i
    double  *xr, *xi, *zr, *zi;
    mwSize  rows, cols, N;
    
    /* check for the proper number of arguments */
    if(nrhs !=1)
      mexErrMsgTxt("Two inputs required.");
    if(nlhs > 1)
      mexErrMsgTxt("Too many output arguments.");
    /*Check that both inputs are row vectors*/
    if( mxGetM(prhs[0]) != 1 || mxGetM(prhs[1]) != 1 )
      mexErrMsgTxt("Both inputs must be row vectors.");
    rows = 1; 
    /* Check that both inputs are complex*/
    if( !mxIsComplex(prhs[0]) || !mxIsComplex(prhs[1]) )
      mexErrMsgTxt("Inputs must be complex.\n");
  
    /* get the length of each input vector */
    N = mxGetN(prhs[0]);
    


    /* get pointers to the real and imaginary parts of the inputs */
    xr = mxGetPr(prhs[0]);
    xi = mxGetPi(prhs[0]);
   
  
    /* create a new array and set the output pointer to it */
    cols = nx;
    plhs[0] = mxCreateDoubleMatrix(rows, cols, mxCOMPLEX);
    zr = mxGetPr(plhs[0]);
    zi = mxGetPi(plhs[0]);


fftw_complex *in,*out, *in2; // array for I/O

double z,f=0;
cout << "Size of fftw_complex: " << sizeof(fftw_complex) << endl;
in = (fftw_complex*) fftw_malloc(N * sizeof(fftw_complex)); // for Inputting
in2 = (fftw_complex*) fftw_malloc(N * sizeof(fftw_complex)); // for output
out = (fftw_complex*) fftw_malloc(N * sizeof(fftw_complex)); // for comparison

// filling the arry 
for(i = 0; i < N; i++){
   in[i][0] = *(xr+i);
   in[i][1] = *(xi+i);
  
  
}



// making a plan and execute it, then destroy it. Forward FFT
fftw_plan pf = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
fftw_execute(pf);
fftw_destroy_plan(pf);
//do hilbert transform do a phase shift f(w)*-i (w<0)
//                                      f(w)*i(w>0)
//                                      f(w)*0(w=0)

for( i= 0; i < N; i++)
{
	
	if(i<N/2)
	{
		
			out[i][1]=-1*out[i][1];

			
		    
	        	
	}
	if(i==N/2)
	{ 
		  out[i][0]=0;
	      out[i][1]=0;


		
	}
	
	if(i>N/2)
	{
		out[i][0]=-1*out[i][0];
	
	
	}
if(i==0)
	{ 
		  out[i][0]=0;
	      out[i][1]=0;


		
	}

            z=out[i][1];
			out[i][1]=out[i][0];
			out[i][0]=z;



}

// making a plan and execute it, then destroy it. iFFT
fftw_plan pb = fftw_plan_dft_1d(N, out, in2, FFTW_BACKWARD, FFTW_ESTIMATE);
fftw_execute(pb);
fftw_destroy_plan(pb);

// output the fore N results
for( i= 0; i < N; i++){
   *(zr+i)= in2[i][0]/N ;
   *(zi+i)= in2[i][1]/N ;
   
}

// free space
fftw_free(in);
fftw_free(in2);
fftw_free(out);
return 0;
}

