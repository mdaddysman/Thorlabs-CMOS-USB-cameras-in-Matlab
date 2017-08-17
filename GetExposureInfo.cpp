/*
 * This file gets the exposure info on the ThorLabs camera. 
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
    double *expptr = (double *)mxCalloc(4,sizeof(double));
    double exptemp = 0; 
    
    INT32_T *rhd = (INT32_T *)mxGetData(prhs[0]);
    phCam = (HCAM)rhd[0];
    
    is_Exposure(phCam,IS_EXPOSURE_CMD_GET_EXPOSURE,&exptemp,8);
    expptr[0] = exptemp;
    is_Exposure(phCam,IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MIN,&exptemp,8);
    expptr[1] = exptemp;
    is_Exposure(phCam,IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MAX,&exptemp,8);
    expptr[2] = exptemp;
    is_Exposure(phCam,IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_INC,&exptemp,8);
    expptr[3] = exptemp;
    
    plhs[0] = mxCreateNumericMatrix(0,0,mxDOUBLE_CLASS,mxREAL);
    mxSetData(plhs[0],expptr);
    mxSetM(plhs[0],4);
    mxSetN(plhs[0],1);

}
        
        