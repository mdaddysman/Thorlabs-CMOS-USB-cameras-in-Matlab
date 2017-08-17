/*
 * This file sets the gain on the ThorLabs camera. 
 * By Matthew K. Daddysman 10/31/2014
 */

#include <windows.h>
#include <stdlib.h>
#include "mex.h"
#include "matrix.h"
#include "uc480.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nlhs > 1)
        mexErrMsgTxt("Too many outputs!");
    if(nrhs != 2)
        mexErrMsgTxt("There must be three inputs: hCam and Gain");
    
    HCAM phCam = 0;
    INT result;
    int success = 0;
    int *gainptr = (int *)mxCalloc(1,sizeof(int));
    
    INT32_T *rhd = (INT32_T *)mxGetData(prhs[0]);
    phCam = (HCAM)rhd[0];
    double *rhd2 = (double *)mxGetData(prhs[1]);
    double gain = rhd2[0];
    
    result = is_SetHWGainFactor(phCam,IS_SET_MASTER_GAIN_FACTOR,gain*100);
    
    if(nlhs == 1)
    {
        success = (result == IS_SUCCESS);
        *gainptr = success;
        plhs[0] = mxCreateNumericMatrix(0,0,mxINT32_CLASS,mxREAL);
        mxSetData(plhs[0],gainptr);
        mxSetM(plhs[0],1);
        mxSetN(plhs[0],1);
    }
}
        
        