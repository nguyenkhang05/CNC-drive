//$Id: re_security.cpp,v 1.9 2010/04/19 09:34:53 watanama Exp $
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
/// @date    $Date: 2010/04/19 09:34:53 $  $Revision: 1.9 $                                                    @n
/// @author  $Author: watanama $                                                              @n
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#include "re_security.h"
#include <cstdlib>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <cstdarg>
#include <string>

#ifndef _WIN32
#ifndef RE_SEQURITY_BUF_SIZE
#define RE_SEQURITY_BUF_SIZE 64
#endif
typedef unsigned int errno_t;

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Copies the values of num bytes from the location pointed by source  directly
// to the memory block pointed by destination
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
errno_t memcpy_s(void *dest, size_t numberOfElements, const void *src, size_t count)
{
  #if RE_SECURITY_DEBUG
  const char usage[] = "memcpy_s(dest, numberOfElements, src, count) :";
  #endif//RE_SECURITY_DEBUG

  // argument check
  if (dest == NULL) {
    #if RE_SECURITY_DEBUG
    fprintf(stderr, "%s dest is NULL\n", usage);
    #endif//RE_SECURITY_DEBUG
    return EINVAL;
  }

  if (numberOfElements < count) {
    #if RE_SECURITY_DEBUG
    fprintf(stderr, "%s numberOfElements(%d) is less than count(%d)\n", usage, numberOfElements, count);
    #endif//RE_SECURITY_DEBUG
    return ERANGE;
  }

  if (src == NULL) {
    #if RE_SECURITY_DEBUG
    fprintf(stderr, "%s src is NULL\n", usage);
    #endif//RE_SECURITY_DEBUG
    return EINVAL;
  }

  // call original function
  memcpy(dest, src, count);

  return 0;
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Opens the file whose name is specified in the parameter filename
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
errno_t fopen_s(FILE** pFile, const char *filename, const char *mode)
{
  #if RE_SECURITY_DEBUG
  const char usage[] = "fopen_s(pFile, filename, mode) :";
  #endif//RE_SECURITY_DEBUG

  // argument check
  if (pFile == NULL) {
    #if RE_SECURITY_DEBUG
    fprintf(stderr, "%s pFile is NULL\n", usage);
    #endif//RE_SECURITY_DEBUG
    return EINVAL;
  }

  if (filename == NULL) {
    #if RE_SECURITY_DEBUG
    fprintf(stderr, "%s filename is NULL\n", usage);
    #endif//RE_SECURITY_DEBUG
    return EINVAL;
  }

  if (mode == NULL) {
    #if RE_SECURITY_DEBUG
    fprintf(stderr, "%s mode is NULL\n", usage);
    #endif//RE_SECURITY_DEBUG
    return EINVAL;
  }

  // call original function
  *pFile = fopen(filename, mode);
  if (*pFile == NULL) {
    return EINVAL;
  }
  else {
    return 0;
  }
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Check whether a specified keyword is reserved or not
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
bool FindType(const char keyword)
{
  bool ret_val = false;

  switch(keyword) {
    case 'c':
    case 'f':
    case 'F':
    case 'a':
    case 'A':
    case 'd':
    case 'u':
    case 'i':
    case 'o':
    case 'x':
    case 'X':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
    case 's':
    case 'S':
    case 'p':
    case 'n':
      ret_val = true;
      break;
    default:ret_val = false;
      break;
  }
  return ret_val;
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Skip an argument to specify the previous array's size
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
bool SkipCharSize(const char *format, unsigned int *buf, unsigned int *vl_p)
{
  int buf_i = 0;
  int vl_p_i = 0;

  if (format == NULL) return false;
  if (buf    == NULL) return false;
  if (vl_p   == NULL) return false;

  for (unsigned int i=0 ; format[i] != '\0'; i++) {
    if (format[i] != '%')
      continue;

    while (!FindType(format[i])) {
      i++;
    }
    
    buf[buf_i++] = vl_p[vl_p_i++];
    
    if (format[i] == 's' || format[i] == 'S') {
      vl_p_i++;
    }
    
    if (buf_i >= RE_SEQURITY_BUF_SIZE) {
      printf("Error (scanf_s/sscanf_s/fscanf_s) Too many arguments are specified\n");
      return false;
    }
  }

  return true;
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Reads data from stdin and stores them according to the parameter format into
// the locations pointed by the additional arguments.
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#if 0
int scanf_s(const char *format, ...)
{
  #if RE_SECURITY_DEBUG
  const char usage[] = "scanf_s(format, ...) :";
  #endif//RE_SECURITY_DEBUG

  // argument check
  if (format == NULL) {
    #if RE_SECURITY_DEBUG
    fprintf(stderr, "%s format is NULL\n", usage);
    #endif//RE_SECURITY_DEBUG
    return 0;
  }

  unsigned int buf[RE_SEQURITY_BUF_SIZE];
  va_list vl;
  va_start(vl, format);

  if (SkipCharSize(format, buf, (unsigned int *) vl) == false) {
    return 0;
  }

  return vscanf(format, (va_list) buf);
}
#endif

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Reads data from str and stores them according to the parameter format  into
// the locations given by the additional arguments.
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#if 0
int sscanf_s(const char *str, const char *format, ...)
{
  unsigned int buf[RE_SEQURITY_BUF_SIZE];
  va_list vl;
  va_start(vl, format);

  if (SkipCharSize(format, buf, (unsigned int *) vl) == false) {
    return 0;
  }

  return vsscanf(str, format, (va_list) buf);
}
#endif

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Reads data from str and stores them according to the parameter format  into
// the locations given by the additional arguments.
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#if 0
int fscanf_s(FILE * stream, const char *format, ...)
{
  unsigned int buf[RE_SEQURITY_BUF_SIZE];
  va_list vl;
  va_start(vl, format);

  if (SkipCharSize(format, buf, (unsigned int *) vl) == false) {
    return 0;
  }

  return vfscanf(stream, format, (va_list) buf);
}
#endif

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Converts the time_t object pointed by timer to a C string containing 
// a human-readable version of the corresponding local time and date.
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
errno_t ctime_s( char* buffer, size_t numberOfElements, const time_t *time )
{
  if (buffer == NULL) {
    return EINVAL;
  }

  if (numberOfElements == 0) {
    return EINVAL;
  }

  if (1 <= numberOfElements && numberOfElements < 26) {
    buffer[0] = '\0';
    return EINVAL;
  }

  if (numberOfElements >= 26 && time == NULL) {
    buffer[0] = '\0';
    return EINVAL;
  }

  if (numberOfElements >= 26 && *time < 0) {
    buffer[0] = '\0';
    return EINVAL;
  }

  char *temp = ctime(time);
  if (temp != NULL) {
    for (unsigned int i=0 ; i<numberOfElements ; i++) {
      buffer[i] = temp[i];
    }
  }

  return 0;
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Retrieves a C string containing the value of the environment variable
// whose name is specified as argument.
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
errno_t getenv_s( size_t *pReturnValue, char* buffer, size_t numberOfElements, const char *varname )
{
  if (pReturnValue == NULL) {
    return EINVAL;
  }

  if (buffer == NULL && numberOfElements > 0) {
    return EINVAL;
  }

  if ( varname == NULL) {
    return EINVAL;
  }

  std::string val;
  val = getenv(varname);
  *pReturnValue = val.size();

  if (numberOfElements != 0) {
    unsigned int i = 0;
    for (; i<numberOfElements && i<*pReturnValue ; i++) {
      buffer[i] = val[i];
    }
    buffer[i] = '\0';
  }

  return 0;
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// A sequence of calls to this function split str into tokens, which are sequences
// of contiguous characters separated by any of the characters that are part of
// delimiters.
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
char *strtok_s( char *strToken, const char *strDelimit, char **context )
{
  if (strToken == NULL) {
    return NULL;
  }

  if (strDelimit == NULL) {
    return NULL;
  }

  if (context == NULL) {
    return NULL;
  }

  return strtok(strToken, strDelimit);
}


//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// Converts the time_t object pointed by timer to a C string containing
// a human-readable version of the corresponding local time and date.
//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
errno_t _gmtime_s( struct tm* _tm, const time_t* time )
{
  if (_tm == NULL) {
    return EINVAL;
  }
  else if (time == NULL ) {
    _tm->tm_sec   = -1;
    _tm->tm_min   = -1;
    _tm->tm_hour  = -1;
    _tm->tm_mday  = -1;
    _tm->tm_mon   = -1;
    _tm->tm_year  = -1;
    _tm->tm_wday  = -1;
    _tm->tm_yday  = -1;
    _tm->tm_isdst = -1;

    return EINVAL;
  }
  else if (*time < 0) {
    _tm->tm_sec   = -1;
    _tm->tm_min   = -1;
    _tm->tm_hour  = -1;
    _tm->tm_mday  = -1;
    _tm->tm_mon   = -1;
    _tm->tm_year  = -1;
    _tm->tm_wday  = -1;
    _tm->tm_yday  = -1;
    _tm->tm_isdst = -1;

    return EINVAL;
  }

  struct tm *tm_tmp = gmtime(time);

  if ( tm_tmp != NULL) {
    _tm->tm_sec   = tm_tmp->tm_sec;
    _tm->tm_min   = tm_tmp->tm_min;
    _tm->tm_hour  = tm_tmp->tm_hour;
    _tm->tm_mday  = tm_tmp->tm_mday;
    _tm->tm_mon   = tm_tmp->tm_mon;
    _tm->tm_year  = tm_tmp->tm_year;
    _tm->tm_wday  = tm_tmp->tm_wday;
    _tm->tm_yday  = tm_tmp->tm_yday;
    _tm->tm_isdst = tm_tmp->tm_isdst;
  }

  return 0;
}


#endif //_WIN32
