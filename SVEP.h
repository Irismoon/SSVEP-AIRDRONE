#pragma once
#include "Head.h"

static DWORD WINAPI FileThreadEntry(PVOID arg);
static DWORD WINAPI MatlabThreadEntry(PVOID arg);
static DWORD WINAPI SerialportThreadEntry(PVOID arg);

class CSVEP
{
	//����SSVEP������
public:
	int		data[DATA_LENGTH][DATA_CHANNEL];
	int		data_index;
	//matlab��C++����ʱ��������,0-��û������1-C++�ڲ�����2-matlab�ڲ���
	int		critical;
	int     serialnum;

	//����C++��matlab֮�䴫����,
	//C++->matlab:��ʱ���ǩ�ͱ��
	//matlab->C++:��ʵʱ���ؽ��
	double	signal;		
	double  velocity;
	double  variance;
	int		take_var;
	int     land_var;
	HANDLE	m_hThread_file;							//�����߳̿���			
	HANDLE	m_hThread_mat;							//�����߳̿���
	HANDLE  m_hThread_port;

	char	file_mat[100];
	int		state;                                 //SVEP����״̬��play/stop/end/rest��ͨ���ո�������л�
	int		mark[DATA_TYPE];						//���ڱ�ǿ���״̬���жϵķ���
	int		trigger[DATA_LENGTH];
	//ofstream	evafile;
	//LARGE_INTEGER tstart, tend, fequency;//64λ�з������� 

	CSVEP()
	{		
		data_index=-1;
		take_var=0;
		land_var = 0;
		signal=-1;
		velocity = 0;
		variance = 0;
		state=STATE_STOP;
		serialnum = 0;
		//if (Evaluation) evafile.open("evaluation.txt");
		//QueryPerformanceFrequency(&fequency);
		for(int i=0;i<DATA_TYPE;i++)
			mark[i]=0;
		m_hThread_file=CreateThread(NULL,0,FileThreadEntry,(PVOID)this,0,NULL);	
		m_hThread_mat=CreateThread(NULL,0,MatlabThreadEntry,(PVOID)this,0,NULL);
		m_hThread_port = CreateThread(NULL, 0, SerialportThreadEntry, (PVOID)this, 0, NULL);
	}

	~CSVEP()
	{
		state=STATE_END;
		Sleep(1000);
		//if (DEBUG) evafile.close();
		if(m_hThread_file!=NULL) 
		{
			CloseHandle(m_hThread_file);
			m_hThread_file=NULL;
		}
		if(m_hThread_mat!=NULL) 
		{
			CloseHandle(m_hThread_mat);
			m_hThread_mat=NULL;
		}
		if (m_hThread_port != NULL)
		{
			CloseHandle(m_hThread_port);
			m_hThread_mat = NULL;
		}
	}

	void SetData(double *p,int nBlockPnts)
	{//�����ݽ�data
		for (int i=0;i<nBlockPnts;i++)
		{
			if(state==STATE_PLAY)
			{
				data_index++;
				trigger[data_index%DATA_LENGTH]=trigger[(data_index-1+DATA_LENGTH)%DATA_LENGTH];
				if (Evaluation) {
					if (data_index%TRIAL == 0)//��ǩ����TRIALΪ��λ�仯��
					{
						trigger[data_index%DATA_LENGTH] = rand() % DATA_TYPE+1;
						switch (trigger[data_index%DATA_LENGTH]) {
						case 1:
						PlaySound("up.wav", NULL, SND_ASYNC);
						break;
						case 4:
						PlaySound("right.wav", NULL, SND_ASYNC);
						break;
						case 3:
						PlaySound("left.wav", NULL, SND_ASYNC);
						break;
						case 2:
						PlaySound("down.wav", NULL, SND_ASYNC);
						break;
						}
					}
					if ((data_index + 1) % (SEGMENT) == 0)
						state = STATE_REST;
				}
				for (int j=0;j<DATA_CHANNEL;j++)
				{
					data[data_index%DATA_LENGTH][j]=*p;
					p++;//һ����һ��һ���ش�
				}
				p++;//����һ��

			}
		}
	}

	void Write()
	{//�����ݱ������ļ�
		char	file_date[100];
		char	file_cnt[100];
		char	file_mark[100];

		//�ļ���¼
		sprintf(file_date,"2008.12.26_09.32");
		char buff[9];
		_strdate_s( buff, 9 );//Copy the current system date to a buffer. A pointer to a buffer which will be filled in with the formatted date string,Size of the buffer
		file_date[2]=buff[6];
		file_date[3]=buff[7];
		file_date[5]=buff[0];
		file_date[6]=buff[1];
		file_date[8]=buff[3];
		file_date[9]=buff[4];

		_strtime_s( buff, 9 );
		file_date[11]=buff[0];
		file_date[12]=buff[1];
		file_date[14]=buff[3];
		file_date[15]=buff[4];


		char fn[60];
		sprintf(fn,"md data-%s",file_date);system(fn);//��fn���ļ���
		sprintf(file_cnt,"data-%s\\data_cnt-%s.txt",file_date,file_date);//�ļ�
		sprintf(file_mark,"data-%s\\data_mark-%s.txt",file_date,file_date);
		sprintf(file_mat,"data=data(1:data_index,:);save(\'..\\\\data-%s\\\\data_mark-%s.mat\', \'data\')",file_date,file_date);


		ofstream file;
		file.open(file_cnt);

		int write_index=-1;
		while(state!=STATE_END)
		{			
			if(write_index<data_index)
			{
				write_index++;
				for(int j=0;j<DATA_CHANNEL;j++)
					file<<data[write_index%DATA_LENGTH][j]<<'\t';
				if(write_index<SEGMENT)
					file<<trigger[write_index%DATA_LENGTH];
				else
					file<<-1;
				file<<'\n';
			}
			else
				Sleep(5);
		}
		file.close();
	}

	void Serial()
	{
		CSerial serial;
		while (state != STATE_END) {
			if (!Evaluation) {
				if (!TAKEOFF&&(take_var>=TAKE_THLD))//if copter has not taken off from land and variance>0.01 comes up to 10 times
				{
					TAKEOFF = true;//take off
					LAND = false;
					serial.SentSerial('t',0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					serial.SentSerial('t', 0.0000);
					take_var = 0;
					serialnum = 0;
				}
				else if (TAKEOFF&&land_var>=LAND_THLD)//if copter has taken off and  variance<0.005 comes up to 6 times
				{
					//serial.SentSerial(0,0.0000);
                    serial.SentSerial('l',0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					serial.SentSerial('l', 0.0000);//sent land command 
					LAND = true;
					TAKEOFF = false;//ready for the next takeoff
					land_var = 0;
					serialnum = 0;
				}
				else if(TAKEOFF&&!LAND)//if the copter has taken off and not landed
				{
					if (mark[0] >= VALUE_THRESHOLD)//
					{
						mark[0] -= VALUE_MINUS;
						//evafile << '1' << '\t';
						serial.SentSerial('1',velocity);
						serialnum = 1;
					}//����
					else if (mark[1] >= VALUE_THRESHOLD)
					{
						mark[1] -= VALUE_MINUS;
						//evafile << '2' << '\t';
						serial.SentSerial('2',velocity);
						serialnum = 2;
					}//���
					else if (mark[2] >= VALUE_THRESHOLD)
					{
						mark[2] -= VALUE_MINUS;
						//evafile << '3' << '\t';
						serial.SentSerial('3',velocity);
						serialnum = 3;
					}//����
					else if (mark[3] >= VALUE_THRESHOLD)
					{
						mark[3] -= VALUE_MINUS;
						//evafile << '4' << '\t';
						serial.SentSerial('4',velocity);
						serialnum = 4;
					}//����
				}
				
			}
			else {
				if (mark[0] >= VALUE_THRESHOLD)//��ֵ���ԣ�ͬһֵ�����������μ����ϲŷ���ָ��
				{
					mark[0] -= VALUE_MINUS;
					serial.SentSerial('1',velocity);
					serialnum = 1;
				}//����
				else if (mark[1] >= VALUE_THRESHOLD)
				{
					mark[1] -= VALUE_MINUS;
					serial.SentSerial('2',velocity);
					serialnum = 2;
				}//����
				else if (mark[2] >= VALUE_THRESHOLD)
				{
					mark[2] -= VALUE_MINUS;
					serial.SentSerial('3',velocity);
					serialnum = 3;
				}//����
				else if (mark[3] >= VALUE_THRESHOLD)
				{
					mark[3] -= VALUE_MINUS;
					serial.SentSerial('4',velocity);
					serialnum = 4;
				}//���
			}
		}
	}
 
	//�������ݣ��ж�Ŀǰʵ�鴦����һ���裬��������Ӧ����
	void MatlabEng()
	{	
		//CSerial serial;
		int mat_index=-1;//���ں����������˽�б�������ֹ���������޸Ĵ�ֵ
		critical=2;		
		Engine *ep; 
		ep=engOpen(NULL);
		engSetVisible(ep,true); 

		TCHAR	mat_path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, mat_path);
		char buff[500];
		sprintf(buff,"cd('%s\\mat_file');",mat_path);
		engEvalString(ep,buff);//����ִ���ַ���buff�еı��ʽ�������ļ���
		engEvalString(ep,"head");//ִ��head�ļ������ǳ�ʼ�������ʴ���
		//if(!IS_TRAIN)
			//engEvalString(ep,"playing");//ִ��playing������ѵ���õ�ϵ��

		double *s; 
		mxArray *xx = mxCreateDoubleMatrix(1,DATA_CHANNEL+1, mxREAL); //������СΪ1x(DATA_CHANNEL+1)��˫���Ⱦ��󣬶�Ϊʵ��
		mxArray *ss= mxCreateDoubleMatrix(1,3, mxREAL);		
		critical=0;

		while(state!=STATE_END)
		{
			if(data_index>mat_index)
			{	
				critical=2;
				mat_index++;
				s=mxGetPr(xx);//�õ�ʵ�����ݵ�ָ�룬�ı�s�൱�ڸı�xx��ֵ
				for(int i=0;i<DATA_CHANNEL;i++)
					s[i]=data[mat_index%DATA_LENGTH][i];//ȡdata��һ�е�����
				if(mat_index<SEGMENT)
					s[DATA_CHANNEL]=trigger[mat_index%DATA_LENGTH];
				else
					s[DATA_CHANNEL]=-1;
				engPutVariable(ep, "x",xx);//��xxָ���������x�ı�����д��Matlab����ռ�
				engEvalString(ep,"data_add");
				ss=engGetVariable(ep, "output");//�������л�ȡ����
				s = mxGetPr(ss);
				signal = s[0]; velocity = s[1]; variance = s[2];
				//һ��trial����һ��signal��signal��0��1��2��3
				if (Evaluation) {
					if (signal > 0 && signal < 5)
					{
						//trigger[0]=int(signal);//������
						mark[int(signal) - 1] += VALUE_POSITIVE;
						for (int i = 0; i < 4; i++) {
							if (mark[i] >= VALUE_NEGITIVE)
							{
								mark[i] -= VALUE_NEGITIVE;
							}
						}
					}
				}
				else {
						if (!TAKEOFF) {
							if (variance >= TAKEVAR) {
								take_var += VALUE_POSITIVE;
							}
							if (signal > 0 && take_var > 0)
							{
								take_var -= VALUE_NEGITIVE;
							}
						}
						else if (TAKEOFF&&!LAND) {
							if (signal > 0 && signal < 5)
							{
								//trigger[0]=int(signal);//������
								mark[int(signal) - 1] += VALUE_POSITIVE;
								for (int i = 0; i < 4; i++) {
									if (mark[i] >= VALUE_NEGITIVE)
									{
										mark[i] -= VALUE_NEGITIVE;
									}
								}
								if (variance < LANDVAR)
									land_var += VALUE_POSITIVE;
								else if (land_var>0)
									land_var -= VALUE_NEGITIVE;

							}
						}
				}
				critical=0;
				
			}
			if(data_index==mat_index)
				Sleep(10);//10ms
		}
		critical=2;
		mxDestroyArray(ss);
		mxDestroyArray(xx);

		engClose(ep);
		critical=0;
	}

};

static DWORD WINAPI FileThreadEntry(PVOID arg)
{		
	((CSVEP *)arg)->Write();
	return 0;
}

static DWORD WINAPI MatlabThreadEntry(PVOID arg)
{		
	((CSVEP *)arg)->MatlabEng();
	return 0;
}
 static DWORD WINAPI SerialportThreadEntry(PVOID arg)
{
	((CSVEP *)arg)->Serial();
	return 0;
}