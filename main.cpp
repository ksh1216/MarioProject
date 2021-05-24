#include <windows.h>		//--- 윈도우 헤더 파일
#include <tchar.h>
#include <atlimage.h>

typedef struct grid {
	int x, y;
}Grid;

typedef struct charcter {
	Grid currPos;
	Grid hitBox;
	int pose;
}Character;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Lio vs Easy";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);		// 윈도우 크기 변경 가능

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc, bufferdc;
	PAINTSTRUCT ps;
	static CImage lio, walk, bg;
	static Character Lio;
	static HBITMAP hBitmap;
	static int offset;

	switch (iMessage) {
	case WM_CREATE:
		lio.Load(L"Lio_stand.png");
		walk.Load(L"Lio_walk.png");
		bg.Load(L"BG.jpg");
		Lio.currPos.x = 200;
		Lio.pose = 0;
		offset = 0;
		SetTimer(hWnd, 1, 50, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, 800, 600);
		memdc = CreateCompatibleDC(hdc);

		SelectObject(memdc, hBitmap);

		//bg
		bg.Draw(memdc, 0, 0, 800, 600, 0, 0, 1200, 581);

		if (Lio.pose == 0) {
			//lio
			lio.Draw(memdc, Lio.currPos.x, 0, 32 * 4, 64 * 4, 0, 0, 32, 64);
		}
		else if (Lio.pose == 1) {
			//walk
			walk.Draw(memdc, Lio.currPos.x, 0, 32 * 4, 64 * 4, offset * 32, 0, 32, 64);
		}

		BitBlt(hdc, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if (Lio.pose == 1) {
			offset++;
			offset = offset % 6;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_CHAR:
		if (wParam == 'j') {
			Lio.pose = 1;
			Lio.currPos.x += 5;
		} 
		break;
	case WM_KEYUP:
		if (wParam == 'j') {
			Lio.pose = 0;
			offset = 0;
		}
		break;
	case WM_DESTROY:
		lio.Destroy();
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

