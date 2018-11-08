// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#define glCheck(expr) do { expr; glCheckError(__FILE__, __LINE__, __FUNCTION__, #expr); } while (0)

void glCheckError(const char* file, unsigned int line, const char* method, const char * op);
