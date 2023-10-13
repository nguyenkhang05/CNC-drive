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

//#include <cstring>
//#include <vector>
//#include "./trace_info.h"
#include "./forest_common.h"

/*!
 * @brief Branch Target Buffer
 */
class Btb {
public:
    // BHT
    virtual BpResult PredictBranchAndStudyBht
                        (PhysAddr branch_pc, bool is_taken, bool is_forward) = 0;
    virtual void UpdateGhr (bool is_taken) = 0;
    virtual void ReserveUpdateGhr (bool is_taken) = 0;

    // G3M BHT
    virtual BpResult PredictBranchAndStudy
                        (PhysAddr branch_pc, PhysAddr target_pc, bool is_taken) = 0;

    // RAS
    virtual void StudyRas (PhysAddr return_pc) = 0;
    virtual BpResult PredictBranchAndPopRas (PhysAddr target_pc) = 0;

    // BTAC
    virtual BpResult PredictBranchAndStudyBtac (PhysAddr branch_pc, PhysAddr target_pc) = 0;

    // BUncond
    virtual BpResult PredictBranch (void) = 0;

    // BTAC and BHT
    virtual BpResult PredictBranchAndStudyBtacBht (PhysAddr branch_pc, PhysAddr target_pc,
            bool is_taken, bool is_forward, bool is_uncond) = 0;

    virtual void ClearBtb (void) = 0;
    virtual void Reset (void) = 0;

    virtual void ShowPerfInfo (void) const = 0;
    virtual void ResetPerfInfo (void) = 0;
    virtual ~Btb () {}
};
