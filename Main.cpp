#pragma once
#include "GL.h"
CGL vr;

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			//��Ϣ// Message For This Window
							WPARAM	wParam,			// �����ʶ��Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{//ÿ�����ڻ���һ����Ϊ���ڹ��̵Ļص�����(WndProc)
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?������Ϣ
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{////���û��رմ��ڣ��������٣���������������˳���Ϣ�����˳���Ϣѭ��
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:				//ĳ����������			// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;		//wParam��ʶ���������ֵΪ1			// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			vr.ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc,//������Ϣ������ϵͳ�ṩ��ȱʡ������
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	vr.client.Connect();	                        //����缫ñ����
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Lopo
	// ����OpenGL����
	if (!vr.CreateGLWindow("BCI SYSTEM",WIN_WIDE,WIN_HEIGHT,32,!vr.fullscreen))
		return 0;											// ʧ���˳�

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{//�յ��˳�����Ϣ
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (active)										// ���򼤻��ô?
			{
				if (keys[VK_ESCAPE])						// ESC ������ô?
				{
					done=TRUE;								// ESC �����˳��źţ�û����Ϣ����ESC���˳�ѭ��
				}
				else										// �����˳���ʱ��ˢ����Ļ
				{
					vr.DrawGLScene();//û����Ϣ��ʱ���һֱˢ����Ļ���»�
											// �������� (˫����)
				}
			}
			if(!vr.key_press())            //ִ��press����
				return 0;                  //����л�ȫ���ؽ�����ʧ�ܳ������
		}
	}

	// �رճ���
	

	return (msg.wParam);							// Exit The Program
}
