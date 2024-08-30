#include <stdio.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "2in13_epd.h"
#include "Ap_213demo.h"	
#include "epd_gui.h"
#include "fonts.h"

/***define pin conection in wiringpi number
2.13inch_EPD    Raspberry Pi
      VCC   --->   3.3V
      GND   --->   GND
      RST   --->   P0
      BUSY  --->   P5
      D/C   --->   P6
      MOSI  --->   MOSI
      CLK   --->   CLK
      CS    --->   (CE0)P10 
*****************************/

//Tips//
/*
1.When the e-paper is refreshed in full screen, the picture flicker is a normal phenomenon, and the main function is to clear the display afterimage in the previous picture.
2.When the partial refresh is performed, the screen does not flash.
3.After the e-paper is refreshed, you need to put it into sleep mode, please do not delete the sleep command.
4.Please do not take out the electronic paper when power is on.
5.Wake up from sleep, need to re-initialize the e-paper.
6.When you need to transplant the driver, you only need to change the corresponding IO. The BUSY pin is the input mode and the others are the output mode.
*/
int	main(void)
{
	unsigned char Fen_L,Fen_H,Miao_L,Miao_H;
	printf("2.13inch epaper demo\r\n");
	EPD_GPIO_Init();       //EPD GPIO  initialization
	while(1)
	{
		/************Fast picture display(1.5s)*******************/
		EPD_HW_Init_Fast(); //EPD init Fast
		EPD_WhiteScreen_White();//EPD Clear
		EPD_WhiteScreen_ALL_Fast(gImage_0,gImage_1);//EPD_picture0 
	    //TEST_PIN = 0;
		EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
		delay(2000); //2s	
					
// 		//Clear
		EPD_HW_Init(); //EPD init
		EPD_WhiteScreen_ALL(gImage_2,gImage_3);//EPD_picture1		
		EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
		delay(2000); //2s	

//////////////////////Partial refresh time demo/////////////////////////////////////
		EPD_HW_Init(); //Electronic paper initialization	
		EPD_SetRAMValue_BaseMap(gImage_basemap0,gImage_basemap1); //Partial refresh background color 2.1s	 				
		for(Fen_H=0;Fen_H<6;Fen_H++)
		{
			for(Fen_L=0;Fen_L<10;Fen_L++)
			{
				for(Miao_H=0;Miao_H<6;Miao_H++) 	
				{
					for(Miao_L=0;Miao_L<10;Miao_L++)
					{
                        //EPD_Dis_Part_myself function need 650~670ms											
						EPD_Dis_Part_myself(40,188,Num[Miao_L],  //x-A,y-A,DATA-A
											40,156,Num[Miao_H],         //x-B,y-B,DATA-B
											40,114,gImage_numdot,      //x-C,y-C,DATA-C
											40,74,Num[Fen_L],         //x-D,y-D,DATA-D
											40,42,Num[Fen_H],32,64);	 //x-E,y-E,DATA-E,Resolution 32*64 
                         delay(100);
						 if((Fen_L==0)&&(Miao_H==1)&&(Miao_L==0))
						    goto Clear;					
					}
				}		
			}
		}
		
////////////////////////////////////////////////////////////////////////	
	//Clear screen
	Clear:
		EPD_HW_Init(); //Electronic paper initialization
		EPD_WhiteScreen_White(); //Show all white need 2.08s
		EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
		delay(3000);				
				
		///////////////////////////GUI///////////////////////////////////////////////////////////////////////////////////
    	printf("NewImage:BlackImage and RedImage\r\n");
		//Data initialization settings
		Image_Init(BWimage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE); //Set screen size and display orientation
    	Image_Init(RWimage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
//		Gui_SelectImage(image);//Set the virtual canvas data storage location

		/**************Drawing**********************/
		EPD_HW_Init_GUI(); //EPD init GUI
		Gui_Clear(WHITE);
		//Point   
		Gui_Draw_Point(3, 1, BLACK, PIXEL_1X1, DOT_STYLE_DFT);
		Gui_Draw_Point(3, 15, BLACK, PIXEL_2X2, DOT_STYLE_DFT);
		Gui_Draw_Point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT);
		Gui_Draw_Point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT);
		//Line
		Gui_Draw_Line(5, 5, 30, 110, BLACK, PIXEL_1X1, SOLID);
		Gui_Draw_Line(5, 30, 45, 30, BLACK, PIXEL_1X1, SOLID);
		//Rectangle
		Gui_Draw_Rectangle(15, 5, 55, 45, BLACK, EMPTY, PIXEL_1X1);
		Gui_Draw_Rectangle(75, 5,110, 30, BLACK, FULL, PIXEL_1X1);
		//Circle
		Gui_Draw_Circle(150, 25, 18, BLACK, EMPTY, PIXEL_1X1);
		Gui_Draw_Circle(216, 25, 18, RED, FULL, PIXEL_1X1);

		Gui_Draw_Str(20, 50, "abcdefg", &Font12, WHITE, BLACK); //7*12
		Gui_Draw_Str(100,50, "ABCabc012345", &Font16, WHITE, BLACK); //11*16
		Gui_Draw_Str(40, 70, "2.13\" E-Paper", &Font20, WHITE, RED); //14*20
		Gui_Draw_Str(60, 95, "SEENGREAT", &Font24, WHITE, RED); //17*24
		EPD_Display(BWimage,RWimage);//display image
		EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
		delay_xms(3000); //2s					
		
		//Clear screen
		EPD_HW_Init(); //Electronic paper initialization
		EPD_WhiteScreen_White(); //Show all white
		EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
		delay(2000);		
        printf("end\n");
		return 0;	
	}
}	


