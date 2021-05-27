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
	static CImage lio, walk, bg, riohpbar, riohp, hurt;
	static Character Lio;
	static HBITMAP hBitmap;
	static int offset;

	static int rio_hp;

	switch (iMessage) {
	case WM_CREATE:
		lio.Load(L"mario\\stand.png");
		walk.Load(L"mario\\run.png");
		hurt.Load(L"mario\\hurt.png");
		bg.Load(L"BG.jpg");
		riohpbar.Load(L"mario\\hp_bar.png");
		riohp.Load(L"mario\\hp.png");
		Lio.currPos.x = 200;
		Lio.pose = 0;
		offset = 0;

		rio_hp = 363;

		SetTimer(hWnd, 1, 80, NULL);
		SetTimer(hWnd, 2, 1, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, 800, 600);
		memdc = CreateCompatibleDC(hdc);

		SelectObject(memdc, hBitmap);

		//bg
		bg.Draw(memdc, 0, 0, 800, 600, 0, 0, 1200, 581);

		riohpbar.Draw(memdc, 5, 25, 369, 39, 0, 0, 124, 14);

		if (rio_hp > 0) {
			riohp.Draw(memdc, 8, 28, rio_hp, 33, 0, 0, 120, 10);
		}

		if (Lio.pose == 0) {
			//lio
			lio.Draw(memdc, Lio.currPos.x, 380, 25 * 4, 35 * 4, offset * 26, 0, 25, 36);
		}
		else if (Lio.pose == 1) {
			//walk
			walk.Draw(memdc, Lio.currPos.x, 380, 35 * 4, 36 * 4, offset * 35, 0, 35, 36);
		}
		else if (Lio.pose == 5) {
			//hult
			hurt.Draw(memdc, Lio.currPos.x, 380, 35 * 4, 36 * 4, 0, 0, 30, 38);
		}

		BitBlt(hdc, 0, 0, 800, 600, memdc, 0, 0, SRCCOPY);
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (Lio.pose == 0) {
				offset++;
				offset = offset % 7;
			}
			if (Lio.pose == 1) {
				offset++;
				offset = offset % 8;
			}
			InvalidateRect(hWnd, NULL, false);
			break;
		case 2:
			if (GetAsyncKeyState(0x44)) // 현재 키의 토글 상태를 알 수 있다
			{
				Lio.pose = 1;
				Lio.currPos.x += 2;
			}
			else if (GetAsyncKeyState(0x41)) // 현재 키의 토글 상태를 알 수 있다
			{
				Lio.pose = 0;
				Lio.currPos.x -= 2;
			}
			else if (GetAsyncKeyState(VK_SPACE)) // 현재 키의 토글 상태를 알 수 있다
			{
				Lio.pose = 5;
				rio_hp -= 5;
			}
			else {
				Lio.pose = 0;
				//offset = 0;
			}
			break;
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
