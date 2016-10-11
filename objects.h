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

                              /*! тип объекта (точка, лини€, ...)         */
typedef enum {O_OBJECT, O_POINT, O_LINE, O_RECTANGLE, O_CIRCLE} obj_type;

/*!
 * @class  : CObject
 * @parent : none
 *
 * @brief: основной абстрактный объект геометрической фигуры графического редактора
 *         родитель всех специализированных объектов
 */

class CObject
{
public:
                              /*! главные координаты объекта              */
	int x,y;
                              /*! тип объекта                             */
	obj_type type;
		    
                              /*! конструктор по умолчанию                */
	CObject();
                              /*! конструктор, получающий гл. координаты  */
	CObject(int X,int Y);
                              /*! деструктор (наследуетс€)                */
	~CObject();
                              /*! метод, реализующий отрисовку объекта    */
	virtual void Draw(HDC hdc) = 0;
                              /*! метод, определ€ющий попадание в объект  */
	virtual bool Hit(HDC hdc,int X,int Y) = 0;
                              /*! метод, сдвигающий фигуру на заданные сме
							  щени€ */
	virtual void Translate(int dx, int dy) = 0;
                              /*! установка толщины и цвета контура       */
	COLORREF SetContour(int width, COLORREF Color);
                              /*! устновка цвета заливки дл€ фигуры       */
	COLORREF SetFillColor(COLORREF FColor);
                              /*! возвращает текущую толщину контура      */
	int GetContourWidth();
                              /*! возвращает текущий цвет контура         */
	COLORREF Color();
                              /*! возвращает текущий цвет заливки         */
	COLORREF FillColor();
                              /*! устанавливает цветовые настройки фигуры 
							  на переданном контексте устройства GDI */
	void SetupColors(HDC hdc);
private:
                              /*! толщина контура объекта                 */
	int contour;
                              /*! цвет контура и заливки                  */
	COLORREF color,fill;
                              /*! объекты GDI, используемые при рисовании 
							  фигуры на устройстве */
	HBRUSH hBrush;
	HPEN hPen;
};

/*!
 * @class  : CPoint
 * @parent : public CObject
 *
 * @brief: описывает свойства и методы геом. фигуры "точка"
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
 * @brief: описывает свойства и методы геом. фигуры "лини€"
 *         
 */

class CLine : public CObject
{
public:
                              /*! координаты конца линии                  */
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
 * @brief: описывает свойства и методы геом. фигуры "пр€моугольник"
 *         
 */

class CRectangle : public CObject
{
public:
                              /*! координаты правого нижнего угла пр€моуго
							  льника */
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
 * @brief: описывает свойства и методы геом. фигуры "окружность"
 *         
 */

class CCircle : public CObject
{
public:
                              /*! радиус окружности                       */
	int radius;

	CCircle();
	CCircle(int X,int Y,int Radius);
	void Draw(HDC hdc);
	bool Hit(HDC hdc,int X,int Y);
	void Translate(int dx, int dy);
};

#endif