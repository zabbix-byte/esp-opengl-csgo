#include "wtypes.h"
#include <iostream>

/* Code make by zabbix | zabbix@ztrunks.space */

void get_screen_resolution(int& x, int& y)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	x = desktop.right;
	y = desktop.bottom;
}