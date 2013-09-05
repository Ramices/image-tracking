%Template-Based Tracking
clc
clear

vid = videoinput('winvideo', 1,'YUY2_320x240');
set(vid,'ReturnedColorSpace','rgb','TriggerRepeat',1000,'FramesPerTrigger',1);
vid_src=getselectedsource(vid);
set(vid_src,'BacklightCompensation','off','ExposureMode','manual','WhiteBalanceMode','auto');%,'WhiteBalance',6500);
triggerconfig(vid,'manual');
start(vid);

%aviobj = avifile('TemplateBasedTracking.avi');

flag=0;

for step=1:1:inf
   trigger(vid);
   I1=getdata(vid,1);
   
   if (flag==0)
      Template=imcrop(I1);
      Template=rgb2gray(Template);
      flag=1;
   end
   
   if(flag>0)
    [p q]=TemplateSearching(I1,Template);
        if(p>1&&q>1)    
            I1=draw_rectangle_marker(I1,20,p,q,0,255,0);
        end
    imshow(I1);
    %F=getframe;
    %aviobj = addframe(aviobj,F);
   end
end
    %aviobj = close(aviobj);