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

#include <vector>

class TraceCtrl;

class StatisticsCtrl {
public:
    void DisplayInstructionCount (uint32_t pe_id, ContextId tc_id, FILE* local_file);
    void ClearInstructionCount (void);
    void CountInstructionCount (TraceCtrl* trace);

    explicit StatisticsCtrl (int32_t num_of_inst);
    ~StatisticsCtrl () {}

private:
    std::vector<uint64_t> m_instruction_count;
};
