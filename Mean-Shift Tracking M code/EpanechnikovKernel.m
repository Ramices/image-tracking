function [mask]=EpanechnikovKernel(mask_size)
%Epanechnikov
%     3
%K(u)=-*(1-u^2); |u|<=1
%     4 
%K(u)=0;         otherwise

%Izhisliawane na razmera na stupkata i generirane na stoinosti w interwala
%[-1;1]
step=1/(mask_size/2);
j=0;
for i=-1:step:1
    j=j+1;
    %Izhisliawane na wsiaka stoinost ot ednomernia wektor po formula
    vec(j)=(3/4)*(1-i^2);
end
%Izchisliawane na matricata na maskata
mask=vec'*vec;