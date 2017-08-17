/*
 * This file loads the ThorLabs Camera and Returns the Sensor size and the 
 * Camera ID. It also sets up the camera for RAW8 Image type. 
 * By Matthew K. Daddysman 10/31/2014
 */

#include <windows.h>
#include <stdlib.h>
#include "mex.h"
#include "matrix.h"
#include "uc480.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nlhs != 3)
        mexErrMsgTxt("Must have three outputs: CamID, Width, Height");
    if(nrhs != 0)
        mexErrMsgTxt("There are no inputs.");
    
    INT result;
    int numcamera = 0;
    HCAM phCam = 0;
    int width = 0;
    int height = 0;
    HCAM *cam = (HCAM *)mxCalloc(1,sizeof(HCAM));
    int *pwidth = (int *)mxCalloc(1,sizeof(int));
    int *pheight = (int *)mxCalloc(1,sizeof(int));
    
    is_GetNumberOfCameras(&numcamera);

    if(numcamera != 1)
    {   
        mexErrMsgTxt("Too many cameras.");        
        return;
    }
    
    result = is_InitCamera(&phCam,NULL);
    if(result != IS_SUCCESS)
    {
        mexErrMsgTxt("Failed to Initalize Camera.");
        return;
    }
    
    SENSORINFO si; 
    result = is_GetSensorInfo(phCam,&si);
    if(result != IS_SUCCESS)
    {
        is_ExitCamera(phCam);
        mexErrMsgTxt("Failed to Get Sensor Info.");
        return;
    }
    
    width = si.nMaxWidth;
    height = si.nMaxHeight;
    
    result = is_SetColorMode(phCam,IS_CM_SENSOR_RAW8);
    if(result != IS_SUCCESS)
    {
        is_ExitCamera(phCam);
        mexErrMsgTxt("Failed to Set Color Mode.");
        return;
    }
    
    //Now that setup is complete return the values. 
    *cam = phCam;
    plhs[0] = mxCreateNumericMatrix(0,0,mxINT32_CLASS,mxREAL);
    mxSetData(plhs[0],cam);
    mxSetM(plhs[0],1);
    mxSetN(plhs[0],1);
    
    *pwidth = width;
    plhs[1] = mxCreateNumericMatrix(0,0,mxINT32_CLASS,mxREAL);
    mxSetData(plhs[1],pwidth);
    mxSetM(plhs[1],1);
    mxSetN(plhs[1],1);
    
    *pheight = height;
    plhs[2] = mxCreateNumericMatrix(0,0,mxINT32_CLASS,mxREAL);
    mxSetData(plhs[2],pheight);
    mxSetM(plhs[2],1);
    mxSetN(plhs[2],1);
}
