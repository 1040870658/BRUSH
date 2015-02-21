#include<windows.h>
#include<time.h>
#include<stdlib.h>

static RECT rect;
static int wsize;
static int lsize;
LRESULT CALLBACK wndpro(HWND,UINT,WPARAM,LPARAM);
int a[10][10] ={0,1,0,1,0,1,0,1,0,0,
	0,0,0,1,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,0,1,
	0,0,1,0,0,0,0,0,1,0,
	0,0,0,0,1,0,0,0,1,0,

	0,0,0,0,0,0,1,0,0,1,

	0,1,0,0,1,1,1,0,1,0,
	0,0,1,0,0,0,1,0,0,1,
	1,0,0,0,0,1,1,0,0,0,
	0,0,0,1,0,1,0,1,0,0
};
int flag;
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR IpCmd,int nShowcmd){
	TCHAR szName[] = TEXT("BRUSH");
	HWND hwnd;
	WNDCLASS wnd;
	MSG msg;

	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL,IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = wndpro;
	wnd.lpszClassName = szName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClass(&wnd)){
		MessageBox(NULL,TEXT("NEED WINDOWS NT"),szName,MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szName,TEXT("BRUSH"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT
		,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);

	ShowWindow(hwnd,nShowcmd);
	UpdateWindow(hwnd);

	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(flag == 1){
			MessageBox(NULL,TEXT("GAME OVER"),szName,MB_ICONERROR);
			return 0;
		}
		else if(flag == 2){
			MessageBox(NULL,TEXT("YOU WIN!"),szName,MB_OK);
			return 0;
		}
	}
	return msg.wParam;
}


LRESULT CALLBACK wndpro(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
	HDC hdc,hdcSrc;
	static int cxCLient,cyClient;
	PAINTSTRUCT PS;
	static HBRUSH hBrush;
	static HBITMAP hBitmap;
	
	static RECT DIS;
	static int mvx;
	static int mvy;

	switch(msg){
	case WM_CREATE:
		SetTimer(hwnd,1,200,NULL);
		SetTimer(hwnd,2,2000,NULL);
		hBitmap=LoadBitmap(((LPCREATESTRUCT)lparam)->hInstance,TEXT("MyPic"));
		return 0;

	case WM_TIMER:
		if(wparam == 2){
			a[5][6] = a[5][6]^1;
			a[5][7] = a[5][7]^1;
			InvalidateRect(hwnd,NULL,TRUE);
		}
		if(wparam == 1){
		InvalidateRect(hwnd,&rect,TRUE);
		OffsetRect(&rect,mvx,mvy);
		InvalidateRect(hwnd,&rect,true);
		}
		return 0;

	case WM_KEYDOWN:
		switch(wparam){
		case VK_UP:
			mvy = -cyClient/60;
			return 0;
		case VK_RIGHT:
			mvx = cxCLient/80;
			return 0;
		case VK_LEFT:
			mvx = -cxCLient/80;
			return 0;
		case VK_DOWN:
			mvy = cyClient/60;
			return 0;
		}
		return 0;

	case WM_SIZE:
		cxCLient = LOWORD(lparam);
		cyClient = HIWORD(lparam);
		DIS.left = cxCLient/100*99;
		DIS.top = cyClient/100*99;
		DIS.right = cxCLient;
		DIS.bottom = cyClient;
		rect.left = 0;
		rect.top = 0;
		wsize = cxCLient/60 * cyClient/cxCLient;
		lsize = cyClient / 60;
		rect.right = wsize;
		rect.bottom = lsize;
		mvx = cxCLient/80;
		mvy = cyClient/80;
		hBrush = CreateSolidBrush(RGB(255,0,0));
		//init();
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd,&PS);
		hdcSrc=CreateCompatibleDC(hdc);
		SelectObject(hdcSrc,hBitmap);
		BitBlt(hdc,0,0,100,100,hdcSrc,0,0,SRCCOPY);
		flag = 0;
		for(int i = 0;i != 10;i ++){
			for(int j = 0;j != 10;j ++){
				if(a[i][j]){
					SelectObject(hdc,GetStockObject(BLACK_BRUSH));
					int leftx = cxCLient/10*j;
					int lefty = cyClient/10*i;
					int rightx = cxCLient/10*(j + 1);
					int righty = cyClient/10*(i + 1);
					for(int k = leftx; k < rightx;k ++){
						for(int x = lefty;x < righty;x ++){
							POINT apt;
							apt.x = k;
							apt.y = x;
							if(PtInRect(&rect,apt))
								flag = 1;
						}
					}
					Rectangle(hdc,leftx,lefty,rightx,righty);
					
					
				}
			}
		}
		FillRect(hdc,&DIS,hBrush);
		for(int k = DIS.left; k < DIS.right;k ++){
			for(int x = DIS.top;x < DIS.bottom;x ++){
				POINT apt;
				apt.x = k;
				apt.y = x;
				if(PtInRect(&rect,apt))
					flag = 2;
			}
		}
		if(flag){
			KillTimer(hwnd,1);
			KillTimer(hwnd,2);
		}
		FillRect(hdc,&rect,hBrush);
		//SaveDC(hdc);
		//SelectObject(hdc,hBrush);
		//Ellipse(hdc,rect.left,rect.top,rect.right,rect.bottom);
		//RestoreDC(hdc,-1);
		EndPaint(hwnd,&PS);
		return 0;

	case WM_DESTROY:
		DeleteObject(hBrush);
		KillTimer(hwnd,1);
		KillTimer(hwnd,2);
		PostQuitMessage(0);
		DeleteObject(hBitmap);
		DeleteDC(hdcSrc);
		return 0;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}