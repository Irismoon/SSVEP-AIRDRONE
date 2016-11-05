#pragma once
#include "Head.h"
#include "Serial.h"
#include "Video.h"
#include "CClient.h"
#include "SVEP.h"


int Width = 100;
int Height = 50;
int Length = 50;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		// WndProc�Ķ���
bool	keys[256];											// ������̰���������
bool	active=true;										// ���ڵĻ��־��ȱʡΪTRUE


class CGL : public CVideo
{
public:

	float			xpos;
	float			zpos;
	GLfloat			yrot;				// Y Rotation

	CClient client;	
	CSVEP	svep;
	LARGE_INTEGER t1,t2,feq;//64λ�з������� 
	double t;

	HDC			hDC;	
	HGLRC		hRC;	
	HWND		hWnd;
	HINSTANCE	hInstance;
	GLuint		base;
	GLuint		base2;
	bool		fullscreen;	
	GLuint		VideoTexture[1];
	GLuint      SkyboxTexture[6];
	GLuint      BallTexture[1];
	ofstream	outfile;

	CGL()
	{
		client.data=&svep;//һ��ָ��csvep���͵�ָ�� = svep�����ã����ݵ�ַ��
		if(DEBUG)outfile.open("record.txt");
		hRC=NULL;
		hDC=NULL;
		hWnd=NULL;
		fullscreen=true;
		//�����溯����ԭ�ͣ�������ȡ�߾��ȵļ�ʱ
		QueryPerformanceFrequency(&feq);//�ڶ�ʱǰӦ���ȵ���QueryPerformanceFrequency()������û����ڲ���ʱ����ʱ��Ƶ�ʡ�
		QueryPerformanceCounter(&t1);
		//����Ҫ�ϸ��ʱ���¼�����ǰ�ͷ���֮��ֱ����QueryPerformanceCounter()���������λ�õļ���֮���ʱ��Ƶ�ʣ��Ϳ��Լ�����¼������ľ�ȷʱ�䡣
		xpos=0;
		zpos=0;
	}

	~CGL()
	{
		if(DEBUG)outfile.close();
		KillGLWindow();
	}

	double get_clock()
	{
		QueryPerformanceCounter(&t2);
		t=((double)t2.QuadPart-(double)t1.QuadPart)*1000/((double)feq.QuadPart);
		return t;//����˫���ȵ�ʱ��
	}

	AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
	{
		FILE *File=NULL;									// File Handle

		if (!Filename)										// Make Sure A Filename Was Given
		{
			return NULL;									// If Not Return NULL
		}

		File=fopen(Filename,"r");							// Check To See If The File Exists

		if (File)											// Does The File Exist?
		{
			fclose(File);									// Close The Handle
			return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
		}
		return NULL;										// If Load Failed Return NULL
	}

	int LoadGLTextures()									// Load Bitmaps And Convert To Texturesλͼת��Ϊ����
	{
		int Status=FALSE;									// Status Indicator��������Ƿ�ɹ�����

		AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture����һ������ͼ�����飬����ָ�������СΪ1
		//һ��RGBͼ��ṹ����,������������Ա��ͼ��Ŀ��,�߶�,ͼ�������������ݣ���ʵҲ���Ǹ�ͼ�����ڴ��е��������ݵ�һ��ָ��
		memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

		// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
		glGenTextures(6, &SkyboxTexture[0]);  
		if (TextureImage[0]=LoadBMP("front.bmp"))
		{			
			Status=TRUE;
			glBindTexture(GL_TEXTURE_2D, SkyboxTexture[0]); //������OpenGL�����������κβ������Ƕ������󶨵��������ģ�����glBindTexture(GL_TEXTURE_2D, 1)����OpenGL��������ж�2D������κ����ö����������Ϊ1�������
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}
		if (TextureImage[0]=LoadBMP("back.bmp"))
		{
			glBindTexture(GL_TEXTURE_2D, SkyboxTexture[1]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}
		if (TextureImage[0]=LoadBMP("left.bmp"))
		{
			glBindTexture(GL_TEXTURE_2D, SkyboxTexture[2]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}
		if (TextureImage[0]=LoadBMP("right.bmp"))
		{
			glBindTexture(GL_TEXTURE_2D, SkyboxTexture[3]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}		
		if (TextureImage[0] = LoadBMP("up.bmp"))
		{
			glBindTexture(GL_TEXTURE_2D, SkyboxTexture[4]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}
		if (TextureImage[0] = LoadBMP("down.bmp"))
		{
			glBindTexture(GL_TEXTURE_2D, SkyboxTexture[5]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}
		glGenTextures(1, &BallTexture[0]);
		if (TextureImage[0] = LoadBMP("Ball.bmp"))
		{
			glBindTexture(GL_TEXTURE_2D, BallTexture[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		}
		glGenTextures(1, &VideoTexture[0]);
		if(DIS_MODE==1&&is_capture)//��Ƶģʽ
		{
			glBindTexture(GL_TEXTURE_2D,VideoTexture[0]);//��������λͼ�����ݴ���NEAREST����
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,capture.mWidth,capture.mHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)capture.mTargetBuf);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		}

		if (TextureImage[0])									// If Texture Exists
		{
			if (TextureImage[0]->data)							// If Texture Image Exists
			{
				free(TextureImage[0]->data);					// Free The Texture Image Memory
			}

			free(TextureImage[0]);								// Free The Image Structure
		}
		return Status;										// Return The Status
	}


	GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
	{
		if (height==0)										// Prevent A Divide By Zero By
		{
			height=1;										// Making Height Equal One
		}

		glViewport(0,0,width,height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

	int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
	{
		if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
		{
			return FALSE;									// If Texture Didn't Load Return FALSE
		}
		glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		return TRUE;										// Initialization Went OK
	}



	GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
	{
		if (fullscreen)										// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
			ShowCursor(TRUE);								// Show Mouse Pointer
		}

		if (hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			hRC=NULL;										// Set RC To NULL
		}

		if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hDC=NULL;										// Set DC To NULL
		}

		if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hWnd=NULL;										// Set hWnd To NULL
		}

		if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hInstance=NULL;									// Set hInstance To NULL
		}
	}

	/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
	*	title			- Title To Appear At The Top Of The Window				*
	*	width			- Width Of The GL Window Or Fullscreen Mode				*
	*	height			- Height Of The GL Window Or Fullscreen Mode			*
	*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
	*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

	BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
	{
		GLuint		PixelFormat;			// Holds The Results After Searching For A Match
		WNDCLASS	wc;						// Windows Class Structure���ƴ�����ṹ
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left=(long)0;			// Set Left Value To 0
		WindowRect.right=(long)width;		// Set Right Value To Requested Width
		WindowRect.top=(long)0;				// Set Top Value To 0
		WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

		fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

		hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc		= (WNDPROC) WndProc;//������Ϣ������					// WndProc Handles Messages
		wc.cbClsExtra		= 0;									// No Extra Window Data
		wc.cbWndExtra		= 0;									// No Extra Window Data
		wc.hInstance		= hInstance;//ʵ�����							// Set The Instance
		wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);//ͼ��			// Load The Default Icon
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);//���			// Load The Arrow Pointer
		wc.hbrBackground	= NULL;									// No Background Required For GL
		wc.lpszMenuName		= NULL;									// We Don't Want A Menu
		wc.lpszClassName	= "OpenGL";//������								// Set The Class Name
		//ע��
		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;											// Return FALSE
		}

		if (fullscreen)												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
			dmScreenSettings.dmDisplayFrequency	= 75;				// Selected Bits Per Pixel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;

			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
				if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
				}
				else
				{
					// Pop Up A Message Box Letting User Know The Program Is Closing.
					MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
					return FALSE;									// Return FALSE
				}
			}
		}

		if (fullscreen)												// Are We Still In Fullscreen Mode?
		{
			dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
			dwStyle=WS_POPUP;										// Windows Style
			ShowCursor(FALSE);										// Hide Mouse Pointer
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

		// Create The Window
		if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
			"OpenGL",							// Class Name
			title,								// Window Title
			dwStyle |							// Defined Window Style
			WS_CLIPSIBLINGS |					// Required Window Style
			WS_CLIPCHILDREN,					// Required Window Style
			0, 0,								// Window Position
			WindowRect.right-WindowRect.left,	// Calculate Window Width
			WindowRect.bottom-WindowRect.top,	// Calculate Window Height
			NULL,								// No Parent Window
			NULL,								// No Menu
			hInstance,							// Instance
			NULL)))								// Dont Pass Anything To WM_CREATE
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		ShowWindow(hWnd,SW_SHOW);						// Show The Window
		SetForegroundWindow(hWnd);						// Slightly Higher Priority
		SetFocus(hWnd);									// Sets Keyboard Focus To The Window
		ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen
		//ǰ�����ú��˹ܵ����ſ�����gl��ͼ
		if (!InitGL())									// Initialize Our Newly Created GL Window
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		return TRUE;									// Success
	}

	bool key_press()
	{
		if (keys[VK_F1])								// F1��������ô?
		{
			keys[VK_F1]=FALSE;							// ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE
			KillGLWindow();								// ���ٵ�ǰ�Ĵ���
			fullscreen=!fullscreen;						// �л� ȫ�� / ���� ģʽ

			// �ؽ� OpenGL ����
			if (!CreateGLWindow("BCI SYSTEM",WIN_WIDE,WIN_HEIGHT,32,fullscreen))
			{
				return false;								// �������δ�ܴ����������˳�
			}
		}
			
		//���ݴ���ָ�������ͼ
		switch(svep.serialnum)
		{
		case 9:
			break;
		case 0:
			break;//���治��
		case 2:
			xpos += (float)sin(yrot*PI_OVER) * 0.01f;//svep.velocity
			zpos += (float)cos(yrot*PI_OVER) * 0.01f;
			break;//��
		case 1:
			xpos -= (float)sin(yrot*PI_OVER) * 0.01f;
			zpos -= (float)cos(yrot*PI_OVER) * 0.01f;
			break;//ǰ
		case 4:			
			yrot -= 0.1f;
			break;//��
		case 3:			
			yrot += 0.1f;
			break;//��
		}

		if(keys[VK_SPACE])
		{			
			keys[VK_SPACE]=FALSE;
			switch(svep.state)
			{
			case STATE_STOP:
				svep.state=STATE_PLAY;
				break;
			case STATE_REST:
				if(!Evaluation)
					svep.state=STATE_PLAY;;
				break;
			case STATE_PLAY:
				if(!Evaluation)
					svep.state=STATE_REST;;
				break;
			}
		}
		return true;
	}	

	int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		if(DIS_MODE==1)
			DrawVideo();
		if(DIS_MODE==2)
			DrawVirtural();
		/*if (svep.state == STATE_PLAY&&IS_TRAIN)
		{
			DrawTrainFlash();
		}*/
		if (svep.state == STATE_PLAY/*&&!IS_TRAIN*/)
			DrawFlash();
		
		//������
		/*if(svep.state==STATE_PLAY&&!IS_TRAIN)
			DrawResult();*/
		::SwapBuffers(hDC);	//�������壬����ͼ�ο⣬���ơ�	
		if(is_updated)//�������
		{//����ͷ������Ҫһֱ����
			glBindTexture(GL_TEXTURE_2D, VideoTexture[0]);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,capture.mWidth,capture.mHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)capture.mTargetBuf);
			is_updated=false;
		}
		return TRUE;										// Keep Going
	}

	void DrawVideo()
	{
		float w=3.2,h=1.65;		
		glLoadIdentity();									// Reset The View
		glTranslatef(0.0f,0.0f,-5.0f);//ƽ��
		glColor3d(1,1,1);	//��ǰ����ɫ��ɫ
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, VideoTexture[0]);
		
		glBegin(GL_QUADS);//��������
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -w, -h,  1.0f);//ָ����ǰ��ʹ�ã�0��1���������꣬���������
		glTexCoord2f(1.0f, 1.0f); glVertex3f(w, -h,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(w,  h,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -w,  h,  1.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	void DrawFlash()
	{
		float w1=0.04,w2=0.02,h=-0.1;
		glLoadIdentity();// ���õ�ǰָ���ľ���Ϊ��λ����.��������glLoadIdentity()֮����ʵ���Ͻ���ǰ���Ƶ�����Ļ���ģ�
		//OpenGL��Ļ���ĵ�����ֵ��X��Y���ϵ�0.0f�㡣/�������������ֵ�Ǹ�ֵ����������ֵ��������Ļ��������ֵ��������Ļ�׶��Ǹ�ֵ��������Ļ��Ǹ�ֵ���Ƴ���Ļ������ֵ��
		glColor3d(1,1,1);//һ�������õ�ǰɫΪ��ɫ

		//glBindTexture(GL_TEXTURE_2D, texture[5]); //������һ���󶨵�Ŀ������������Ƶ�������texture[5]������󶨵�ǰ�߲�����
		//   ѡ����������texture[5]������������
		get_clock();
		if(DEBUG)outfile<<t<<'\t';

		int  index;
		bool flash_temp=true;
		index=int(t*RefreshHz/1000)%int(RefreshHz*period[0]/1000);//����һ��Ƶ�ʶ�Ӧ��������
		//t�ǵ�ǰ�Ѿ���ȥ�˶೤ʱ�䣬���Ը���Ƶ�ʶ�Ӧ�����ڣ����Ǹ���Ƶ�ʶ�Ӧ������
		glColor3d(c[0][index],c[0][index],c[0][index]);	//red,green ,blue
		if(DEBUG)outfile<<c[0][index]<<'\t';
		glBegin(GL_QUADS); 
		glVertex3d(w1/2,w2,h);
		glVertex3d(w1/2,w1,h);
		glVertex3d(-w1/2,w1,h);
		glVertex3d(-w1/2,w2,h);
		glEnd();//��ʱ�뻭�ģ�������ĳ�����

		index=int(t*RefreshHz/1000)%int(RefreshHz*period[1]/1000);//�ڶ���Ƶ�ʶ�Ӧ��������
		glColor3d(c[1][index],c[1][index],c[1][index]);
		if(DEBUG)outfile<<c[1][index]<<'\t';
		glBegin(GL_QUADS);
		glVertex3d(1.5*w1,w2/2,h);
		glVertex3d(1.5*w1,-w2/2,h);
		glVertex3d(2*w1,-w2/2,h);
		glVertex3d(2*w1,w2/2,h);
		glEnd();//���½ǵ�������

		index=int(t*RefreshHz/1000)%int(RefreshHz*period[2]/1000);//������Ƶ�ʶ�Ӧ��������
		glColor3d(c[2][index],c[2][index],c[2][index]);
		if(DEBUG)outfile<<c[2][index]<<'\t';
		glBegin(GL_QUADS);
		glVertex3d(-1.5*w1,w2/2,h);
		glVertex3d(-1.5*w1,-w2/2,h);
		glVertex3d(-2*w1,-w2/2,h);
		glVertex3d(-2*w1,w2/2,h);//���½ǵ�������
		glEnd();

		index = int(t * RefreshHz / 1000) % int(RefreshHz * period[3] / 1000);//���ĸ�Ƶ�ʶ�Ӧ��������
		glColor3d(c[3][index], c[3][index], c[3][index]);
		if (DEBUG)outfile << c[3][index] << '\n';
		glBegin(GL_QUADS);
		glVertex3d(w1/2,-w1, h);
		glVertex3d(w1/2, -w2, h);
		glVertex3d(-w1/2, -w2, h);
		glVertex3d(-w1/2,-w1, h);//���������˸����
		glEnd();
		
	}

	void DrawTrainFlash()
	{
		float w1 = 0.04, w2 = 0.02, h = -0.1;
		glLoadIdentity();// ���õ�ǰָ���ľ���Ϊ��λ����.��������glLoadIdentity()֮����ʵ���Ͻ���ǰ���Ƶ�����Ļ���ģ�
		//OpenGL��Ļ���ĵ�����ֵ��X��Y���ϵ�0.0f�㡣/�������������ֵ�Ǹ�ֵ����������ֵ��������Ļ��������ֵ��������Ļ�׶��Ǹ�ֵ��������Ļ��Ǹ�ֵ���Ƴ���Ļ������ֵ��
		glColor3d(1, 1, 1);//һ�������õ�ǰɫΪ��ɫ

		get_clock();
		if (DEBUG)outfile << t << '\t';

		int  index;
		bool flash_temp = true;
		switch (svep.trigger[svep.data_index%DATA_LENGTH])
		{
		case 1://��ǰ
			index = int(t * RefreshHz / 1000) % int(RefreshHz * period[0] / 1000);//����һ��Ƶ�ʶ�Ӧ��������
			//t�ǵ�ǰ�Ѿ���ȥ�˶೤ʱ�䣬���Ը���Ƶ�ʶ�Ӧ�����ڣ����Ǹ���Ƶ�ʶ�Ӧ������
			glColor3d(c[0][index], c[0][index], c[0][index]);	//red,green ,blue
			if (DEBUG)outfile << c[0][index] << '\t';
			glBegin(GL_QUADS);
			glVertex3d(w1 / 2, w2, h);
			glVertex3d(w1 / 2, w1, h);
			glVertex3d(-w1 / 2, w1, h);
			glVertex3d(-w1 / 2, w2, h);
			glEnd();//��ʱ�뻭�ģ�������ĳ�����
			break;
		case 4://����
			index = int(t * RefreshHz / 1000) % int(RefreshHz * period[1] / 1000);//�ڶ���Ƶ�ʶ�Ӧ��������
			glColor3d(c[1][index], c[1][index], c[1][index]);
			if (DEBUG)outfile << c[1][index] << '\t';
			glBegin(GL_QUADS);
			glVertex3d(2 * w1, w2 / 2, h);
			glVertex3d(2 * w1, -w2 / 2, h);
			glVertex3d(1.5 * w2, -w2 / 2, h);
			glVertex3d(1.5 * w2, w2 / 2, h);
			glEnd();//���½ǵ�������
			break;
		case 3://����
			index = int(t * RefreshHz / 1000) % int(RefreshHz * period[2] / 1000);//������Ƶ�ʶ�Ӧ��������
			glColor3d(c[2][index], c[2][index], c[2][index]);
			if (DEBUG)outfile << c[2][index] << '\t';
			glBegin(GL_QUADS);
			glVertex3d(-1.5 * w2, w2 / 2, h);
			glVertex3d(-1.5 * w2, -w2 / 2, h);
			glVertex3d(-2 * w1, -w2 / 2, h);
			glVertex3d(-2 * w1, w2 / 2, h);//���½ǵ�������
			glEnd();
			break;
		case 2://���
			index = int(t * RefreshHz / 1000) % int(RefreshHz * period[3] / 1000);//���ĸ�Ƶ�ʶ�Ӧ��������
			glColor3d(c[3][index], c[3][index], c[3][index]);
			if (DEBUG)outfile << c[3][index] << '\n';
			glBegin(GL_QUADS);
			glVertex3d(w1 / 2, -w2, h);
			glVertex3d(w1 / 2, -w1, h);
			glVertex3d(-w1 / 2, -w1, h);
			glVertex3d(-w1 / 2, -w2, h);//���������˸����
			glEnd();
			break;
		}
	}

	void DrawVirtural()
	{
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glLoadIdentity();									// Reset The View
		glColor3f(1.0f, 1.0f, 1.0f);	
		GLfloat x = -Width / 2;
		GLfloat y = -Height / 2;
		GLfloat z = -Length / 2;
		GLfloat xtrans = -xpos;
		GLfloat ztrans = -zpos;
		GLfloat ytrans = -0.25f;


		glRotatef(360.0f - yrot,0,1.0f,0);
		glTranslatef(xtrans, ytrans, ztrans);

		glBindTexture(GL_TEXTURE_2D, SkyboxTexture[0] );
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z+Length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y+Height, z+Length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+Width, y+Height, z+Length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+Width, y, z + Length);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, SkyboxTexture[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+Width, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+Width, y + Height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + Height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, SkyboxTexture[2]);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y+Height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + Height, z + Length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + Length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, SkyboxTexture[3]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+Width, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+Width, y, z + Length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+Width, y + Height, z + Length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+Width, y+Height, z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, SkyboxTexture[4]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+Width, y+Height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+Width, y + Height, z + Length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + Height, z + Length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y+Height, z);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, SkyboxTexture[5]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + Length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + Width, y, z + Length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + Width, y, z);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
};

static DWORD WINAPI PlaySoundEntry(PVOID arg)
{		
	PlaySound("3.wav",NULL, SND_ASYNC);
	return 0;
}