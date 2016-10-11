/*! Time-stamp: <@(#)graphics.cpp   19.12.2005 - 19.12.2005 16:27:25   Dmitry Chuchva>
 *********************************************************************
 *  @file   : graphics.cpp
 *
 *  Project : graphics
 *
 *  Package : without packages
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 19.12.2005
 *
 *  Purpose : user interface implementation functions (WIN32API)
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  19.12.2005  BN : First Revision
 *
 *********************************************************************
 */

                                        /*! ����������� ������������ �����
										� */
#include "graphics.h"
                                        /*! ����������� ��������          */
                                        /*! ����� ������ �� �����.        */
#define MAX_LOADSTRING 500
                                        /*! � ���������� �����������(1/0) */
#define DOUBLEBUFFER 1
                                        /*! ��������� ������� ������� �� �
										�� */
#define INCRSTEP 1
                                        /*! ������ �������                */
#define MARKSIZE 6
                                        /*! ������ �������, ��� ����������
										� ������� ������� ���������� �����
										��������� �������������� */
#define MOVE_AREA 10
                                        /*! ���������� ������� ������� ���
										�������������� �������������� */
#define INCR_WND 20
                                        /*! ��� ������ �����              */
#define DEFFILENAME "UNNAMED"
                                        /*! ��������� ���� �� �����.      */
#define DEFWNDTEXT "Graphics - UNNAMED"

                                        /*! ���������� ����������         */
                              /*! ��������� ���������� ���������          */
HINSTANCE hInst;
                              /*! �������� ���������� (�� ��������)       */
TCHAR szTitle[MAX_LOADSTRING];
                              /*! ����� ���� (�� ��������)                */
TCHAR szWindowClass[MAX_LOADSTRING];
                              /*! ��������� ������������ ����             */
HMENU hPopup = NULL;
                              /*! ������ �������� �������                 */
vector<CObject*> objects;
                              /*! ��� ��������� ����������� ������        */
int next_drawing = IDM_DRAW_POINT;
                              /*! ������, ���� ���������� �������������� �
							  �� ��������� �������� ������� */
bool sizing = false;
                              /*! ��������� �� ������� ������             */
CObject *current;
                              /*! ������, ������ ������� �������          */
int wwidth,wheight;
                              /*! ��������� ���������� ������� (����, ����
							  ���, ������� �������) */
COLORREF last_color = RGB(0,0,0),last_contour = 1,last_fill = RGB(255,255,255);
                              /*! ��������� ������ �������                */
int marker = 0;
                              /*! ������������ � ������� ����������       */
bool passed_save_dialog = false;
                              /*! ������� ��� �����                       */
char filename[MAX_LOADSTRING] = DEFFILENAME;
                              /*! ������� ��������� ����                  */
char wndtext[MAX_LOADSTRING] = DEFWNDTEXT;
                              /*! ���� �����������, ������� ����������    */
int modified = 0; int count_saves = 0;
                              /*! ��� ��������� ������� EMF               */
HENHMETAFILE hEnhMeta = NULL;
RECT rcMeta;
                              /*! �����, ������������ ������� ��������� ��
							  ����� */
int x_orig = 0, y_orig = 0;
                              /*! ������ �������                          */
RECT irc;

                                        /*! ���������, ��������������     */
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void UpdateScrolls(HWND hWnd,bool update_irc);
int HitMarker(CObject *obj,int x, int y);
CObject *GlobalHitTest(HDC hdc,int x,int y);
bool Near(int x,int y,int xtest, int ytest);
void DrawMarkers(HDC hdc,CObject *obj);
void Mark(HDC hdc,int x,int y);
void MarkMain(HDC hdc, int x, int y);
int LoadEmf(HWND hWnd,char *filename,vector<CObject*> &obj);
int SaveEmf(HWND hWnd,char *filename,vector<CObject*> &obj);
void DrawObjects(HDC hdc, bool with_markers);
void GetImageRect(RECT &rc);
void ClearDevice(HDC hdc);


/*!
 * @function : _tWinMain
 * @brief    : ����� ����� � windows-����������
 *
 * @param HINSTANCE hInstance                            : ��������� ���������� ����������
 * @param HINSTANCE hPrevInstance                        : �� �������.
 * @param LPTSTR    lpCmdLine                            : �������, ����. ����������
 * @param int       nCmdShow                             : ��������� �� ��, ��� �������� ���� 
 *
 * @return int APIENTRY : ��� �������� ���������, 0 - ���������� �����
 */

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

                                        /*! ������������� ���������� �����*/
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GRAPHICS, szWindowClass, MAX_LOADSTRING);
	
                                        /*! ����������� ������ ����       */
	MyRegisterClass(hInstance);

                                        /*! ������������� ����������      */
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

                                        /*! �������� ������������ ����    */
	hPopup = CreatePopupMenu();
	AppendMenu(hPopup,MF_ENABLED | MF_STRING | MF_UNCHECKED,IDM_BRINGTOFRONT,"Bring to Front");
	AppendMenu(hPopup,MF_ENABLED | MF_STRING | MF_UNCHECKED,IDM_SENDTOBACK,"Send to Back");
	
                                        /*! �������� "������� ������"     */
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GRAPHICS);

                                        /*! �������� ����                 */
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

                                        /*! ����������� ����              */
	if (hPopup)
	{
		DestroyMenu(hPopup);
		hPopup = NULL;
	}

	return (int) msg.wParam;
}


/*!
 * @function : MyRegisterClass
 * @brief    : ����������� ������ ���� ����������
 *
 * @param HINSTANCE hInstance : ��� ������ ����������
 *
 * @return ATOM : ��������� ����������� (���� 0 - ������)
 */

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GRAPHICS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_GRAPHICS;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


/*!
 * @function : InitInstance
 * @brief    : ������������� ���� ����������
 *
 * @param HINSTANCE hInstance  : ��������� ����������
 * @param int nCmdShow         : ��� �������� ����
 *
 * @return BOOL : ��������� ������������� (�� / ���)
 */

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


/*!
 * @function : WndProc
 * @brief    : ������������ ��������� ���� ����������
 *
 * @param HWND hWnd      : ����� ����
 * @param UINT message   : ����� ���������
 * @param WPARAM wParam  : ������ �������� ���������
 * @param LPARAM lParam  : ������ �������� ���������
 *
 * @return LRESULT CALLBACK : 0 - ���� ��������� ����������
 */

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message) 
	{
	case WM_CREATE:
		SetWindowText(hWnd,wndtext);
		UpdateScrolls(hWnd,true);
		break;
	case WM_SIZE:
		wwidth = LOWORD(lParam);
		wheight = HIWORD(lParam);
                                        /*! ���� ������� ������� ���������
										� ������� ��� ����������� �������,
										���������� ����� ��������� � 0 */
		if (wwidth > irc.right)
			x_orig = 0;
		if (wheight > irc.bottom)
			y_orig = 0;
        UpdateScrolls(hWnd,true);
		break;
                                        /*! ��������� ����, ������������� */
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		switch (wmId)
		{
		case IDM_ABOUT:
                                        /*! ������� "About..."            */
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			SendMessage(hWnd,WM_CLOSE,0,0);
			break;
		case IDM_FILE_NEW:
			{
                                        /*! ���� ���� ��� �������, �������
										��� ��������� ��������� ����� ����
										����� ������ ����� */
				if (modified)
				{
					switch (MessageBox(hWnd,"File not saved. Do you wish to save?","Confirm",MB_ICONQUESTION | MB_YESNOCANCEL))
					{
					case IDYES:
						SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(IDM_FILE_SAVE,0),0);
						break;
					case IDNO:
						break;
					case IDCANCEL:
						return 0;
					}
				}
                                        /*! ������� ������� ������� ������
										� */
				DeleteObjects(objects);
                InvalidateRect(hWnd,NULL,false);
                                        /*! ������������� ������ �������  */
				modified = 0; count_saves = 0;
				last_contour = last_color = 0;
				last_fill = RGB(255,255,255);
				sprintf(wndtext,"%s",DEFWNDTEXT);
				SetWindowText(hWnd,wndtext);
				sprintf(filename,"%s",DEFFILENAME);
				if (hEnhMeta)
				{
					DeleteEnhMetaFile(hEnhMeta);
					hEnhMeta = NULL;
				}
				x_orig = y_orig = 0;
				UpdateScrolls(hWnd,true);
			}
			break;
		case IDM_FILE_OPEN:
			{
										/*! ���� ���� ��� �������, �������
										��� ��������� ��������� ����� ����
										����� ����� */
                if (modified)
				{
					switch (MessageBox(hWnd,"File not saved. Do you wish to save?","Confirm",MB_ICONQUESTION | MB_YESNOCANCEL))
					{
					case IDYES:
						SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(IDM_FILE_SAVE,0),0);
						if (passed_save_dialog)
						{
							passed_save_dialog = false;
							return 0;
						}
						break;
					case IDNO:
						break;
					case IDCANCEL:
						return 0;
					}
				}

				OPENFILENAME ofn;

                                        /*! �������������� ��������� ��� �
										������ ������ ����� ��� �������� */
				memset(&ofn,0,sizeof(ofn));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = "RAW images\0*.raw\0EMF images\0*.emf\0\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFile = filename;
				ofn.nMaxFile = MAX_LOADSTRING;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

                                        /*! ���� ������������ ���-�� �����
										� */
				if (GetOpenFileName(&ofn))
				{
					int res = 0;

					if (hEnhMeta)
					{
						DeleteEnhMetaFile(hEnhMeta);
						hEnhMeta = NULL;
					}
                                        /*! ����������� ���������         */
					if (strstr(ofn.lpstrFile,".raw"))
						res = LoadRaw(ofn.lpstrFile,objects);
					else
						if (strstr(ofn.lpstrFile,".emf"))
							res = LoadEmf(hWnd,ofn.lpstrFile,objects);
                    if (!res)
					{
                                        /*! � ������ �������, ��������    */
						MessageBox(hWnd,"Error while loading file","Error",MB_OK | MB_ICONSTOP);
                        sprintf(filename,"%s",DEFFILENAME);
						sprintf(wndtext,"%s",DEFWNDTEXT);
						SetWindowText(hWnd,wndtext);
					}
					else
					{
                                        /*! ����������� ����� �������     */
						sprintf(filename,"%s",ofn.lpstrFile);
						sprintf(wndtext,"%s - %s",szTitle,ofn.lpstrFile + ofn.nFileOffset);
						SetWindowText(hWnd,wndtext);
						modified = 0; count_saves = 0;
						last_contour = last_color = 0;
						last_fill = RGB(255,255,255);
						InvalidateRect(hWnd,NULL,false);
						x_orig = y_orig = 0;
						UpdateScrolls(hWnd,true);
					}
				}
			}
			break;
		case IDM_FILE_SAVE:
							            /*! ���� ���� ���������� �����    */
			if ((strcmp(filename,DEFFILENAME) == 0) && (!count_saves))
			{
                                        /*! ����������� Save as...        */
				SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(IDM_FILE_SAVEAS,0),0);
				break;
			}
			else
			{
				int res = 0;
                                        /*! ����� ������ ���������        */
				if (strstr(filename,".raw"))
					res = SaveRaw(filename,objects);
				else
                    if (strstr(filename,".emf"))
						res = SaveEmf(hWnd,filename,objects);
				if (!res)
					MessageBox(hWnd,"Error while saving file","Error",MB_OK | MB_ICONSTOP);
				else
				{
					modified = 0;
					count_saves++;
				}
			}
			break;
		case IDM_FILE_SAVEAS:
			{
				OPENFILENAME ofn;
                                        /*! ������� ������ ����������     */
				memset(&ofn,0,sizeof(ofn));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = "RAW images\0*.raw\0EMF images\0*.emf\0\0";
				ofn.nFilterIndex = 1;
				if (strstr(filename,"."))
					memset(strstr(filename,"."),0,strlen(filename) - (UINT)(strstr(filename,".") - filename) + 1);
				ofn.lpstrFile = filename;
				ofn.nMaxFile = MAX_LOADSTRING;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

                                        /*! �������� ������               */
				if (GetSaveFileName(&ofn))
				{
					passed_save_dialog = false;
					if (!strstr(ofn.lpstrFile,"."))
					{
						if (ofn.nFilterIndex == 1)
							strcat(ofn.lpstrFile,".raw");
						else
							strcat(ofn.lpstrFile,".emf");

					}
					                    /*! � ������ ������ ���������     */
                    int res = 0;
					if (strstr(filename,".raw"))
						res = SaveRaw(filename,objects);
					else
						if (strstr(filename,".emf"))
							res = SaveEmf(hWnd,filename,objects);
					if (!res)
                                        /*! ����� �������� �� ������      */
						MessageBox(hWnd,"Error while saving file","Error",MB_OK | MB_ICONSTOP);
					else
					{
						modified = 0;
						count_saves++;
						sprintf(filename,"%s",ofn.lpstrFile);
						sprintf(wndtext,"%s - %s",szTitle,ofn.lpstrFile + ofn.nFileOffset);
						SetWindowText(hWnd,wndtext);
					}					
				}
				else
					passed_save_dialog = true;
			}
			break;
                                        /*! ��������� �������� ������� ��
										������ ���� */
		case IDM_SENDTOBACK:
			if (!current)
				break;
			for (i = 0; i < (int)objects.size(); i++)
				if (objects[i] == current)
				{
					objects.erase(objects.begin() + i);
					break;
				}
			objects.insert(objects.begin(),current);
			modified = 1;
			InvalidateRect(hWnd,NULL,false);
			break;
                                        /*! ��������� �������� ������� �� 
										�������� ���� */
		case IDM_BRINGTOFRONT:
			if (!current)
				break;
			for (i = 0; i < (int)objects.size(); i++)
				if (objects[i] == current)
				{
					objects.erase(objects.begin() + i);
					break;
				}
			objects.insert(objects.end(),current);
			modified = 1;
			InvalidateRect(hWnd,NULL,false);
			break;
                                        /*! ���������� ����� ������ ��� ��
										������� */
		case IDM_DRAW_POINT:
		case IDM_DRAW_LINE:
		case IDM_DRAW_RECTANGLE:
		case IDM_DRAW_CIRCLE:
			next_drawing = wmId;
			break;
                                        /*! ����� ����� ������� � ������� */
		case IDM_OPTIONS_CONTOURCOLOR:
		case IDM_OPTIONS_FILLCOLOR:
			{
				if (!current)
					break;

				COLORREF color = (wmId == IDM_OPTIONS_CONTOURCOLOR ? current -> Color() :
																	current -> FillColor());
				CHOOSECOLOR chclr;
				static COLORREF initbclr[16];

                                        /*! ������� ������ ������ �����   */
				for (int i = 0; i < 16; i++)
					initbclr[i] = RGB(255,255,255);
				memset(&chclr,0,sizeof(CHOOSECOLOR));
				initbclr[0] = color;
				chclr.lStructSize = sizeof(CHOOSECOLOR);
				chclr.hwndOwner = hWnd;
				chclr.lpCustColors = initbclr;
                                        /*! �������� ���                  */
				if (ChooseColor(&chclr))
				{
                                        /*! ������������� ����    */
					if (wmId == IDM_OPTIONS_CONTOURCOLOR)
					{
						current -> SetContour(current -> GetContourWidth(),chclr.rgbResult);
						last_color = chclr.rgbResult;
					}
					else
					{
                        current -> SetFillColor(chclr.rgbResult);
						last_fill = chclr.rgbResult;
					}
					modified = 1;
                                        /*! ������������                  */
					InvalidateRect(hWnd,NULL,false);
				}
			}
			break;
                                        /*! ���������� ������� ������� ���
										����� ������� */
		case IDM_OPTIONS_INCCONTOURWIDTH:
			if (!current)
				break;
                                        /*! ��������� �������� ��� �������
										��� ������� */
			last_contour = current -> GetContourWidth() + INCRSTEP;
			current -> SetContour(last_contour,current -> Color());
			modified = 1;
			InvalidateRect(hWnd,NULL,false);
			break;
                                        /*! ������� ������� ������� ���
										����� ������� */
		case IDM_OPTIONS_DECCONTOURWIDTH:
			if (!current)
				break;
                                        /*! ��������� ��������            */
			last_contour = current -> GetContourWidth() - INCRSTEP;
			current -> SetContour(last_contour,current -> Color());
			modified = 1;
			InvalidateRect(hWnd,NULL,false);
			break;
		default:
                                        /*! ��������� �� ���������        */
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
                                        /*! ����������                    */
	case WM_PAINT:
		{
			RECT rc;
			GetClientRect(hWnd,&rc);
                                        /*! �������� ���������... :)      */
			hdc = BeginPaint(hWnd, &ps);
#if (DOUBLEBUFFER)
                                        /*! � ������ � ������������ ������
										�������� ����� */
			HDC hdcmem = CreateCompatibleDC(hdc);
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc,wwidth,wheight);
			SelectObject(hdcmem,hBitmap);
			PatBlt(hdcmem,0,0,wwidth,wheight,WHITENESS);
#else
#define hdcmem hdc
#endif
                                        /*! ������ �������                */
			DrawObjects(hdcmem,true);

#if (DOUBLEBUFFER)
                                        /*! ���� �������� �����������, ���
										���� ��, ��� � ������ */
			BitBlt(hdc,0,0,wwidth,wheight,hdcmem,0,0,SRCCOPY);
			DeleteObject(hBitmap);
			DeleteDC(hdcmem);
#else
#undef hdcmem
#endif
                                        /*! ����� ���������               */
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

                                        /*! ���� �� ������ � ������       */
			if (!(marker = HitMarker(current,x + x_orig,y + y_orig)))
			{
                                        /*! �������� ����� ������         */
				current = CreateObject(next_drawing,x + x_orig,y + y_orig);
				current -> SetContour(last_contour,last_color);
				current -> SetFillColor(last_fill);
				objects.push_back(current);
				modified = 1;
                                        /*! �������� �������� �� �����, ��
										�� ��� �� ����� */
				if (next_drawing != IDM_DRAW_POINT)
					sizing = true;
				InvalidateRect(hWnd,NULL,false);
			}
			else
                                        /*! �������� �������� �������� ���
										���, �� ������� �������� ���������
										������ */
				sizing = true;
		}
		break;
                                        /*! ������ ������ ���� - ����� ���
										���� */
	case WM_RBUTTONDOWN:
		{
            if (sizing)
				break;

			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
                                        /*! ����������                    */
			HDC hdc = GetDC(hWnd);
			HDC hdcm = CreateCompatibleDC(hdc);
			HBITMAP hBitmap = CreateCompatibleBitmap(hdc,irc.right,irc.bottom);
			SelectObject(hdcm,hBitmap);

                                        /*! ���������� ��������� ������   */
			CObject *newcurr = GlobalHitTest(hdcm,x + x_orig,y + y_orig);
                                        /*! ���� �� ���-�� ������         */
			if (newcurr)
			{
                                        /*! ���� ��� ��� �� ������, �� ���
										���� ����������� ���� ��� ����� ��
										����� */
				if (current == newcurr)
				{
					POINT p;

					GetCursorPos(&p);
					TrackPopupMenu(hPopup,TPM_LEFTALIGN | TPM_LEFTBUTTON,p.x,p.y,0,hWnd,NULL);
				}
				else
				{
                                        /*! ����� ���������� ��������� �� 
										����� ������ */
					current = newcurr;
					InvalidateRect(hWnd,NULL,false);
				}
			}
			else
			{
                                        /*! ���� �� ������ � ������, �����
										��������� */
				current = NULL;
				InvalidateRect(hWnd,NULL,false);
			}
                                        /*! ��������������� ������ �������*/
			DeleteDC(hdcm);
			DeleteObject(hBitmap);
			ReleaseDC(hWnd,hdc);
		}
		break;
	case WM_LBUTTONUP:
        UpdateScrolls(hWnd,true);
                                        /*! ��������������� ����� ��������
										� ����� */
		if ((irc.right > wwidth) && (x_orig + wwidth > irc.right))
		{
			x_orig = irc.right - wwidth;
			InvalidateRect(hWnd,NULL,false);
		}
		if ((irc.bottom > wheight) && (y_orig + wheight > irc.bottom))
		{
			y_orig = irc.bottom - wheight;
			InvalidateRect(hWnd,NULL,false);
		}
		                                /*! ���������� ��������� ���������
										� ������� */
		sizing = false;
		break;
	case WM_MOUSEMOVE:
                                        /*! ���� ���������� ��������� ����
										������ ������� */
		if ((sizing) && (current))
		{
			int xnew = LOWORD(lParam) + x_orig, ynew = HIWORD(lParam) + y_orig;

                                        /*! ���� ������ � ������� ��������
										�, ������ �������������� ����� */
			if (xnew - x_orig >= wwidth - MOVE_AREA)
			{
				xnew += INCR_WND;
				x_orig += INCR_WND;
				if (xnew > irc.right)
                    irc.right = xnew;
				UpdateScrolls(hWnd,false);
			}

			if (ynew - y_orig >= wheight - MOVE_AREA)
			{
				ynew += INCR_WND;
				y_orig += INCR_WND;
				if (ynew > irc.bottom)
                    irc.bottom = ynew;
				UpdateScrolls(hWnd,false);
			}

			if ((xnew - x_orig <= MOVE_AREA) && (irc.right > wwidth))
			{
				
				xnew -= INCR_WND;
				x_orig -= INCR_WND;
//				irc.right -= INCR_WND;
				if (xnew < 0)
					xnew = 0;
				if (x_orig < 0)
					x_orig = 0;
				UpdateScrolls(hWnd,false);
			}

			if ((ynew - y_orig <= MOVE_AREA) && (irc.bottom > wheight))
			{
				
				ynew -= INCR_WND;
				y_orig -= INCR_WND;
//				irc.bottom -= INCR_WND;
				if (ynew < 0)
					ynew = 0;
				if (y_orig < 0)
					y_orig = 0;
				UpdateScrolls(hWnd,false);
			}

                                        /*! ���� ��������� �� �������     */
			if (marker)
			{
				int dx = 0, dy = 0;
                                        /*! � ������������ � ������� �����
										�� �������� ������ */
				if (marker == 1)
				{
					dx = xnew - current -> x;
					dy = ynew - current -> y;
                    current -> x = xnew;
					current -> y = ynew;
				}
                switch (current -> type)
				{
				case O_LINE:
					{
						CLine *line = reinterpret_cast<CLine*>(current);

						if (marker == 1)
						{
							line -> x_end += dx;
							line -> y_end += dy;
						}
						else
						{
                            line -> x_end = xnew;
							line -> y_end = ynew;
						}
					}
					break;
				case O_RECTANGLE:
					{
						CRectangle *rect = reinterpret_cast<CRectangle*>(current);

						if (marker == 1)
						{
                            rect -> x_right += dx;
							rect -> y_bottom += dy;
						}
						else
						{
                            rect -> x_right = xnew;
							rect -> y_bottom = ynew;
						}
					}
					break;
				case O_CIRCLE:
					{
						CCircle *circ = reinterpret_cast<CCircle*>(current);

						if (marker != 1)
                            circ -> radius = xnew - circ -> x;
					}
					break;
				}
			}
			else
			{
                                        /*! �����, � ����������� �� ���� �
										���������� ������, �������� �� */
				switch (next_drawing)
				{
				case IDM_DRAW_LINE:
					{
						CLine *line = reinterpret_cast<CLine*>(current);

						line -> x_end = xnew;
						line -> y_end = ynew;
					}
					break;
				case IDM_DRAW_RECTANGLE:
					{
						CRectangle *rect = reinterpret_cast<CRectangle*>(current);

						rect -> x_right = xnew;
						rect -> y_bottom = ynew;
					}
					break;
				case IDM_DRAW_CIRCLE:
					{
						CCircle *circ = reinterpret_cast<CCircle*>(current);

						circ -> radius = xnew - circ -> x;
					}
					break;
				}
			}
			modified = 1;
            InvalidateRect(hWnd,NULL,false);
		}
		break;
	case WM_HSCROLL:
                                        /*! ��������� ��������� ���. �����
										�� */
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			x_orig--;
			if (x_orig < 0)
				x_orig = 0;
			break;
		case SB_LINERIGHT:
			x_orig++;
			if (x_orig > irc.right - wwidth)
				x_orig = irc.right - wwidth;
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			x_orig = HIWORD(wParam);
			break;
		case SB_PAGELEFT:
            x_orig -= wwidth;
			if (x_orig < 0)
				x_orig = 0;			
			break;
		case SB_PAGERIGHT:
			x_orig += wwidth;
			if (x_orig > irc.right - wwidth)
                x_orig = irc.right - wwidth;
			break;
		}
                                        /*! ������������                  */
		InvalidateRect(hWnd,NULL,false);
                                        /*! �������� ��� ���. ����������  */
		UpdateScrolls(hWnd,true);
		break;
	case WM_VSCROLL:
                                        /*! ���. ��������� ����. �������  */
		switch (LOWORD(wParam))
		{
		case SB_LINEDOWN:
			y_orig++;
			if (y_orig > irc.bottom - wheight)
				y_orig = irc.bottom - wheight;
			break;
		case SB_LINEUP:
			y_orig--;
			if (y_orig < 0)
				y_orig = 0;
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			y_orig = HIWORD(wParam);
			break;
		case SB_PAGEUP:
			y_orig -= wheight;
			if (y_orig < 0)
				y_orig = 0;			
			break;
		case SB_PAGEDOWN:
			y_orig += wheight;
			if (y_orig > irc.bottom - wheight)
                y_orig = irc.bottom - wheight;
			break;
		}
                                        /*! ������������                  */
		InvalidateRect(hWnd,NULL,false);
                                        /*! �������� ��� ����������       */
		UpdateScrolls(hWnd,true);
		break;
	case WM_KEYDOWN:
                                        /*! ��������� ������� ������      */
		switch (wParam)
		{
                                        /*! Delete                        */
		case VK_DELETE:
                                        /*! ������� ������� ������        */
			if (current)
			{
				for (i = 0; i < (int)objects.size(); i++)
					if (objects[i] == current)
					{
						objects.erase(objects.begin() + i);
						break;
					}
				delete current;
				if (!objects.empty())
                    current = objects[objects.size() - 1];
				else
					current = NULL;
			}
			modified = 1;
			UpdateScrolls(hWnd,true);
			InvalidateRect(hWnd,NULL,false);
			break;
                                        /*! Escape                        */
		case VK_ESCAPE:
                                        /*! �����                         */
			SendMessage(hWnd,WM_CLOSE,0,0);
			break;
		}
        break;
	case WM_CLOSE:
                                        /*! ���� ���� ��� �������, �������
										��� ��������� ��������� ����� ����
										��� */
		if (modified)
		{
			switch (MessageBox(hWnd,"File not saved. Do you wish to save before exit?","Confirm exit",MB_ICONQUESTION | MB_YESNOCANCEL))
			{
			case IDYES:
				SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(IDM_FILE_SAVE,0),0);
				break;
			case IDNO:
				break;
			case IDCANCEL:
				return 0;
			}
		}
                                        /*! ���������� ����               */
		SendMessage(hWnd,WM_DESTROY,0,0);
		break;
	case WM_DESTROY:
                                        /*! ���������� �������            */
		DeleteObjects(objects);
                                        /*! ����� �� ���. �����           */
		PostQuitMessage(0);
		break;
	default:
                                        /*! ��������� �� ���������        */
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


/*!
 * @function : About
 * @brief    : ��������� ��������� "About..." �������
 *
 * @param HWND hDlg      : ����� �������
 * @param UINT message   : ���������
 * @param WPARAM wParam  : ������ �������� ���������
 * @param LPARAM lParam  : ������ �������� ��������� 
 *
 * @return LRESULT CALLBACK : ��������� ��������� (TRUE / FALSE)
 */

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}


/*!
 * @function : CreateObject
 * @brief    : ������� ����� ������ ������������� ����
 *
 * @param int next  : ��� �������
 * @param int x     : �������� �������
 * @param int y     : �������� �������
 *
 * @return CObject* : ��������� �� ��������� ������
 */

CObject *CreateObject(int next, int x, int y)
{
    CObject *object = NULL;
	switch (next)
	{
	case IDM_DRAW_POINT:
		object = new CPoint(x,y);
		break;
	case IDM_DRAW_LINE:
		object = new CLine(x,y,x,y);
		break;
	case IDM_DRAW_RECTANGLE:
		object = new CRectangle(x,y,x,y);
		break;
	case IDM_DRAW_CIRCLE:
		object = new CCircle(x,y,1);
		break;
	}
	return object;
}


/*!
 * @function : UpdateScrolls
 * @brief    : ��������� ��� �����������, ������������ ����� ���� ������ �������
 *
 * @param HWND hWnd        : ����� ����
 * @param bool update_irc  : ���������� ������������� ������ �������? (�� / ���)
 *
 * @return void : 
 */

void UpdateScrolls(HWND hWnd, bool update_irc)
{
	SCROLLINFO si;

	if (update_irc)
		GetImageRect(irc);
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
	si.nMax = irc.right;
	si.nMin = 0;
	si.nPage = wwidth;
	si.nPos = x_orig;
	SetScrollInfo(hWnd,SB_HORZ,&si,false);
	if (!EnableScrollBar(hWnd,SB_HORZ,ESB_DISABLE_BOTH))
	{
		x_orig = 0;
		InvalidateRect(hWnd,NULL,false);
	}
	else
		EnableScrollBar(hWnd,SB_HORZ,ESB_ENABLE_BOTH);

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL | SIF_DISABLENOSCROLL;
	si.nMax = irc.bottom;
	si.nMin = 0;
	si.nPage = wheight;
	si.nPos = y_orig;
	SetScrollInfo(hWnd,SB_VERT,&si,false);
	if (!EnableScrollBar(hWnd,SB_VERT,ESB_DISABLE_BOTH))
	{
		y_orig = 0;
		InvalidateRect(hWnd,NULL,false);
	}
	else
		EnableScrollBar(hWnd,SB_VERT,ESB_ENABLE_BOTH);
}


/*!
 * @function : HitMarker
 * @brief    : ���������� ��������� � ������ � ���������� ����� �������, ������� ��� ������
 *
 * @param CObject *obj  : ��� ������ ������� ���������
 * @param int x         : 
 * @param int y         : ���������� ������� �������
 *
 * @return int : ����� �������, ��� 0 - ���� �������� ���
 */

int HitMarker(CObject *obj,int x, int y)
{
	if (!obj)
		return 0;
	switch (obj -> type)
	{
	case O_POINT:
		{
			CPoint *point = reinterpret_cast<CPoint*>(obj);

			if (Near(x,y,point -> x,point -> y))
				return 1;
		}
		break;
	case O_LINE:
		{
			CLine *line = reinterpret_cast<CLine*>(obj);

			if (Near(x,y,line -> x,line -> y))
				return 1;

			if (Near(x,y,line -> x_end,line -> y_end))
				return 2;
		}
		break;
	case O_RECTANGLE:
		{
			CRectangle *rect = reinterpret_cast<CRectangle*>(obj);

			if (Near(x,y,rect -> x,rect -> y))
				return 1;

			if (Near(x,y,rect -> x_right,rect -> y_bottom))
				return 2;
		}
		break;
	case O_CIRCLE:
		{
			CCircle *circ = reinterpret_cast<CCircle*>(obj);

			if (Near(x,y,circ -> x,circ -> y))
				return 1;

			if (Near(x,y,circ -> x + circ -> radius,circ -> y + circ -> radius))
				return 2;
		}
		break;
	}

	return 0;
}


/*!
 * @function : *GlobalHitTest
 * @brief    : ��������� ��������� �������� � ������� �����
 *
 * @param HDC hdc  : �������� ��������� ��������
 * @param int x    : 
 * @param int y    : ���������� ������� �������
 *
 * @return CObject : 
 */

CObject *GlobalHitTest(HDC hdc,int x,int y)
{
	for (int i = (int)objects.size() - 1; i >= 0; i--)
		if (objects[i] -> Hit(hdc,x,y))
			return objects[i];
	return NULL;
}


/*!
 * @function : Near
 * @brief    : ���������� ��������� � �������, ������ �������� ����� MARKSIZE
 *
 * @param int x      : 
 * @param int y      : ���������� ������� �������
 * @param int xtest  : 
 * @param int ytest  : ���������� ����� ��������
 *
 * @return bool : ������ - true / ��� - false
 */

bool Near(int x,int y,int xtest, int ytest)
{
	return ((x >= xtest - MARKSIZE / 2) &&
			(x <= xtest + MARKSIZE / 2) &&
			(y >= ytest - MARKSIZE / 2) &&
			(y <= ytest + MARKSIZE / 2));
}


/*!
 * @function : DrawMarkers
 * @brief    : ������������� ������� ������� 
 *
 * @param HDC hdc      : �� ����� ��������� ���������� ��������
 * @param CObject *obj : ��� ������ �������
 *
 * @return void : 
 */

void DrawMarkers(HDC hdc,CObject *obj)
{
	if (!obj)
		return;
    MarkMain(hdc,obj -> x, obj -> y);	
	switch (obj -> type)
	{
	case O_LINE:
		{
			CLine *line = reinterpret_cast<CLine*>(obj);

            Mark(hdc,line -> x_end, line -> y_end);
		}
		break;
	case O_RECTANGLE:
		{
			CRectangle *rect = reinterpret_cast<CRectangle*>(obj);

            Mark(hdc,rect -> x_right, rect -> y_bottom);
		}
		break;
	case O_CIRCLE:
		{
			CCircle *circ = reinterpret_cast<CCircle*>(obj);

            Mark(hdc,circ -> x + circ -> radius, circ -> y + circ -> radius);
		}
		break;
	}
}


/*!
 * @function : Mark
 * @brief    : ������ ������
 *
 * @param HDC hdc  : �� ����� ��������� ���������� ��������
 * @param int x    : 
 * @param int y    : ���������� ����� ������� 
 *
 * @return void : 
 */

void Mark(HDC hdc, int x, int y)
{
	SelectObject(hdc,(HBRUSH)GetStockObject(WHITE_BRUSH));
    SelectObject(hdc,(HPEN)GetStockObject(BLACK_PEN));
	Rectangle(hdc,x - MARKSIZE / 2,y - MARKSIZE / 2,
		x + MARKSIZE / 2,y + MARKSIZE / 2);
}


/*!
 * @function : MarkMain
 * @brief    : ������ ������ ������ �����
 *
 * @param HDC hdc  : �� ����� ��������� ���������� ��������
 * @param int x    : 
 * @param int y    : ���������� ����� �������
 *
 * @return void : 
 */

void MarkMain(HDC hdc, int x, int y)
{
    SelectObject(hdc,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
    SelectObject(hdc,(HPEN)GetStockObject(BLACK_PEN));
	Rectangle(hdc,x - MARKSIZE / 2,y - MARKSIZE / 2,
		x + MARKSIZE / 2,y + MARKSIZE / 2);
}


/*!
 * @function : DeleteObjects
 * @brief    : ������� ������� �����
 *
 * @param vector<CObject*> &objects : ������ ��������� ��������
 *
 * @return void : 
 */

void DeleteObjects(vector<CObject*> &objects)
{
	int i;
	for (i = 0; i < (int)objects.size(); i++)
		if (objects[i])
		{
			delete objects[i];
			objects[i] = NULL;
		}
	objects.clear();
	if (current)
		current = NULL;
}


/*!
 * @function : SaveEmf
 * @brief    : ���������� ���������� ������� � ������ EMF
 *
 * @param HWND hWnd             : ����
 * @param char *filename        : ��� �����
 * @param vector<CObject*> &obj : ������ �������� ��� ����������
 *
 * @return int : 1 - �����, 0 - �������
 */

int SaveEmf(HWND hWnd,char *filename,vector<CObject*> &obj)
{
	if (hEnhMeta)
	{
		MessageBox(hWnd,"It's impossible to save changes to metafile. Just export from raw...","Error",MB_OK | MB_ICONINFORMATION);
		return 1;
	}
    HDC hdc = GetDC(hWnd);
	HDC hdcmeta = CreateEnhMetaFile(hdc,filename,NULL,"Graphics v.0.1a written by Dmitry Chuchva (c) 2005\0Picture\0\0");
	if (!hdcmeta)
		return 0;

	DrawObjects(hdcmeta,false);

	DeleteEnhMetaFile(CloseEnhMetaFile(hdcmeta));
        
	ReleaseDC(hWnd,hdc);
	return 1;
}


/*!
 * @function : LoadEmf
 * @brief    : ���������� �������� EMF �����
 *
 * @param HWND hWnd             : ����
 * @param char *filename        : ��� �����
 * @param vector<CObject*> &obj : ������ �������� �����
 *
 * @return int : 1 - �����, 0 - �������
 */

int LoadEmf(HWND hWnd,char *filename,vector<CObject*> &obj)
{
	hEnhMeta = GetEnhMetaFile(filename);
	DeleteObjects(objects);
	if (!hEnhMeta)
		return 0;
	ENHMETAHEADER emh;
	
	GetEnhMetaFileHeader(hEnhMeta,sizeof(ENHMETAHEADER),&emh);
	memcpy(&rcMeta,&emh.rclBounds,sizeof(RECTL));
	return 1;
}


/*!
 * @function : DrawObjects
 * @brief    : ������������� �����
 *
 * @param HDC hdc            : �������� ��������� ��� ���������
 * @param bool with_markers  : �������� �������? (�� / ���)
 *
 * @return void : 
 */

void DrawObjects(HDC hdc, bool with_markers)
{
	int i;

	if (hEnhMeta)
		PlayEnhMetaFile(hdc,hEnhMeta,&rcMeta);
	for (i = 0; i < (int)objects.size(); i++)
	{
		if (objects[i])
		{
			objects[i] -> Translate(-x_orig,-y_orig);
			if (objects[i] == current)
				if (with_markers)
                    DrawMarkers(hdc,current);
			objects[i] -> Draw(hdc);
            objects[i] -> Translate(x_orig,y_orig);
		}
	}
}


/*!
 * @function : GetImageRect
 * @brief    : ������������ ������ �������
 *
 * @param RECT &rc : ������������ ������ �������
 *
 * @return void : 
 */

void GetImageRect(RECT &rc)
{
	rc.left = 0;	rc.top = 0;
	rc.right = 1;	rc.bottom = 1;
	for (int i = 0; i < (int)objects.size(); i++)
	{
		switch (objects[i] -> type)
		{
		case O_POINT:
			{
				CPoint *point = reinterpret_cast<CPoint*>(objects[i]);
				if (point -> x > rc.right)
					rc.right = point -> x;
				if (point -> y > rc.bottom)
					rc.bottom = point -> y;
			}
			break;
		case O_LINE:
			{
				CLine *line = reinterpret_cast<CLine*>(objects[i]);
				if (line -> x > rc.right)
					rc.right = line -> x;
				if (line -> y > rc.bottom)
					rc.bottom = line -> y;
                if (line -> x_end > rc.right)
					rc.right = line -> x_end;
				if (line -> y_end > rc.bottom)
					rc.bottom = line -> y_end;
			}
			break;
		case O_RECTANGLE:
			{
				CRectangle *rect = reinterpret_cast<CRectangle*>(objects[i]);
				if (rect -> x > rc.right)
					rc.right = rect -> x;
				if (rect -> y > rc.bottom)
					rc.bottom = rect -> y;
				if (rect -> x_right > rc.right)
					rc.right = rect -> x_right;
				if (rect -> y_bottom > rc.bottom)
					rc.bottom = rect -> y_bottom;
			}
			break;
		case O_CIRCLE:
			{
				CCircle *circ = reinterpret_cast<CCircle*>(objects[i]);
				if (circ -> x > rc.right)
					rc.right = circ -> x;
				if (circ -> y > rc.bottom)
					rc.bottom = circ -> y;
				if (circ -> x + circ -> radius > rc.right)
					rc.right = circ -> x + circ -> radius;
				if (circ -> y + circ -> radius > rc.bottom)
					rc.bottom = circ -> y + circ -> radius;
			}
			break;
		}
	}
}


/*!
 * @function : ClearDevice
 * @brief    : ���������� ���������� ������� ��������� ��������� ����� ������
 *
 * @param HDC hdc : �������� ����������
 *
 * @return void : 
 */

void ClearDevice(HDC hdc)
{
	PatBlt(hdc,0,0,irc.right,irc.bottom,WHITENESS);
}