#include "quakedef.h"
#include "winquake.h"

static BOOL isRunning = TRUE;

static double gTimePassed = 0;
static double secondsPerTick = 0;
static __int64 gTimeCount = 0;

float Sys_InitFloatTime(void)
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	secondsPerTick = 1.0 / (double)frequency.QuadPart;

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	gTimeCount = counter.QuadPart;

	return 0;
}

float Sys_FloatTime(void)
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	__int64 interval = counter.QuadPart - gTimeCount;
	gTimeCount = counter.QuadPart;
	double secondsPassed = (double)interval * secondsPerTick;
	gTimePassed += secondsPassed;

	return (float)gTimePassed;
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

int32 CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	COM_ParseCmdLine(lpCmdLine);
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
		"Cool",
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

	Host_Init();

	float oldTime = Sys_InitFloatTime();

	MSG msg;

	while (isRunning)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		float newTime = Sys_FloatTime();
		Host_Frame(newTime - oldTime);
		oldTime = newTime;
	}

	Host_Shutdown();
	return 0;
}