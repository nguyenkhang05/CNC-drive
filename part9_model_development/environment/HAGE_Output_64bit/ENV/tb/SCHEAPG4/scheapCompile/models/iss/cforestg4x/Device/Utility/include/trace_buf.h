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

#include <queue>
#include <vector>
#include "./forest_common.h"
#include "./trace_info.h"

class TraceBuf {
private:
    std::vector <TraceCtrl> m_traces_body;
    std::queue <TraceCtrl*> m_used_buf;
    std::queue <TraceCtrl*> m_unused_buf;

public:
    bool IsEmpty (void) const;
    bool IsFull (void) const;
    const TraceCtrl* Dequeue (void);
    TraceCtrl* Reserve (void);
    void Queue (TraceCtrl* trace);
    void Init (void);
    CfApiRetern SetMaxSize (uint32_t num);
    uint32_t GetUsedSize (void) const;

    explicit TraceBuf (uint32_t num);
    ~TraceBuf () {}
};
