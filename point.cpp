/*! Time-stamp: <@(#)point.cpp   20.12.2005 - 20.12.2005 17:46:14   Dmitry Chuchva>
 *********************************************************************
 *  @file   : point.cpp
 *
 *  Project : graphics
 *
 *  Package : classes package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : definition of CPoint class
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

CPoint::CPoint()
{
	CObject::CObject();
	type = O_POINT;
}

CPoint::CPoint(int X, int Y)
{
	CObject::CObject();
	x = X; y = Y;
	type = O_POINT;
	SetContour(1,RGB(0,0,0));
	SetFillColor(RGB(0,0,0));
}

void CPoint::Draw(HDC hdc)
{
	if (Color() != FillColor())
		SetFillColor(Color());
	int cont = GetContourWidth();
	SetupColors(hdc);
                                        /*! если толщина контура единичная
										 - рисуем пиксель, иначе окружност
										 ь диаметра толщины контура */
	if (cont <= 1)
		SetPixel(hdc,x,y,Color());
	else
		Ellipse(hdc,x - cont / 2,y - cont / 2,x + cont / 2,y + cont / 2);
}

bool CPoint::Hit(HDC hdc,int X,int Y)
{
	int cont = GetContourWidth();
                                        /*! если толщина контура единичная
										- просто сравниваем координаты */
	if (cont == 1)
	{
		if ((X == x) && (Y == y))
			return true;
		else
			return false;
	}
	
                                        /*! иначе делаем графическую прове
										рку */
	ClearDevice(hdc);
	SetupColors(hdc);
	Ellipse(hdc,x - cont / 2,y - cont / 2,x + cont / 2,y + cont / 2);
                                        /*! графическая проверка - проверк
										а на совпадение цвета точки курсор
										а и цвета фигуры */
	if (GetPixel(hdc,X,Y) == Color())
		return true;
	else
		return false;
}

void CPoint::Translate(int dx, int dy)
{
	x += dx; y += dy;
}