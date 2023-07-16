///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,��ɿƼ�
// * All rights reserved.
// * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
// *
// * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
// * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
// *
// * @file       		isr
// * @company	   		�ɶ���ɿƼ����޹�˾
// * @author     		��ɿƼ�(QQ790875685)
// * @version    		�鿴doc��version�ļ� �汾˵��
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC16F40K128
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"
#include "data.h"
#include "speed.h"
#include "math.h"
#include "kaerman.h"
#include "filter.h"
#include "keyboard.h"
#include "isr.h"
char *p = uart_str;
int16 spL;
int16 spR;
float k1;
// UART1�ж�
void UART1_Isr() interrupt 4
{
    uint8 res;
    static uint8 dwon_count;
    if (UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if (UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        *p = (char)SBUF;
        if (*p == '#')
        {
            *++p = '\0';
            p = uart_str;
            finish_flag = 1;
        }
        else
        {
            p++;
        }

        // uart_putchar(DEBUG_UART,SBUF);
        //�����Զ�����
        if (res == 0x7F)
        {
            if (dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

// UART2�ж�
void UART2_Isr() interrupt 8
{
    if (UART2_GET_TX_FLAG)
    {
        UART2_CLEAR_TX_FLAG;
        busy[2] = 0;
    }
    if (UART2_GET_RX_FLAG)
    {
        UART2_CLEAR_RX_FLAG;
        //�������ݼĴ���Ϊ��S2BUF
    }
}

// UART3�ж�
void UART3_Isr() interrupt 17
{
    if (UART3_GET_TX_FLAG)
    {
        UART3_CLEAR_TX_FLAG;
        busy[3] = 0;
    }
    if (UART3_GET_RX_FLAG)
    {
        UART3_CLEAR_RX_FLAG;
        //�������ݼĴ���Ϊ��S3BUF
    }
}

// UART4�ж�
void UART4_Isr() interrupt 18
{
    if (UART4_GET_TX_FLAG)
    {
        UART4_CLEAR_TX_FLAG;
        busy[4] = 0;
    }
    if (UART4_GET_RX_FLAG)
    {
        UART4_CLEAR_RX_FLAG;
        //�������ݼĴ���Ϊ��S4BUF;

        //�������ݼĴ���Ϊ��S4BUF;
        if (wireless_type == WIRELESS_SI24R1)
        {
            wireless_uart_callback(); //����ת���ڻص�����
        }
        else if (wireless_type == WIRELESS_CH9141)
        {
            bluetooth_ch9141_uart_callback(); //����ת���ڻص�����
        }
        else if (wireless_type == WIRELESS_CH573)
        {
            wireless_ch573_callback(); // CH573����ģ��ص�����
        }
    }
}

void INT0_Isr() interrupt 0
{
    encoder_count++;
}
void INT1_Isr() interrupt 2
{
}
void INT2_Isr() interrupt 10
{
    INT2_CLEAR_FLAG; //����жϱ�־
}
void INT3_Isr() interrupt 11
{
    INT3_CLEAR_FLAG; //����жϱ�־
}

void INT4_Isr() interrupt 16
{
    INT4_CLEAR_FLAG; //����жϱ�־
}

void TM0_Isr() interrupt 1
{
}
void TM1_Isr() interrupt 3
{
}
void TM2_Isr() interrupt 12
{
    TIM2_CLEAR_FLAG; //����жϱ�־
}
void TM3_Isr() interrupt 19
{
    TIM3_CLEAR_FLAG; //����жϱ�־
}

#define DIRL P35
#define DIRR P53

#define MAX_MIDDLE 769.0
#define MIN_MIDDLE 213.0
void TM4_Isr() interrupt 20
{
    TIM4_CLEAR_FLAG;     //����жϱ�־
    dl1a_get_distance(); //��ȡTOF����

    tof_distance = dl1a_distance_mm;
    time_count++;
    //��ȡ��еĶ���
    adc_select();
    /*adc_data[0] = MovingAverageFilter0(adc_once(ADC_P00, ADC_10BIT));
    adc_data[1] = MovingAverageFilter1(adc_once(ADC_P01, ADC_10BIT));
    adc_data[2] = MovingAverageFilter2(adc_once(ADC_P05, ADC_10BIT));
    adc_data[3] = MovingAverageFilter3(adc_once(ADC_P06, ADC_10BIT));
    adc_data[4] = MovingAverageFilter4(adc_once(ADC_P02, ADC_10BIT));
    adc_data[5] = MovingAverageFilter5(adc_once(ADC_P10, ADC_10BIT));
    adc_data[6] = MovingAverageFilter6(adc_once(ADC_P11, ADC_10BIT));
    adc_data[7] = MovingAverageFilter7(adc_once(ADC_P15, ADC_10BIT));
    adc_data[8] = MovingAverageFilter8(adc_once(ADC_P16, ADC_10BIT));
    adc_data[9] = MovingAverageFilter9(adc_once(ADC_P14, ADC_10BIT));*/
    /*adc_data[0] = adc_once(ADC_P00, ADC_10BIT);
    adc_data[1] = adc_once(ADC_P01, ADC_10BIT);
    adc_data[2] = adc_once(ADC_P05, ADC_10BIT);
    adc_data[3] = adc_once(ADC_P06, ADC_10BIT);
    adc_data[4] = adc_once(ADC_P02, ADC_10BIT);
    adc_data[5] = adc_once(ADC_P10, ADC_10BIT);
    adc_data[6] = adc_once(ADC_P11, ADC_10BIT);
    adc_data[7] = adc_once(ADC_P15, ADC_10BIT);
    adc_data[8] = adc_once(ADC_P16, ADC_10BIT);
    adc_data[9] = adc_once(ADC_P14, ADC_10BIT);*/
    Angle_Cal();                           //��̬����
    all_distance += (speedL + speedR) / 2; //ȫ����ʻ����
    // imu963ra_get_mag();                         //��ȡ����������
    gyro1 = (float)KalmanFilter((double)imu963ra_gyro_z); //������ֵ���˲���
    // gyro1 = (float)imu963ra_gyro_z;
    gyro = gyro1 - gyro_add; //����Ʈ
    if (gyro > 5000)
    {
        gyro = 5000;
    }
    if (gyro < -5000)
    {
        gyro = -5000;
    }
    all_angle += gyro / 2922.22; //ת��Ϊʵ��������
    // if (all_angle < -180)
    //     all_angle += 360;
    // if (all_angle > 180)
    //     all_angle -= 360;
    // all_angle =all_angle % 180;
    //��ͨ��Ⱥͣ����ԶȽϲ�
    //   k=50*(adc_data[3]-adc_data[0])/(adc_data[3]+adc_data[0]);

    //�������K��ʹ�ô����ŵĲ�Ⱥ��㷨��������
    if (adc_data[4] > 0 && adc_data[0] > 0)
        k = -50 * (sqrt(adc_data[4]) - sqrt(adc_data[0])) / (sqrt(adc_data[4]) + sqrt(adc_data[0]));
    if (adc_data[5] > 0 && adc_data[9] > 0)
        k1 = -50 * (sqrt(adc_data[9]) - sqrt(adc_data[5])) / (sqrt(adc_data[9]) + sqrt(adc_data[5]));

    if (adc_data[4]<120 && adc_data[0]<120 && annulus_flag == 0)
    {
        k = k1;
    }
    // k = 50 * (sqrt(adc_data[3]) - sqrt(adc_data[0])) / ( sqrt(adc_data[3]) + sqrt(adc_data[2]) + sqrt(adc_data[0]) );
    // else
    //    k = 0;
    // if (sqrt(MAX_MIDDLE) - sqrt((float)adc_data[2]) > 0)
    // {
    //     // k=50*(sqrt(MAX_MIDDLE)-sqrt((float)adc_data[2]))/sqrt(MAX_MIDDLE);
    //     k = 50 * fabs((sqrt(MAX_MIDDLE) - sqrt((float)adc_data[2])) / (sqrt(MAX_MIDDLE)));
    //     if (k > 50)
    //         k = 50;
    // }
    // else
    // {
    //     k = 0;
    // }

    // if (adc_data[4] > adc_data[0])
    //     k *= -1;

    // if (adc_data[2] > MAX_MIDDLE + 10)
    // {
    //     k = 50 * (sqrt(adc_data[4]) - sqrt(adc_data[0])) / (sqrt(adc_data[4]) + sqrt(adc_data[0]));
    //     if (k > 50)
    //         k = 50;
    // }

    

    renew(); //���ƺ���
    //��ȡʵ���ٶ�
    spL = ctimer_count_read(CTIM0_P34);
    spR = ctimer_count_read(CTIM3_P04);

    if (DIRL == 1)
    {
        if (spL < 600)
            speedL = spL;
    }
    else
    {
        if (spL < 600)
            speedL = spL * -1;
    }
    ctimer_count_clean(CTIM0_P34);
    if (DIRR != 1)
    {
        if (spR < 600)
            speedR = spR;
    }
    else
    {
        if (spR < 600)
            speedR = spR * -1;
    }
    ctimer_count_clean(CTIM3_P04);

    if (P67 != 1)
    {
        speed3 = -(int16)encoder_count;
    }
    else
    {
        speed3 = encoder_count;
    }
    encoder_count = 0;
}

// void  INT0_Isr()  interrupt 0;
// void  TM0_Isr()   interrupt 1;
// void  INT1_Isr()  interrupt 2;
// void  TM1_Isr()   interrupt 3;
// void  UART1_Isr() interrupt 4;
// void  ADC_Isr()   interrupt 5;
// void  LVD_Isr()   interrupt 6;
// void  PCA_Isr()   interrupt 7;
// void  UART2_Isr() interrupt 8;
// void  SPI_Isr()   interrupt 9;
// void  INT2_Isr()  interrupt 10;
// void  INT3_Isr()  interrupt 11;
// void  TM2_Isr()   interrupt 12;
// void  INT4_Isr()  interrupt 16;
// void  UART3_Isr() interrupt 17;
// void  UART4_Isr() interrupt 18;
// void  TM3_Isr()   interrupt 19;
// void  TM4_Isr()   interrupt 20;
// void  CMP_Isr()   interrupt 21;
// void  I2C_Isr()   interrupt 24;
// void  USB_Isr()   interrupt 25;
// void  PWM1_Isr()  interrupt 26;
// void  PWM2_Isr()  interrupt 27;