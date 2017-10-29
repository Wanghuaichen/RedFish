/*
 * CPGCore.c
 *
 * Created: 2014/4/21 9:53:09
 * Debug v1:2014/5/12 19:12:00
 *  Author: Meano
 */ 
#include "CPGCore.h"
/*
double	F;									//Ƶ��
double	Miu;								//����Ƶ�ʸı����Ӧ�ٶ�
*/
u8 CPGMode=0;

u8 Speed=0;
u8 Direct=7;
u8 Mode=0;

u8 ModeCount=0;
u8 ModeKeep=0;
u8 ModeTotal=0;
u8 ReadyGetData=0;

double	rNow[JointCount];						//�ڶ���ֵ
double	rLast[JointCount];						//�ڶ���ֵ��¼
double	drNow[JointCount];						//�ڶ���ֵһ��΢��
double	drLast[JointCount];						//�ڶ���ֵһ��΢�ּ�¼
double	RSet[JointCount];						//�趨�ڶ���ֵ

double	xNow[JointCount];						//ƫ�ý�
double	xLast[JointCount];						//ƫ�ýǼ�¼
double	dxNow[JointCount];						//ƫ�ý�һ��΢��
double	dxLast[JointCount];						//ƫ�ý�һ��΢�ּ�¼
double	XSet[JointCount];						//�趨ƫ�ý�

double	FaiBuffer[JointCount][JointCount];		//�ͺ�Ǳ仯����
double	FaiNow[JointCount];						//�ͺ�ǵ�ǰֵ
double	FaiLast[JointCount];					//�ͺ����һ�ε�ֵ��¼
double	dFaiNow[JointCount];					//�ͺ��һ��΢��
double	dFaiLast[JointCount];					//�ͺ��һ��΢�ּ�¼

double	Sita[JointCount];						//�����
int		ServoOffset[JointCount];				//���ƫ������

CPGPara CPGP;

//Ƶ��
/*const double FTem[16]=
{ 0, 1 ,0.375+KF,0.4+KF,0.429+KF,0.462+KF,0.5+KF,0.546+KF,0.6+KF,0.66+KF,0.75+KF,0.858+KF,1.0+KF,1.2+KF,1.5+KF,2.0+KF
};*/
const double FTem[16]=
{ 0, 2.0+KF ,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF,2.0+KF
};

//�ͺ������
const double FaiTem[JointCount][16]=
{
	{0      , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    , 0    },
	{0      , 4.147, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698, 0.698},
	{0      , 2.953, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513, 2.513}
};

//16�ְڷ��ٶ�  JointCount(�ؽ���Ŀ)==3
/*const float RTem[JointCount][16] = 
{
	{0.1*KR   ,	17,	0.2*KR   , 0.25*KR   , 0.3*KR   ,0.35*KR   ,0.4*KR   ,0.45*KR   ,0.5*KR   ,0.55*KR   ,0.6*KR   ,0.65*KR   ,0.7*KR   ,0.75*KR   ,0.85*KR   ,0.90*KR},
	{0.1*KR+10,	7,  0.2*KR+10, 0.25*KR+10, 0.3*KR+10,0.35*KR+10,0.4*KR+10,0.45*KR+10,0.5*KR+10,0.55*KR+10,0.6*KR+10,0.65*KR+10,0.7*KR+10,0.75*KR+10,0.85*KR+10,0.90*KR+10},
	{0.1*KR+15, 17, 0.2*KR+15, 0.25*KR+15, 0.3*KR+15,0.35*KR+15,0.4*KR+15,0.45*KR+15,0.5*KR+15,0.55*KR+15,0.6*KR+15,0.7*KR+15,0.75*KR+15,0.8*KR+15,0.85*KR+15,0.90*KR+15},
};*/
const float RTem[JointCount][16] = 
{
	{0.9*KR   ,	17,	0.9*KR   , 0.9*KR   , 0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.9*KR   ,0.90*KR},
	{0.9*KR+10,	7,  0.9*KR+10, 0.9*KR+10, 0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.9*KR+10,0.90*KR+10},
	{0.9*KR+15, 17, 0.9*KR+15, 0.9*KR+15, 0.9*KR+15,0.9*KR+15,0.9*KR+15,0.9*KR+15,0.9*KR+15,0.9*KR+15,0.9*KR+15,0.7*KR+15,0.9*KR+15,0.9*KR+15,0.9*KR+15,0.90*KR+15},
};

//16��ת��ֵ
const float XTem[JointCount][17] = 
{
	{-1.0*KX	, -0.9*KX	, -0.8*KX   , -0.7*KX   , -0.5*KX   ,-0.3*KX   ,-0.1*KX   ,0.0*KX   ,0.1*KX   ,0.2*KX   ,0.3*KX   ,0.5*KX   ,0.7*KX   ,0.8*KX   ,0.9*KX   ,1.0*KX   ,-70},
	{-1.0*KX-5	, -0.9*KX-5	, -0.8*KX-5 , -0.7*KX-5 , -0.5*KX-5 ,-0.3*KX-5 ,-0.1*KX-5 ,0.0*KX	,0.1*KX-5 ,0.2*KX-5 ,0.3*KX-5 ,0.5*KX-5 ,0.7*KX-5 ,0.8*KX-5 ,0.9*KX-5 ,1.0*KX-5 ,-75},
	{-1.0*KX-10	, -0.9*KX-10, -0.8*KX-10, -0.7*KX-10, -0.5*KX-10,-0.3*KX-10,-0.1*KX-10,0.0*KX	,0.1*KX-10,0.2*KX-10,0.3*KX-10,0.5*KX-10,0.7*KX-10,0.8*KX-10,0.9*KX-10,1.0*KX-10,-80},
};


void ResetData(void)
{
	u8 i;
	for (i=0;i<JointCount;i++)
	{
		rNow[i]=0;
		xNow[i]=0;
		rLast[i]=0;
		xLast[i]=0;
		drNow[i]=0;
		dxNow[i]=0;
		drLast[i]=0;
		dxLast[i]=0;
		FaiNow[i]=0;
		FaiLast[i]=0;
		dFaiNow[i]=0;
		dFaiLast[i]=0;
		Sita[i]=0;
		XSet[i]=0;
		RSet[i]=0;
	}
	FaiBuffer[0][1]=0;
	FaiBuffer[0][2]=0;
	FaiBuffer[1][0]=0;
	FaiBuffer[2][0]=0;
}
void CalculateR(double A,double* R)
{
	u8 i;
	for(i=0;i<JointCount;i++)
	{
		drNow[i] = drLast[i]   - 0.02*A*(A*(rLast[i]-R[i])+2*drLast[i]);
		rNow[i]   = rLast[i]    + 0.02*drNow[i];
		drLast[i]  = drNow[i];
		rLast[i]    = rNow[i];
	}
}

void CalculateX(double B,double* X)
{
	u8 i;
	for(i=0;i<JointCount;i++)
	{
		dxNow[i] = drLast[i]   - 0.02*(B*B*(xLast[i]-X[i])+2*B*dxLast[i]);
		xNow[i]   = xLast[i]    + 0.02*dxNow[i];
		dxLast[i]  = dxNow[i];
		xLast[i]    = xNow[i];
	}
}

void CalculateFai(double Miu,double F,double* Fai)
{
	u8 i,j;								//˫ѭ�������������
	double D[JointCount],a,b;
	for(i=0;i<JointCount;i++)			//ѭ��ȡֵ
	{
		FaiBuffer[0][i]=Fai[i];			//��λ��
	}
	for (i=1;i<JointCount;i++)
	{
		for (j=0;j<JointCount;j++)
		{
			if (i!=j)
			{
				FaiBuffer[i][j]=FaiBuffer[0][j]-FaiBuffer[0][i];
			}
		}
	}
	for(i=0;i<JointCount;i++)
	{
		D[i]=(JointCount-1)*FaiLast[i];
	}
	for (i=0;i<JointCount;i++)
	{
		for (j=0;j<JointCount;j++)
		{
			if (j!=i)
			{
				D[i]=D[i]-FaiLast[j]-FaiBuffer[i][j];
			}
		}
	}
	a=0.02*Miu*Miu;
	b=0.08*Miu;
	for(i=0;i<JointCount;i++)
	{
		dFaiNow[i]=dFaiLast[i]-a*D[i]-b*(dFaiLast[i]-pi2*F);
		FaiNow[i]=FaiLast[i]+0.02*dFaiNow[i];
		dFaiLast[i]=dFaiNow[i];
		FaiLast[i]=FaiNow[i];
	}
}
double cos(double x)
{
	double y;   //���㷵��ֵ
	double z ;  //��ʱ����

	while(x>=pi2)
	{
		x=x-pi2;
	}
	while(x<0)
	{
		x=x+pi2;  
	}
	 
//********************����cosֵ*********************
     if(x>=0&&x<pi/2)
	 {
		 y=1-0.5*x*x+x*x*x*x/24.0;
	 }
	 else if(x>=pi/2&&x<pi)
	 {
	     z=pi-x;
		 y=0.5*z*z-1-z*z*z*z/24.0;
	 }
	 else if(x>=pi&&x<1.5*pi)
	 {
	     z=x-pi;
		 y=0.5*z*z-1-z*z*z*z/24.0;
	 }
	 else
	 {
		z=2*pi-x;
		y=1-0.5*z*z+z*z*z*z/24.0;
	 }
	 return y;
}
/*
	                 Miu ��Ӧ�ٶ�; F Ƶ��;	  A ��;	       B ��;	  R[JointCount]:�ڷ�;   X[JointCount]:ƫ�ý�;  Fai[JointCount]:�ͺ��;
*/
void CalculateSita(double Miu,double F,double A,double B,double* R,double* X,double* Fai)
{
	u8 i;
	double FaiPush[JointCount+1];											//��һ����Ϊ����
	CalculateR(A,R);														//ƫ�ýǼ���
	CalculateX(B,X);														//��ֵ����
	CalculateFai(Miu,F,Fai);												//��λ�����
	memcpy((void *)FaiPush,(void *)FaiLast,JointCount*sizeof(double));		//Debug@5.12 2014 memcpyΪ�����ֽ� ԭ�����޷���������
	for (i=0;i<(JointCount-1);i++)
	{
		if (FaiPush[i]<FaiPush[i+1])
		{
			FaiPush[JointCount]=FaiPush[i];
			FaiPush[i]=FaiPush[i+1];
			FaiPush[i+1]=FaiPush[JointCount];
		}
	}
	if (FaiPush[JointCount-1]>pi2)
	{
		while (FaiPush[JointCount-1]>pi2)
		{
			for (i=0;i<JointCount;i++)
			{
				FaiLast[i]=FaiLast[i]-pi2;
				FaiNow[i]=FaiNow[i]-pi2;
				FaiPush[i]=FaiPush[i]-pi2;
			}
		}
	}
	for(i=0;i<JointCount;i++)
	{
		Sita[i]=KSita*(xNow[i]+rNow[i]*cos(FaiNow[i]));
	}
}

void CalculateSitaf(float Miu,float F,float A,float B,float* R,float* X,float* Fai)
{
	int i=0;
	double Rf[JointCount];
	double Xf[JointCount];
	double Faif[JointCount];
	for(i=0;i<JointCount;i++){
		Rf[i]=R[i];
		Xf[i]=X[i];
		Faif[i]=Fai[i];
	}
	CalculateSita(Miu,F,A,B,Rf,Xf,Faif);
}

void CalculateSitaLevel(double Miu,double A,double B,u8 V,u8 D)				//�����ٶȺͷ��� �ӱ���ȡFai F X R��ֵ
{
	u8 i;
	double F,Fai[JointCount],X[JointCount],R[JointCount];
	F=FTem[V];																//Ƶ��
	for(i=0;i<JointCount;i++)												//ѭ��ȡֵ
	{
		Fai[i]=FaiTem[i][V];												//��λ��
		X[i]=XTem[i][D];													//ƫ�ý�
		R[i]=RTem[i][V];													//�ڷ�
	}
	CalculateSita(Miu,F,A,B,R,X,Fai);
}
