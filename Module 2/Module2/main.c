#include <Windows.h>
#include <stdio.h>

static BOOL IsRunning = TRUE;

static double GTimePassed = 0; 
static double SecondsPerTick = 0;
static __int64 GTimeCount = 0; 

float Sys_InitFloatTime(void)
{
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

	SecondsPerTick = 1.0 / (double)Frequency.QuadPart;

	LARGE_INTEGER Counter; 
	QueryPerformanceCounter(&Counter);
	GTimeCount = Counter.QuadPart;

	return 0; 
}

float Sys_FloatTime(void)
{
	LARGE_INTEGER Counter;
	QueryPerformanceCounter(&Counter);

	__int64 Interval = Counter.QuadPart - GTimeCount;
	GTimeCount = Counter.QuadPart;
	double SecondsPassed = (double)Interval * SecondsPerTick;
	GTimePassed += SecondsPassed;

	return (float)GTimePassed;
}

void Sys_Shutdown(void)
{
	IsRunning = FALSE;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
	switch (uMsg)
	{
	case WM_DESTROY:
		Sys_Shutdown();
		break;
	default: 
		Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return Result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 2";

	if (!RegisterClass(&wc))
		exit(EXIT_FAILURE);

	HWND mainWindow; 
	DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	RECT r;
	r.top = r.left = 0;
	r.right = 800;
	r.bottom = 600;

	AdjustWindowRect(&r, WindowStyle, FALSE);

	mainWindow = CreateWindowEx(
		0,
		"Module 2",
		"Lesson 2.3",
		WindowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		r.right - r.left,
		r.bottom - r.top,
		NULL,
		NULL,
		hInstance,
		0);

	HDC DeviceContext = GetDC(mainWindow);
	PatBlt(DeviceContext, 0, 0, 800, 600, BLACKNESS);
	ReleaseDC(mainWindow, DeviceContext);

	float timecount = Sys_InitFloatTime();
	
	MSG msg;

	while (IsRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		float newtime = Sys_FloatTime();

		char buffer[64];
		sprintf_s(buffer, 64, "%3.7f \n", newtime);
		OutputDebugString(buffer);
		Sleep(100);
	}
	return 0; 
}