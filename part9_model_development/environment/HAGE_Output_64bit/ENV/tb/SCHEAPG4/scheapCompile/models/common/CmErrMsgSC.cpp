/*************************************************************************
*
*  CmErrMsg.cpp
*
* Copyright(c) 2011-2013 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
*************************************************************************/

/* $Id:$ */

#include "CmErrMsg.h"
#include <sstream>
#include <string>



/// <summary>initialize CmErrMsg</summary>
/// <param name="ConfigFile">configration file name</param>
void CmErrMsg::CmErrMsgInitSet(const char *ConfigFile)
{
    //Do nothing
}


/// <summary>print error message</summary>
/// <param name="moduleName">module name</param>
/// <param name="funcName">function name</param>
/// <param name="message">message</param>
void CmErrMsg::printErrorMessage(const char *moduleName, 
const char *funcName, 
const char *message)
{
    std::ostringstream msgStream;
    if( funcName != NULL )
        msgStream << endl << "\t[ Function ] " << funcName;
    msgStream << endl << "\t[ Message  ] " << message  << endl;
    
    SC_REPORT_ERROR(moduleName, msgStream.str().c_str());
}


/// <summary>print warning message</summary>
/// <param name="moduleName">module name</param>
/// <param name="funcName">function name</param>
/// <param name="message">message</param>
void CmErrMsg::printWarningMessage(const char *moduleName, 
const char *funcName, 
const char *message)
{
    std::ostringstream msgStream;
    if( funcName != NULL )
        msgStream << endl << "\t[ Function ] " << funcName;
    msgStream << endl << "\t[ Message  ] " << message  << endl;
    
    SC_REPORT_WARNING(moduleName, msgStream.str().c_str());

}


/// <summary>set where message is output</summary>
/// <param name="flag">file or stdout</param>
void CmErrMsg::CmErrMsgSetToFile(bool flag)    {
    // Do Nothing
}

