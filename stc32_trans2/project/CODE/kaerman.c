#include "kaerman.h"

float accel_x;      // X����ٶ�ֵ
float accel_y;      // Y����ٶ�ֵ
float accel_z;      // Z����ٶ�ֵ
float gyro_x;       // X�������ǽ��ٶ�����
float gyro_y;       // Y�������ǽ��ٶ�����
float gyro_z;       // Z�������ǽ��ٶ�����
float accel_x_rea;  // X����ٶ�ֵ
float accel_y_rea;  // Y����ٶ�ֵ
float accel_z_rea;  // Z����ٶ�ֵ
float gyro_x_rea;   // X�������ǽ��ٶ�����
float gyro_y_rea;   // Y�������ǽ��ٶ�����
float gyro_z_rea;   // Z�������ǽ��ٶ�����
float yaw_raw;      //���߽�yawԭʼ����
float yaw_kalman;   // yaw�˲�������
float pitch_kalman; // pitch�˲�������
float pitch_raw;    //������pitchԭʼ����
float roll_kalman;  // roll�˲�������
float roll_raw;     //�����rollԭʼ����

//�������˲����ɣ�
static double p_last = 0;
static double x_last = 0;
double R = 0.1; //�˲�������ԽСԽƽ��
double Q = 15;  //�˲�������Խ��Խƽ��
double KalmanFilter(const double ResrcData)
{

  double x_mid;
  double x_now;

  double p_mid;
  double p_now;

  double kg;

  //����p_last ���� kalmanFilter_A ��pֱ��ȡ0
  x_mid = x_last;     // x_last=x(k-1|k-1),x_mid=x(k|k-1)
  p_mid = p_last + Q; // p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����

  /*
   *  �������˲��������Ҫ��ʽ
   */
  kg = p_mid / (p_mid + R);                 // kgΪkalman filter��R Ϊ����
  x_now = x_mid + kg * (ResrcData - x_mid); //���Ƴ�������ֵ
  p_now = (1 - kg) * p_mid;                 //����ֵ��Ӧ��covariance
  p_last = p_now;                           //����covariance ֵ
  x_last = x_now;                           //����ϵͳ״ֵ̬

  return x_now;
}

void Angle_Cal()
{
  imu963ra_get_acc();
  imu963ra_get_gyro();
  // 1.��ȡ���ٶȺͽǶ�����
  accel_x = imu963ra_acc_x;
  accel_y = imu963ra_acc_y;
  accel_z = imu963ra_acc_z;
  gyro_x = imu963ra_gyro_x;
  gyro_y = imu963ra_gyro_y;
  gyro_z = imu963ra_gyro_z;

  // 2.�Ǽ��ٶ�ԭʼֵ�������
  if (accel_x < 32764)
    accel_x_rea = accel_x / 4098.0;
  else
    accel_x_rea = 1 - (accel_x - 49152) / 4098.0;
  if (accel_y < 32764)
    accel_y_rea = accel_y / 4098.0;
  else
    accel_y_rea = 1 - (accel_y - 49152) / 4098.0;
  if (accel_z < 32764)
    accel_z_rea = accel_z / 4098.0;
  else
    accel_z_rea = 1 - (accel_z - 49152) / 4098.0;

  //���ٶȷ����й�ʽ�����������ˮƽ������ϵ֮��ļн�
  pitch_raw = (atan(accel_y_rea / accel_z_rea)) * 180 / 3.14;
  roll_raw = (atan(accel_x_rea / accel_z_rea)) * 180 / 3.14;
  yaw_raw = (atan(accel_y_rea / accel_x_rea)) * 180 / 3.14;

  // 3.���ٶ�ԭʼֵ�������
  if (gyro_x_rea < 32768)
    gyro_x_rea = -(gyro_x_rea / 14.3);
  if (gyro_x_rea > 32768)
    gyro_x_rea = +(65535 - gyro_x_rea) / 14.3;
  if (gyro_y_rea < 32768)
    gyro_y_rea = -(gyro_y_rea / 14.3);
  if (gyro_y_rea > 32768)
    gyro_y_rea = +(65535 - gyro_y_rea) / 14.3;
  if (gyro_z_rea < 32768)
    gyro_z_rea = -(gyro_z_rea / 14.3);
  if (gyro_z_rea > 32768)
    gyro_z_rea = +(65535 - gyro_z_rea) / 14.3;

  // 4.���ÿ���������
  Kalman_Cal_Pitch(pitch_raw, gyro_x_rea); //�������˲����㸩����
                                           //	Kalman_Cal_Roll(roll_raw,gyro_y_rea);  //�������˲�������б��
                                           //	Kalman_Cal_Yaw(yaw_raw,gyro_z_rea);  //�������˲�����ƫ����
		if (pitch_kalman > 15)
		{
			puodao = 1;
		}
		else if(puodao == 1 && pitch_kalman < -10)
		{
			puodao = 2;
		}
	 else if(puodao == 2 && pitch_kalman <5)
		{
			puodao = 0;
		}
		else
		{
			puodao = 0;
		}
}

//����������
static float Q_angle = 0.0003; //�Ƕ��������Ŷȣ��Ƕ�������Э����
static float Q_gyro = 0.0009;  //���ٶ��������Ŷȣ����ٶ�������Э����
static float R_angle = 0.8;    //���ٶȼƲ���������Э����
static float dt = 0.005;       //�������ڼ�������������5ms

void Kalman_Cal_Pitch(float acc, float gyro) //�������˲�pitch�����
{
  static float Q_bias0;                      // Q_bias:�����ǵ�ƫ��
  static float K_00, K_10;                   //����������  K_0:���ڼ������Ź���ֵ  K_1:���ڼ������Ź���ֵ��ƫ�� t_0/1:�м����
  static float PP0[2][2] = {{1, 0}, {0, 1}}; //����Э�������P����ʼֵΪ��λ��

  /*
    �������˲���ʹ�ò���
    (1) ѡ��״̬�����۲���
    (2) ��������
    (3) ��ʼ������
    (4) ���빫ʽ����
    (5) ���ڳ�����P��Q
  */
  /*
  X(k)��kʱ��ϵͳ״̬				Z(k)��kʱ�̲���ֵ
  U(k)��kʱ�̶�ϵͳ������		H������ϵͳ����
                                           ����
  A/F��״̬ת�ƾ���					W(k)���������� ----> Q
                                           ����
  B�����ƾ���								V(k)���������� ----> R

                    ��ɢ����ϵͳ
  ϵͳ������X(k|k-1) = AX(k-1|k-1) + BU(k) + (W(k))
  ����ֵ��Z(k) = HX(k) + V(k)
  */
  /*
  1. �������
* * *��ʽ1��X(k|k-1) = AX(k-1|k-1) + BU(k) + (W(k))

    X = (Angle,Q_bias)
    A(1,1) = 1,A(1,2) = -dt
    A(2,1) = 0,A(2,2) = 1
    ע����������[���������
    Ԥ�⵱ǰ�Ƕ�ֵ��
    [ angle ] 	[1 -dt][ angle ]	 [dt]
    [ Q_bias] = [0  1 ][ Q_bias] + [ 0] * newGyro(���ٶȼƲ���ֵ)
    ��
    angle = angle - Q_bias*dt + newGyro * dt
    Q_bias = Q_bias
  */
  pitch_kalman += (gyro - Q_bias0) * dt; //״̬����,�Ƕ�ֵ�����ϴ����ŽǶȼӽ��ٶȼ���Ư�����

  /*
  2. Ԥ��Э�������
* * *��ʽ2��P(k|k-1)=AP(k-1|k-1)A^T + Q

    �����������ϵͳ����
        [1 -dt]
    A = [0  1 ]

    ϵͳ����Э��������Q���壺
    | cov(angle,angle)  cov(Q_bias,angle) |
    |	cov(angle,Q_bias) cov(Q_bias,Q_bias)|
       �Ƕ������ͽ��ٶ�Ư�������໥����
    | D( angle )  			0 	 |
  = |			0 			D( Q_bias )|
    ��Q_angle��Q_bias�ķ���Ϊ������
    ���ɾ�������ó�

    ��D( angle )  = Q_angle
      D( Q_bias ) = Q_gyro

    ����һ��Ԥ��Э�������ΪP(k-1)
                      |a(k-1)  b(k-1)|
                      |c(k-1)  d(k-1)|
    ����Ԥ��Э�������P(k)
                      |a(k)  b(k)|
                      |c(k)  d(k)|
    �ɹ�ʽ2��P(k|k-1)=AP(k-1|k-1)A^T + Q ��
    |a(k)  b(k)|		|1 -dt|	|a(k-1) b(k-1)| |1 	 0|		| D( angle )  			0 	 |
    |c(k)  d(k)| =  |0  1 | |c(k-1) d(k-1)| |-dt 1| + |			0 			D( Q_bias )|

    ��һ����
    |a(k)  b(k)|		|a(k-1) - [c(k-1) + b(k-1)]*dt + d(dt)^2		b(k-1) - d(k-1)*dt|		| D( angle )  			0 	 |
    |c(k)  d(k)| =  |				c(k-1) - d(k-1)*dt													d(k-1)		| + |			0 			D( Q_bias )|

    ����dt^2̫С����dt^2ʡ��
  */

  PP0[0][0] = PP0[0][0] + Q_angle - (PP0[0][1] + PP0[1][0]) * dt;
  PP0[0][1] = PP0[0][1] - PP0[1][1] * dt;
  PP0[1][0] = PP0[1][0] - PP0[1][1] * dt;
  PP0[1][1] = PP0[1][1] + Q_gyro;

  /*
    3. ������������
      ϵͳ�������� Z(k) = HX(k) + V(k)
      ϵͳ����ϵ�� H = [1, 0]
      ��Ϊ����������Դ�����
      ����
      measure = newAngle
  */

  /*
    4. ���㿨��������
* * *��ʽ3��Kg(k)= P(k|k-1)H^T/(HP(k|k-1)H^T+R)
        Kg = (K_0,K_1) ��Ӧangle,Q_bias����
        H = (1,0)
  */
  K_00 = PP0[0][0] / (PP0[0][0] + R_angle);
  K_10 = PP0[1][0] / (PP0[0][0] + R_angle);

  /*
    5. ���㵱ǰ���Ż�����ֵ
* * *��ʽ4��X(k|k) = X(k|k-1) + kg(k)[z(k) - HX(k|k-1)]
    angle = angle + K_0*(newAngle - angle)
    Q_bias = Q_bias + K_1*(newAngle - angle)
  */

  pitch_kalman = pitch_kalman + K_00 * (acc - pitch_kalman);
  Q_bias0 = Q_bias0 + K_10 * (acc - pitch_kalman);

  /*
    6. ����Э�������
* * *��ʽ5��P(k|k)=[I-Kg(k)H]P(k|k-1)
  */
  PP0[0][0] = PP0[0][0] - K_00 * PP0[0][0];
  PP0[0][1] = PP0[0][1] - K_00 * PP0[0][1];
  PP0[1][0] = PP0[1][0] - K_10 * PP0[0][0];
  PP0[1][1] = PP0[1][1] - K_10 * PP0[00][1];
}

void Kalman_Cal_Roll(float acc, float gyro) //�������˲�roll�����
{
  static float Q_bias;                      // Q_bias:�����ǵ�ƫ��  Angle_err:�Ƕ�ƫ��
  static float K_0, K_1;                    //����������  K_0:���ڼ������Ź���ֵ  K_1:���ڼ������Ź���ֵ��ƫ�� t_0/1:�м����
  static float PP[2][2] = {{1, 0}, {0, 1}}; //����Э�������P����ʼֵΪ��λ��
  roll_kalman += (gyro - Q_bias) * dt;      //״̬����,�Ƕ�ֵ�����ϴ����ŽǶȼӽ��ٶȼ���Ư�����
  PP[0][0] = PP[0][0] + Q_angle - (PP[0][1] + PP[1][0]) * dt;
  PP[0][1] = PP[0][1] - PP[1][1] * dt;
  PP[1][0] = PP[1][0] - PP[1][1] * dt;
  PP[1][1] = PP[1][1] + Q_gyro;
  K_0 = PP[0][0] / (PP[0][0] + R_angle);
  K_1 = PP[1][0] / (PP[0][0] + R_angle);
  roll_kalman = roll_kalman + K_0 * (acc - roll_kalman);
  Q_bias = Q_bias + K_1 * (acc - roll_kalman);
  PP[0][0] = PP[0][0] - K_0 * PP[0][0];
  PP[0][1] = PP[0][1] - K_0 * PP[0][1];
  PP[1][0] = PP[1][0] - K_1 * PP[0][0];
  PP[1][1] = PP[1][1] - K_1 * PP[0][1];
}

void Kalman_Cal_Yaw(float acc, float gyro) //�������˲�roll�����
{
  static float Q_bias;                      // Q_bias:�����ǵ�ƫ��  Angle_err:�Ƕ�ƫ��
  static float K_0, K_1;                    //����������  K_0:���ڼ������Ź���ֵ  K_1:���ڼ������Ź���ֵ��ƫ�� t_0/1:�м����
  static float PP[2][2] = {{1, 0}, {0, 1}}; //����Э�������P����ʼֵΪ��λ��
  yaw_kalman += (gyro - Q_bias) * dt;       //״̬����,�Ƕ�ֵ�����ϴ����ŽǶȼӽ��ٶȼ���Ư�����
  PP[0][0] = PP[0][0] + Q_angle - (PP[0][1] + PP[1][0]) * dt;
  PP[0][1] = PP[0][1] - PP[1][1] * dt;
  PP[1][0] = PP[1][0] - PP[1][1] * dt;
  PP[1][1] = PP[1][1] + Q_gyro;
  K_0 = PP[0][0] / (PP[0][0] + R_angle);
  K_1 = PP[1][0] / (PP[0][0] + R_angle);
  yaw_kalman = yaw_kalman + K_0 * (acc - yaw_kalman);
  Q_bias = Q_bias + K_1 * (acc - yaw_kalman);
  PP[0][0] = PP[0][0] - K_0 * PP[0][0];
  PP[0][1] = PP[0][1] - K_0 * PP[0][1];
  PP[1][0] = PP[1][0] - K_1 * PP[0][0];
  PP[1][1] = PP[1][1] - K_1 * PP[0][1];
}