/*! Time-stamp: <@(#)objects.h   20.12.2005 - 20.12.2005 17:41:49   Dmitry Chuchva>
 *********************************************************************
 *  @file   : objects.h
 *
 *  Project : graphics
 *
 *  Package : classes package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : base classes declaration
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  20.12.2005  BN : First Revision
 *
 *********************************************************************
 */


#ifndef __OBJECTS_H
#define __OBJECTS_H

#include <windows.h>

void ClearDevice(HDC hdc);

                              /*! ��� ������� (�����, �����, ...)         */
typedef enum {O_OBJECT, O_POINT, O_LINE, O_RECTANGLE, O_CIRCLE} obj_type;

/*!
 * @class  : CObject
 * @parent : none
 *
 * @brief: �������� ����������� ������ �������������� ������ ������������ ���������
 *         �������� ���� ������������������ ��������
 */

class CObject
{
public:
                              /*! ������� ���������� �������              */
	int x,y;
                              /*! ��� �������                             */
	obj_type type;
		    
                              /*! ����������� �� ���������                */
	CObject();
                              /*! �����������, ���������� ��. ����������  */
	CObject(int X,int Y);
                              /*! ���������� (�����������)                */
	~CObject();
                              /*! �����, ����������� ��������� �������    */
	virtual void Draw(HDC hdc) = 0;
                              /*! �����, ������������ ��������� � ������  */
	virtual bool Hit(HDC hdc,int X,int Y) = 0;
                              /*! �����, ���������� ������ �� �������� ���
							  ����� */
	virtual void Translate(int dx, int dy) = 0;
                              /*! ��������� ������� � ����� �������       */
	COLORREF SetContour(int width, COLORREF Color);
                              /*! �������� ����� ������� ��� ������       */
	COLORREF SetFillColor(COLORREF FColor);
                              /*! ���������� ������� ������� �������      */
	int GetContourWidth();
                              /*! ���������� ������� ���� �������         */
	COLORREF Color();
                              /*! ���������� ������� ���� �������         */
	COLORREF FillColor();
                              /*! ������������� �������� ��������� ������ 
							  �� ���������� ��������� ���������� GDI */
	void SetupColors(HDC hdc);
private:
                              /*! ������� ������� �������                 */
	int contour;
                              /*! ���� ������� � �������                  */
	COLORREF color,fill;
                              /*! ������� GDI, ������������ ��� ��������� 
							  ������ �� ���������� */
	HBRUSH hBrush;
	HPEN hPen;
};

/*!
 * @class  : CPoint
 * @parent : public CObject
 *
 * @brief: ��������� �������� � ������ ����. ������ "�����"
 *         
 */

class CPoint : public CObject
{
public:
	CPoint();
	CPoint(int X,int Y);
	void Draw(HDC hdc);
	bool Hit(HDC hdc,int X,int Y);
	void Translate(int dx, int dy);
};

/*!
 * @class  : CLine
 * @parent : public CObject
 *
 * @brief: ��������� �������� � ������ ����. ������ "�����"
 *         
 */

class CLine : public CObject
{
public:
                              /*! ���������� ����� �����                  */
	int x_end, y_end;

	CLine();
	CLine(int Xstart,int Ystart,int Xend,int Yend);
	void Draw(HDC hdc);
	bool Hit(HDC hdc,int X,int Y);
	void Translate(int dx, int dy);
};

/*!
 * @class  : CRectangle
 * @parent : public CObject
 *
 * @brief: ��������� �������� � ������ ����. ������ "�������������"
 *         
 */

class CRectangle : public CObject
{
public:
                              /*! ���������� ������� ������� ���� ��������
							  ������ */
	int x_right, y_bottom;

	CRectangle();
	CRectangle(int Xleft,int Ytop,int Xright,int Ybottom);
	void Draw(HDC hdc);
	bool Hit(HDC hdc,int X,int Y);
	void Translate(int dx, int dy);
};

/*!
 * @class  : CCircle
 * @parent : public CObject
 *
 * @brief: ��������� �������� � ������ ����. ������ "����������"
 *         
 */

class CCircle : public CObject
{
public:
                              /*! ������ ����������                       */
	int radius;

	CCircle();
	CCircle(int X,int Y,int Radius);
	void Draw(HDC hdc);
	bool Hit(HDC hdc,int X,int Y);
	void Translate(int dx, int dy);
};

#endif