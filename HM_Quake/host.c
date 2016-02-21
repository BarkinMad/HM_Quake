#include "quakedef.h"

double realTime = 0; 
double oldRealTime = 0; 
double host_FrameTime = 0; 

qboolean Host_FilterTime(float time)
{
	realTime += time;
	if (realTime - oldRealTime < 1.0 / 72.0)
		return false; 
	host_FrameTime = realTime - oldRealTime;
	oldRealTime = realTime; 

	return true; 
}

void Host_Init(void) 
{
}

void Host_Frame(float timeStep)
{
	if (!Host_FilterTime(timeStep))
		return; 
	//update
	//render
}

void Host_Shutdown(void)
{
}