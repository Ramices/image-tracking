#include"MeanShiftTracking2.h"
#include <stdio.h>
#include <math.h>

#define startYpoint 150
#define startXpoint 150//primerno
#define windowSize 40//primerno

extern char HristoFlag;

void MeanShiftTracking(uint8_t *frame)
{
	static char flag=0;

	static unsigned int initialCoordinates[2]={startYpoint+(windowSize/2),startXpoint+(windowSize/2)};
	unsigned int currentCoordinates[2]={0,0};

	static unsigned char  rgbTemplateImage[3*windowSize*windowSize];//3*windowSize*windowSize
	unsigned char  rgbCandidateImage[3*windowSize*windowSize];//3*windowSize*windowSize
	static	unsigned char  grayTemplateImage[windowSize*windowSize];//windowSize*windowSize
	unsigned char  grayCandidateImage[windowSize*windowSize];//windowSize*windowSize
	static unsigned char  filteredTemplateImage[windowSize*windowSize];//windowSize*windowSize
	unsigned char  filteredCandidateImage[windowSize*windowSize];//windowSize*windowSize
	static double  epanechnikovMask[windowSize*windowSize];//windowSize*windowSize
	static unsigned int templateHistogram[256]={0}; //unsigned char
	unsigned int candidateHistogram[256]={0};

  
	

	if (flag==0)
	{
		
		DrawRectangle(frame,initialCoordinates,0);
		if (HristoFlag)
		{
			GrabImage(frame,initialCoordinates,rgbTemplateImage);
			ConvertRgb2Gray(rgbTemplateImage,grayTemplateImage);
			EstimateEpanechnikovMask(epanechnikovMask);
			FilterEpanechnikov(grayTemplateImage,filteredTemplateImage,epanechnikovMask);
			EstimateHistogram(filteredTemplateImage,templateHistogram);
			flag=1;
		}
	}

	if (flag==1)
	{
		GrabImage(frame,initialCoordinates,rgbCandidateImage);
		ConvertRgb2Gray(rgbCandidateImage,grayCandidateImage);
		FilterEpanechnikov(grayCandidateImage,filteredCandidateImage,epanechnikovMask);
		EstimateHistogram(filteredCandidateImage,candidateHistogram);
		MeanShiftVectEstimate(initialCoordinates,currentCoordinates,templateHistogram,candidateHistogram,filteredCandidateImage);
		initialCoordinates[0]=currentCoordinates[0];
		initialCoordinates[1]=currentCoordinates[1];
		DrawRectangle(frame,initialCoordinates,1);
	}
}

/****************************************************************************************************/

int MyRound(double f)
{
	return (int)floor(f+0.49999);
}

void EstimateEpanechnikovMask(double *epanechnikovMask)
{
	double step=(1/(windowSize*0.5));
	double vec[windowSize]={0};
	double i;
	unsigned char j,r,c;

	for (i=-1.0,j=0;j<windowSize;i=i+step,j++)
	{
		*(vec+j)=(1-i*i)*3/4;//*3/4
	}

	for (r=0;r<windowSize;r++)
		for(c=0;c<windowSize;c++)
			*(epanechnikovMask+r*windowSize+c)=(*(vec+r))*(*(vec+c));
}

void DrawRectangle(uint8_t *frame,unsigned int *initialCoordinates,char colour)
{
	//colour 0-red,1-green,2-blue;
	unsigned int StartYpoint,StartXpoint;
	unsigned int r=0,c=0;

	StartYpoint=MyRound(initialCoordinates[0]-windowSize/2);
	StartXpoint=MyRound(initialCoordinates[1]-windowSize/2);
	
	for(r=StartYpoint;r<=StartYpoint+windowSize;r++)
		for(c=StartXpoint;c<=StartXpoint+windowSize;c++)
			if ((r==StartYpoint)||(r==StartYpoint+windowSize)||(c==StartXpoint)||(c==StartXpoint+windowSize))
			{
				*(frame+(r*QVGA_WIDTH+c)*3+colour)=255;
				*(frame+(r*QVGA_WIDTH+c)*3+colour+1)=0;
				*(frame+(r*QVGA_WIDTH+c)*3+colour+2)=0;
			}
}

void GrabImage(uint8_t *frame,unsigned int *initialCoordinates,unsigned char *rgbCropedImage)
{
	unsigned int StartYpoint;
	unsigned int StartXpoint;
	
	unsigned int r,rl;
	unsigned int c,cl;
	unsigned int pixGlobalImage;
	unsigned int pixLocalImage;

	StartYpoint=MyRound(initialCoordinates[0]-windowSize/2);
	StartXpoint=MyRound(initialCoordinates[1]-windowSize/2);

	for(r=StartYpoint,rl=0;r<StartYpoint+windowSize;r++,rl++)
		for(c=StartXpoint,cl=0;c<StartXpoint+windowSize;c++,cl++)
		{
			pixGlobalImage=(r*QVGA_WIDTH+c)*3;
			pixLocalImage=(rl*windowSize+cl)*3;
			*(rgbCropedImage+pixLocalImage)=*(frame+pixGlobalImage);
			*(rgbCropedImage+pixLocalImage+1)=*(frame+pixGlobalImage+1);
			*(rgbCropedImage+pixLocalImage+2)=*(frame+pixGlobalImage+2);
		}
}

void ConvertRgb2Gray(unsigned char *rgbCropedImage,unsigned char *grayCropedImage)
{
	unsigned char r,c;

	for(r=0;r<windowSize;r++)
		for(c=0;c<windowSize;c++)
		*(grayCropedImage+r*windowSize+c)=(unsigned char)(((*(rgbCropedImage+3*(r*windowSize+c)))*0.299+(*(rgbCropedImage+3*(r*windowSize+c)+1))*0.587+(*(rgbCropedImage+3*(r*windowSize+c)+2))*0.114));
}


void FilterEpanechnikov(unsigned char *grayCropedImage,unsigned char *filteredCropedImage,double *epanechnikovMask)
{
	unsigned char r,c;

	for(r=0;r<windowSize;r++)
		for(c=0;c<windowSize;c++)
		*(filteredCropedImage+r*windowSize+c)=(unsigned char)((*(grayCropedImage+r*windowSize+c))*(*(epanechnikovMask+r*windowSize+c)));
}

void EstimateHistogram(unsigned char *filteredCroppedImage,unsigned int *Histogram)
{
	unsigned char pixValue;
	unsigned char r,c;

	for(r=0;r<windowSize;r++)
		for(c=0;c<windowSize;c++)
		{
			pixValue=*(filteredCroppedImage+r*windowSize+c);
			Histogram[pixValue]=Histogram[pixValue]+1;
		}
}

void MeanShiftVectEstimate(unsigned int *initialCoordinates,unsigned int *currentCoordinates,unsigned int *templateHistogram,unsigned int *candidateHistogram,unsigned char *filteredCandidateImage)
{
	unsigned char CenterY=(windowSize/2);
	unsigned char CenterX=(windowSize/2);
	unsigned char r,c;
	unsigned char PixValue;
	unsigned int pixValTempl;
	unsigned int pixValCand;
	double wi;
	double Wi[windowSize*windowSize]={0};
	double SumWiY=0;
	double SumWiX=0;
	double WiSum=0;
	double MhY=0;
	double MhX=0;


	
	for(r=0;r<windowSize;r++)			//niama smisul da izchisliawam stoinostite za r=0 i c=0 sled kato niama da giizpolzwam sled towa
		for(c=0;c<windowSize;c++)
		{
			wi=0;
			PixValue=*(filteredCandidateImage+r*windowSize+c);

			pixValTempl=templateHistogram[PixValue];
			pixValCand=candidateHistogram[PixValue];
			
			if(pixValCand==0)
			{
				wi=0;
			}
			else
			{
				wi=sqrt((float)pixValTempl/(float)pixValCand);
			}

			*(Wi+r*windowSize+c)=wi;
		}

	for(r=1;r<windowSize;r++)							// celta e simetria (1:39) wmesto (0:39) ili (1:40)(ako ciklite zapochwat s r=0 i c=0 i izpozlwam (CenterY-r+1) i (c-CenterX+1))
	{
		for(c=1;c<windowSize;c++)
		{
			SumWiY+=(*(Wi+r*windowSize+c))*(CenterY-r);//(CenterY-r)
			SumWiX+=(*(Wi+r*windowSize+c))*(c-CenterX);//(c-CenterX)

		}
	}

	for(r=1;r<windowSize;r++)							// sled kato w predhodnia cikul izchisliawam SumWiY i SumWiX ot r=1 i c=1 triabwa i WiSum da se izchisliawa ot r=1 i c=1
		for(c=1;c<windowSize;c++)						// dori ne samo che e logichno a i rezultatite ot proslediawaneto sa po-dobri
		{
			WiSum+=(*(Wi+r*windowSize+c)); 
		}
	
	if(WiSum==0)
	{
		MhY=0;
		MhX=0;
	}
	else
	{
		//MhY=SumWiY/WiSum;
		//MhX=SumWiX/WiSum;
		//printf("SumWiY/WiSum: %f\n",SumWiY/WiSum);
		//printf("SumWiX/WiSum: %f\n",SumWiX/WiSum);
		if((SumWiY/WiSum<0.1)||(SumWiX/WiSum<0.1))
		{
			MhY=10*SumWiY/WiSum;
			MhX=10*SumWiX/WiSum;
		}

		if((SumWiY/WiSum>0.1)||(SumWiX/WiSum>0.1))
		{
			MhY=30*SumWiY/WiSum;
			MhX=30*SumWiX/WiSum;
		}
	}
	
	*currentCoordinates=MyRound((*initialCoordinates)-MhY);//-
	*(currentCoordinates+1)=MyRound((*(initialCoordinates+1))+MhX);//+
}

