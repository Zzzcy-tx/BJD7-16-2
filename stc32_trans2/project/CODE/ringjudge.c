#include "ringjudge.h"
#include "math.h"

uint8 Ring_forecast = 0;
int8 annulus_flag = 0;
uint8 last_annulus_flag = 0;
uint8 annulus_flag_k_index = 0;

uint8 Ring_in = 0;
uint8 Ring_running = 0;
uint8 Ring_out = 0;
uint8 Left_Annulus = 0;
uint8 Ring_finish = 0;
uint8 first_flag = 0;
uint8 straight_flag = 0;
uint16 gyro_x_count = 0;

int8 many_ring = 0;

uint8 angle_integral_flag1 = 0;
uint8 angle_integral_flag2 = 0;
uint8 distance_integral_flag1 = 0; //·�̻��ֱ�־λ
uint8 distance_integral_flag2 = 0; //·�̻��ֱ�־λ

uint8 if_first_dis1 = 0;
int16 k_add = 0; // kֵ�ۼ�

float angle1 = 0;
float angle2 = 0;
float obstacle_angle = 0;
int16 distance1 = 0;
int16 distance2 = 0;

uint16 tof_count = 0;	  //��¼ʮ���вɼ���TOF����С��320�Ĵ���
uint8 tof_count_flag = 0; //��⵽�ϰ���Ĵ���
uint8 dodge_flag = 0;	  //���ϱ�־λ

void Ring_control(void)
{
	last_annulus_flag = annulus_flag;

	//����������
	if (distance_integral_flag1)
	{
		distance1 += (abs(speedL) + abs(speedR)) / 2;
	}

	if (angle_integral_flag1)
	{
		angle1 += gyro / 2922.22;
	}

	if (annulus_flag > 0)
	{
		P77 = 1;
		setSpeedLR(sp_set / 1.8);
	}
	else
	{
		P77 = 0;
	}

	Ring_forecast = 1;

	//----------Բ����----------//
	if (adc_data[7] > 575 && annulus_flag == 0 && adc_data[1] < 350 && adc_data[3] < 350)
	{
		Ring_in = 1;	  //Բ�������־λ
		annulus_flag = 1; //�򿪱�־λ1
		distance1 = 0;	  //�������
		distance_integral_flag1 = 1;
		k_add = 0;
		angle_integral_flag1 = 1; //���������ǻ���
	}

	if (annulus_flag == 1 && annulus_flag != 0)
	{
		if (distance1 < dis1 && annulus_flag != 0)
		{
			if (adc_data[1] < adc_data[3])
			{
				k_add += 1;
			}
			else
			{
				k_add -= 1;
			}
		}

		if (distance1 > dis1 && distance1 < dis2 && annulus_flag != 0) //���ƹ̶���ǵ�ʱ��50---100   �ǳ���Ҫ������������������������
		{
			if (first_flag == 0) //�жϻ���
			{
				if (adc_data[1] < adc_data[3])
				{
					Left_Annulus = 1;
				}
				first_flag = 1;
			}

			//�������ʶ��Բ�������ǽ����˵Ļ��޸����������Ƿ��Ȼ����޸������ʱ��
			if (Left_Annulus == 1) //����
			{
				// adc_data[0] = adc_data[3]+80;
				// k=-40;
				// if (adc_data[2] > 500)
				// {
				k = (-exp(-8*((float)(distance1-dis1)/(dis2-dis1)))+1)*-ring_set_k[annulus_flag_k_index];
				// }

				// pidR.setpoint *= 1.8;
				//	pwm_duty(PWB_CH1_P74,angle_max);  //ǿ�������ǣ��ҵ�����٣�
			}
			else if (!Left_Annulus) //�һ���
			{
				// if (adc_data[2] > 500)
				// {
				k =  (-exp(-8*(((float)distance1-dis1)/(dis2-dis1)))+1)*ring_set_k[annulus_flag_k_index];
				// }
				// pidS.result = 2000;
				// pidL.setpoint *= 1.8;
				//  adc_data[3] = adc_data[0]+80;
				//	pwm_duty(PWMB_CH1_P74,640); //ǿ�����Ҵ�ǣ��������٣�
			}
		}
		else if (distance1 >= dis2 && annulus_flag != 0) //�ս�ȥ������ѭ��
		{
			if_first_dis1 = 0;
			annulus_flag = 2;
			distance1 = 0;
			distance_integral_flag1 = 1;
			k_add = 0;
		}
	}
	// else if (annulus_flag == 2)
	// {
	// 	if (distance1 > 5000)
	// 	{
	// 		annulus_flag = many_ring;
	// 		distance1 = 0;
	// 		distance_integral_flag1 = 0; //ֹͣ·�̻���
	// 	}
	// }
	else if (annulus_flag == 2) //�ڻ�����
	{
		if (abs(angle1) > 9500)
		{
			distance_integral_flag1 = 1; //����·�̻���
			annulus_flag = -1;
			// annulus_flag = 0; //����
			annulus_flag_k_index++;
		}
	}
	// else if (annulus_flag == 3) //��ǳ���
	// {
	// 	if (distance1 < 2600 /*&&Annulus_Distance>500*/) //���ƹ̶���ǵ�ʱ��50---100   �ǳ���Ҫ������������������������
	// 	{
	// 		if (Left_Annulus == 1) //�󻷳�
	// 		{
	// 			pidS.result = 1200;
	// 		}
	// 		else if (!Left_Annulus) //�һ���
	// 		{
	// 			pidS.result = -1200;
	// 		}
	// 	}
	// 	else if (/*distance > 2600*/ 1) //��������
	// 	{
	// 		annulus_flag = 4; //ת�����
	// 		distance1 = 0;
	// 		distance_integral_flag1 = 1; //����·�̻��ַ�ֱֹ�ӽ��뻷��
	// 		k_add = 0;
	// 	}
	// }
	// else if (annulus_flag == 4)
	// {
	// 	if (distance1 > 10000)
	// 	{
	// 		all_reset();
	// 	}
	// }

	//�µ�������Ϊ����
	if (abs(imu963ra_gyro_x) > 1000)
	{
		gyro_x_count++;
	}
	else
	{
		gyro_x_count = 0;
	}
	if (gyro_x_count >= 3)
	{
		all_reset();
	}
}

void obstacle_control(void)
{
	if(dodge_flag == 1)
	{
		P77 = 1;
	}else
	{
		P77 = 0;
	}
	if (distance_integral_flag2)
	{
		distance2 += (spL + spR) * 0.005 / 2;
	}
	if (1)
	{
		obstacle_angle += gyro / 2922.22;
		angle2 = MovingAverageFilter_0(obstacle_angle);
	}
	/***********************����******************************/
	if (dodge_flag == 0 && all_distance > 20000	/*&& leave_flag >= 3*/)
	{
		//���ʮ��  tof
		if (tof_distance <= tof_dis)
		{
			tof_count++;
		}
		tof_count_flag++; //���������++
		if (tof_count_flag == 10)
		{
			if (tof_count >= 8)
			{
				distance2 = 0;
				distance_integral_flag2 = 1; //��ʼ���־���

				angle1 = 0;
				angle_integral_flag1 = 1;//��ʼ���ֽǶ�

				dodge_flag++;
			}
			tof_count_flag = 0;
		}
	}
	else if (dodge_flag == 1) //Ҫת��
	{
		//����
		pidS.setpoint = exp(-((0.002*distance2*distance2)/(2.0*100*100)))*250;
		if (angle1 > 25 && distance2 > 40 )
		{
				angle1 = 0;
				distance2 = 0;
				dodge_flag++;
		}
	}
	else if (dodge_flag == 2)
	{
		//�������
		pidS.result = exp(-((0.002*distance2*distance2)/(2.0*100*100)))*-150;
		if ( angle1 < -25 && distance2 > 40 )
		{
			angle1 = 0;
			distance2 = 0;
			dodge_flag++;
		
		}
	}
	else if (dodge_flag == 3)
	{
		//�Ҵ��
		pidS.result = exp(-((0.003*distance2*distance2)/(2.0*100*100)))*-125;
		if (angle1 < -25 && distance2 > 40)
		{
			angle1 = 0;
			distance2 = 0;
			dodge_flag++;
		}
	}
	else if (dodge_flag == 4) 
	{
		//���ǻ���
		pidS.result = exp(-((0.003*distance2*distance2)/(2.0*100*100)))*125;
		if (angle1 > 25 && distance2 > 40)
		{
			angle1 = 0;
			distance2 = 0;

			distance_integral_flag2 = 0;//ֹͣ·�̻���
			angle_integral_flag1 = 0;//ֹͣ�ǶȻ���

			dodge_flag++;
		}
	}
	else if (dodge_flag == 5)
	{
		distance_integral_flag2 = 1;
		if(distance2 > 10000)
		{
			dodge_flag=6;
			//obstacle_reset();	//����������ȡ��ע��
		}
	}
}

void all_reset()
{
	angle = 0;
	distance1 = 0;
	Ring_forecast = 0;
	annulus_flag = 0;
	Ring_in = 0;
	Ring_running = 0;
	Ring_out = 0;
	Left_Annulus = 0;
	Ring_finish = 0;
	first_flag = 0;
	k_add = 0;
	// tof_count = 0;
	// dodge_flag = 0;
	// angle2 = 0;
	// distance2 = 0;
	 angle_integral_flag1 = 0;
	// angle_integral_flag2 = 0;
	 distance_integral_flag1 = 0; //·�̻��ֱ�־λ
	// distance_integral_flag2 = 0; //·�̻��ֱ�־λ
}

void obstacle_reset()
{	
	tof_count = 0;
	dodge_flag = 0;	//���±���

	angle2 = 0;
	angle_integral_flag2 = 0;

	angle1 = 0;
	angle_integral_flag1 = 0;

	distance2 = 0;
	distance_integral_flag2 = 0;

}