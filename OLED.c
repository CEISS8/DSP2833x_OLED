/*
 * OLED.c
 *
 *  Created on: 2022��7��26��
 *      Author: cai'heng'yi
 */

/*
 * OLED.c
 *
 *  Created on: 2022��7��22��
 *      Author: cai'heng'yi
 *  @brief               OLED��ʾ�����ӿ�
 *  *******************************************************************************************
 *  @attention
 *  ʵ��ƽ̨��SK-F28335Mini   ���İ�
 *  CSDN���ͣ�https://blog.csdn.net/weixin_46556696
 *  �Ա���https://shop409670932.taobao.com
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
 *  @brief               OLED��ʼ��
 *  @parameter            ��
 *  @return_value           ��
 */
void OLED_Init(void)
{
    OLED_RES_CLR();
    delay_1ms(200);
    OLED_RES_SET();
    OLED_WR_Byte(0xAE,OLED_CMD);//�ر�OLED��Ļ
    OLED_WR_Byte(0x00,OLED_CMD);//���õ��е�ַ
    OLED_WR_Byte(0x10,OLED_CMD);//���ø��е�ַ
    OLED_WR_Byte(0x40,OLED_CMD);//��������ʼ��ַ������ӳ�� RAM ��ʾ��ʼ�� (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//���öԱȶȿ��ƼĴ���
    OLED_WR_Byte(0xCF,OLED_CMD);//����SEG����
    OLED_WR_Byte(0xA1,OLED_CMD);//���� SEG/��ӳ��   0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0xC8,OLED_CMD);//���� COM/Row ɨ�跽��   0xc0���·��� 0xc8����
    OLED_WR_Byte(0xA6,OLED_CMD);//����������ʾ
    OLED_WR_Byte(0xA8,OLED_CMD);//���ø����ʣ�1 �� 64��
    OLED_WR_Byte(0x3f,OLED_CMD);//1/64
    OLED_WR_Byte(0xD3,OLED_CMD);//������ʾƫ��   ��λӳ�� RAM ������ (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//��ƫ��
    OLED_WR_Byte(0xd5,OLED_CMD);//������ʾʱ�ӷ�Ƶ��/����Ƶ��
    OLED_WR_Byte(0x80,OLED_CMD);//���÷�Ƶ�ȣ�����ʱ��Ϊ 100 ֡/��
    OLED_WR_Byte(0xD9,OLED_CMD);//����Ԥ�������
    OLED_WR_Byte(0xF1,OLED_CMD);//����Ԥ��� Ϊ 15 ��ʱ�� & �ŵ� Ϊ 1 ��ʱ��
    OLED_WR_Byte(0xDA,OLED_CMD);//���� COM ����Ӳ������
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//���� VCOM
    OLED_WR_Byte(0x30,OLED_CMD);//���� VCOM ȡ��ѡ�񼶱�
    OLED_WR_Byte(0x20,OLED_CMD);//����ҳ��Ѱַģʽ (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//���õ�ɱ�����/����
    OLED_WR_Byte(0x14,OLED_CMD);//����(0x10) ����
    OLED_Clear();
    OLED_WR_Byte(0xAF,OLED_CMD);
}

//***************RES����������GPIO27*****************
void RES_GPIO_Config(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO27=0;//��ͨIOģʽ
    GpioCtrlRegs.GPAPUD.bit.GPIO27=0;//ʹ���ڲ�����
    GpioCtrlRegs.GPADIR.bit.GPIO27=1;//���ó����
    GpioDataRegs.GPASET.bit.GPIO27=1;//��1
    EDIS;
}

/**
 *  @brief                   ���Ժ���
 *  @parameter                  i=0Ϊ������ʾ��i=1Ϊ��ɫ��ʾ
 *  @return_value               ��
 */
void OLED_ColorTurn(Uint8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
    }
    if(i==1)
    {
        OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
    }
}

/**
 *  @brief                   ��Ļ��ת180��
 *  @parameter                  i=0Ϊ������ʾ��i=1Ϊ��ת��ʾ
 *  @return_value               ��
 */
void OLED_DisplayTurn(Uint8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
        OLED_WR_Byte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
        OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

/*************************************************************************
 *  @brief                   ��OLED����һ���ֽ�
 *  @parameter                  dat�� д������ݣ� mode������/���� 0��ʾ���1��ʾ����
 *  @return_value               ��
 */
void OLED_WR_Byte(Uint8 dat,Uint8 mode)
{
    if(mode)
        Send_Byte(dat,0x40);//д���� mode��0ʱд����
    else
        Send_Byte(dat,0x00);//д���� mode=0ʱд����
}
/**************************************************************************
 *  @brief          д��һ���ֽ�
 *  @parameter      dat��д������� �� rom��׼��д�����ݵļĴ�����ַ
 *  @return_value    ��      ��iic.c ��8λdat д��16λrom��ַ��
 */
void Send_Byte(Uint8 dat,Uint16 rom)
{
    //while(WriteData(devAddr,rom,1,&dat));//�ȴ����ͳɹ�
    OLED_WriteOneByte(rom,dat);
}

/*******************************************************************************
* �� �� ��         : OLED_WriteOneByte
* ��������         : ��OLEDָ����ַд��һ�����ݣ�����WriteAddr��д��DataToWrite
* ��    ��         : WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
                     DataToWrite:Ҫд������ݡ��� 1���ֽ�
* ��    ��         : ��
*******************************************************************************/
void OLED_WriteOneByte(Uint16 WriteAddr,unsigned char DataToWrite)
{
    IIC_Start();         //��ʼ�ź�
                         //�������ʱ ��WriteAddr = 0
    IIC_Send_Byte(0X78+((WriteAddr/256)<<1));   //����������ַ0X78,д����
    IIC_Wait_Ack();                     // ʵ���ϻ���0x78
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ  16λ %256 ʣ�µ�8λ��ַ 0
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);     //�����ֽ� IIC_Send_Byte()�����ǰ�DataToWrite���͵�������
    IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ������д�����
    DELAY_US(10);
    //DELAY_US(10*1000);
}

/**
 *  @brief                   ������ʾ
 *  @parameter                 ��
 *  @return_value               ��
 */
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
    OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

/**
 *  @brief                   �ر���ʾ
 *  @parameter                 ��
 *  @return_value               ��
 */
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
    OLED_WR_Byte(0xAE,OLED_CMD);//�ر���Ļ
}

/**
 *  @brief                   �����Դ浽OLED
 *  @parameter                 ��
 *  @return_value               ��
 */
void OLED_Refresh(void)
{
    Uint8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD); //��������ʼ��ַ
        OLED_WR_Byte(0x00,OLED_CMD);   //���õ�����ʼ��ַ
        OLED_WR_Byte(0x10,OLED_CMD);   //���ø�����ʼ��ַ
        for(n=0;n<128;n++)
        {
            OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
            //Send_Byte(OLED_GRAM[n][i]);
        }
    }
}

/**
 *  @brief                   ��������
 *  @parameter                 ��
 *  @return_value               ��
 */
void OLED_Clear(void)
{
    Uint8 i,n;
    for(i=0;i<8;i++)
    {
       for(n=0;n<128;n++)
       {
           OLED_GRAM[n][i]=0;//�����������
       }
    }
    OLED_Refresh();//������ʾ
}



/*****************************************************
** �������ƣ�DisplayChar_12X06
** �������ܣ���ʾ12*06ASCII�ַ���
** ��    ����1.x: �ַ�����ʾ����ʼҳ��ַ
             2.y���ַ�����ʾ����ʼ�е�ַ
             3.*dispdata��ָ����Ҫ��ʾ���ַ���ָ��
** �������أ���
** ��д���ߣ�326���ӹ�����
** ��дʱ�䣺2015��5��19��
** ��    ַ��  ttsemb.taobao.com

******************************************************/
void DisplayChar_12X06(unsigned char x,unsigned char y,unsigned char *dispdata)
{
    unsigned char i,j,fontposition,addyl,addyh;
    unsigned char *databuf;

    while((*dispdata)!=0)
    {
        /*��ȡ�ַ����ֿ��е�λ��*/
        fontposition=*dispdata-0x20;                    //�����ַ����ֿ��е�λ��
        databuf=(unsigned char *)(ASCII_1206+fontposition);     //��ȡ�ַ�����������ʼ��ַ

        /*���㵱ǰ�ַ���ʾ����ʼ�е�ַ*/
        addyl=y%16;                                     //�����е͵�ַ
        addyh=y/16+0x10;                                //�����иߵ�ַ

        /*��ʾ�ַ�*/
        for(i=0;i<2;i++)
        {
            OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //����ҳ��ַ
            OLED_WR_Byte(addyl,OLED_CMD);              //�����е͵�ַ
            OLED_WR_Byte(addyh,OLED_CMD);              //�����иߵ�ַ

            for(j=0;j<6;j++)
            {
                OLED_WR_Byte(*(databuf++),OLED_DATA);
            }
        }

        y+=6;                                           //�е�ַ�Լ�6
        dispdata++;
    }
}

void DisplayChar_16X08(unsigned char x,unsigned char y,unsigned char *dispdata)
{
    unsigned char i,j,fontposition,addyl,addyh;
    unsigned char *databuf;

    while((*dispdata)!=0)
    {
//      if((y>127)||(x>7))   break;                     //�ж��ַ���ʾ��λ���Ƿ񳬳���Χ

        /*��ȡ�ַ����ֿ��е�λ��*/
        fontposition=*dispdata-0x20;                    //�����ַ����ֿ��е�λ��
        databuf=(unsigned char *)(ASCII_1608+fontposition);

        /*���㵱ǰ�ַ���ʾ����ʼ�е�ַ*/
        addyl=y%16;                                     //�����е͵�ַ
        addyh=y/16+0x10;                                //�����иߵ�ַ

        /*��ʾ�ַ�*/
        for(i=0;i<2;i++)
        {
            OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //����ҳ��ַ
            OLED_WR_Byte(addyl,OLED_CMD);              //�����е͵�ַ
            OLED_WR_Byte(addyh,OLED_CMD);              //�����иߵ�ַ

            for(j=0;j<8;j++)
            {
                OLED_WR_Byte(*(databuf++),OLED_DATA);
            }
        }

        y+=8;                                           //�е�ַ�Լ�6
        dispdata++;
    }
}
/*****************************************************
** �������ƣ�DisplayFont_16X16
** �������ܣ���ʾ16*16���󣨱�������Ҫ������ʾ�Ѿ�ȡģ�ĺ��ֵ���
** ��    ����1.x: �ַ�����ʾ����ʼҳ��ַ
             2.y���ַ�����ʾ����ʼ�е�ַ
             3.len���������ݳ���
             4.*dispdata��ָ����Ҫ��ʾ�ĵ�������ָ��
** �������أ���
** ��д���ߣ�326���ӹ�����
** ��дʱ�䣺2015��5��19��
** ��    ַ��  ttsemb.taobao.com

******************************************************/
void DisplayFont_16X16(unsigned char x,unsigned char y,Uint8 len,unsigned char *dispdata)
{
    unsigned char i,j,z,count,addyl,addyh;

    count=len/32;

    for(z=0;z<count;z++)
    {
        /*���㵱ǰ������ʾ����ʼ�е�ַ*/
        addyl=y%16;                                     //�����е͵�ַ
        addyh=y/16+0x10;

        /*��ʾ16*16��������*/
        for(i=0;i<2;i++)
        {
            OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //����ҳ��ַ
            OLED_WR_Byte(addyl,OLED_CMD);              //�����е͵�ַ
            OLED_WR_Byte(addyh,OLED_CMD);              //�����иߵ�ַ
            for(j=0;j<16;j++)
            {
                OLED_WR_Byte(*(dispdata++),OLED_DATA);
            }
        }
        y+=16;
    }
}
/*****************************************************
** �������ƣ�OledDisp128X64
** �������ܣ���ʾ128*64����ͼ��
** ��    ����display����Ҫ��ʾ��ͼ��洢λ��
** �������أ���
** ��д���ߣ�326���ӹ�����
** ��дʱ�䣺2015��5��19��
** ��    ַ��  ttsemb.taobao.com

******************************************************/
void OledDisp128X64(unsigned char *display)
{
   unsigned char i,j;

   for(i=0;i<8;i++)
   {
        OLED_WR_Byte(0xb0+i,OLED_CMD);             //����ҳ��ַ
        OLED_WR_Byte(0x00,OLED_CMD);               //�����е͵�ַ
        OLED_WR_Byte(0x10,OLED_CMD);               //�����иߵ�ַ

        for(j=0;j<128;j++)
        {
            OLED_WR_Byte(*(display++),OLED_DATA);
        }
   }
}

/*****************************************************
** �������ƣ�OledDispPicture
** �������ܣ�ָ��λ����ʾ�����СͼƬ��128*64�ڣ�
** ��    ����1.x: ͼƬ��ʾ����ʼҳ��ַ
             2.y��ͼƬ��ʾ����ʼ�е�ַ
             3.width��ͼƬ���
             4.fontlen��ͼƬ�������ֽ���
             5.*picture��ָ��ͼƬ��������
** �������أ���
******************************************************/
void OledDispPicture(unsigned char x,unsigned char y,unsigned char width,Uint16 fontlen,unsigned char *picture)
{
     unsigned char i,j,pagenum,addyl,addyh;

     /*������ʾͼ����Ҫ�Ĳ���*/
     pagenum=fontlen/width;                         //����ͼ��ʹ�õ�ҳ������
     addyl=y%16;                                    //������ʼ�е͵�ַ
     addyh=y/16+0x10;                                   //������ʼ�и����

     /*��ʾͼ��*/
     for(i=0;i<pagenum;i++)
     {
         OLED_WR_Byte(0xb0+x+i,OLED_CMD);           //����ҳ��ַ
         OLED_WR_Byte(addyl,OLED_CMD);              //�����е͵�ַ
         OLED_WR_Byte(addyh,OLED_CMD);              //�����иߵ�ַ
        for(j=0;j<width;j++)
        {
            OLED_WR_Byte(*(picture++),OLED_DATA);
        }
     }
}

/**
 *  @brief                ָ����ʾ
 *  @parameter            m:������ n:ָ��
 *  @return_value           ���
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
** �������ƣ�OledVerticalDisplay
** �������ܣ�ʵ��OLED��ֱ������Χ����
** ��    ����1.toprow�����ù�����ʼ��
**           2.scrollrow�����ù�������
**           ע�⣺toprow+scrollrow<64
** �������أ���
** ��д���ߣ�326���ӹ�����
** ��дʱ�䣺2015��5��19��
** ��    ַ��  ttsemb.taobao.com

******************************************************/
void OledVerticalDisplay(unsigned char toprow,unsigned char scrollrow)
{
    OLED_WR_Byte(0xA3,OLED_CMD);   //0xA3 --���ô�ֱ������Χ����
    OLED_WR_Byte(toprow,OLED_CMD);
    OLED_WR_Byte(scrollrow,OLED_CMD);
}
/*****************************************************
** �������ƣ�OledHorizontalDisplay
** �������ܣ�ʵ��OLEDˮƽ������ʾ
** ��    ����1.direction��ˮƽ������������/����
**           2.startpage����Ҫˮƽ��������ʼPAGE��ȡֵ��Χ0-7
**           3.endpage����Ҫˮƽ�����Ľ���PAGE��ȡֵ��Χ0-7
**           4.interval��ÿ���ƶ���ʱ������ȡֵ��Χ0-7
** �������أ���
******************************************************/
void OledHorizontalDisplay(unsigned char direction,unsigned char startpage,unsigned char endpage,unsigned char interval)
{
    OLED_WR_Byte(direction,OLED_CMD);                      //����ˮƽ�ƶ�����
    OLED_WR_Byte(0x00,OLED_CMD);                  //0x00 --����ˮƽ�ƶ�ʱ����Ҫ�������ֽ�
    OLED_WR_Byte(startpage,OLED_CMD);                      //����ˮƽ�ƶ�����ʼPAGE
    OLED_WR_Byte(interval,OLED_CMD);                       //����ÿ���ƶ���ʱ����
    OLED_WR_Byte(endpage,OLED_CMD);                        //����ˮƽ�ƶ��Ľ���PAGE
    OLED_WR_Byte(0x00,OLED_CMD);                //������������������ֽ�
    OLED_WR_Byte(0xFF,OLED_CMD);   //0xFF--������������������ֽ�
    OLED_WR_Byte(0x2F,OLED_CMD);                //0x2F--����OLED������ʾ
}

/*****************************************************
** �������ƣ�OledVHScrollDisplay
** �������ܣ�ʵ��OLEDˮƽ����ֱ������ʾ
** ��    ����1.Hdirection��ˮƽ������������/����
**           2.startpage����Ҫˮƽ��������ʼPAGE��ȡֵ��Χ0-7
**           3.endpage����Ҫˮƽ�����Ľ���PAGE��ȡֵ��Χ0-7
**           4.interval��ÿ���ƶ���ʱ������ȡֵ��Χ0-7
**           5.Voffset: �������¹�����ƫ������ȡֵ��Χ0x01-0x3F
** �������أ���
******************************************************/
void OledVHScrollDisplay(unsigned char Hdirection,unsigned char startpage,unsigned char endpage,unsigned char interval,unsigned char Voffset)
{
    OLED_WR_Byte(Hdirection,OLED_CMD);         //����ˮƽ�ƶ�����
    OLED_WR_Byte(0x00,OLED_CMD);              //0x00-- ������������������ֽ�
    OLED_WR_Byte(startpage,OLED_CMD);          //����ˮƽ�ƶ�����ʼPAGE
    OLED_WR_Byte(interval,OLED_CMD);             //����ÿ���ƶ���ʱ����
    OLED_WR_Byte(endpage,OLED_CMD);              //����ˮƽ�ƶ��Ľ���PAGE
    OLED_WR_Byte(Voffset,OLED_CMD);              //���������ƶ�����
    OLED_WR_Byte(0x2F,OLED_CMD);      //0x2F ---����������ʾ����
}

/*****************************************************
** �������ƣ�OledScrollStop
** �������ܣ��رչ�������
** ��    ������
** �������أ���
** ��д���ߣ�326���ӹ�����
** ��дʱ�䣺2015��5��19��
** ��    ַ��  ttsemb.taobao.com

******************************************************/
void OledScrollStop(void)
{
    OLED_WR_Byte(0x2E,OLED_CMD);    //0x2E --�رչ�����ʾ����
}
/*****************************************************
** �������ƣ�OledScrollTest
** �������ܣ�ʵ��OLED������ʾ
** ��    ������
** �������أ���
** ��д���ߣ�326���ӹ�����
** ��дʱ�䣺2015��5��19��
** ��    ַ��  ttsemb.taobao.com
** ����֧��Ⱥ��QQ427165833
******************************************************/
void OledScrollTest(void)
{
    OLED_Clear();

    DisplayFont_16X16(0,16,192,TestUI_Page4);
    /*�������*/
    DisplayFont_16X16(4,32,128,Scroll_Horizontal);
    OledHorizontalDisplay(0x27,4,5,0);  //0x27 ---����ˮƽ�ƶ�
    delay_1ms(4000);
    OledHorizontalDisplay(0x26,4,5,0); //0x26---����ˮƽ�ƶ�
    delay_1ms(4000);
    /*�������*/
    OledScrollStop();
    DisplayFont_16X16(4,32,32,Scroll_Vertical);
    OledVerticalDisplay(16,48);
    OledVHScrollDisplay(0x2A,7,7,0,1);   //0x2A---�ƶ�
    delay_1ms(4000);
    OledVerticalDisplay(0,64);
    OledVHScrollDisplay(0x2A,7,7,0,63);//0x2A---�ƶ�
    /* б�����*/
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
** �������ƣ�Pow
** �������ܣ���һ����������N�η�
** ��    ����num����Ҫ��η�����������count����Ҫ��Ĵη���
** �������أ�num��count�η�
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
** �������ƣ�NumToString
** �������ܣ���һ�����ĸ�λת���ɶ�Ӧ��ASCII��
** ��    ����num����Ҫת������
**           numbit:��Ҫת��������λ��
**           *string��ת������ַ����洢λ��
** �������أ���
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
** �������ƣ�OledClearPageCom
** �������ܣ����ָ��������ʾ
** ��    ����1��pagestart��ָ��ҳ��ʼ��ַ
             2��pageend��ָ��ҳ������ַ
             3��comstart��ָ������ʼ��ַ
             4��comend��ָ���н�����ַ
** �������أ���
** ��д���ߣ�326���ӹ�����
** ��дʱ�䣺2015��5��19��
** ��    ַ��  ttsemb.taobao.com

******************************************************/
void OledClearPageCom(unsigned char pagestart,unsigned char pageend,unsigned char comstart,unsigned char comend)
{
  unsigned char i,j;
  unsigned char addyl,addyh;

  addyl=comstart%16;                                      //�����е͵�ַ
  addyh=comstart/16+0x10;                             //�����иߵ�ַ

  for(i=pagestart;i<pageend+1;i++)
  {
      OLED_WR_Byte(0xb0+i,OLED_CMD);             //����ҳ��ַ
      OLED_WR_Byte(addyl,OLED_CMD);              //�����е͵�ַ
      OLED_WR_Byte(addyh,OLED_CMD);              //�����иߵ�ַ

      for(j=comstart;j<comend+1;j++)
      {
          OLED_WR_Byte(0x00,OLED_DATA);
      }
  }
}









