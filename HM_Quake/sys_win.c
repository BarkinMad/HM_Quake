#include "winquake.h"
#include "quakedef.h"


int32 CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
{
	COM_ParseCmdLine(lpCmdLine);

	int32 test = COM_CheckParm("-setalpha");
	int32 value = Q_atoi(com_largv[test+1]);

	uint8 *str = "Hellothere";
	uint32 len = Q_strlen(str);

	return 0;
}