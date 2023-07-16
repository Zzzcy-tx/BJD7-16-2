/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"
#include "data.h"
#include "other.h"
#include "speed.h"
#include "pid.h"
#include "element.h"
#include "kaerman.h"
#include "keyboard.h"
#include "filter.h"
/*
 *�����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
 *��board_init��,�Ѿ���P54��������Ϊ��λ
 *�����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */
int i_count = 20;
#define HALL_PIN P26
extern float k1;
uint32 last_time_count = 0;
void main()
{

  // sys_clk��ѡֵ:33177600, 30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600��
  //����ϵͳƵ�ʣ���Ƶ����Ҫ��STC-ISP����е� <�����û���������ʱ��IRCƵ��>ѡ���Ƶ��һ�¡�
  //���Ƶ�����ò��ԣ����ᵼ�´��ڵ����ݲ�����,PWM�Ĺ����������ȵȡ�
  sys_clk = 30000000; //����ϵͳƵ��Ϊ30MHz

  board_init();
  allInit();
  // ips200_init();
  // ips200_clear(WHITE);
  // delay_ms(1000);
  pit_timer_ms(TIM_4, 5); //�����ж�
  exit_init(INT0_P32, FALLING_EDGE);
   setPidLR(20, 8, 0, 16000);
   setSpeedLR(110);
   setPidS(8, 0, 1);
   pidS.setpoint = 0;
   pidW.setpoint = 0;

  while (i_count)
  {
    delay_ms(10);
    i_count--;
    gyro_add += (float)KalmanFilter((double)imu963ra_gyro_z);
  }
  gyro_add /= 20;
  sp_set = 0;
  P77 = 0;
  while (1)
  {
    if (  /*P50 && */fanL < 10)
    {
      Keystroke_Scan();
      Keystroke_Menu();
    }
    if (finish_flag)
    {
      rx_analysis();
      finish_flag = 0;
    } //���ߵ��ν�������

    // if (last_time_count != time_count && sp_set != 0)
    // {
    //   printf("%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", time_count, adc_data[0], adc_data[1], adc_data[2], adc_data[3], adc_data[4], adc_data[5], adc_data[6], adc_data[7], adc_data[8], adc_data[9]);
    //   printf(",%d,%f,%d,%d,%f,%f,%f,%f,%f,%f", sp_set, k, speedL, speedR, gyro, pidL.setpoint, pidL.proportiongain, pidL.integralgain, pidR.proportiongain, pidR.integralgain);
    //   printf(",%f,%f,%f,%f,%f", pidL.result, pidR.result, pidS.proportiongain, pidS.derivativegain, pidS.result);
    //   printf(",%f,%f,%f,%f,%f,%f,%f\n", G_S * gyro, pidL.preerror, pidL.integral, pidR.preerror, pidR.integral, pidS.preerror, pidS.derivativegain);
    //   last_time_count = time_count;
    // }
    // printf("%d,%d,%f,%f,%f,%d,%f\n", speedL, speedR, pidL.result, pidR.result, k, speed3, gyro);
    setPidLR(P_LR, I_LR, 0, 15000); //�����ٶȻ�����
                                    // setPidS(P_S, 0, D_S);           //���÷��򻷲���
    // setPidS(P_S, 0, D_S);        //���÷��򻷲���
    setPidW(P_W, 0, D_W); //���ýǶȻ�����

    if (sp_set <= 2)
    {
      fanL = 0;
    }
    pwm_duty(PWMB_CH1_P74, fanL);

    sp_set_max = sp_set;
    //  printf("%d,%d,%d,%f,%f,%f,%d,%d,%d,%f,%f,%d\n", adc_data[0], adc_data[4], aimed_into_speed, into_R, angle, all_angle, adc_data[2], tof_distance, leave_flag, k, pidS.result, dodge_flag);
    //   printf("%d,%d,%d\n", imu963ra_mag_x, imu963ra_mag_y, imu963ra_mag_z);
    //   printf("%d,%d,%d,%d,%d,%d",imu963ra_acc_x,imu963ra_acc_y,imu963ra_acc_z,imu963ra_gyro_x,imu963ra_gyro_y,imu963ra_gyro_z);
    //  printf("%d,%d,%d,%d,%f,%d,%d,%f,%f,%f,%d,%d,%f,%f\n", adc_data[2], adc_data[0], adc_data[4], dodge_flag, angle1, distance1, sp_set, P_S, D_S, G_S, annulus_flag, k_add, gyro, pidS.result);
    //     printf("%f,%f,%f,%f,%f,%f,%f\n", angle, pidW.result / 90, pidL.setpoint, pidR.setpoint, pidL.result, pidR.result, k, aimed_angle);
    //   printf("%d\n", dl1a_distance_mm);
    //   printf("%f,%f\n",k,pidS.result);
    //  printf("%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,", adc_data[9] - adc_data[2], adc_data[9], adc_data[2], adc_data[0], adc_data[4], pidL.setpoint, pidR.setpoint, pidL.result, pidR.result, pidS.result, k);
    //  printf("%f,%d,%d,%f,%d\n", k1, speedL, speedR, ((float)speedL + (float)speedR) / 2 * 200 / 10100, into_flag);
    //  printf("%d,%d,%d,%d,%d,%d,%f,%f,%f,%f\n",adc_data[0],adc_data[1],adc_data[2],adc_data[3],adc_data[4],sp_set,P_LR,I_LR,P_S,D_S);
    //  printf("%d,%f,%f,%f,%d,%d,%f,%f,%d,%f,%d,%d,%d\n", adc_data[5], pidS.result, pidL.result, pidR.result, speedL, speedR, gyro, all_angle, distance1, k, dl1a_distance_mm, dodge_flag, annulus_flag,(adc_data[]));
    //    printf("%d,%d,%d,%f,%f,%f,%f,%f,%d,%d,%f,%f,%f\n",adc_data[0],adc_data[2],adc_data[3],pidL.proportiongain,pidL.integralgain,pidS.proportiongain,pidS.derivativegain,k,speedL,speedR,pidL.result,pidR.result);
    //    printf("%f\n",pidS.result);
    // printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", adc_sum[0], adc_sum[1], adc_sum[2], adc_sum[3], adc_sum[4], adc_sum[5], adc_sum[6], adc_sum[7], adc_sum[8], adc_sum[9], Gui_flag, t);
    printf("%f,%f,%f,%f,%f,%d\n",pidL.setpoint,pidR.setpoint,pidL.result,pidS.result,angle2,dodge_flag);
    // printf("%d,%d,%d,%d,%d,%d,%d\n",adc_data[0],adc_data[1],adc_d[2],adc_d[3],adc_d[4],Gui_flag,t);
    // printf("%d,%d,%f,%f,%f,%f,%f,%f,%f,%f\n",adc_data[0],Max_Ad[0],Adc_gui[0],Adc_gui[1],Adc_gui[2],Adc_gui[3],Adc_gui[4],Adc_gui[5],Adc_gui[6],Adc_gui[7]);
  }
}
