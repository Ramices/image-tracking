#include"TemplateBasedTracking.h"
#include<math.h>

#define STARTyPOINT 150
#define STARTxPOINT 150
#define WINDOWSIZE 40

extern char HristoFlag;

void TemplateBasedTracking(uint8_t *frame)
{
	static char flag=0;
	unsigned short int currentStartYpoint=0,currentStartXpoint=0;
	unsigned short int r,c;
	unsigned short int rt,ct;
	unsigned int maxSSD=WINDOWSIZE*WINDOWSIZE*255;
	unsigned int middleSSD=0;

	static unsigned char  grayTemplateImage[WINDOWSIZE*WINDOWSIZE];
	unsigned char grayFrame[QVGA_WIDTH*QVGA_HEIGHT*2];

	if (flag==0)
	{
		DrawRectangle(frame,STARTyPOINT,STARTxPOINT,0);
		if (HristoFlag)
		{
			ConvertRgb2Gray(frame,grayFrame);
			GrabImage(grayFrame,STARTyPOINT,STARTxPOINT,grayTemplateImage);
			flag=1;
		}
	}

	if (flag==1)
	{
		ConvertRgb2Gray(frame,grayFrame);
		for(r=0;r<QVGA_HEIGHT*2-WINDOWSIZE;r++)
		{
			for(c=0;c<QVGA_WIDTH-WINDOWSIZE;c++)
			{
				middleSSD=0;
				for(rt=0;rt<WINDOWSIZE;rt++)
				{
					for(ct=0;ct<WINDOWSIZE;ct++)
					{
						int differance; 
						differance=(grayFrame[(r+rt)*QVGA_WIDTH+(c+ct)])-(grayTemplateImage[rt*WINDOWSIZE+ct]);
						middleSSD+=differance*differance;
					}
				}
				if(middleSSD<maxSSD)
				{
					maxSSD=middleSSD;
					currentStartYpoint=r;
					currentStartXpoint=c;
				}
			}	
		}
	}
		DrawRectangle(frame,currentStartYpoint,currentStartXpoint,1);
}



/******************************************************************************************************************************/ 

void DrawRectangle(uint8_t *frame,unsigned short int startYpoint,unsigned short int startXpoint,char colour)
{
	//colour 0-red,1-green,2-blue;
	unsigned short int r,c;

	for(r=startYpoint;r<=startYpoint+WINDOWSIZE;r++)
		for(c=startXpoint;c<=startXpoint+WINDOWSIZE;c++)
			if ((r==startYpoint)||(r==startYpoint+WINDOWSIZE)||(c==startXpoint)||(c==startXpoint+WINDOWSIZE))
			{
				*(frame+(r*QVGA_WIDTH+c)*3+colour)=255;
				*(frame+(r*QVGA_WIDTH+c)*3+colour+1)=0;
				*(frame+(r*QVGA_WIDTH+c)*3+colour+2)=0;
			}
}

void GrabImage(uint8_t *frame,unsigned short int startYpoint,unsigned short int startXpoint,unsigned char *rgbCropedImage)
{	
	unsigned short int r,rl;
	unsigned short int c,cl;
	unsigned int pixGlobalImage;
	unsigned int pixLocalImage;

	for(r=startYpoint,rl=0;r<startYpoint+WINDOWSIZE;r++,rl++)
		for(c=startXpoint,cl=0;c<startXpoint+WINDOWSIZE;c++,cl++)
		{
			pixGlobalImage=r*QVGA_WIDTH+c;
			pixLocalImage=rl*WINDOWSIZE+cl;
			*(rgbCropedImage+pixLocalImage)=*(frame+pixGlobalImage);
		}
}

void ConvertRgb2Gray(unsigned char *rgbImage,unsigned char *grayImage)
{
	unsigned int i;
	for(i=0;i<QVGA_WIDTH*QVGA_HEIGHT*2;i++)
		*(grayImage+i)=(unsigned char)(((*(rgbImage+3*i))*0.299+(*(rgbImage+3*i+1))*0.587+(*(rgbImage+3*i+2))*0.114));
}