function [draw_img]=draw_rectangle_marker(img,RectSize,y,x,R,G,B)
% Draw Marker
%[draw_img]=draw_rectangle_marker(img,RectSize,y,x,R,G,B)
%Drawing Markers in image img. The marker is a rectangle. There is no other choice.
%The color of the marker is defining by R,G,B values.R,G and B values
%should be in range [0,255]. RectSize - the size of the rectangle.
%"y" and "x" are vectors with equal lenghts containing coordinates of y and x in matrix coordinate
%system of the points where should be put a marker.
%The function take raw a image img and return the same image with markers on it.
SemiSize=round(RectSize/2);
y1=y-SemiSize;
x1=x-SemiSize;
for i=x1:1:x1+RectSize
    for j=y1:1:y1+RectSize
    if (((i==x1)||(i==x1+RectSize))||((j==y1)||(j==y1+RectSize)))
        img(j,i,1)=R;
        img(j,i,2)=G;
        img(j,i,3)=B;
    end
    end
end
draw_img=img;
        
        
        
