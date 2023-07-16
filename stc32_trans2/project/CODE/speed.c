#include "speed.h"

//设置电机pwm
//设置电机pwm
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

//差速计算
float Diff_Factor = 0.006;            //差速系数
int16 Differential(float Diff_Factor) //阿克曼原理，根据舵机进行差速控制，参数整定依赖工程实践
{
  float Diff;
  Diff = Diff_Factor * ((pidL.setpoint + pidR.setpoint) / 2) * pidS.result; // diff_factor是差速系数
  return (int16)Diff;
}

//计算两轮的速度目标值
void diffSpeedSet()
{
  int16 Diff_Speed = Differential(Diff_Factor); //通过函数计算差速值
    //if (Diff_Speed > 1500)	//差速限幅
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

// //计算角度环输出
// void diffSpeedSet1()
// {
//   setSpeedL(pidL.setpoint + pidW.result / 90);
//   setSpeedR(pidR.setpoint - pidW.result / 90);
// }

//中断中被循环调用




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
    setSpeedLR(sp_set_max); //设置速度初始值为目标值
    pidS.proportiongain = P_S;
    pidS.derivativegain = D_S_z;
    
  }
  else
  {
    pidS.proportiongain = P_S_MAX;
    pidS.derivativegain = D_S;
    setSpeedLR(sp_set); //设置速度初始值为目标值
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
  //设置转向PID参数
  setPidS(P_S + (fabs(k) / 50) * (P_S_MAX - P_S), 0, D_S_z + (fabs(k) / 50) * (D_S - D_S_z));
  PIDRegulation1(&pidS, k); //方向环计算(加陀螺仪积分)
  pidS.result += G_S * gyro;
  if(Ocon_flag == 1 && obstacle_count == 0)
  {
	//  obstacle_control();//一定要放setpids后面
  }
  diffSpeedSet(); //差速设置（输入量为pidS.result和pidL.result）
  // control();      //赛道元素控制
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
  PIDRegulation(&pidL, (float)speedL); //速度环计算
  PIDRegulation(&pidR, (float)speedR); //速度环计算

  if (pidL.result >= -pidL.deadband && pidL.result <= pidL.deadband)
  {
    setMotorL(-(int16)pidL.result);
  }

  if (pidR.result >= -pidR.deadband && pidR.result <= pidR.deadband)
  {
    setMotorR((int16)pidR.result);
  }
}