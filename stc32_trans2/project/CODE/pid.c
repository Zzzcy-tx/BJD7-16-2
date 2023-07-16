#include "pid.h"

// pid������
PID pidL; //�ٶȻ�
PID pidR; //�ٶȻ�
PID pidS; //����
PID pidW; //�ǶȻ�

//����ʽpid����(pi����)
void PIDRegulation(PID *vPID, float processValue)
{

    float thisError;

    float increment;

    float pError, dError, iError;

    thisError = vPID->setpoint - processValue;

    pError = thisError - vPID->lasterror;

    iError = thisError;

    dError = thisError - 2 * (vPID->lasterror) + vPID->preerror;

    increment = vPID->proportiongain * pError + vPID->integralgain * iError + vPID->derivativegain * dError; //????

    vPID->preerror = vPID->lasterror;

    vPID->lasterror = thisError;

    vPID->result += increment;

    if (vPID->result > vPID->deadband)
        vPID->result = vPID->deadband;
    if (vPID->result < -vPID->deadband)
        vPID->result = -vPID->deadband;
}

//λ��ʽpid����(pd����)
void PIDRegulation1(PID *vPID, float processValue)

{
    float thisError;

    thisError = processValue - vPID->setpoint;

    vPID->integral += thisError;

    vPID->result = vPID->proportiongain * thisError + vPID->integralgain * vPID->integral + vPID->derivativegain * (thisError - vPID->lasterror);

    vPID->lasterror = thisError;
}

//������pid����
void PIDRegulation2(PID *vPID, float processValue)
{
	float index;
    float thisError;
    thisError = processValue - vPID->setpoint;
	if(vPID->result > 15000)
	{
		if(abs(thisError) > 300)
		{
			index = 0.0;
		}else
		{
			if(thisError < 0)
			{
				index = 1.0;
				vPID->integral += thisError;
			}
		}
	}else if(vPID->result < -15000)
	{
		if(abs(thisError) > 300)
		{
			index = 0.0;
		}else
		{
			if(thisError > 0)
			{
				index = 1.0;
				vPID->integral += thisError;
			}
		}
	}else
	{
		if(abs(thisError) > 300)
		{
			index = 0.0;
		}else
		{
			if(abs(thisError) < 100)
			{
				index = 1.0;
				vPID->integral += thisError;
			}
			else
			{
				index = (300-abs(thisError)) / 200;
				vPID->integral += thisError;
			}
		}
	}
	
    vPID->result = vPID->proportiongain * thisError + index * vPID->integralgain * vPID->integral / 2 + vPID->derivativegain * (thisError - vPID->lasterror ) + vPID->Gkd * gyro;

    vPID->lasterror = thisError;
}

//�����ٶȻ�Ŀ��ֵ
void setSpeedLR(int16 speed)
{
    pidL.setpoint = (float)speed;
    pidR.setpoint = (float)speed;
}
//�����ٶȻ�Ŀ��ֵ
void setSpeedL(int16 speed)
{
    pidL.setpoint = (float)speed;
}
//�����ٶȻ�Ŀ��ֵ
void setSpeedR(int16 speed)
{
    pidR.setpoint = (float)speed;
}
//�����ٶȻ�����
void setPidL(int16 p, int16 i, int16 d, int16 deadband)
{
    pidL.proportiongain = p;
    pidL.integralgain = i;
    pidL.derivativegain = d;
    pidL.deadband = deadband;
}
//�����ٶȻ�����
void setPidR(int16 p, int16 i, int16 d, int16 deadband)
{
    pidR.proportiongain = p;
    pidR.integralgain = i;
    pidR.derivativegain = d;
    pidR.deadband = deadband;
}
//�����ٶȻ�����
void setPidLR(int16 p, int16 i, int16 d, int16 deadband)
{
    pidL.proportiongain = p;
    pidL.integralgain = i;
    pidL.derivativegain = d;
    pidR.proportiongain = p;
    pidR.integralgain = i;
    pidR.derivativegain = d;
    pidL.deadband = deadband;
    pidR.deadband = deadband;
}
//���÷��򻷲���
void setPidS(float p, float i, float d)
{
    pidS.proportiongain = p;
    pidS.integralgain = i;
    pidS.derivativegain = d;
}
//���÷��򻷲���(��������)
void setPidS_2(float p, float i, float d, float gk)
{
    pidS.proportiongain = p;
    pidS.integralgain = i;
    pidS.derivativegain = d;
	pidS.Gkd = gk;
}
//���ýǶȻ�����
void setPidW(float p, float i, float d)
{
    pidW.proportiongain = p;
    pidW.integralgain = i;
    pidW.derivativegain = d;
}