// My own headers
#include<custom_code.h>



// My own declarations
void TemplateBasedTracking(uint8_t *frame);
void DrawRectangle(uint8_t *frame,unsigned short int startYpoint,unsigned short int startXpoint,char colour);
void GrabImage(uint8_t *frame,unsigned short int startYpoint,unsigned short int startXpoint,unsigned char *rgbCropedImage);
void ConvertRgb2Gray(unsigned char *rgbImage,unsigned char *grayImage);