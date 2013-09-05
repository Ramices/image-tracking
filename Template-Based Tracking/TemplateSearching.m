function [p q]=TemplateSearching(IM,Icrop)
%[p q]=TemplateSearching(IM,Icrop)
%Wrushta koordinatite "p" i "q" na centura na prozoreca w koito ima nai-dobro
%suwpadenie mejdu shablona i dadenata chast ot izobrajenieto. Izobrajenieto 
%se obhojda piksel po piksel i na wsiaka nowa stupka se izchisliawa SSD. 
%SSD triabwa da bude minimizirano. Ako SSD na
%tekushtata stupka e po-malko ot towa na predishnata, se zapametiawa nowata
%tekushta stoinost zaedno s koordinatite na prozoreca. 
[imcrop_y imcrop_x]=size(Icrop);
max=imcrop_y*imcrop_x*255;%
thresh=10*max/100;%thresholding 10% ot stoinosta na "max"
p=0;
q=0;
IM=rgb2gray(IM);
[im_y im_x]=size(IM);
 for i=1:1:im_y-imcrop_y
      for j=1:1:im_x-imcrop_x
        u=i:1:i+imcrop_y-1;
        v=j:1:j+imcrop_x-1;
        middleIm=IM(u,v);
        middleSum=sum(sum((middleIm-Icrop).^2)); %SSD (Sum of Squared Differences)
        if(middleSum<max)
            max=middleSum;
            p=i+round(imcrop_y/2);%centur na prozoreca na suwpadenie po y
            q=j+round(imcrop_x/2);%centur na prozoreca na suwpadenie po x
        end
      end
 end
  if(max>thresh)
     p=0;
     q=0; 
  end
  