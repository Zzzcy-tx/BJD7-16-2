#include "data.h"
#include "other.h"
#include "math.h"
#define HALL_PIN P26
extern float angle;
extern int16 distance;
extern uint8 into_flag;
extern uint8 leave_flag;

extern float angle1;
extern float angle2;
extern float obstacle_angle;
extern int16 distance1;
extern int16 distance2;
extern uint8 hall_trigged;

void control(void); // ������ж�
void angle_integral(void);
void LR_go();   //���ҷ�Ԫ��˳�����