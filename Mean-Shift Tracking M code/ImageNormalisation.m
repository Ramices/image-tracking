function [NormalizedImage]=ImageNormalisation(img)
%Normalising RGB Image
%[NormalizedImage]=ImageNormalisation(img)
%img - RGB image of UINT8 type
%NormalizedImage - output image of type UINT8 represented as RGB again
%converting an RGB image into normalized RGB removes the effect of any 
%intensity variations. 
%R'=R*sqrt(3)/sqrt(R^2 + G^2 + B^2);
%G'=G*sqrt(3)/sqrt(R^2 + G^2 + B^2);
%B'=B*sqrt(3)/sqrt(R^2 + G^2 + B^2);
%If black and R=B=G=0 then R'=G'=B'=1/sqrt(3);

[ImSizeY ImSizeX ImSizeZ]=size(img);
img=double(img);
NormalizedImage=zeros(ImSizeY,ImSizeX,ImSizeZ);
for i=1:1:ImSizeX
    for j=1:1:ImSizeY
        R=img(j,i,1);
        G=img(j,i,2);
        B=img(j,i,3);
        if ((R+G+B)>0)
            NormalizedImage(j,i,1)=R/sqrt(R^2+G^2+B^2);
            NormalizedImage(j,i,2)=G/sqrt(R^2+G^2+B^2);
            NormalizedImage(j,i,3)=B/sqrt(R^2+G^2+B^2);
        else
            NormalizedImage(j,i,1)=1/sqrt(3); 
            NormalizedImage(j,i,2)=1/sqrt(3); 
            NormalizedImage(j,i,3)=1/sqrt(3); 
        end
    end
end
%NormalizedImage=NormalizedImage*sqrt(3);NormalizedImage=NormalizedImage*10
%0;
NormalizedImage=NormalizedImage*255;
NormalizedImage=uint8(NormalizedImage);

