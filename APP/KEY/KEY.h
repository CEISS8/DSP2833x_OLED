/*
 * key.h
 *
 *  Created on: 2018-1-22
 *      Author: Administrator
 */

#ifndef KEY_H_
#define KEY_H_
#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件

#define KEY_L1_SetL			(GpioDataRegs.GPBCLEAR.bit.GPIO48=1)
#define KEY_L2_SetL			(GpioDataRegs.GPBCLEAR.bit.GPIO49=1)
#define KEY_L3_SetL			(GpioDataRegs.GPBCLEAR.bit.GPIO50=1)

#define KEY_L1_SetH			(GpioDataRegs.GPBSET.bit.GPIO48=1)
#define KEY_L2_SetH			(GpioDataRegs.GPBSET.bit.GPIO49=1)
#define KEY_L3_SetH			(GpioDataRegs.GPBSET.bit.GPIO50=1)

#define KEY_H1			(GpioDataRegs.GPADAT.bit.GPIO12)
#define KEY_H2			(GpioDataRegs.GPADAT.bit.GPIO13)
#define KEY_H3			(GpioDataRegs.GPADAT.bit.GPIO14)

#define KEY1_PRESS		1
#define KEY2_PRESS		2
#define KEY3_PRESS		3
#define KEY4_PRESS		4
#define KEY5_PRESS		5
#define KEY6_PRESS		6
#define KEY7_PRESS		7
#define KEY8_PRESS		8
#define KEY9_PRESS		9
#define KEY_UNPRESS		0

void KEY_Init(void);
char KEY_Scan(char mode);

#endif /* KEY_H_ */
