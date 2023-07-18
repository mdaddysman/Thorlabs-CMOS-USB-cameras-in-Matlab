# Software to use Thorlabs CMOS USB cameras in Matlab
This software uses the C++ API for [Thorlabs CMOS USB cameras](https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=4024) to display the camera images in Matlab. A lightwight display program is also included. 

This software was developed and tested on [Thorlabs DCC1545M](https://www.thorlabs.com/thorproduct.cfm?partnumber=DCC1545M), Windows 7 and Matlab 2014b. Newer version of Matlab should work. The software can also be modified to include the 'parula' lookup table in Matlab 2015 or later. 

## FreezeVideo() fix
A common error when using the original code from user 'mdaddysman' was "Failed to 1 FreezeVideo: r = 178." This was due to the Thorlabs function is_FreezeVideo() returning error 178 and would halt the Matlab script and return an error to Matlab. 
This error corresponds to "IS_TRANSFER_ERROR" and is fixed by changing lines 62-82 of GetImage.cpp to call is_FreezeVideo() > 1 time until a successful image capture occurs. Anecdotely, I have only noticed 1 failed call to is_FreezeVideo() at a time. I do not think the 1 failed call has a significant impact to image capture with this code. 
Besides this one change, all code is identical to what user 'mdaddysman' provided. Install with the directions below.

## Installing 
1. Matlab must be configured with a [C++ compiler](https://www.mathworks.com/support/compilers.html). 
2. Install the uc480 Version 4.20 SDK [(Direct Link)](https://www.thorlabs.com/software//MUC/DCx/Software/ThorlabsDCx_camera_V4.20.zip) from the [Thorlabs Website](https://www.thorlabs.com/software_pages/ViewSoftwarePage.cfm?Code=ThorCam). 
3. Copy `uc480.h` and `uc480_64.lib` from the Thorlabs C++ SDK into the code directory.
4. Run `Install64.m`.

## License
This software is made available under the [GPL-3.0](LICENSE).
