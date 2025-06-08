// GameOfLife.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GameOfLife.h"
#include <ctime>

#define MAX_LOADSTRING 100
#define MAXN 900

// 全局变量:
HINSTANCE hInst;							
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			
// the var about GameOfLife 
int iSelect,preSelect;
const int nUint = 50;
int iUint;
BOOL bStart,bPause;
int cxClient,cyClient,xPos,yPos,xUint,yUint,iLength,prexPos,preyPos;
int arrGame[2][MAXN][MAXN];
int tmp,tmp1,age,speed;
int iSave,iDont;

TCHAR* arrStatic[] = {TEXT("Burn"),TEXT("Live"),TEXT("lvpengms"),
	TEXT("Ver 1.0"),TEXT("lvpengms@sina.com")};
struct Button{
	int id;
	TCHAR* text;
}arrButtonText[] = {ID_START,TEXT("start"),
	ID_PAUSE,TEXT("pause"),
	ID_CONTINUE,TEXT("random"),
	ID_STOP,TEXT("stop"),
	ID_EDIT,TEXT("edit")},
	arrTimer[] = {ID_SLOW,TEXT("Slow"),
	ID_FAST,TEXT("Fast"),
	ID_HYPER,TEXT("Hyper")};
int arrTime[3] = {1000,200,30};
HICON hIcon;
HPEN hPen;
HWND hButton[5],hRadio[2][10],hStatic[5],hMain,hTimer[3];
RECT rect;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK		TimerProc(HWND,UINT,UINT,DWORD);
int Live(const int& ,const int& );
int InitWindow(HWND);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG msg;
	HACCEL hAccelTable;
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMEOFLIFE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEOFLIFE));
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MY));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH) (COLOR_BTNFACE + 1) ;//(HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1000, 650, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
	hMain = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_STOP:
			bStart = FALSE;
			EnableWindow(hButton[3],FALSE);
			EnableWindow(hButton[0],TRUE);
			EnableWindow(hButton[1],FALSE);
			EnableWindow(hButton[2],TRUE);
			EnableWindow(hButton[4],FALSE);
			KillTimer(hWnd,ID_TIMER);
			memset(arrGame,0,sizeof(arrGame));
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case ID_CONTINUE:
			memset(arrGame,0,sizeof(arrGame));
			for(int i = 0;i < nUint; ++i)
				for(int j = 0;j < nUint; ++j)
					if(rand() < 6000)
						arrGame[iSelect][i][j] = 1;
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case ID_START:
			bStart = TRUE;
			bPause = FALSE;
			age = 0;
			SetTimer(hWnd,ID_TIMER,arrTime[speed],TimerProc);
			EnableWindow(hButton[0],FALSE);
			EnableWindow(hButton[1],TRUE);
			EnableWindow(hButton[3],TRUE);
			EnableWindow(hButton[2],FALSE);
			EnableWindow(hButton[4],TRUE);
			SetWindowText(hButton[1],TEXT("pause"));
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case ID_EDIT:
			bStart = FALSE;
			KillTimer(hWnd,ID_TIMER);
			EnableWindow(hButton[1],FALSE);
			EnableWindow(hButton[2],FALSE);
			EnableWindow(hButton[3],FALSE);
			EnableWindow(hButton[0],TRUE);
			EnableWindow(hButton[4],FALSE);
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_PAUSE:
			if(bPause)
			{
				bPause = FALSE;
				SetWindowText(hButton[1],TEXT("pause"));
			}else
			{
				bPause = TRUE;
				SetWindowText(hButton[1],TEXT("continue"));
			}
			break;
		case ID_S1:case ID_S2:case ID_S3:case ID_S4:
		case ID_S5:case ID_S6:case ID_S7:case ID_S8:
			if(LOWORD(wParam)-ID_S1 == iDont - 1)
				break;
			CheckRadioButton(hWnd,ID_S1,ID_S8,LOWORD(wParam));
			iSave = LOWORD(wParam)-ID_S1+1;
			break;
		case ID_D1:	case ID_D2:	case ID_D3:case ID_D4:
		case ID_D5:case ID_D6:case ID_D7:case ID_D8:
			if(LOWORD(wParam)-ID_D1 == iSave - 1)
				break;
			CheckRadioButton(hWnd,ID_D1,ID_D8,LOWORD(wParam));
			iDont = LOWORD(wParam)-ID_D1+1;
			break;
		case ID_SLOW:case ID_FAST:case ID_HYPER:
			if(bStart)
			{
				KillTimer(hWnd,ID_TIMER);
				SetTimer(hWnd,ID_TIMER,arrTime[LOWORD(wParam)-ID_SLOW],TimerProc);
			}
			speed = LOWORD(wParam)-ID_SLOW;
			CheckRadioButton(hWnd,ID_SLOW,ID_HYPER,LOWORD(wParam));
			break;
		}
		break;
	case WM_MOUSEMOVE:
		if(bStart)
			break;
		if(wParam& MK_LBUTTON||wParam&MK_RBUTTON)
		{
			xPos = HIWORD(lParam)/iUint;
			yPos = LOWORD(lParam)/iUint;
			if(xPos == prexPos && yPos == preyPos)
				break;
			else
			{
				prexPos = xPos;
				preyPos = yPos;
			}
		}
		else 
			break;
	case WM_LBUTTONDOWN:case WM_RBUTTONDOWN:
		if(bStart)
			break; 
		yPos = LOWORD(lParam)/iUint;
		xPos = HIWORD(lParam)/iUint;
		if(yPos>=nUint || xPos>=nUint)
			break;
		prexPos = xPos;
		preyPos = yPos;
		hdc = GetDC(hWnd);
		if(message == WM_LBUTTONDOWN||wParam&MK_LBUTTON)
		{
			arrGame[iSelect][xPos][yPos] = 1;
			SelectObject(hdc,GetStockObject(NULL_PEN));
			SelectObject(hdc,GetStockObject(BLACK_BRUSH));
			Rectangle(hdc,yPos*iUint+1,xPos*iUint+1,(1+yPos)*iUint+1,(1+xPos)*iUint+1);
		}else
		{
			arrGame[iSelect][xPos][yPos] = 0;
			SelectObject(hdc,GetStockObject(NULL_PEN));
			SelectObject(hdc,GetStockObject(WHITE_BRUSH));
			Rectangle(hdc,yPos*iUint+1,xPos*iUint+1,(1+yPos)*iUint+1,(1+xPos)*iUint+1);
		}
		ReleaseDC(hWnd,hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if(!bStart) //have not started
		{
			SelectObject(hdc,GetStockObject(WHITE_BRUSH));
			Rectangle(hdc,0,0,iUint*nUint+10,iUint*nUint+10);
			SelectObject(hdc,hPen);
			for(int i = 0;i <= nUint; ++i)
			{
				tmp = i*iUint;
				tmp1 = nUint*iUint;
				MoveToEx(hdc,0,tmp,NULL);
				LineTo(hdc,tmp1,tmp);
				MoveToEx(hdc,tmp,0,NULL);
				LineTo(hdc,tmp,tmp1);
			}
			for(int i = 0;i < nUint ; ++i)
				for(int j = 0;j < nUint ; ++j)
					if(arrGame[iSelect][i][j])
					{
						SelectObject(hdc,GetStockObject(NULL_PEN));
						SelectObject(hdc,GetStockObject(BLACK_BRUSH));
						Rectangle(hdc,j*iUint+1,i*iUint+1,(j+1)*iUint+1,(i+1)*iUint+1);
					}
		}else
		{
			SelectObject(hdc,GetStockObject(WHITE_BRUSH));
			SelectObject(hdc,GetStockObject(BLACK_PEN));
			Rectangle(hdc,0,0,iUint*nUint+10,iUint*nUint+10);
			for(int i = 0;i < nUint ; ++i)
				for(int j = 0;j < nUint ; ++j)
					if(arrGame[iSelect][i][j])
					{
						SelectObject(hdc,GetStockObject(NULL_PEN));
						SelectObject(hdc,GetStockObject(BLACK_BRUSH));
						Rectangle(hdc,j*iUint+1,i*iUint+1,(j+1)*iUint+1,(i+1)*iUint+1);
					}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		if(bStart)
			KillTimer(hWnd,ID_TIMER);
		DeleteObject(hPen);
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		hdc = GetDC(hWnd);
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		iUint = cyClient/(nUint);
		MoveWindow(hButton[0],cyClient+2*iUint,cyClient/10,80,30,TRUE);
		MoveWindow(hButton[2],cyClient+2*iUint,cyClient/10+cyClient/10,80,30,TRUE);
		MoveWindow(hButton[1],cyClient+2*iUint,cyClient/10+cyClient/5,80,30,TRUE);
		MoveWindow(hButton[4],cyClient+2*iUint,cyClient/10+cyClient*3/10,80,30,TRUE);
		MoveWindow(hButton[3],cyClient+2*iUint,cyClient/10+cyClient*2/5,80,30,TRUE);
		for(int i = 0;i < 8; ++i)
		{
			  MoveWindow(hRadio[0][i],cyClient+iUint+130,cyClient*(1+i)/16,40,20,TRUE);
			  MoveWindow(hRadio[1][i],cyClient+iUint+180,cyClient*(1+i)/16,40,20,TRUE);
		}
		MoveWindow(hStatic[0],cyClient+iUint+130,iUint,40,20,TRUE);
		MoveWindow(hStatic[1],cyClient+iUint+180,iUint,40,20,TRUE);
		for(int i = 2;i < 5; ++i)
		{
			MoveWindow(hStatic[i],cxClient-160,cyClient*4/5+(i-2)*20,150,20,TRUE);
			MoveWindow(hTimer[i-2],cxClient-120,(i)*30,70,20,TRUE);
		}
		InvalidateRect(hWnd,NULL,TRUE);
		ReleaseDC(hWnd,hdc);
		break;
	case WM_CREATE:
		InitWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
int InitWindow(HWND hWnd)
{
	srand(unsigned(time(NULL)));
	TCHAR str[10];
	bStart = FALSE;
	iSelect = 0;
	iSave = 3;
	iDont = 2;
	speed = 1;
	prexPos = preyPos = -1;
	hPen = CreatePen(PS_SOLID,1,RGB(95,141,89));
	for(int i = 0;i < 5; ++i)
		hButton[i] = CreateWindow(TEXT("button"),arrButtonText[i].text, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			0,0,0,0,hWnd,(HMENU)(arrButtonText[i].id),hInst,NULL);
	for(int i = 0;i < 8; ++i)
	{
		wsprintf(str,TEXT("%d"),i+1);
		hRadio[0][i] = CreateWindow(TEXT("button"),str,WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
			0,0,0,0,hWnd,(HMENU)(ID_S1+i),hInst,NULL);
		hRadio[1][i] = CreateWindow(TEXT("button"),str,WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
			0,0,0,0,hWnd,(HMENU)(ID_D1+i),hInst,NULL);
	}
	for(int i = 0;i < 5; ++i)
	{
		hStatic[i] = CreateWindow(TEXT("static"),arrStatic[i],WS_CHILD|WS_VISIBLE|SS_LEFT,
			0,0,0,0,hWnd,(HMENU)ID_TXT1,hInst,NULL);
	}
	for(int i = 0;i < 3; ++i)
	{
		hTimer[i] = CreateWindow(TEXT("button"),arrTimer[i].text,WS_CHILD|WS_VISIBLE|BS_RADIOBUTTON,
			0,0,0,0,hWnd,(HMENU)(arrTimer[i].id),hInst,NULL);
	}
	EnableWindow(hButton[0],TRUE);
	EnableWindow(hButton[1],FALSE);
	EnableWindow(hButton[2],TRUE);
	EnableWindow(hButton[3],FALSE);
	EnableWindow(hButton[4],FALSE);
	CheckRadioButton(hWnd,ID_S1,ID_S8,ID_S1+iSave-1);
	CheckRadioButton(hWnd,ID_D1,ID_D8,ID_D1+iDont-1);
	CheckRadioButton(hWnd,ID_SLOW,ID_HYPER,ID_FAST);
	return 0;
}
VOID CALLBACK TimerProc(HWND hWnd,UINT message,UINT id,DWORD dwTime)
{
	if(bPause)
		return ;
	HDC hdc;
	hdc = GetDC(hWnd);
	SetBkColor (hdc, GetSysColor (COLOR_BTNFACE)) ;
	preSelect = iSelect;
	iSelect = 1 - iSelect;
	int temp;
	TCHAR str[50];
	int num = 0;
	for(int i = 0;i < nUint; ++i)
		for(int j = 0;j < nUint; ++j)
		{
			temp = Live(i,j);
			if(temp == 1)
			{
				SelectObject(hdc,GetStockObject(NULL_PEN));
				SelectObject(hdc,GetStockObject(BLACK_BRUSH));
				Rectangle(hdc,j*iUint+1,i*iUint+1,(j+1)*iUint+1,(i+1)*iUint+1);
			}else if(temp == 0)
			{
				SelectObject(hdc,GetStockObject(NULL_PEN));
				SelectObject(hdc,GetStockObject(WHITE_BRUSH));
				Rectangle(hdc,j*iUint+1,i*iUint+1,(j+1)*iUint+1,(i+1)*iUint+1);
			}
			if(arrGame[iSelect][i][j]==1)
				++num;
		}
	iLength = wsprintf(str,TEXT("Current Life: %d    "),num);
	++age;
	TextOut(hdc,cyClient+2*iUint,cyClient*2/3,str,iLength);
	iLength = wsprintf(str,TEXT("age: %d    "),age);
	TextOut(hdc,cyClient+2*iUint,cyClient*2/3+20,str,iLength);
	ReleaseDC(hWnd,hdc);
}
//每个元胞每一轮的活动
int Live(const int& i,const int& j)
{
	int num_life = 0;
	if(i-1>= 0&&j-1>=0&&arrGame[preSelect][i-1][j-1])
		++num_life;
	if(i-1>=0 && arrGame[preSelect][i-1][j] )
		++num_life;
	if(i-1>=0 && j+1<nUint && arrGame[preSelect][i-1][j+1])
		++num_life;
	if(j-1>=0 && arrGame[preSelect][i][j-1])
		++num_life;
	if(j+1<nUint && arrGame[preSelect][i][j+1])
		++num_life;
	if(i+1<nUint && j-1>=0 && arrGame[preSelect][i+1][j-1] )
		++num_life;
	if(i+1<nUint && arrGame[preSelect][i+1][j])
		++num_life;
	if(i+1<nUint && j+1<nUint && arrGame[preSelect][i+1][j+1])
		++num_life;
	if(num_life == iSave)
	{
		arrGame[iSelect][i][j] = 1;
		return 1;
	}
	else if(num_life == iDont)
	{
		arrGame[iSelect][i][j] = arrGame[preSelect][i][j];
		return -1;
	}
	else
	{
		arrGame[iSelect][i][j] = 0;
		return 0;
	}
	return 0;
}