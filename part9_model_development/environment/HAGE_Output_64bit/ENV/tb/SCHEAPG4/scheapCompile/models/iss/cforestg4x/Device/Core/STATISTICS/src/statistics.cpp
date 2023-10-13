/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./statistics.h"
#include "./micro_arch_db.h"


void StatisticsCtrl::DisplayInstructionCount (uint32_t pe_id, ContextId tc_id, FILE* local_file)
{
    if (tc_id == NC) {
        fprintf (local_file, "P%d:NC: ", pe_id);
    } else {
        fprintf (local_file, "P%d:T%d ", pe_id, tc_id);
    }
    fprintf (local_file, "Program Instruction Counter>\n");

    // 0番目はダミー命令なのでスキップ
    for (uint32_t i = 1; i < m_instruction_count.size (); i++) {
        const char* funcname = MicroArchDB::GetInstName (i);
        if (funcname != NULL) {
            fprintf (local_file, "\t%" PRIu64 "\t%s\n", m_instruction_count[i], funcname);
        }
    }
}


void StatisticsCtrl::ClearInstructionCount (void)
{
    size_t num_of_inst = m_instruction_count.size ();
    m_instruction_count.assign (num_of_inst, 0);
}


void StatisticsCtrl::CountInstructionCount (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId ();
    m_instruction_count[inst_id] += 1;
}


StatisticsCtrl::StatisticsCtrl (int32_t num_of_inst)
{
    m_instruction_count.assign (num_of_inst, 0);
}
