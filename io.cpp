/*! Time-stamp: <@(#)io.cpp   20.12.2005 - 20.12.2005 18:01:28   Dmitry Chuchva>
 *********************************************************************
 *  @file   : io.cpp
 *
 *  Project : graphics
 *
 *  Package : IO package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : Function for loading files of varios formats. Add here 
 *			  support for your format
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  20.12.2005  BN : First Revision
 *
 *********************************************************************
 */


#include "io.h"
#include <stdio.h>
#include "graphics.h"


/*!
 * @function : LoadRaw
 * @brief    : загружает бинарный файл рисунка собственного формата редактора
 *
 * @param char *filename        : имя файла 
 * @param vector<CObject*> &obj : куда загрузить
 *
 * @return int : 1 - успешно, 0 - неудача
 */

int LoadRaw(char *filename,vector<CObject*> &obj)
{
	FILE *f = fopen(filename,"rb");
	if (!f)
        return 0;

	unsigned int count;
	size_t read = 0;

	DeleteObjects(obj);
	read = fread(&count,sizeof(count),1,f);
    for (unsigned int i = 0; i < count; i++)
	{
		obj_type type;
		COLORREF color,fill;
        int contour, x, y;

		read = fread(&type,sizeof(type),1,f);

		read = fread(&x,sizeof(int),1,f);
		read = fread(&y,sizeof(int),1,f);
		read = fread(&color,sizeof(COLORREF),1,f);
		read = fread(&fill,sizeof(COLORREF),1,f);
		read = fread(&contour,sizeof(int),1,f);

		if (!read)
		{
			fclose(f);
			DeleteObjects(obj);
			return 0;
		}

		switch (type)
		{
		case O_POINT:
			{
                CPoint *point = new CPoint(x,y);

                point -> SetContour(contour,color);
				point -> SetFillColor(fill);
				point -> type = type;
				obj.push_back(point);
			}
			break;
		case O_LINE:
			{
				CLine *line = new CLine;

				read = fread(&(line -> x_end),sizeof(int),1,f);
				read = fread(&(line -> y_end),sizeof(int),1,f);
                line -> x = x;
				line -> y = y;
				line -> SetContour(contour,color);
				line -> SetFillColor(fill);
				line -> type = type;
				obj.push_back(line);
			}
			break;
		case O_RECTANGLE:
			{
				CRectangle *rect = new CRectangle;

                read = fread(&(rect -> x_right),sizeof(int),1,f);
				read = fread(&(rect -> y_bottom),sizeof(int),1,f);
				rect -> x = x;
				rect -> y = y;
				rect -> SetContour(contour,color);
				rect -> SetFillColor(fill);
				rect -> type = type;
				obj.push_back(rect);
			}
			break;
		case O_CIRCLE:
			{
				CCircle *circ = new CCircle;

				read = fread(&(circ -> radius),sizeof(int),1,f);
				circ -> x = x;
				circ -> y = y;
				circ -> SetContour(contour,color);
				circ -> SetFillColor(fill);
				circ -> type = type;
				obj.push_back(circ);
			}
			break;
		}
		if (!read)
		{
			fclose(f);
			DeleteObjects(obj);
			return 0;
		}
	}
	fclose(f);
	return 1;
}


/*!
 * @function : SaveRaw
 * @brief    : сохраняет рисунок в собственном формате редактора
 *
 * @param char *filename        : имя файла
 * @param vector<CObject*> &obj : что сохранять
 *
 * @return int : 1 - успешно, 0 - неудача
 */

int SaveRaw(char *filename,vector<CObject*> &obj)
{
	FILE *f = fopen(filename,"wb");
	if (!f)
        return 0;

	unsigned int count = (unsigned int)obj.size();

	fwrite(&count,sizeof(count),1,f);
	for (unsigned int i = 0; i < count; i++)
	{
		obj_type type = obj[i] -> type;
		COLORREF color = obj[i] -> Color(), fill = obj[i] -> FillColor();
        int contour = obj[i] -> GetContourWidth();

		fwrite(&type,sizeof(type),1,f);

		fwrite(&(obj[i] -> x),sizeof(int),1,f);
		fwrite(&(obj[i] -> y),sizeof(int),1,f);
		fwrite(&color,sizeof(COLORREF),1,f);
		fwrite(&fill,sizeof(COLORREF),1,f);
		fwrite(&contour,sizeof(int),1,f);

		switch (type)
		{
		case O_LINE:
			{
				CLine *line = reinterpret_cast<CLine*>(obj[i]);

				fwrite(&(line -> x_end),sizeof(int),1,f);
				fwrite(&(line -> y_end),sizeof(int),1,f);
			}
			break;
		case O_RECTANGLE:
			{
				CRectangle *rect = reinterpret_cast<CRectangle*>(obj[i]);

                fwrite(&(rect -> x_right),sizeof(int),1,f);
				fwrite(&(rect -> y_bottom),sizeof(int),1,f);
			}
			break;
		case O_CIRCLE:
			{
				CCircle *circ = reinterpret_cast<CCircle*>(obj[i]);

				fwrite(&(circ -> radius),sizeof(int),1,f);
			}
			break;
		}
	}
	fclose(f);
	return 1;
}
