/*! Time-stamp: <@(#)rectangle.cpp   20.12.2005 - 20.12.2005 17:56:31   Dmitry Chuchva>
 *********************************************************************
 *  @file   : rectangle.cpp
 *
 *  Project : graphics
 *
 *  Package : classes package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : definition of CRectangle class
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  20.12.2005  BN : First Revision
 *
 *********************************************************************
 */

#include "objects.h"
#include <windows.h>

CRectangle::CRectangle()
{
	x_right = y_bottom = 0;
	CObject::CObject();
	type = O_RECTANGLE;
}

CRectangle::CRectangle(int Xleft,int Ytop,int Xright,int Ybottom)
{
	CObject::CObject();
	x = Xleft; y = Ytop; x_right = Xright; y_bottom = Ybottom;
	type = O_RECTANGLE;
	SetContour(1,RGB(0,0,0));
	SetFillColor(RGB(255,255,255));
}

void CRectangle::Draw(HDC hdc)
{
	SetupColors(hdc);
	Rectangle(hdc,x,y,x_right,y_bottom);
}

bool CRectangle::Hit(HDC hdc,int X,int Y)
{
	int _x = x,_y = y,_x_right = x_right, _y_bottom = y_bottom;
	int cont = GetContourWidth();

                                        /*! если прямоугольник нарисован т
										ак, что точки верхнего левого и пр
										авого нижнего углов поменялись рол
										ями, то исправить положение на вре
										мя выполнения проверки */
	if (x > x_right)
	{
        _x = x_right;
		_x_right = x;
	}

	if (y > y_bottom)
	{
		_y = y_bottom;
		_y_bottom = y;
	}

                                        /*! так как попадание в прямоуголь
										ник легко проверяется геометрическ
										им методом, не выполняем графическ
										ой проверки, а проверяем попадание
										сравнением координат */
	return ((X >= _x - cont / 2) &&
		(X <= _x_right + cont / 2) &&
		(Y >= _y - cont / 2) &&
		(Y <= _y_bottom + cont / 2));
}

void CRectangle::Translate(int dx, int dy)
{
	x += dx; y += dy;
	x_right += dx; y_bottom += dy;
}