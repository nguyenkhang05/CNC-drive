//$Id: re_security.h,v 1.7 2010/04/19 09:34:53 watanama Exp $
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
/// @file
/// @brief Reconstructed functions in C++ standard library to surpress
///        the security warning in Visual C++
///
/// Microsoft(c) provides a standard library's function improved that security.    @n
/// For example, sprintf_s, scanf_s, and so on. If a designer uses sprintf         @n
/// and builds it on Visual C++, the compiler dumps the following warning message. @n
/// warning C4996: 'sprintf': This function or variable may be unsafe. Consider    @n
/// using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.  @n
///                                                                                @n
/// In this file, some functions in a standard library are reconstructed to        @n
/// surpress this warning message. Of course, we can supress it by                 @n
/// _CRT_SECURE_NO_WARNINGS macro but we don't use it.                             @n
///                                                                                @n
/// memcpy   => re_memcpy                                                          @n
/// fopen    => re_fopen                                                           @n
/// ctime    => ctime_s                                                            @n
/// getenv   => getenv_s                                                           @n
/// strtok   => re_strok                                                           @n
/// gmtime   => _gmtime_s                                                          @n
///                                                                                @n
/// Never use the following functions.                                             @n
/// fscanf, scanf, sscanf                                                          @n
///                                                                                @n
/// Instead of them, please use the following functions even if on Linux           @n
/// fscanf_s, scanf_s, sscanf_s                                                    @n
///                                                                                @n
/// You can use the following functions as an original name if only you include    @n
/// this file.                                                                     @n
/// gets, strcant, strcpy, fprintf_s, sprintf_s                                    @n
///                                                                                @n
/// This file is part of the Virtual Platform Common Library (vpcl).               @n
///                                                                                @n
/// (C) Copyright 2010 RENESAS Technology Corp.  All rights reserved.              @n
///                                                                                @n
/// @date    $Date: 2010/04/19 09:34:53 $  $Revision: 1.7 $                                                    @n
/// @author  $Author: watanama $                                                              @n
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#ifndef __RE_SEQURITY_H
#define __RE_SEQURITY_H

// Use the function improved that security on Windows
#if ( !defined(_MSC_VER) || _MSC_VER == 1500 )
#  define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#else
#if ( !defined(_MSC_VER) || _MSC_VER == 1400 )
#  define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#endif
#endif

#include <cstdio>
#include <cerrno>
#include <ctime>

#ifndef _WIN32
typedef unsigned int errno_t;

// Public function
errno_t memcpy_s(void *dest, size_t numberOfElements, const void *src, size_t count);
errno_t fopen_s(FILE** pFile, const char *filename, const char *mode);
#if 0
int scanf_s(const char *format, ...);
int sscanf_s(const char *str, const char *format, ...);
int fscanf_s(FILE * stream, const char *format, ...);
#endif
errno_t ctime_s( char* buffer, size_t numberOfElements, const time_t *time );
errno_t getenv_s(size_t *pReturnValue, char* buffer, size_t numberOfElements, const char *varname);
char *strtok_s(char *strToken, const char *strDelimit, char **context);
errno_t _gmtime_s(struct tm* _tm, const time_t* time);


// Not public function
bool FindType(const char keyword);
bool SkipCharSize(const char *format, unsigned int *buf, unsigned int *vl_p);

#else
#include <Time.h>
#define _gmtime_s _gmtime32_s
#endif //_WIN32
#endif //__RE_SEQURITY_H
