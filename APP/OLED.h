/ * OLED.h
 *  Created on: 2022年7月26日
 *      Author: CEISS8
 *  @brief    OLED函数接口头文件
 */
 
#ifndef OLED_H_
#define OLED_H_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#define Uint8 unsigned char
#define OLED_RES_SET()        GpioDataRegs. GPASET.bit.GPIO27=1
#define OLED_RES_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO27=1

//#define OLED_SDA_SET()        GpioDataRegs. GPASET.bit.GPIO16=1
//#define OLED_SDA_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO16=1

//#define OLED_SCK_SET()        GpioDataRegs. GPASET.bit.GPIO18=1
//#define OLED_SCK_CLR()        GpioDataRegs. GPACLEAR.bit.GPIO18=1

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据

#define devAddr  0x78 //OLED的从机地址

void delay_1ms(Uint16 t);
void OLED_Init(void);
void RES_GPIO_Config(void);
void OLED_ColorTurn(Uint8 i);
void OLED_DisplayTurn(Uint8 i);
void Send_Byte(Uint8 dat,Uint16 rom);
void OLED_WR_Byte(Uint8 dat,Uint8 mode);
void OLED_WriteOneByte(Uint16 WriteAddr,unsigned char DataToWrite);

void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);

void DisplayChar_12X06(unsigned char x,unsigned char y,unsigned char *dispdata);
void DisplayChar_16X08(unsigned char x,unsigned char y,unsigned char *dispdata);
void DisplayFont_16X16(unsigned char x,unsigned char y,Uint8 len,unsigned char *dispdata);
void OledDisp128X64(unsigned char *display);
void OledDispPicture(unsigned char x,unsigned char y,unsigned char width,Uint16 fontlen,unsigned char *picture);

Uint32 OLED_Pow(Uint8 m,Uint8 n);
void OledVerticalDisplay(unsigned char toprow,unsigned char scrollrow);
void OledHorizontalDisplay(unsigned char direction,unsigned char startpage,unsigned char endpage,unsigned char interval);
void OledVHScrollDisplay(unsigned char Hdirection,unsigned char startpage,unsigned char endpage,unsigned char interval,unsigned char Voffset);
void OledScrollStop(void);
void OledScrollTest(void);
void OledClearPageCom(unsigned char pagestart,unsigned char pageend,unsigned char comstart,unsigned char comend);

/*提供给外部的点阵字模数据    注意要加上extern，否则不能调用。
//修饰符extern用在变量或者函数的声明前，用来说明此变量/函数是在别处定义的，要在此处引用  */
extern const unsigned char ASCII_1206[95][12];
extern const unsigned char ASCII_1608[95][16];
extern const unsigned char Table1[160];
extern const unsigned char Table2[32];
extern const unsigned char Table3[106];
extern const unsigned char TestUI_Page4[192];
extern const unsigned char Scroll_Horizontal[128];
extern const unsigned char Scroll_Vertical[32];
extern const unsigned char Scroll_VH[32];
extern const unsigned char SK1[4][32];
extern const unsigned char Picture[512];


#endif /*OLED_H_ */


