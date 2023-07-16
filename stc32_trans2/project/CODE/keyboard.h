#ifndef __IPS114_H
#define __IPS114_H
#include "headfile.h"
#include "data.h"
#include "ringjudge.h"

#define ButtonCnt 5 //���尴���������������Ŷ�����.c��
#define KeystrokeONE 1
#define KeystrokeTWO 2
#define KeystrokeTHREE 3
#define KeystrokeFOUR 4
#define KeystrokeFIVE 5

#define KeyONE P70
#define KeyTWO P71
#define KeyTHREE P72
#define KeyFOUR P73
#define KeyFIVE P54

#define Switch1 P50
#define Switch2 P51

#define EEPROM_Garage_Out_Dir (0)
#define EEPROM_Circle_Switch (1)
#define EEPROM_Speed (2)
#define EEPROM_Direction (3)
#define EEPROM_DistanceG (4)
#define EEPROM_DistanceC (5)
#define EEPROM_THRESHOLD (6)

//������ǩö��
typedef enum
{
    ButtonOne,
    ButtonTwo,
    ButtonThree,
    ButtonFour,
    ButtonFive
} ButtonLabel_enum;

/**************************************��������**************************************/
extern const PIN_enum ButtonPins[ButtonCnt];           //��������
extern const ButtonLabel_enum ButtonLabels[ButtonCnt]; //���Ŷ�Ӧ�ı�ǩ
extern uint16 KeystrokeLabel;
extern uint16 mode;
extern int16 Dispay_Codename;
extern float step;
extern uint16 display;
extern uint16 step_back;

void Keystroke_Scan(void);
void Keystroke_Menu(void);
void Keystroke_Menu_STEP(void);          //-1
void Keystroke_Menu_HOME(void);          // 0
void Keystroke_Menu_ONE(void);           // 1
void Keystroke_Menu_ONE_ONE(void);       // 1.1
void Keystroke_Menu_TWO(void);           // 2
void Keystroke_Menu_TWO_ONE(void);       // 2.1
void Keystroke_Menu_TWO_TWO(void);       // 2.2
void Keystroke_Menu_THREE(void);         // 3
void Keystroke_Menu_FOUR(void);          //4
void Keystroke_Menu_FIVE(void);          //5
//void Keystroke_Menu_RUNNING(void);       //666

#endif