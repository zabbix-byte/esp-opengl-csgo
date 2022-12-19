#include <iostream>
#include <Windows.h>
#include "datatypes.h"
#include "get_screen_resolution.h"

/* Code make by zabbix | zabbix@ztrunks.space */
/* if you dont understand the code please read this
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/*/


void convert_to_range(Vec2& point)
{

	int x = 0;
	int y = 0;
	get_screen_resolution(x, y);


	point.X /= x;
	point.X *= 2.0f;
	point.X -= 1.0f;

	point.Y /= y;
	point.Y *= 2.0f;
	point.Y -= 1.0f;
}

/* 3d object to 2d object*/
bool world_to_screen(const Vec3& vector_origin, Vec2& vector_screen, float* matrix)
{

	int x = 0;
	int y = 0;
	get_screen_resolution(x, y);

	/* model matrix*/
	vector_screen.X = vector_origin.X * matrix[0] + vector_origin.Y * matrix[1] + vector_origin.Z * matrix[2] + matrix[3];
	
	/* view matrix, like position, rotation, pitch */
	vector_screen.Y = vector_origin.X * matrix[4] + vector_origin.Y * matrix[5] + vector_origin.Z * matrix[6] + matrix[7];

	/* camara coords */
	float w = vector_origin.X * matrix[12] + vector_origin.Y * matrix[13] + vector_origin.Z * matrix[14] + matrix[15];

	if (w < 0.0f)
	{
		return false;
	}

	Vec2 ndc;

	ndc.X = vector_screen.X / w;
	ndc.Y = vector_screen.Y / w;

	vector_screen.X = (x / 2 * ndc.X) + (ndc.X + x / 2);
	vector_screen.Y = (y / 2 * ndc.Y) + (ndc.Y + y / 2);

	convert_to_range(vector_screen);
	return true;
}