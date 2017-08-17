/*
 * This file sets the gain boost and qurries on the ThorLabs camera. 
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
        mexErrMsgTxt("There must be two inputs: hCam and GainBoost On/Off");
    
    HCAM phCam = 0;
    INT result;
    int success = 0;
    int *gainptr = (int *)mxCalloc(1,sizeof(int));
    
    INT32_T *rhd = (INT32_T *)mxGetData(prhs[0]);
    phCam = (HCAM)rhd[0];
    int *rhd2 = (int *)mxGetData(prhs[1]);
    int gainboost = rhd2[0];
    
    if(nlhs == 1)
        plhs[0] = mxCreateNumericMatrix(0,0,mxINT32_CLASS,mxREAL);

    
    switch(gainboost)
    {
        case 1: //turn boost on!
            result = is_SetGainBoost(phCam,IS_SET_GAINBOOST_ON);
            if(nlhs == 1)
            {
                success = (result == IS_SUCCESS);
                *gainptr = success;
                mxSetData(plhs[0],gainptr);
            }
            break;
        case 0: //turn boost off!
            result = is_SetGainBoost(phCam,IS_SET_GAINBOOST_OFF);
            if(nlhs == 1)
            {
                success = (result == IS_SUCCESS);
                *gainptr = success;
                mxSetData(plhs[0],gainptr);
            }
            break;
        default: //for any other value quarry the state
            result = is_SetGainBoost(phCam,IS_GET_GAINBOOST);
            if(nlhs == 1)
            {
                if(result == IS_SET_GAINBOOST_ON)
                    *gainptr = 1;
                else
                    *gainptr = 0;
                mxSetData(plhs[0],gainptr);
            }
            break;
    }
    
    if(nlhs == 1)
    {
        mxSetM(plhs[0],1);
        mxSetN(plhs[0],1);
    }
    
}
        
        