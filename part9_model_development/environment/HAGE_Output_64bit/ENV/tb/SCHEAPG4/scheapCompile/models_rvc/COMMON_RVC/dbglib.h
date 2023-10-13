// -----------------------------------------------------------------------------
// $Id: dbglib.h,v 1.3 2012/03/15 16:25:24 dungvannguyen Exp $
//
// Copyright(c) 2008, 2012 Renesas Electronics Corporation
// Copyright(c) 2008, 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __DEBUG_LIB__
#define __DEBUG_LIB__

#include <cstdio>
#include <cstdarg>
using namespace std;

void dbg_header(const char *inst_name, const char *f_name, int line)
{
  if (inst_name != NULL && f_name != NULL) {
    printf("[%s: %s at %d line] ", inst_name, f_name, line);
  }
}

void dbg_printf(const char *format, ... )
{
  if (format != NULL) {
    va_list arg;
//    int rc;

    va_start(arg, format);
//    rc = vprintf(format, arg);
    vprintf(format, arg);
    va_end(arg);
  }
}

//void dbg_printf_nothing (const char *format, ... )
//{
//}

#define DBG_MSG \
    dbg_header(this -> name(), __FILE__, __LINE__); \
    dbg_printf

template<class T, int S>
class dbg_array
{
public:
  char *classname;
  dbg_array() {
    array = new T[S];
    dummy = new T;
    classname = "dbg_array class";
  }
 
  char *name() {
    return classname;
  }

  T& operator [](int size) {
    if (size >= S) {
      DBG_MSG("ERROR: Out of bound\n");
      DBG_MSG("Even if allocated size is %d, %d is accessed\n", S, size);
      return *dummy;
    }
    else {
      return array[size];
    }
  }

private:
  T *array;
  T *dummy;
};


    #ifdef __DEBUG_LIB_EN__

    #define STACK_ARRAY(type,size,inst,name) 'dbg_array<type, size> inst; inst.classname = name;'

    #else

    #define STACK_ARRAY(type,size,inst,name) 'type inst[size];'

    #endif

#endif
