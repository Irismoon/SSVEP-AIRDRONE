#pragma once
//Windows����
#define     RefreshHz           75                  //��Ļˢ���ʣ�Hz
#define		DATA_CHANNEL		8					//ͨ��
#define		DATA_TYPE			4                  //�жϳ��ķ���
#define		Fs					128
#define		DATA_LENGTH			int(Fs*3)			//����ش�С��SVEPÿ�δ����ݴ�3s������������
#define		SEGMENT				(Fs*160)		    //ÿ��ʱ��
#define		TRIAL				(Fs*5)				//ÿ��trial��ʱ����һ��TRIALָ����ĳһ��˸������˸��ʱ��
#define		FRE_SEGMENT			int(Fs*0.9)			//ÿ��trial��ʱ��

/* */
#define		DIS_MODE			2					//1����ʾ��Ƶ;3������ѵ�����������ֻҪֱ�ӻ�flash�ͺ��ˣ���Ļ�Զ������Ϊ��ɫ
#define		WIN_WIDE			640			//���ڿ��
#define		WIN_HEIGHT			480 				//���ڸ߶�

//#define		DIS_MODE			2					//2�����⻷��
//#define		WIN_WIDE			1280				//���ڿ��
//#define		WIN_HEIGHT			1024				//���ڸ߶�


//#define     IS_TRAIN			false				//trueѵ����false��ʾ
#define     Evaluation          true                 //evaluation
#define		TRAIN_MOVE			false               //ָѵ����ʱ���Ƿ�Ҫ��Ӧʵ�ʵ�С���ƶ�

#define		STATE_STOP			0
#define		STATE_PLAY			1
#define		STATE_REST			2
#define		STATE_END			3
#define		DEBUG				0 

#define KEYMODE false

//�������
bool TAKEOFF = false;
#define     TAKEVAR       0.01
#define     TAKE_THLD     80//������������ʮ�β����

//��������
bool LAND = false;
#define     LANDVAR       0.002
#define     LAND_THLD     100

#define		VALUE_POSITIVE		20
#define		VALUE_NEGITIVE		10
#define		VALUE_THRESHOLD		30
#define		VALUE_MINUS			10




//const int ch[8]={9,45,53,57,58,59,60,61};
const double freq[4]=			{12.5,9.37,8.33,6.8182};//�̼�Ƶ�ʣ���ʾ��ˢ��Ƶ����75HZ��75/6��/8,/9��/11
//const double freq[3]=			{37.5,25,18.75};

//const double period[3]=			{80,120,200};
const double period[4]=			{80,106.7236,120.048,146.666};//period=1/freq����˸һ�κ�ʱms
//const double period[3]=			{26.6667,40,53.3333};
//const double c[3][15]={{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//					   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//					   {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	
const double c[4][15]={{1,1,1,0,0,0,0,0,0,0,0,0,0,0},
					   {1,1,1,1,0,0,0,0,0,0,0,0,0,0},
					   {1,1,1,1,1,0,0,0,0,0,0,0,0,0},
					   {1,1,1,1,1,1,0,0,0,0,0,0,0,0}};


#define		PI					3.1415926535
#define		PI_OVER				0.0174532925//�Ƕ�ת��Ϊ���ȵ�ϵ��
#define		COM_PORT			"COM7"