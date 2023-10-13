/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include "./forest_common.h"

class ForestUtil;
class ForestCmdOpt;

class ForestSetup {
private:
    ForestUtil* m_forest_util;
    ForestCmdOpt* m_cmdopt;

public:
    ForestSetup (ForestUtil* forest_util, ForestCmdOpt* cmdopt);
    ~ForestSetup () {}

    void DefaultSetup (void);
};
