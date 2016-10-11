/*! Time-stamp: <@(#)circle.cpp   20.12.2005 - 20.12.2005 18:00:08   Dmitry Chuchva>
 *********************************************************************
 *  @file   : circle.cpp
 *
 *  Project : graphics
 *
 *  Package : classes package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : definition of CCircle class
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

CCircle::CCircle()
{
	radius = 0;
    CObject::CObject();
	type = O_CIRCLE;
}

CCircle::CCircle(int X, int Y, int Radius)
{
	CObject::CObject();
    x = X; y = Y; radius = Radius;
	type = O_CIRCLE;
	SetContour(1,RGB(0,0,0));
	SetFillColor(RGB(255,255,255));
}

void CCircle::Draw(HDC hdc)
{
	SetupColors(hdc);
	Ellipse(hdc,x,y,x + radius,y + radius);
}

bool CCircle::Hit(HDC hdc,int X,int Y)
{
	ClearDevice(hdc);
                                        /*! так как окружность - единствен
										ная фигура, для которой выпоняется
										графическая проверка и у которой м
										огут быть различные цвета заливки 
										и контура, то необходимо утсановит
										ь на время выпонения проверки како
										й-либо один цвет для однознчности 
										определения попадания, например цв
										ет контура */
	COLORREF temp = SetFillColor(Color());
	SetupColors(hdc);
	Ellipse(hdc,x,y,x + radius,y + radius);
	if (GetPixel(hdc,X,Y) == Color())
	{
                                        /*! возвращаем цвет заливки на мес
										то */
		SetFillColor(temp);
		return true;
	}
	else
	{
		SetFillColor(temp);
		return false;
	}
}

void CCircle::Translate(int dx, int dy)
{
	x += dx; y += dy;
}