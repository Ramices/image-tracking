function [draw_img]=draw_rectangle_marker(img,RectSize,y,x,R,G,B)
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
        
        
        
