#include "data.h"
#include "math.h"

extern float accel_x;     // X����ٶ�ֵ
extern float accel_y;     // Y����ٶ�ֵ
extern float accel_z;     // Z����ٶ�ֵ
extern float gyro_x;      // X�������ǽ��ٶ�����
extern float gyro_y;      // Y�������ǽ��ٶ�����
extern float gyro_z;      // Z�������ǽ��ٶ�����
extern float accel_x_rea; // X����ٶ�ֵ
extern float accel_y_rea; // Y����ٶ�ֵ
extern float accel_z_rea; // Z����ٶ�ֵ
extern float gyro_x_rea;  // X�������ǽ��ٶ�����
extern float gyro_y_rea;  // Y�������ǽ��ٶ�����
extern float gyro_z_rea;  // Z�������ǽ��ٶ�����

extern float yaw_raw;      //���߽�yawԭʼ����
extern float yaw_kalman;   // yaw�˲�������
extern float pitch_raw;    //������pitchԭʼ����
extern float pitch_kalman; // pitch�˲�������
extern float roll_raw;     //�����rollԭʼ����
extern float roll_kalman;  // roll�˲�������

double KalmanFilter(const double ResrcData);
void Angle_Cal(void);
void Kalman_Cal_Pitch(float acc, float gyro);
void Kalman_Cal_Roll(float acc, float gyro);
void Kalman_Cal_Yaw(float acc, float gyro);