// ---------------------------------------------------------------------
// $Id: SFMA_TlmExtension.h,v 1.3 2020/01/30 08:01:36 huepham Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __SFMA_TLMEXTENSION_H__
#define __SFMA_TLMEXTENSION_H__

#include "tlm.h"

using namespace tlm;

// SFMA_TlmExtension
class SFMA_TlmExtension : public tlm_extension<SFMA_TlmExtension>
{
public:
    // SFMA Transaction command type
    enum eTransCommandType {
        emTrans_Read_Command = 0
        ,emTrans_Write_Command = 1
        ,emTrans_ReadWrite_Command = 2
    };

    // SFMA data field enable
    enum eDataFieldEnable {
        emCommand_Enable        = 1 << 0
        ,emOpCommand_Enable     = 1 << 1
        ,emAddress_Enable       = 1 << 2
        ,emOpData_Enable        = 1 << 3
        ,emWriteData_Enable     = 1 << 4
        ,emReadData_Enable      = 1 << 5
    };

    // Constructor
    SFMA_TlmExtension(void);

    // De-constructor
    ~SFMA_TlmExtension(void);

    // Overwrite clone function
    tlm_extension_base* clone(void) const;
    // Overwrite copy_from function
    void copy_from(const tlm_extension_base &extension);
    
    // Get/set extension value
    void setTransactionCommand(eTransCommandType command);
    eTransCommandType getTransactionCommand(void);
    void setReadWriteDataSize(unsigned int size);
    unsigned int getReadWriteDataSize(void);
    void setDataFieldEnable(unsigned char value);
    unsigned char getDataFieldEnable(void);
private:
    eTransCommandType mTransactionCommand;   // Transaction command type
    unsigned int mReadWriteDataSize;         // Read/write data size
    unsigned char mDataFieldEnable;          // Data field enable
};
#endif //__SFMA_TLMEXTENSION_H__
