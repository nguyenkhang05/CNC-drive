/***********************************************************************
*
* NSMVRH850LITE.h
*
* Copyright(c) 2016-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
***********************************************************************/

#ifndef NSMVRH850LITE_H
#define NSMVRH850LITE_H

class NSMVG4SSLITE;

class NSMVRH850LITE : public sc_module
{
private:

public:
    NSMVRH850LITE(sc_module_name moduleName, const char *configFile);
    ~NSMVRH850LITE(void);

    NSMVG4SSLITE *G4SS; // pointer of G4SS
};

#endif /* NSMVRH850LITE_H */
