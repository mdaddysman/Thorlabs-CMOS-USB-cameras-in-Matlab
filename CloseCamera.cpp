/*
 * This file closes the ThorLabs Camera. MUST CALL at end of program. 
 * By Matthew K. Daddysman 10/31/2014
 */

#include <windows.h>
#include <stdlib.h>
#include "mex.h"
#include "matrix.h"
#include "uc480.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nlhs != 0)
        mexErrMsgTxt("No outputs.");
    if(nrhs != 1)
        mexErrMsgTxt("There must be one input.");
    
    HCAM phCam = 0;
    INT result;
    
    INT32_T *rhd = (INT32_T *)mxGetData(prhs[0]);
    
    phCam = (HCAM)rhd[0];
    
    result = is_ExitCamera(phCam);
    if(result != IS_SUCCESS)
    {
        mexErrMsgTxt("Failed to Close Camera.");
        return;
    }
}