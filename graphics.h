/*! Time-stamp: <@(#)graphics.h   20.12.2005 - 20.12.2005 18:06:24   Dmitry Chuchva>
 *********************************************************************
 *  @file   : graphics.h
 *
 *  Project : graphics
 *
 *  Package : without package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : Header of main module
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  20.12.2005  BN : First Revision
 *
 *********************************************************************
 */

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commdlg.h>
#include <vector>
using namespace std;

#include "resource.h"
#include "objects.h"
#include "io.h"

CObject *CreateObject(int next,int x,int y);
void DeleteObjects(vector<CObject*> &objects);
