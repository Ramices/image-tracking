#include"visual_tracking.h"

extern char HristoFlag;

void ColourTracking(uint8_t *pixbuf_data)
{
	
	if(HristoFlag)
	{
  Custom_rgb2hsv(pixbuf_data);
  Noise_reduction(pixbuf_data);
  Centroid_finder(pixbuf_data);
	}
	else 
	TakeSampleMarker(pixbuf_data);
}

void TakeSampleMarker(uint8_t *pixbuf_data)
{
	char i,j;
	uint8_t *marker;

	for(j=-2;j<=2;j++)						// redowe po j (t.e. y)
		{
		for(i=-2;i<=2;i++)
			{
				marker=pixbuf_data+(QVGA_WIDTH*(QVGA_HEIGHT+j)+(QVGA_WIDTH/2+i))*3;
				*marker=0;
				*(marker+1)=255;
				*(marker+2)=0;
			}
		}
}


void Custom_rgb2hsv(uint8_t *pixbuf_data)
{
//Preorazmeriawane na izobrajenieto "horizontal split screen".RGB kum HSV transformacia. priemane na tursenata HSV stoinost namirashta se w centura na
//kadura pri natiskane na suotwetnia buton

	unsigned short int i,j,r;						// uprawliawashti promenliwi w cikli "for"
	uint8_t *pix_data_source,*pix_data_dest;		// ukazateli kum iztochnik i destinacia na cwetowi komponenti 
	float R,G,B,H,V,S,min,max,Cr,Cg,Cb;				// parametri izpozwani pri algorituma za HSV transformacia


//flag i shabloni za ustanowiawane na tursenite HSV stoinosti w izobrajenieto
	static float pattern_H=0,pattern_S=0,pattern_V=0;
	static char flag=0;


	for(j=2,r=1;j<QVGA_HEIGHT*2;j+=2,r++)					// redowe po j (t.e. y)
	{
		for(i=0;i<QVGA_WIDTH;i++)							// stulbowe po i (t.e. x)
			{
				pix_data_source=pixbuf_data;
				pix_data_dest=pixbuf_data;

				pix_data_source=pix_data_source+((QVGA_WIDTH*j+i)*3);
				pix_data_dest=pix_data_dest+((QVGA_WIDTH*r+i)*3);
				*pix_data_dest=*pix_data_source;
				*(pix_data_dest+1)=*(pix_data_source+1);
				*(pix_data_dest+2)=*(pix_data_source+2);
			}
	}
 



	for(j=0;j<QVGA_HEIGHT;j++)						// redowe po j (t.e. y; w star wariant QVGA_HEIGHT/2 pri DRONE_VIDEO_MAX_WIDTH
	{
		for(i=0;i<QVGA_WIDTH;i++)                   // stulbowe po i (t.e. x)
		{
			pix_data_source=pixbuf_data;
			pix_data_source=pix_data_source+(QVGA_WIDTH*j+i)*3;
			R=*pix_data_source;
			G=*(pix_data_source+1);
			B=*(pix_data_source+2);					
								
			if((B>=R) && (B>=G))
				{
					max=B;
					if(R>=G)
						min=G;
					else
						min=R;
				}
			else if((G>=R) && (G>=B))
				{
					max=G;
					if(B>=R)
						min=R;
					else
						min=B;
				}
			else if((R>=B) && (R>=G))
				{
					max=R;
					if(B>=G)
						min=G;
					else
						min=B;
				}

			V = max; //[0,255]
			S = (max-min)/max; //[0,1]
			if(S>0.1)
			{
				Cr = (max-R)/(max-min);	// YCrCb
				Cg = (max-G)/(max-min);
				Cb = (max-B)/(max-min); //Cr,Cg,Cb [0,1]
				if(max==R) 
				{
					H = Cb-Cg;//[1,-1]
				}	
				else if(max==G)
				{
					H = 2+Cr-Cb;//[1,3]
				}
				else if(max==B)
				{
					H = 4+Cg-Cr;//[3,5]
				}
				H = H*60;//[-60,300]
			}
			if(H<0) H=H+360;
			else if(H>360) H=H-360;  //[0,360]
			

			if((j==QVGA_HEIGHT/2)&&(i==QVGA_WIDTH/2)&&(flag==0)){
			  pattern_H=H;
			  pattern_S=S;
			  pattern_V=V;
			  flag=1;
			}
				
			pix_data_dest=pix_data_source+240*QVGA_WIDTH*3; 
			if((flag==1)&&(pattern_H+10>H)&&(pattern_H-10<H)&&(pattern_S+0.2>S)&&(pattern_S-0.2<H)&&(pattern_V+20>V)&&(pattern_V-20<V))
			{
				*pix_data_dest=255;
				*(pix_data_dest+1)=255;
				*(pix_data_dest+2)=255;
			} 
			else
			{
				*pix_data_dest=0;
				*(pix_data_dest+1)=0;
				*(pix_data_dest+2)=0;
			} 
		}
	}
}
 
//
/**********************************************************************************************/

 
/**********************************************************************************************/
//
// Premahwane na shumowe

void Noise_reduction(uint8_t *pixbuf_data)
{
	unsigned short int i,j;
	unsigned char r,c;
	uint8_t *init_halfpic_pointer,*mask_pointer,*pointer;//*init_mask_pointer,*mask_pointer;
	unsigned char mask_size=5;
	unsigned char threshold=3;
	unsigned char sum;

	init_halfpic_pointer=pixbuf_data;
	init_halfpic_pointer=init_halfpic_pointer+240*QVGA_WIDTH*3;
	

for(j=0;j<QVGA_HEIGHT;j=j+mask_size)						// redowe po j (t.e. y)
{
	for(i=0;i<QVGA_WIDTH;i=i+mask_size)                   // stulbowe po i (t.e. x)
	{
		mask_pointer=init_halfpic_pointer+(QVGA_WIDTH*j+i)*3;
		sum=0;
		for(r=0;r<mask_size;r++)
		{
			for(c=0;c<mask_size;c++)
			{
				pointer=mask_pointer+(QVGA_WIDTH*r+c)*3;
				if((*pointer)==255) sum++;
			}
		}
		if(sum<=(mask_size*mask_size-threshold))
		{
			mask_pointer=init_halfpic_pointer+(QVGA_WIDTH*j+i)*3;
			for(r=0;r<mask_size;r++)
			{
				for(c=0;c<mask_size;c++)
				{
					pointer=mask_pointer+(QVGA_WIDTH*r+c)*3;
					*pointer=0;
					*(pointer+1)=0;
					*(pointer+2)=0;	
				}
			}
		}
	}
}


}
//
/**********************************************************************************************/


/**********************************************************************************************/
//
// Namirane na centura na obekta
void Centroid_finder(uint8_t *pixbuf_data)
{
	short int i=0,j=0;
	unsigned int number=0,i_coord=0,j_coord=0; // broi beli pikseli; suma ot "i" i "j" koordinatite na wsichki beli pikseli
	unsigned short int i_coord_final=0,j_coord_final=0;
	uint8_t *init_halfpic_pointer,*pix_pointer,*marker;

	init_halfpic_pointer=pixbuf_data+240*QVGA_WIDTH*3;
	
	for(j=0;j<QVGA_HEIGHT;j+=1)						// redowe po j (t.e. y)
	{
		for(i=0;i<QVGA_WIDTH;i+=1)                   // stulbowe po i (t.e. x)
		{
			pix_pointer=init_halfpic_pointer+(QVGA_WIDTH*j+i)*3;
			if((*pix_pointer)>1)
			{	
				number+=1;
				i_coord+=i;
				j_coord+=j;
			}
		}
	}
	
	if(number>0)
	{
		i_coord_final=i_coord/number;
		j_coord_final=j_coord/number;
		for(j=-2;j<=2;j++)						// redowe po j (t.e. y)
		{
			for(i=-2;i<=2;i++)
			{
				marker=init_halfpic_pointer+(QVGA_WIDTH*(j_coord_final+j)+(i_coord_final+i))*3;
				*marker=255;
				*(marker+1)=0;
				*(marker+2)=0;
			}
		}
	}
}






			






