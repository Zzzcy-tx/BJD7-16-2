#include "data.h"

uint16 t = 0;
int16 PO_or_OBSTACLE[5] = {-1, -1, -1, -1, -1};
int16 ring_set_k[5] = {18, 0, 0, 0, 0};
uint16 sp_ready = 1;

int16 mag;
int16 sp_check = 0;
uint16 check_flag = 0;

int16 speedL;
int16 speedR;
int8 irs_count = 0;
float k;
int32 adc_data[10];
uint16 adc_d[10];  //ʵʱ
float Adc_gui[10]; //��һ��
uint16 AD_value[10];
int16 test1;

int32 all_distance = 0;

float test2;

float all_angle = 0;
float gyro_add = 0;

char str_item[100];       //������
int str_num;              //����ֵ
char uart_str[100] = {0}; //�����ַ���
int8 finish_flag = 0;
uint16 Gui_flag = 0;
int8 start_flag = 0;

uint8 Rcon_flag = 0;
uint8 Ocon_flag = 1;
uint8 protect_flag = 1;

float P_LR = 63.18;
float I_LR = 50.1;
uint8 puodao = 0;
float P_S = 68.27;

float P_S_MAX = 77.0;
float D_S = -24.19;
float D_S_z = -33;
float G_S = -0.198;
float G_S_1 = -0.17;
float G_S_zhi = -0.151;

float P_W = 1.83;
float D_W = 0.86;

float aimed_angle = 0;

float gyro;  //�˲���������z����ٶ�
float gyro1; //�˲���������z����ٶ�

int16 dis1 = 2000;
int16 dis2 = 10000;
int16 ring_angle = 1600;

int16 wSet;
float gyro;
int16 sp_set = 0;
int16 sp_set_max = 200;

int16 aimed_into_speed = 60; //�����ٶ�
float into_R = 0.25;
//����ת��뾶(m)
int16 ku_distance = 1500;

int8 obstacle_count = 0;
int16 tof_dis = 850;
// int16 tof_dis1 = 7950;
int16 tof_dis1 = 7300;
int16 tof_dis2 = 6650;
int16 tof_dis3 = 6000;

int16 fanL = 0;
int16 fanR = 0;

uint8 start_diration = 1;

int16 tof_distance = 0;

//����
int8 keys = 0;
uint8 SINGLE_CLICK = 0;
uint8 DOUBLE_CLICK = 1;
int16 strm = -1;
//���뿪��
uint8 R_or_L = 0;

uint32 time_count = 0;
uint16 mode = 0;

uint32 encoder_count = 0;
int16 speed3 = 0;


