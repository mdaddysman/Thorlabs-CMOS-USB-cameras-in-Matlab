function ThorVideo_Functions()

rh = ThorCam_Open();

while(ishghandle(rh.fh))
    ThorCam_RunLoop(rh);
end

close all
clearvars;
end

function ThorCam_RunLoop(runhandles)
tic
image = GetImage(runhandles.hCam,runhandles.width,runhandles.height);
image = flipud(rot90(reshape(image,runhandles.width,runhandles.height)));
set(runhandles.ih,'CData',image);
drawnow
ctime = toc;
if(ishghandle(runhandles.fh))
    set(runhandles.th,'String',['FPS: ' num2str(1/ctime,'%.2f') ' Ms/frame: ' num2str(ctime*1000,'%.0f')]);
end
end

function runhandles = ThorCam_Open()
[hCam, width, height] = LoadCamera();

maxgain = GetMaxGain(hCam);
gain = GetGain(hCam);
expset = GetExposureInfo(hCam);
gboost = SetGainBoost(hCam,-1);
cm = 1;

if exist('ThorCamSaveSettings.mat', 'file') == 2
    settings = load('ThorCamSaveSettings.mat');
    gain = settings.sgain;
    result = SetGain(hCam,gain);
    expset(1) = settings.sexp;
    result = SetExposure(hCam,expset(1));
    gboost = settings.sgainb;
    SetGainBoost(hCam,gboost);
    cm = settings.scm;
end

image = GetImage(hCam,width,height);
image = flipud(rot90(reshape(image,width,height)));

fh = figure('Name','ThorCam Image','NumberTitle','off');
ih = imshow(image,'InitialMagnification',67);
ThorCam_ColorMapLookup(cm); 
colorbar
th = title(['FPS: ' num2str(0) ' Ms/frame: ' num2str(0)]); 
set(fh,'Resize','off','Position',[329 166 1022 773],'MenuBar','none','ToolBar','none','CloseRequestFcn',{@ThorCam_SafeShutdown,hCam});

exph = uicontrol('Style','slider','Position',[80 50 370 15]);
gainh = uicontrol('Style','slider','Position',[500 50 370 15]);
expth = uicontrol('Style','text','Position',[80 30 370 15],'String',['Exposure Time: ' num2str(expset(1)) ' ms']);
set(expth,'BackgroundColor',get(fh,'Color'));
gainth = uicontrol('Style','text','Position',[500 30 370/2 15],'String',['Gain: ' num2str(gain,'%.2f') 'X']);
set(gainth,'BackgroundColor',get(fh,'Color'));
gainbh = uicontrol('Style','checkbox','Position',[500+370/2+370/4 30 370/4 15],'String','Gain Boost');
set(gainbh,'BackgroundColor',get(fh,'Color'),'Value',gboost,'Callback',{@ThorCam_SetGainBoostCallback,hCam});
minexpstep = expset(4)/(expset(3) - expset(2));
maxexpstep = 10*minexpstep;
set(exph,'Callback',{@ThorCam_ExpCallback,expth,expset(4),hCam},'Min',expset(2),'Max',expset(3),'Value',expset(1),'SliderStep',[minexpstep maxexpstep]);
mingainstep = 0.1/(maxgain-1);
maxgainstep = 1/(maxgain-1);
set(gainh,'Callback',{@ThorCam_GainCallback,gainth,hCam},'Min',1,'Max',maxgain,'Value',gain,'SliderStep',[mingainstep maxgainstep]);
cmh = uicontrol('Style','popupmenu','Position',[82 726 75 15],'String',{'Jet','Gray','Hot','Cool','Spring','Summer','Autumn','Winter'},'Value',cm,'Callback',{@ThorCam_ChangeColorMap,fh});
uicontrol('Style','pushbutton','String','Save Image','Position',[772 720 100 30],'Callback',{@ThorCam_SaveImageCallback,ih});
uicontrol('Style','pushbutton','String','Save Settings','Position',[425 5 100 30],'Callback',{@ThorCam_SaveSettingsCallback,exph,gainh,gainbh,cmh});

runhandles.fh = fh;
runhandles.hCam = hCam;
runhandles.width = width;
runhandles.height = height;
runhandles.ih = ih;
runhandles.th = th;

end

function ThorCam_ExpCallback(src,~,expth,inc,hCam)
exptime = get(src,'Value');
factor = floor(exptime/inc);
exptime = factor*inc;
set(src,'Value',exptime);
set(expth,'String',['Exposure Time: ' num2str(exptime) ' ms']);
result = SetExposure(hCam,exptime);
end

function ThorCam_GainCallback(src,~,gainth,hCam)
gain = get(src,'Value');
gain = floor(gain*100)/100;
set(src,'Value',gain);
set(gainth,'String',['Gain: ' num2str(gain,'%.2f') 'X']);
result = SetGain(hCam,gain);
end

function ThorCam_SetGainBoostCallback(src,~,hCam)
check = get(src,'Value');
SetGainBoost(hCam,check);
end

function ThorCam_SafeShutdown(src,~,hCam)
CloseCamera(hCam);
delete(src);
end

function ThorCam_SaveImageCallback(~,~,ih)
image = get(ih,'CData');
imwrite(image,[datestr(now,'yymmdd_HHMMSS') '_ThorCamImage.bmp']);
end

function ThorCam_SaveSettingsCallback(~,~,exph,gainh,gainbh,cmh)
sexp = get(exph,'Value');
sgain = get(gainh,'Value');
sgainb = get(gainbh,'Value');
scm = get(cmh,'Value');
save('ThorCamSaveSettings.mat','sexp','sgain','sgainb','scm');
end

function ThorCam_ChangeColorMap(src,~,fh)
%{'Jet','Gray','Hot','Cool','Spring','Summer','Autumn','Winter'}
cm = get(src,'Value');
figure(fh);
ThorCam_ColorMapLookup(cm);   
end

function ThorCam_ColorMapLookup(cm)
switch cm
    case 1
        colormap jet
    case 2
        colormap gray
    case 3
        colormap hot
    case 4
        colormap cool
    case 5 
        colormap spring
    case 6
        colormap summer
    case 7 
        colormap autumn
    case 8
        colormap winter
    otherwise
        colormap jet
end     
end