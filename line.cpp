/*! Time-stamp: <@(#)line.cpp   20.12.2005 - 20.12.2005 17:51:06   Dmitry Chuchva>
 *********************************************************************
 *  @file   : line.cpp
 *
 *  Project : graphics
 *
 *  Package : classes package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : definition of CLine class
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

CLine::CLine()
{
	x_end = y_end = 0;
	CObject::CObject();
	type = O_LINE;
}

CLine::CLine(int Xstart,int Ystart,int Xend,int Yend)
{
    CObject::CObject();
	type = O_LINE;
	x = Xstart; y = Ystart; x_end = Xend; y_end = Yend;
	SetContour(1,RGB(0,0,0));
}

void CLine::Draw(HDC hdc)
{
	SetupColors(hdc);
	MoveToEx(hdc,x,y,NULL);
	LineTo(hdc,x_end,y_end);
}

bool CLine::Hit(HDC hdc,int X,int Y)
{
	ClearDevice(hdc);
	SetupColors(hdc);
	MoveToEx(hdc,x,y,NULL);
	LineTo(hdc,x_end,y_end);
                                        /*! графическая проверка          */
	if (GetPixel(hdc,X,Y) == Color())
		return true;
	else
		return false;
}

void CLine::Translate(int dx, int dy)
{
	x += dx; y += dy;
	x_end += dx; y_end += dy;
}