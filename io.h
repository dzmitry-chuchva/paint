/*! Time-stamp: <@(#)io.h   20.12.2005 - 20.12.2005 18:05:16   Dmitry Chuchva>
 *********************************************************************
 *  @file   : io.h
 *
 *  Project : graphics
 *
 *  Package : IO package
 *
 *  Company : Mitro Software Inc.
 *
 *  Author  : Dmitry Chuchva                          Date: 20.12.2005
 *
 *  Purpose : Header of IO module
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  20.12.2005  BN : First Revision
 *
 *********************************************************************
 */


#ifndef __IO_H
#define __IO_H

#include <windows.h>
#include <vector>
#include "objects.h"

using namespace std;

int LoadRaw(char *filename,vector<CObject*> &obj);
int SaveRaw(char *filename,vector<CObject*> &obj);


#endif