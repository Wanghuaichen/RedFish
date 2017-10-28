/*
 * CPGCore.h
 *
 * Created: 2014/4/21 9:53:58
 *  Author: Meano
 */ 
#ifndef CPGCoreH
#define CPGCoreH
#include "Fish.h"
#define pi	3.141592654		//����PI
#define pi2	6.283185307		//����2*pi
#define KR	25				//�����趨�ڶ�����
#define KX	60				//�����趨ת�����
#define KF	0.2				//�����趨Ƶ��������
#define KSita 20.0			//Sita��PWM����ת��
#define	JointCount 3		//�ؽ���Ŀ
#define ScarlCPG 100.0

extern u8 CPGMode;
extern u8 Speed;
extern u8 Direct;
extern u8 Mode;
extern u8 ModeCount;
extern u8 ModeKeep;
extern u8 ModeTotal;
extern u8 ReadyGetData;

extern double	F;										//Ƶ��
extern double	Miu;									//����Ƶ�ʸı����Ӧ�ٶ�
extern double	RSet[JointCount];						//�趨�ڶ���ֵ
extern double	XSet[JointCount];						//�趨ƫ�ý�
extern double	xNow[JointCount];						//xNow
extern double	FaiBuffer[JointCount][JointCount];		//�ͺ�Ǳ仯����
extern double	Sita[JointCount];
extern int		ServoOffset[JointCount];

extern const float XTem[JointCount][17];

typedef struct CPGParameter
{
	float Miu;
	float F;
	float A;
	float B;
	float R[3];
	float X[3];
	float Fai[3];
}CPGPara;

extern CPGPara CPGP;

extern void ResetData(void);
extern void CalculateX(double B,double* X);
extern void CalculateSitaLevel(double Miu,double A,double B,u8 V,u8 D);
extern void CalculateSita(double Miu,double F,double A,double B,double* R,double* X,double* Fai);
extern void CalculateSitaf(float Miu,float F,float A,float B,float* R,float* X,float* Fai);
#endif /* CPGCORE_H_ */
