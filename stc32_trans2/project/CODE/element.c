#include "element.h"
#define HALL_PIN P26
uint8 leave_flag = 0;             //����׶�0:ֱ�н׶�1:��ת�׶�2:������ʻ
float angle = 0;                  //���ٶȻ�����������ת�Ƕȣ�ȫ�ֱ���
uint8 angle_integral_flag = 0;    //���ٶȻ��ֱ�־λ��1Ϊ����
int16 distance = 0;               //·�̻��֣�1Ϊ���ã�ȫ�ֱ���
uint8 distance_integral_flag = 0; //·�̻��ֱ�־λ
uint8 into_flag = 1;              //���׶�1:ѡ��2:����
uint8 hall_trigged = 0;

// control������renew�����б�ѭ������
void control()
{
    //�����ǻ���
    if (angle_integral_flag)
    {
        angle += gyro * 0.005;
    }

    //����������
    if (distance_integral_flag)
    {
        distance += (speedL + speedR) / 2;
    }

    //����
    if (leave_flag == 0 && sp_set != 0)
    {
        distance_integral_flag = 1; //����·�̻���
        pidL.setpoint = 80;
        pidR.setpoint = 80;
        hall_trigged = 0;
        //ֱ����ʻһ�ξ���
        
        if (distance >= 3600)
        {
            leave_flag++;
            distance = 0;
            distance_integral_flag = 0; //�ر�·�̻���
        }
    }
    else if (leave_flag == 1)
    {
        angle_integral_flag = 1; //���������ǻ���
        if (start_diration == 0)
        {
            pidL.setpoint = 0; //ת
            pidR.setpoint = 90;
        }
        else
        {
            pidL.setpoint = 90; //ת
            pidR.setpoint = 0;
        }

        hall_trigged = 0;
        if (fabs(angle) >= 700)
        {
            angle_integral_flag = 0; //�ر������ǻ���
            angle = 0;               //�Ƕȸ�λ
            leave_flag++;
            hall_trigged = 0;
        }
    }
    else if (leave_flag == 2)
    {
        distance_integral_flag = 1; //����·�̻���
        hall_trigged = 0;           //��ֱֹ�ӽ������ģʽ
        if (distance >= 15000)
        {
            leave_flag++;
            distance_integral_flag = 0; //�ر�·�̻���
            distance = 0;
        }
    }

    if (HALL_PIN == 0)
    {
        hall_trigged = 1;
    }

    //���
    if (into_flag == 1 && hall_trigged == 1)
    {
        distance_integral_flag = 1;
        if (distance >= ku_distance)
        {
            sp_set = 0;
            if ((speedL + speedR) / 2 <= 7)
            {
                into_flag++;
                distance_integral_flag = 0;
                distance = 0;
            }
        }
    }
    else if (into_flag == 2) //��ת90��
    {
        angle_integral_flag = 1;
        if (start_diration == 0)
        {
            setSpeedL(-(int16)((2 * into_R - 0.18) / (2 * into_R + 0.18) * (float)aimed_into_speed));
            setSpeedR(-aimed_into_speed);
        }
        else
        {
            setSpeedL(-aimed_into_speed);
            setSpeedR(-(int16)((2 * into_R - 0.18) / (2 * into_R + 0.18) * (float)aimed_into_speed));
        }

        // setPidLR(27.4, 12, 0, 9000);
        // setPidW(3.68, 0, 9.86);
        // angle_integral_flag = 1;      //���������ǻ���
        // pidW.setpoint = -2500;        //�ǶȻ�Ŀ��ֵ(��90��)
        // PIDRegulation1(&pidW, angle); //�ǶȻ�����
        // // setSpeedL(-pidW.result / 100 * 3 / 30); //����Ŀ���ٶ�
        // setSpeedL(0);
        // setSpeedR(-pidW.result / 100); //����Ŀ���ٶ�
        if (fabs(angle) >= 2100) //�������ﵽĿ��(90�Ȳ���һ��)
        {
            into_flag++;
            // sp_set = 0;
        }
    }
    else if (into_flag == 3) //ֱ�ߵ���
    {
        // setPidW(6, 0, 11);                  //�ǶȻ�����������̫�̲�̫�õ�
        // pidW.setpoint = -2500;              //�ǶȻ�Ŀ��ֵ
        // PIDRegulation1(&pidW, angle);       // PID����
        // setSpeedR(-pidW.result / 90 - 100); //�ٶ��趨
        // setSpeedL(pidW.result / 90 - 100);  //�ٶ��趨
        setSpeedLR(-aimed_into_speed);
        distance_integral_flag = 1; //����·�̻���

        if (distance <= -2000) //����Ŀ�����
        {
            into_flag++;                //������
            distance = 0;               //���븴λ
            distance_integral_flag = 0; //�ر�·�̻���
            angle_integral_flag = 0;    //�ر������ǻ���
            angle = 0;                  //�Ƕȸ�λ
            sp_set = 0;                 //ͣ��
        }
    }
}
