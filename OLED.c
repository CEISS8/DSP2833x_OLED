/*
 * OLED.c
 *
 *  Created on: 2022年7月26日
 *      Author: cai'heng'yi
 */

/*
 * OLED.c
 *
 *  Created on: 2022年7月22日
 *      Author: cai'heng'yi
 *  @brief               OLED显示函数接口
 *  *******************************************************************************************
 *  @attention
 *  实验平台：SK-F28335Mini   核心板
 *  CSDN博客：https://blog.csdn.net/weixin_46556696
 *  淘宝：https://shop409670932.taobao.com
 */
#include "OLED.h"
#include "iic.h"
#include "oledfont.h"
Uint8 OLED_GRAM[144][8];

void delay_1ms(Uint16 t)
{
    while(t--)
    {
        DELAY_US(1000);
    }
}
/**
 *  @brief               OLED初始化
 *  @parameter            无
 *  @return_value           无
 */
void OLED_Init(void)
{
    OLED_RES_CLR();
    delay_1ms(200);
    OLED_RES_SET();
    OLED_WR_Byte(0xAE,OLED_CMD);//关闭OLED屏幕
    OLED_WR_Byte(0x00,OLED_CMD);//设置低列地址
    OLED_WR_Byte(0x10,OLED_CMD);//设置高列地址
    OLED_WR_Byte(0x40,OLED_CMD);//设置行起始地址，设置映射 RAM 显示起始行 (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//设置对比度控制寄存器
    OLED_WR_Byte(0xCF,OLED_CMD);//设置SEG亮度
    OLED_WR_Byte(0xA1,OLED_CMD);//设置 SEG/列映射   0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//设置 COM/Row 扫描方向   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//设置正常显示
    OLED_WR_Byte(0xA8,OLED_CMD);//设置复用率（1 到 64）
    OLED_WR_Byte(0x3f,OLED_CMD);//1/64
    OLED_WR_Byte(0xD3,OLED_CMD);//设置显示偏置   移位映射 RAM 计数器 (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//不偏置
    OLED_WR_Byte(0xd5,OLED_CMD);//设置显示时钟分频比/振荡器频率
    OLED_WR_Byte(0x80,OLED_CMD);//设置分频比，设置时钟为 100 帧/秒
    OLED_WR_Byte(0xD9,OLED_CMD);//设置预充电周期
    OLED_WR_Byte(0xF1,OLED_CMD);//设置预充电 为 15 个时钟 & 放电 为 1 个时钟
    OLED_WR_Byte(0xDA,OLED_CMD);//设置 COM 引脚硬件配置
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//设置 VCOM
    OLED_WR_Byte(0x30,OLED_CMD);//设置 VCOM 取消选择级别
    OLED_WR_Byte(0x20,OLED_CMD);//设置页面寻址模式 (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//设置电荷泵启用/禁用
    OLED_WR_Byte(0x14,OLED_CMD);//设置(0x10) 禁用
    OLED_Clear();
    OLED_WR_Byte(0xAF,OLED_CMD);
}

//***************RES引脚配置在GPIO27*****************
void RES_GPIO_Config(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO27=0;//普通IO模式
    GpioCtrlRegs.GPAPUD.bit.GPIO27=0;//使能内部上拉
    GpioCtrlRegs.GPADIR.bit.GPIO27=1;//配置成输出
    GpioDataRegs.GPASET.bit.GPIO27=1;//置1
    EDIS;
}

/**
 *  @brief                   反显函数
 *  @parameter                  i=0为正常显示；i=1为反色显示
 *  @return_value               无
 */
void OLED_ColorTurn(Uint8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
    }
    if(i==1)
    {
        OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
    }
}

/**
 *  @brief                   屏幕旋转180度
 *  @parameter                  i=0为正常显示；i=1为反转显示
 *  @return_value               无
 */
void OLED_DisplayTurn(Uint8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
        OLED_WR_Byte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
        OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

/*************************************************************************
 *  @brief                   向OLED发送一个字节
 *  @parameter                  dat： 写入的数据； mode：数据/命令 0表示命令，1表示数据
 *  @return_value               无
 */
void OLED_WR_Byte(Uint8 dat,Uint8 mode)
{
    if(mode)
        Send_Byte(dat,0x40);//写数据 mode≠0时写数据
    else
        Send_Byte(dat,0x00);//写命令 mode=0时写命令
}
/**************************************************************************
 *  @brief          写入一个字节
 *  @parameter      dat：写入的数据 ； rom：准备写入数据的寄存器地址
 *  @return_value    无      用iic.c 把8位dat 写入16位rom地址中
 */
void Send_Byte(Uint8 dat,Uint16 rom)
{
    //while(WriteData(devAddr,rom,1,&dat));//等待发送成功
    OLED_WriteOneByte(rom,dat);
}

/*******************************************************************************
* 函 数 名         : OLED_WriteOneByte
* 函数功能         : 在OLED指定地址写入一个数据，即在WriteAddr中写入DataToWrite
* 输    入         : WriteAddr  :写入数据的目的地址
                     DataToWrite:要写入的数据—— 1个字节
* 输    出         : 无
*******************************************************************************/
void OLED_WriteOneByte(Uint16 WriteAddr,unsigned char DataToWrite)
{
    IIC_Start();         //起始信号
                         //这里调用时 设WriteAddr = 0
    IIC_Send_Byte(0X78+((WriteAddr/256)<<1));   //发送器件地址0X78,写数据
    IIC_Wait_Ack();                     // 实质上还是0x78
    IIC_Send_Byte(WriteAddr%256);   //发送低地址  16位 %256 剩下低8位地址 0
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);     //发送字节 IIC_Send_Byte()函数是把DataToWrite发送到总线上
    IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件，写入完成
    DELAY_US(10);
    //DELAY_US(10*1000);
}

/**
 *  @brief                   开启显示
 *  @parameter                 无
 *  @return_value               无
 */
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//开启屏幕
}

/**
 *  @brief                   关闭显示
 *  @parameter                 无
 *  @return_value               无
 */
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

/**
 *  @brief                   更新显存到OLED
 *  @parameter                 无
 *  @return_value               无
 */
void OLED_Refresh(void)
{
    Uint8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
        OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
        for(n=0;n<128;n++)
        {
            OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
            //Send_Byte(OLED_GRAM[n][i]);
        }
    }
}

/**
 *  @brief                   清屏函数
 *  @parameter                 无
 *  @return_value               无
 */
void OLED_Clear(void)
{
    Uint8 i,n;
    for(i=0;i<8;i++)
    {
       for(n=0;n<128;n++)
       {
           OLED_GRAM[n][i]=0;//清除所有数据
       }
    }
    OLED_Refresh();//更新显示
}



/*****************************************************
** 函数名称：DisplayChar_12X06
** 函数功能：显示12*06ASCII字符串
** 参    数：1.x: 字符串显示的起始页地址
             2.y：字符串显示的起始列地址
             3.*dispdata：指向需要显示的字符串指针
** 函数返回：无
** 编写作者：326电子工作室
** 编写时间：2015年5月19日
** 网    址：  ttsemb.taobao.com

******************************************************/
void DisplayChar_12X06(unsigned char x,unsigned char y,unsigned char *dispdata)
{
    unsigned char i,j,fontposition,addyl,addyh;
    unsigned char *databuf;

    while((*dispdata)!=0)
    {
        /*获取字符在字库中的位置*/
        fontposition=*dispdata-0x20;                    //计算字符在字库中的位置
        databuf=(unsigned char *)(ASCII_1206+fontposition);     //获取字符点阵数据起始地址

        /*计算当前字符显示的起始列地址*/
        addyl=y%16;                                     //计算列低地址
        addyh=y/16+0x10;                                //计算列高地址

        /*显示字符*/
        for(i=0;i<2;i++)
        {
            OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //设置页地址
            OLED_WR_Byte(addyl,OLED_CMD);              //设置列低地址
            OLED_WR_Byte(addyh,OLED_CMD);              //设置列高地址

            for(j=0;j<6;j++)
            {
                OLED_WR_Byte(*(databuf++),OLED_DATA);
            }
        }

        y+=6;                                           //列地址自加6
        dispdata++;
    }
}

void DisplayChar_16X08(unsigned char x,unsigned char y,unsigned char *dispdata)
{
    unsigned char i,j,fontposition,addyl,addyh;
    unsigned char *databuf;

    while((*dispdata)!=0)
    {
//      if((y>127)||(x>7))   break;                     //判断字符显示的位置是否超出范围

        /*获取字符在字库中的位置*/
        fontposition=*dispdata-0x20;                    //计算字符在字库中的位置
        databuf=(unsigned char *)(ASCII_1608+fontposition);

        /*计算当前字符显示的起始列地址*/
        addyl=y%16;                                     //计算列低地址
        addyh=y/16+0x10;                                //计算列高地址

        /*显示字符*/
        for(i=0;i<2;i++)
        {
            OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //设置页地址
            OLED_WR_Byte(addyl,OLED_CMD);              //设置列低地址
            OLED_WR_Byte(addyh,OLED_CMD);              //设置列高地址

            for(j=0;j<8;j++)
            {
                OLED_WR_Byte(*(databuf++),OLED_DATA);
            }
        }

        y+=8;                                           //列地址自加6
        dispdata++;
    }
}
/*****************************************************
** 函数名称：DisplayFont_16X16
** 函数功能：显示16*16点阵（本例程主要用来显示已经取模的汉字点阵）
** 参    数：1.x: 字符串显示的起始页地址
             2.y：字符串显示的起始列地址
             3.len：点阵数据长度
             4.*dispdata：指向需要显示的点阵数据指针
** 函数返回：无
** 编写作者：326电子工作室
** 编写时间：2015年5月19日
** 网    址：  ttsemb.taobao.com

******************************************************/
void DisplayFont_16X16(unsigned char x,unsigned char y,Uint8 len,unsigned char *dispdata)
{
    unsigned char i,j,z,count,addyl,addyh;

    count=len/32;

    for(z=0;z<count;z++)
    {
        /*计算当前点阵显示的起始列地址*/
        addyl=y%16;                                     //计算列低地址
        addyh=y/16+0x10;

        /*显示16*16点阵数据*/
        for(i=0;i<2;i++)
        {
            OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //设置页地址
            OLED_WR_Byte(addyl,OLED_CMD);              //设置列低地址
            OLED_WR_Byte(addyh,OLED_CMD);              //设置列高地址
            for(j=0;j<16;j++)
            {
                OLED_WR_Byte(*(dispdata++),OLED_DATA);
            }
        }
        y+=16;
    }
}
/*****************************************************
** 函数名称：OledDisp128X64
** 函数功能：显示128*64点阵图像
** 参    数：display：需要显示的图像存储位置
** 函数返回：无
** 编写作者：326电子工作室
** 编写时间：2015年5月19日
** 网    址：  ttsemb.taobao.com

******************************************************/
void OledDisp128X64(unsigned char *display)
{
   unsigned char i,j;

   for(i=0;i<8;i++)
   {
        OLED_WR_Byte(0xb0+i,OLED_CMD);             //设置页地址
        OLED_WR_Byte(0x00,OLED_CMD);               //设置列低地址
        OLED_WR_Byte(0x10,OLED_CMD);               //设置列高地址

        for(j=0;j<128;j++)
        {
            OLED_WR_Byte(*(display++),OLED_DATA);
        }
   }
}

/*****************************************************
** 函数名称：OledDispPicture
** 函数功能：指定位置显示任意大小图片（128*64内）
** 参    数：1.x: 图片显示的起始页地址
             2.y：图片显示的起始列地址
             3.width：图片宽度
             4.fontlen：图片点阵总字节数
             5.*picture：指向图片点阵数组
** 函数返回：无
******************************************************/
void OledDispPicture(unsigned char x,unsigned char y,unsigned char width,Uint16 fontlen,unsigned char *picture)
{
     unsigned char i,j,pagenum,addyl,addyh;

     /*计算显示图像需要的参数*/
     pagenum=fontlen/width;                         //计算图像使用的页面数量
     addyl=y%16;                                    //计算起始列低地址
     addyh=y/16+0x10;                                   //计算起始列搞地质

     /*显示图像*/
     for(i=0;i<pagenum;i++)
     {
         OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //设置页地址
         OLED_WR_Byte(addyl,OLED_CMD);              //设置列低地址
         OLED_WR_Byte(addyh,OLED_CMD);              //设置列高地址
        for(j=0;j<width;j++)
        {
            OLED_WR_Byte(*(picture++),OLED_DATA);
        }
     }
}

/**
 *  @brief                指数显示
 *  @parameter            m:底数； n:指数
 *  @return_value           结果
 */
Uint32 OLED_Pow(Uint8 m,Uint8 n)
{
    Uint32 result=1;
    while(n--)
    {
      result*=m;
    }
    return result;
}
/*****************************************************
** 函数名称：OledVerticalDisplay
** 函数功能：实现OLED垂直滚动范围配置
** 参    数：1.toprow：设置滚动起始行
**           2.scrollrow：设置滚动行数
**           注意：toprow+scrollrow<64
** 函数返回：无
** 编写作者：326电子工作室
** 编写时间：2015年5月19日
** 网    址：  ttsemb.taobao.com

******************************************************/
void OledVerticalDisplay(unsigned char toprow,unsigned char scrollrow)
{
    OLED_WR_Byte(0xA3,OLED_CMD);   //0xA3 --设置垂直滚动范围命令
    OLED_WR_Byte(toprow,OLED_CMD);
    OLED_WR_Byte(scrollrow,OLED_CMD);
}
/*****************************************************
** 函数名称：OledHorizontalDisplay
** 函数功能：实现OLED水平滚动显示
** 参    数：1.direction：水平滚动方向，向左/向右
**           2.startpage：需要水平滚动的起始PAGE，取值范围0-7
**           3.endpage：需要水平滚动的结束PAGE，取值范围0-7
**           4.interval：每次移动的时间间隔，取值范围0-7
** 函数返回：无
******************************************************/
void OledHorizontalDisplay(unsigned char direction,unsigned char startpage,unsigned char endpage,unsigned char interval)
{
    OLED_WR_Byte(direction,OLED_CMD);                      //设置水平移动方向
    OLED_WR_Byte(0x00,OLED_CMD);                  //0x00 --配置水平移动时，需要的虚拟字节
    OLED_WR_Byte(startpage,OLED_CMD);                      //设置水平移动的起始PAGE
    OLED_WR_Byte(interval,OLED_CMD);                       //设置每次移动的时间间隔
    OLED_WR_Byte(endpage,OLED_CMD);                        //设置水平移动的结束PAGE
    OLED_WR_Byte(0x00,OLED_CMD);                //发送设置所需的虚拟字节
    OLED_WR_Byte(0xFF,OLED_CMD);   //0xFF--发送设置所需的虚拟字节
    OLED_WR_Byte(0x2F,OLED_CMD);                //0x2F--开启OLED滚动显示
}

/*****************************************************
** 函数名称：OledVHScrollDisplay
** 函数功能：实现OLED水平、垂直滚动显示
** 参    数：1.Hdirection：水平滚动方向，向左/向右
**           2.startpage：需要水平滚动的起始PAGE，取值范围0-7
**           3.endpage：需要水平滚动的结束PAGE，取值范围0-7
**           4.interval：每次移动的时间间隔，取值范围0-7
**           5.Voffset: 设置上下滚屏的偏移量，取值范围0x01-0x3F
** 函数返回：无
******************************************************/
void OledVHScrollDisplay(unsigned char Hdirection,unsigned char startpage,unsigned char endpage,unsigned char interval,unsigned char Voffset)
{
    OLED_WR_Byte(Hdirection,OLED_CMD);         //设置水平移动方向
    OLED_WR_Byte(0x00,OLED_CMD);              //0x00-- 发送设置所需的虚拟字节
    OLED_WR_Byte(startpage,OLED_CMD);          //设置水平移动的起始PAGE
    OLED_WR_Byte(interval,OLED_CMD);             //设置每次移动的时间间隔
    OLED_WR_Byte(endpage,OLED_CMD);              //设置水平移动的结束PAGE
    OLED_WR_Byte(Voffset,OLED_CMD);              //设置上下移动方向
    OLED_WR_Byte(0x2F,OLED_CMD);      //0x2F ---开启滚动显示功能
}

/*****************************************************
** 函数名称：OledScrollStop
** 函数功能：关闭滚屏功能
** 参    数：无
** 函数返回：无
** 编写作者：326电子工作室
** 编写时间：2015年5月19日
** 网    址：  ttsemb.taobao.com

******************************************************/
void OledScrollStop(void)
{
    OLED_WR_Byte(0x2E,OLED_CMD);    //0x2E --关闭滚动显示功能
}
/*****************************************************
** 函数名称：OledScrollTest
** 函数功能：实现OLED滚屏显示
** 参    数：无
** 函数返回：无
** 编写作者：326电子工作室
** 编写时间：2015年5月19日
** 网    址：  ttsemb.taobao.com
** 技术支持群：QQ427165833
******************************************************/
void OledScrollTest(void)
{
    OLED_Clear();

    DisplayFont_16X16(0,16,192,TestUI_Page4);
    /*横向滚屏*/
    DisplayFont_16X16(4,32,128,Scroll_Horizontal);
    OledHorizontalDisplay(0x27,4,5,0);  //0x27 ---向左水平移动
    delay_1ms(4000);
    OledHorizontalDisplay(0x26,4,5,0); //0x26---向右水平移动
    delay_1ms(4000);
    /*纵向滚屏*/
    OledScrollStop();
    DisplayFont_16X16(4,32,32,Scroll_Vertical);
    OledVerticalDisplay(16,48);
    OledVHScrollDisplay(0x2A,7,7,0,1);   //0x2A---移动
    delay_1ms(4000);
    OledVerticalDisplay(0,64);
    OledVHScrollDisplay(0x2A,7,7,0,63);//0x2A---移动
    /* 斜向滚屏*/
    delay_1ms(4000);
    OledScrollStop();
    DisplayFont_16X16(4,32,32,Scroll_VH);
    OledVerticalDisplay(16,48);
    OledVHScrollDisplay(0x2A,4,5,0,1);
    delay_1ms(4000);
    OledVerticalDisplay(0,64);
    OledVHScrollDisplay(0x2A,4,5,0,63);
}
/*****************************************************
** 函数名称：Pow
** 函数功能：求一个正整数的N次方
** 参    数：num：需要求次方的正整数；count：需要求的次方数
** 函数返回：num的count次方
******************************************************/
//ulong Pow(ulong num,uchar count)
//{
//  uchar i;
//  ulong result = 0;
//  if(count == 0)
//  {
//      num = 1;
//      return num;
//  }
//  else
//  {
//      result = num;
//      for(i = 0;i<count-1;i++)
//      {
//          result *= num;
//      }
//  }
//  return result;
//}

/*****************************************************
** 函数名称：NumToString
** 函数功能：将一个数的各位转换成对应的ASCII码
** 参    数：num：需要转换的数
**           numbit:需要转换的数的位数
**           *string：转换后的字符串存储位置
** 函数返回：无
******************************************************/
//void NumToString(ulong num,uchar numbit,uchar *string)
//{
//  uchar i=0,z=0;
//  ulong temp = 0;
//  uchar changestart = 0;
//  uchar j = numbit-1;
//
//  for(i = 0;i<numbit;i++)
//  {
//      temp = num/Pow(10,j);
//
//      if(changestart)
//      {
//          string[z++] = temp+NUM_TO_CHAR;
//      }
//      if((temp>0)&&(changestart == 0))
//      {
//          string[z++] = temp+NUM_TO_CHAR;
//          changestart = 1;
//      }
//
//      temp = Pow(10,j);
//      num %= temp;
//      j--;
//  }
//  string[z] = 0;
//}

/*****************************************************
** 函数名称：OledClearPageCom
** 函数功能：清除指定区域显示
** 参    数：1、pagestart：指定页起始地址
             2、pageend：指定页结束地址
             3、comstart：指定列起始地址
             4、comend：指定列结束地址
** 函数返回：无
** 编写作者：326电子工作室
** 编写时间：2015年5月19日
** 网    址：  ttsemb.taobao.com

******************************************************/
void OledClearPageCom(unsigned char pagestart,unsigned char pageend,unsigned char comstart,unsigned char comend)
{
  unsigned char i,j;
  unsigned char addyl,addyh;

  addyl=comstart%16;                                      //计算列低地址
  addyh=comstart/16+0x10;                             //计算列高地址

  for(i=pagestart;i<pageend+1;i++)
  {
      OLED_WR_Byte(0xb0+i,OLED_CMD);             //设置页地址
      OLED_WR_Byte(addyl,OLED_CMD);              //设置列低地址
      OLED_WR_Byte(addyh,OLED_CMD);              //设置列高地址

      for(j=comstart;j<comend+1;j++)
      {
          OLED_WR_Byte(0x00,OLED_DATA);
      }
  }
}









