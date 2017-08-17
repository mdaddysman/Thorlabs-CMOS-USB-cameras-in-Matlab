/*
 * This file get an image from the ThorLabs camera 
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
        mexErrMsgTxt("One output the image.");
    if(nrhs != 3)
        mexErrMsgTxt("There must be three inputs: hCam Width Height");
    
    HCAM phCam = 0;
    INT result;
    
    INT32_T *rhd = (INT32_T *)mxGetData(prhs[0]);
    phCam = (HCAM)rhd[0];
    rhd = (INT32_T *)mxGetData(prhs[1]);
    int width = (int)rhd[0];
    rhd = (INT32_T *)mxGetData(prhs[2]);
    int height = (int)rhd[0];
    
    char *pImage = NULL;
    int imgID = 0;
    char errbuffer[50];
    
    pImage = (char *) mxCalloc(width*height+1,sizeof(char *));
    if(pImage == NULL)
    {
        is_ExitCamera(phCam);
        mexErrMsgTxt("Failed to mxCalloc Mem.");
        return;
    }
    
    result = is_SetAllocatedImageMem(phCam,width,height,8,pImage,&imgID);
    if(result != IS_SUCCESS)
    {
        mxFree(pImage);
        is_ExitCamera(phCam);
        sprintf(errbuffer,"Failed to Set Alloc Mem: r = %i",result);
        mexErrMsgTxt(errbuffer);
        return;
    }
    
    result = is_SetImageMem(phCam,pImage,imgID);
    if(result != IS_SUCCESS)
    {
        is_FreeImageMem(phCam,pImage,imgID);
        mxFree(pImage);
        is_ExitCamera(phCam);
        sprintf(errbuffer,"Failed to Set Image Mem: r = %i",result);
        mexErrMsgTxt(errbuffer);
        return;
    }
    
    result = is_FreezeVideo(phCam,IS_WAIT);
    if(result != IS_SUCCESS)
    {
        is_FreeImageMem(phCam,pImage,imgID);
        mxFree(pImage);
        is_ExitCamera(phCam);
        sprintf(errbuffer,"Failed to 1 FreezeVideo: r = %i",result);
        mexErrMsgTxt(errbuffer);
        return;
    }
    
    result = is_FreezeVideo(phCam,IS_WAIT);
    if(result != IS_SUCCESS)
    {
        is_FreeImageMem(phCam,pImage,imgID);
        mxFree(pImage);
        is_ExitCamera(phCam);
        sprintf(errbuffer,"Failed to 2 FreezeVideo: r = %i",result);
        mexErrMsgTxt(errbuffer);
        return;
    }
    
    result = is_FreeImageMem(phCam,pImage,imgID);
    if(result != IS_SUCCESS)
    {
        mxFree(pImage);
        is_ExitCamera(phCam);
        mexErrMsgTxt("Failed to Free Mem.");
        return;
    }

    plhs[0] = mxCreateNumericMatrix(0,0,mxUINT8_CLASS,mxREAL);
    mxSetData(plhs[0],pImage);
    mxSetM(plhs[0],width*height);
    mxSetN(plhs[0],1);
    
}
    