#ifndef __PID_H
#define __PID_H
#include "headfile.h"
#include "data.h"
#include "math.h"

typedef struct pid
{
    float setpoint;       
    float proportiongain; 
    float integralgain;   
    float derivativegain; 
    float lasterror;      
    float preerror;      
    float deadband;      
    float result;        
    double integral;
	float Gkd;//������ϵ��
}PID;


extern PID pidL;
extern PID pidR;
extern PID pidS;//�ǶȻ�
extern PID pidW;//����





void PIDRegulation(PID *vPID, float processValue);
void PIDRegulation1(PID *vPID, float processValue);
void PIDRegulation2(PID *vPID, float processValue);
void setSpeedLR(int16 speed);
void setSpeedL(int16 speed);
void setSpeedR(int16 speed);
void setPidL(int16 p, int16 i, int16 d, int16 deadband);
void setPidR(int16 p, int16 i, int16 d, int16 deadband);
void setPidLR(int16 p, int16 i, int16 d, int16 deadband);
void setPidS(float p, float i, float d);
void setPidS_2(float p, float i, float d,float gk);
void setPidW(float p, float i, float d);


#endif