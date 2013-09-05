function [draw_img]=draw_cross_marker(img,y,x,R,G,B)
% Draw Marker
%[draw_img]=draw_cross_marker(img,y,x,R,G,B)
%Drawing Markers in image img. The marker is +. There is no other choice.
%The color of the marker is defining by R,G,B values.R,G and B values
%should be in range [0,255].
%y and x are vectors with equal lenghts containing coordinates of y and x in matrix coordinate
%system of the points where should be put a marker.
%0------>X
%|
%|
%|
%v
%Y
%
%The function take raw
%image img and return the same image with markers on it.
[size_m,~]=size(y);
for i=1:1:size_m
        img(y(i),x(i),1)=R;
        img(y(i),x(i),2)=G;
        img(y(i),x(i),3)=B;

        img(y(i)+1,x(i),1)=R;
        img(y(i)+1,x(i),2)=G;
        img(y(i)+1,x(i),3)=B;

        img(y(i)-1,x(i),1)=R;
        img(y(i)-1,x(i),2)=G;
        img(y(i)-1,x(i),3)=B;

        img(y(i),x(i)+1,1)=R;
        img(y(i),x(i)+1,2)=G;
        img(y(i),x(i)+1,3)=B;

        img(y(i),x(i)-1,1)=R;
        img(y(i),x(i)-1,2)=G;
        img(y(i),x(i)-1,3)=B;
end
    draw_img=img;