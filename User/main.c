/*
 * main.c
 *
 *  Created on: 2022年7月26日
 *   Author: CEISS8
 *

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include "leds.h"
#include "time.h"
#include "key.h"
#include "OLED.h"

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main()
{
//    int i=0;
//    float adc_vol;  //ADC读取的电压值
	unsigned char key=0,k=0;
	InitSysCtrl();
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	LED_Init();
	TIM0_Init(150,200000);//200ms
	UARTa_Init(4800);
	SMG_Init();
	KEY_Init();
 	//AT24CXX_Init();   //即 IICA_Init()函数
	IICA_Init();
   // ADC_Init();

//    PieCtrlRegs.PIEIER8.bit.INTx1 = 1;  //使能PIE组8的INT1
//    IER |= M_INT8; // 使能CPU中断8（INT8）
//
    /*  添加用户功能具体代码*/
    RES_GPIO_Config(); //配置RES管脚的端口 放在OLED.h中
    OLED_Init(); //OLED初始化
    OLED_ColorTurn(1);// 0正常显示，1反色显示
    OLED_DisplayTurn(0);//开启屏幕  这里都是CMD
    OLED_Clear();
    OLED_ColorTurn(0);// 0正常显示，1反色显示

    EINT;   //开全局中断
    ERTM;

//	while(AT24CXX_Check())  //检测AT24C02是否正常
//	{
//		UARTa_SendString("AT24C02检测不正常!\r\n");
//		DELAY_US(500*1000);
//	}
//	UARTa_SendString("AT24C02检测正常!\r\n");


	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY1_PRESS)
		{
			k++;
			if(k>35)
			{
				k=5;
			}
//			AT24CXX_WriteOneByte(0,k);

		}
		else if(key==KEY2_PRESS)
		{
		    k=30;
            OledScrollTest();
//			k=AT24CXX_ReadOneByte(0);
		}
		else if(key==KEY3_PRESS)
		{
			k=0;
		}
//        adc_vol=(float)Read_ADCValue()*3.0/4096;//AD读取得电压值，也可*3.0/4095  量程0~3
//        SMG_DisplayFloat(adc_vol,2);    //数码管保留2位小数

	        DisplayChar_16X08(4,30,"COME ON!");
// 		    DisplayChar_16X08(0,28,"OLED TEST");
// 		    DisplayChar_16X08(2,0,"1.");
//  	    DisplayFont_16X16(2,16,192,TestUI_Page2);
// 		    DisplayChar_16X08(4,0,"2.");
	        OLED_Clear();
	        OLED_ColorTurn(1);
 		    DisplayFont_16X16(4,16,192,TestUI_Page4);

// 		    DisplayChar_16X08(6,0,"3.");
// 		    DisplayFont_16X16(6,16,192,TestUI_Page6);
            OLED_Clear();
            OLED_ColorTurn(0);
            OLED_Clear();
            OledDispPicture(0,0,64,512,Picture);
            OLED_Clear();
            OledScrollTest();
            OLED_Clear();


		LED1_ON;
		LED2_TOGGLE;
//		DELAY_US(1000);
//        SMG_DisplayInt(k);    //数码管显示函数
	}
}





