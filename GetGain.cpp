/*
 * This file gets the gain on the ThorLabs camera. 
 * By Matthew K. Daddysman 10/31/2014
 */

#include <windows.h>
#include <stdlib.h>
#include "mex.h"
#include "matrix.h"
#include "uc480.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nlhs != 1)
        mexErrMsgTxt("One output!");
    if(nrhs != 1)
        mexErrMsgTxt("There must be one input: hCam.");
    
    HCAM phCam = 0;
    INT result;
    double *gainptr = (double *)mxCalloc(1,sizeof(double));
    
    INT32_T *rhd = (INT32_T *)mxGetData(prhs[0]);
    phCam = (HCAM)rhd[0];
    
    result = is_SetHWGainFactor(phCam,IS_GET_MASTER_GAIN_FACTOR,100);
    
    *gainptr = result/100;
    plhs[0] = mxCreateNumericMatrix(0,0,mxDOUBLE_CLASS,mxREAL);
    mxSetData(plhs[0],gainptr);
    mxSetM(plhs[0],1);
    mxSetN(plhs[0],1);

}
        
        