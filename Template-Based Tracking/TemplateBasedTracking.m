%Image tracking alghorithm
%Template-Based Tracking
clc
clear

vid = videoinput('winvideo', 1,'YUY2_320x240');
set(vid,'ReturnedColorSpace','rgb','TriggerRepeat',1000,'FramesPerTrigger',1);
vid_src=getselectedsource(vid);
set(vid_src,'BacklightCompensation','off','ExposureMode','manual','WhiteBalanceMode','auto');%,'WhiteBalance',6500);
triggerconfig(vid,'manual');
start(vid);

flag=0;

for step=1:1:inf
   trigger(vid);
   I1=getdata(vid,1);
   
   if (flag==0)
      Icrop=imcrop(I1);
      Icrop=rgb2gray(Icrop);
      [imcrop_y imcrop_x]=size(Icrop);
      flag=1;
   end
   
   max=imcrop_y*imcrop_x*255;
   p=0;
   q=0;
   
   if(flag>0)
   IM=I1;
       IM=rgb2gray(I1);
   [im_y im_x]=size(IM);
  for i=1:1:im_y-imcrop_y
      for j=1:1:im_x-imcrop_x
        u=i:1:i+imcrop_y-1;
        v=j:1:j+imcrop_x-1;
        middleIm=IM(u,v);
        middleSum=sum(sum(abs(middleIm-Icrop)));
        if(middleSum<max)
            max=middleSum;
            p=i+round(imcrop_y/2);% po y
            q=j+round(imcrop_x/2);% po x
        end
      end
  end
  
  if(p>1&&q>1)    
  I1=draw_rectangle_marker(I1,20,p,q,0,255,0);
  end
  imshow(I1);
   end
end
      