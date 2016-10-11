/*! Time-stamp: <@(#)object.cpp   20.12.2005 - 20.12.2005 17:43:28   Dmitry Chuchva>
 *********************************************************************
 *  @file   : object.cpp
 *
 *  Project : graphics
 *
 *  Package : classes package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : definition of CObject class
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

CObject::CObject()
{
    x = y = contour = color = fill = 0;
	hBrush = NULL;
	hPen = NULL;
	type = O_OBJECT;
}

CObject::CObject(int X,int Y)
{
	x = X; y = Y; hPen = NULL; hBrush = NULL;
	type = O_OBJECT;
	SetContour(1,RGB(0,0,0));
	SetFillColor(RGB(255,255,255));
}

CObject::~CObject()
{
	if (hPen)
	{
		DeleteObject(hPen);
		hPen = NULL;
	}
	if (hBrush)
	{
		DeleteObject(hBrush);
		hBrush = NULL;
	}
}

COLORREF CObject::SetContour(int width, COLORREF Color)
{
	COLORREF old = color;
	color = Color; contour = width;
	if (contour < 1)
		contour = 1;
	if (hPen)
        DeleteObject(hPen);
	hPen = CreatePen(PS_SOLID,contour,color);
	return old;
}

COLORREF CObject::SetFillColor(COLORREF FColor)
{
	COLORREF old = fill;
	fill = FColor;
	if (hBrush)
		DeleteObject(hBrush);

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = fill;
	hBrush = CreateBrushIndirect(&lb);
	return old;
}

int CObject::GetContourWidth()
{
	return contour;
}

COLORREF CObject::Color()
{
	return color;
}

COLORREF CObject::FillColor()
{
	return fill;
}

void CObject::SetupColors(HDC hdc)
{
	SelectObject(hdc,hPen);
	SelectObject(hdc,hBrush);
}