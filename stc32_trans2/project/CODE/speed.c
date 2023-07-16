#include "speed.h"

//���õ��pwm
//���õ��pwm
void setMotorL(int16 motorSetL)
{
  if (sp_set < 10)
  {
    motorSetL = 0;
  }
  pwm_duty(PWMA_CH1P_P60, abs(motorSetL) * 4 / 7);
  if (motorSetL > 0)
    P64 = 1;
  else
    P64 = 0;
}
void setMotorR(int16 motorSetR)
{
  if (sp_set < 10)
  {
    motorSetR = 0;
  }
  pwm_duty(PWMA_CH2P_P62, abs(motorSetR) * 4 / 7);
  if (motorSetR < 0)
    P66 = 0;
  else
    P66 = 1;
}

//���ټ���
float Diff_Factor = 0.006;            //����ϵ��
int16 Differential(float Diff_Factor) //������ԭ�����ݶ�����в��ٿ��ƣ�����������������ʵ��
{
  float Diff;
  Diff = Diff_Factor * ((pidL.setpoint + pidR.setpoint) / 2) * pidS.result; // diff_factor�ǲ���ϵ��
  return (int16)Diff;
}

//�������ֵ��ٶ�Ŀ��ֵ
void diffSpeedSet()
{
  int16 Diff_Speed = Differential(Diff_Factor); //ͨ�������������ֵ
    //if (Diff_Speed > 1500)	//�����޷�
    //Diff_Speed =1500;
   // pidL.setpoint * Diff_Factor;

  if (k < 0)
  {
    setSpeedR(pidR.setpoint + Diff_Speed);
    setSpeedL(pidL.setpoint - Diff_Speed);
	
  }
  else
  {
    setSpeedR(pidR.setpoint + Diff_Speed);
    setSpeedL(pidL.setpoint - Diff_Speed);
  }

  // setSpeedLR(pidL.setpoint);
  // if (pidS.result < 0)
  //  setSpeedR(pidR.setpoint - abs(Diff_Speed));
  // else
  //  setSpeedL(pidR.setpoint - abs(Diff_Speed));
}

// //����ǶȻ����
// void diffSpeedSet1()
// {
//   setSpeedL(pidL.setpoint + pidW.result / 90);
//   setSpeedR(pidR.setpoint - pidW.result / 90);
// }

//�ж��б�ѭ������




void renew(void)
{
	int16 a = 7;
	int16 b = 5;
 if(adc_data[7] < 40 && (dodge_flag == 0||dodge_flag==5) && protect_flag == 1) 
  {
    sp_set = 0;
  }
  /* if(fabs(k) < 7)
  {
    setSpeedLR(sp_set_max); //�����ٶȳ�ʼֵΪĿ��ֵ
    pidS.proportiongain = P_S;
    pidS.derivativegain = D_S_z;
    
  }
  else
  {
    pidS.proportiongain = P_S_MAX;
    pidS.derivativegain = D_S;
    setSpeedLR(sp_set); //�����ٶȳ�ʼֵΪĿ��ֵ
  }
  */
  if(abs(adc_data[1] - adc_data[3]) < 50)
  {
	 check_flag = 0;
	 sp_check = exp(-(2*k*k)/(2*80*80))*sp_set;
	 //sp_check = exp(-(2*k*k)/(2*80*80))*sp_set;
	 //sp_check = (1-0.05*fabs((exp(-fabs(k/a))-1.0)/(exp(-fabs(k/a))+1.0)))*sp_set;
	 setSpeedLR(sp_check);
	 //setSpeedLR(sp_set);
	 G_S = G_S_zhi;
  }
  else if(abs(adc_data[1] - adc_data[3]) < 20)
  {
	 check_flag = 2;
	 //sp_check = exp(-(k*k)/(2*50*50))*sp_set+20;
	 if(sp_set != 0)
	 {
		 sp_check = exp(-(2*k*k)/(2*80*80))*sp_set + 40;
	 }else
	 {
		 
	 }
		 //sp_check = (1-0.05*fabs((exp(-fabs(k/a))-1.0)/(exp(-fabs(k/a))+1.0)))*sp_set;
	 setSpeedLR(sp_check);
	 //setSpeedLR(sp_set);
	 G_S = G_S_zhi;
  }
  else
  {
	 sp_check = (1-0.1*fabs((exp(-fabs(k/b))-1.0)/(exp(-fabs(k/b))+1.0)))*sp_set;
	 //sp_check = exp(-(k*k)/(2*41.4*41.4))*sp_set;
	 setSpeedLR(sp_check);
	 //setSpeedLR(sp_set * 0.85);
     check_flag = 1;
	 G_S = G_S_1;
  }
  if (sp_set < 3)
  {
    setSpeedLR(0);
  }
  if (puodao == 1)
  {
    setSpeedLR(100);
  }
  if (adc_data[2] > 820)
  {
    setSpeedLR(110);
  }
  if(Rcon_flag == 1)
  {
	Ring_control();
  }
  //����ת��PID����
  setPidS(P_S + (fabs(k) / 50) * (P_S_MAX - P_S), 0, D_S_z + (fabs(k) / 50) * (D_S - D_S_z));
  PIDRegulation1(&pidS, k); //���򻷼���(�������ǻ���)
  pidS.result += G_S * gyro;
  if(Ocon_flag == 1 && obstacle_count == 0)
  {
	//  obstacle_control();//һ��Ҫ��setpids����
  }
  diffSpeedSet(); //�������ã�������ΪpidS.result��pidL.result��
  // control();      //����Ԫ�ؿ���
  //          angle += gyro * 0.01;
  //          Jinyun baked cakeJinyun baked cakepidW.setpoint = angle_change(aimed_angle);
  //          PIDRegulation1(&pidW, angle);
  //          diffSpeedSet1();

  if (abs(speedL) > 800 || abs(speedR) > 800)
  {
    sp_set = 0;
  }
  if (pidL.setpoint < -900)
    pidL.setpoint = -900;
  if (pidR.setpoint < -900)
    pidR.setpoint = -900;
  if (pidL.setpoint > 1500)
    pidL.setpoint = 1500;
  if (pidR.setpoint > 1500)
    pidR.setpoint = 1500;
  PIDRegulation(&pidL, (float)speedL); //�ٶȻ�����
  PIDRegulation(&pidR, (float)speedR); //�ٶȻ�����

  if (pidL.result >= -pidL.deadband && pidL.result <= pidL.deadband)
  {
    setMotorL(-(int16)pidL.result);
  }

  if (pidR.result >= -pidR.deadband && pidR.result <= pidR.deadband)
  {
    setMotorR((int16)pidR.result);
  }
}