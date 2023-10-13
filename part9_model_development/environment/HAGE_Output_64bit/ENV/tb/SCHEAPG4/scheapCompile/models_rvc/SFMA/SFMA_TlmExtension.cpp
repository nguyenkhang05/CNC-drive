// ---------------------------------------------------------------------
// $Id: SFMA_TlmExtension.cpp,v 1.2 2020/01/30 08:01:36 huepham Exp $
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

#include "SFMA_TlmExtension.h"

/// Constructor
SFMA_TlmExtension::SFMA_TlmExtension(void) :
            mTransactionCommand(emTrans_Read_Command)
            ,mReadWriteDataSize(0)
            ,mDataFieldEnable(0) 
{//{{{
}//}}}

/// De-constructor
SFMA_TlmExtension::~SFMA_TlmExtension(void)
{//{{{
}//}}}
    
/// Overwrite clone function
tlm_extension_base* SFMA_TlmExtension::clone(void) const 
{//{{{
    SFMA_TlmExtension *t = new SFMA_TlmExtension;
    sc_assert(t != NULL);
    t->mTransactionCommand = this->mTransactionCommand;
    t->mReadWriteDataSize = this->mReadWriteDataSize;
    t->mDataFieldEnable = this->mDataFieldEnable;
    return t;
}//}}}

/// Overwrite copy_from function
void SFMA_TlmExtension::copy_from(const tlm_extension_base &extension) 
{//{{{
    mTransactionCommand = static_cast<SFMA_TlmExtension const &>(extension).mTransactionCommand;
    mReadWriteDataSize = static_cast<SFMA_TlmExtension const &>(extension).mReadWriteDataSize;
    mDataFieldEnable = static_cast<SFMA_TlmExtension const &>(extension).mDataFieldEnable;
}//}}}

/// Set transaction command type
void SFMA_TlmExtension::setTransactionCommand(eTransCommandType command) 
{//{{{
    mTransactionCommand = command;
}//}}}

/// Get transaction command type
SFMA_TlmExtension::eTransCommandType SFMA_TlmExtension::getTransactionCommand(void)
{//{{{
    return mTransactionCommand;
}//}}}

/// Set read/write data size
void SFMA_TlmExtension::setReadWriteDataSize(unsigned int size) 
{//{{{
    mReadWriteDataSize = size;
}//}}}

/// Get read/write data size
unsigned int SFMA_TlmExtension::getReadWriteDataSize(void) 
{//{{{
    return mReadWriteDataSize;
}//}}}

/// Set data field enable
void SFMA_TlmExtension::setDataFieldEnable(unsigned char value) 
{//{{{
    mDataFieldEnable = value;
}

/// Get data field enable
unsigned char SFMA_TlmExtension::getDataFieldEnable(void) 
{//{{{
    return mDataFieldEnable;
}//}}}

// vim600: set foldmethod=marker :
