/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <stdlib.h>
#include "./config.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./sregfile.h"
#include "./cube_suite_if.h"
#include "./forest_message.h"


void SRegTest (cube::CForestIF* cfif, uint32_t peid, CReg regno)
{
    uint32_t data = 0xffffffff;
    cube::APIErrorCode err;
    err = cfif->RegWriteDebug (peid, regno, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Inf: register write to   %03x>\n", regno);
    }
    err = cfif->RegReadDebug (peid, regno, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Inf: register read  from %03x>\n", regno);
    }
}


void SRegTestInvalid (cube::CForestIF* cfif, uint32_t peid, CReg regno)
{
    uint32_t data = 0xffffffff;
    cube::APIErrorCode err;
    err = cfif->RegWriteDebug (peid, regno, &data);
    if (err == cube::API_E_OK) {
        printf ( "<Error: register write to   %03x>\n", regno);
    }
    err = cfif->RegReadDebug (peid, regno, &data);
    if (err == cube::API_E_OK) {
        printf ( "<Error: register read  from %03x>\n", regno);
    }
}


void TestDefaultMemMapG4MH (cube::CForestIF* cfif) {
    cube::MemoryBlock mem_block;

    mem_block.bank_size = NULL;
    mem_block.bank_size_count = 1;

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 6;
    mem_block.mblock_read_latency    = 13;
    mem_block.mblock_write_latency   = 8;
    mem_block.cl_fetch_latency       = 6;
    mem_block.cl_read_latency        = 13;
    mem_block.cl_write_latency       = 8;
    mem_block.exo_cl_fetch_latency   = 6;
    mem_block.exo_cl_read_latency    = 13;
    mem_block.exo_cl_write_latency   = 8;
    mem_block.num_outstanding        = 2;
    mem_block.repeat_rate            = 6;
    mem_block.pe_id                  = 0;
    mem_block.cl_id                  = 0;
    mem_block.start_address = 0x00000000;
    mem_block.end_address   = 0x02FFFFFF;
    mem_block.mblock_area   = cube::MI_CF_FROM;
    mem_block.offset        = 0x0;
    cube::APIErrorCode err = cfif->SetMemBlock(0, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem0>\n");
    }

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 8;
    mem_block.mblock_read_latency    = 0;
    mem_block.mblock_write_latency   = 0;
    mem_block.cl_fetch_latency       = 10;
    mem_block.cl_read_latency        = 6;
    mem_block.cl_write_latency       = 6;
    mem_block.exo_cl_fetch_latency   = 26;
    mem_block.exo_cl_read_latency    = 26;
    mem_block.exo_cl_write_latency   = 26;
    mem_block.num_outstanding        = 2;
    mem_block.repeat_rate            = 1;
    mem_block.pe_id                  = 1;
    mem_block.cl_id                  = 0;
    mem_block.start_address = 0xFEA00000;
    mem_block.end_address   = 0xFEBFFFFF;
    mem_block.mblock_area   = cube::MI_CF_LRAM;
    mem_block.offset        = 0x0;
    err = cfif->SetMemBlock(1, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem1>\n");
    }

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 11;
    mem_block.mblock_read_latency    = 11;
    mem_block.mblock_write_latency   = 11;
    mem_block.cl_fetch_latency       = 11;
    mem_block.cl_read_latency        = 11;
    mem_block.cl_write_latency       = 11;
    mem_block.exo_cl_fetch_latency   = 11;
    mem_block.exo_cl_read_latency    = 11;
    mem_block.exo_cl_write_latency   = 11;
    mem_block.num_outstanding        = 4;
    mem_block.repeat_rate            = 2;
    mem_block.pe_id                  = 0;
    mem_block.cl_id                  = 0;
    mem_block.start_address = 0xFEE00000;
    mem_block.end_address   = 0xFEFFFFFF;
    mem_block.mblock_area   = cube::MI_CF_CRAM;
    mem_block.offset        = 0x0;
    err = cfif->SetMemBlock(2, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem2>\n");
    }

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE);
    mem_block.mblock_fetch_latency   = 10;
    mem_block.mblock_read_latency    = 10;
    mem_block.mblock_write_latency   = 10;
    mem_block.cl_fetch_latency       = 10;
    mem_block.cl_read_latency        = 10;
    mem_block.cl_write_latency       = 10;
    mem_block.exo_cl_fetch_latency   = 10;
    mem_block.exo_cl_read_latency    = 10;
    mem_block.exo_cl_write_latency   = 10;
    mem_block.num_outstanding        = 1;
    mem_block.repeat_rate            = 2;
    mem_block.cl_id                  = 0;
    mem_block.pe_id                  = 1;
    mem_block.start_address = 0xfffec000;
    mem_block.end_address   = 0xfffec3ff;
    mem_block.mblock_area   = cube::MI_CF_LOCAL_IOR;
    mem_block.offset        = 0x0;
    err = cfif->SetMemBlock(3, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem3>\n");
    }

    mem_block.start_address = 0xfffec600;
    mem_block.end_address   = 0xfffec6ff;
    err = cfif->SetMemBlock(4, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem4>\n");
    }

    mem_block.start_address = 0xfffeca00;
    mem_block.end_address   = 0xfffecbf7;
    err = cfif->SetMemBlock(5, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem5>\n");
    }

    mem_block.start_address = 0xfffecc00;
    mem_block.end_address   = 0xfffecc7f;
    err = cfif->SetMemBlock(6, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem6>\n");
    }

    mem_block.start_address = 0xfffecc80;
    mem_block.end_address   = 0xfffeccff;
    err = cfif->SetMemBlock(7, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem7>\n");
    }

    mem_block.pe_id         = 0;
    mem_block.start_address = 0xffff7f00;
    mem_block.end_address   = 0xffff7fff;
    err = cfif->SetMemBlock(8, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem8>\n");
    }

    mem_block.start_address = 0xffff8ffc;
    mem_block.end_address   = 0xffff8fff;
    err = cfif->SetMemBlock(9, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem9>\n");
    }

    mem_block.start_address = 0xffffb000;
    mem_block.end_address   = 0xffffbfff;
    err = cfif->SetMemBlock(10, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem10>\n");
    }

    mem_block.mblock_area   = cube::MI_CF_SELF;
    mem_block.start_address =  0xfffee000;
    mem_block.end_address   =  0xfffeffff;
    mem_block.offset        = -0x00002000;
    err = cfif->SetMemBlock(11, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem11>\n");
    }

    mem_block.mblock_area   = cube::MI_CF_SELF;
    mem_block.start_address =  0xfec00000;
    mem_block.end_address   =  0xfedfffff;
    mem_block.offset        = -0x00200000;
    err = cfif->SetMemBlock(12, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem12>\n");
    }

    for (uint32_t i = 0; i <= 12; i++) {
        err = cfif->DeleteMemBlock (i);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error at Mem13+%u>\n", i);
        }
    }
}


void TestDefaultMemMap (cube::CForestIF* cfif)
{
    cube::MemoryBlock mem_block;

    int32_t bank_size[4];
    bank_size[0] = 1024*1024*2;
    bank_size[1] = 1024*1024*3;
    bank_size[2] = 1024*1024*13;
    bank_size[3] = 1024*1024*14;
    mem_block.bank_size = bank_size;
    mem_block.bank_size_count = 4;

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 6;
    mem_block.mblock_read_latency    = 13;
    mem_block.mblock_write_latency   = 8;
    mem_block.cl_fetch_latency   = 6;
    mem_block.cl_read_latency    = 13;
    mem_block.cl_write_latency   = 8;
    mem_block.exo_cl_fetch_latency   = 6;
    mem_block.exo_cl_read_latency    = 13;
    mem_block.exo_cl_write_latency   = 8;
    mem_block.num_outstanding        = 2;
    mem_block.repeat_rate            = 6;
    mem_block.pe_id                  = 0;
    mem_block.cl_id                  = 0;
    mem_block.start_address = 0x00000000;
    mem_block.end_address   = 0x01FFFFFF;
    mem_block.mblock_area   = cube::MI_CF_FROM;
    mem_block.offset        = 0x0;
    cube::APIErrorCode err = cfif->SetMemBlock(0, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 25A>\n");
    }

    bool is_err = false;
    cube::MemoryBlock mem_block_get;
    err = cfif->GetMemBlock(0, &mem_block_get);
    if (err == cube::API_E_OK) {
        if (mem_block.bank_size_count != mem_block_get.bank_size_count) {
            is_err = true;
        }
        for (int32_t i = 0; i < mem_block_get.bank_size_count; ++i) {
            if (mem_block.bank_size[i] != mem_block_get.bank_size[i]) {
                is_err = true;
            }
        }
    }
    if (is_err) {
        printf ( "<Error: Unexpected error at 25B>\n");
    }

    mem_block.start_address = 0x02000000;
    mem_block.end_address   = 0x02007FFF;
    err = cfif->SetMemBlock(1, mem_block);
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at 25B2>\n");
    }

    mem_block.bank_size_count = 0;
    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 8;
    mem_block.mblock_read_latency    = 0;
    mem_block.mblock_write_latency   = 0;
    mem_block.exo_cl_fetch_latency   = 26;
    mem_block.exo_cl_read_latency    = 26;
    mem_block.exo_cl_write_latency   = 26;
    mem_block.num_outstanding        = 2;
    mem_block.repeat_rate            = 1;
    mem_block.pe_id                  = 1;
    mem_block.start_address = 0xfea00000;
    mem_block.end_address   = 0xfea1FFFF;
    mem_block.mblock_area   = cube::MI_CF_LRAM;
    mem_block.offset        = 0x0;
    err = cfif->SetMemBlock(1, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 25C>\n");
    }

    is_err = false;
    err = cfif->GetMemBlock(1, &mem_block_get);
    if (err == cube::API_E_OK) {
        if ((mem_block_get.bank_size_count != 1)
         || (static_cast<uint32_t>(mem_block_get.bank_size[0]) != (1 + mem_block.end_address - mem_block.start_address))) {
            is_err = true;
        }
    }
    if (is_err) {
        printf ( "<Error: Unexpected error at 25D>\n");
    }

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 11;
    mem_block.mblock_read_latency    = 11;
    mem_block.mblock_write_latency   = 11;
    mem_block.exo_cl_fetch_latency   = 11;
    mem_block.exo_cl_read_latency    = 11;
    mem_block.exo_cl_write_latency   = 11;
    mem_block.num_outstanding        = 4;
    mem_block.repeat_rate            = 2;
    mem_block.pe_id                  = 0;
    mem_block.start_address = 0xfee00000;
    mem_block.end_address   = 0xfeffFFFF;
    mem_block.mblock_area   = cube::MI_CF_CRAM;
    mem_block.offset        = 0x0;
    err = cfif->SetMemBlock(2, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem1>\n");
    }

    err = cfif->DeleteMemBlock (0);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem2>\n");
    }
    err = cfif->DeleteMemBlock (1);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem3>\n");
    }
    err = cfif->DeleteMemBlock (2);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem4>\n");
    }

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 10;
    mem_block.mblock_read_latency    = 10;
    mem_block.mblock_write_latency   = 10;
    mem_block.exo_cl_fetch_latency   = 10;
    mem_block.exo_cl_read_latency    = 10;
    mem_block.exo_cl_write_latency   = 10;
    mem_block.num_outstanding        = 1;
    mem_block.repeat_rate            = 2;
    mem_block.pe_id                  = 1;
    mem_block.start_address = 0xf3000000;
    mem_block.end_address   = 0xf400FFFF;
    mem_block.mblock_area   = cube::MI_CF_LOCAL_IOR;
    mem_block.offset        = 0x0;
    err = cfif->SetMemBlock(3, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem5>\n");
    }

    err = cfif->DeleteMemBlock (3);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at Mem6>\n");
    }
}


cube::CForestIF* GeneratePE (uint32_t clid, uint32_t peid, cube::CoreType core_type, uint32_t num_core)
{
    cube::CForestIF* cforestif = cube::MakeCoreModule (L"test path");
    cube::APIErrorCode err;

    err = cforestif->SetLocalIOROffsetAddr (0xfffee000, 0xfffeffff, -0x00002000);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr1>\n");
    }
    err = cforestif->SetLocalIOROffsetAddr (0xfffee000, 0xfffeffff, -0x00002000);
    if (err != cube::API_E_MULTI_ALLOC) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr2a>\n");
    }
    err = cforestif->SetLocalIOROffsetAddr (0xfffee000, 0xfffeffff, 0);
    if (err != cube::API_E_MULTI_ALLOC) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr2b>\n");
    }
    err = cforestif->SetLocalIOROffsetAddr (0xfffeffff, 0xfffe0000, 0);
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr3>\n");
    }

    err = cforestif->SetIORBaseAddr (cube::BaseAddr_INTC1, 0x00FFFEEA00ULL);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42A>\n");
    }
    err = cforestif->SetIORBaseAddr (cube::BaseAddr_INTC2, 0x00FFF80000ULL);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42B>\n");
    }

    for (uint32_t i = 0; i < num_core; i++) {
        err = cforestif->AddPEInfo (clid, peid + i, core_type);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error at 0>\n");
        }
    }

    err = cforestif->InstantiatePE ();
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 2>\n");
    }

    err = cforestif->HardReset (cube::ALL_PE, 0);

    return cforestif;
}


void TestFenmiIntcApi (void)
{
    { // Test for common G4MH1.x
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        // case1 : Request FENMI
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetFenmiToIntc1 (peid);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FENMI at TestFenmiIntcApi:1>\n");
            }
        }

        // case2 : Request FENMI with wrong PEID
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetFenmiToIntc1 (peid+1);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request FENMI at TestFenmiIntcApi:2>\n");
            }
        }

        // case3 : Request FENMI 2times
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetFenmiToIntc1 (peid);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FENMI at TestFenmiIntcApi:4A>\n");
            }
            err = cforestif->SetFenmiToIntc1 (peid);
            if (err != cube::API_E_MULTI_INTREQ) {
                printf ( "<Error: Request FENMI at TestFenmiIntcApi:4B>\n");
            }
        }

        cube::DestroyCoreModule ();
    }
}


void TestFeintIntcApi (void)
{
    { // Test for common G4MH1.x
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        // case1 : Request FEINT
        {
            for (uint32_t channel = 0; channel < 16; channel++) {
                cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
                err = cforestif->SetFeintToIntc1 (peid, channel);
                if (err != cube::API_E_OK) {
                    printf ( "<Error: Request FEINT at TestFeintIntcApi:1 channel=%u>\n", channel);
                }
            }
        }

        // case2 : Request FEINT with wrong PEID
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetFeintToIntc1 (peid+1, 0);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request FEINT at TestFeintIntcApi:2>\n");
            }
        }

        // case3 : Request FEINT with wrong channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetFeintToIntc1 (peid, 16);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request FEINT at TestFeintIntcApi:3>\n");
            }
        }

        // case4 : Request FEINT 2times for same channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetFeintToIntc1 (peid, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FEINT at TestFeintIntcApi:4A>\n");
            }
            err = cforestif->SetFeintToIntc1 (peid, 0);
            if (err != cube::API_E_MULTI_INTREQ) {
                printf ( "<Error: Request FEINT at TestFeintIntcApi:4B>\n");
            }
        }

        // case5 : Request FEINT 2times for different channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetFeintToIntc1 (peid, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FEINT at TestFeintIntcApi:5A>\n");
            }
            err = cforestif->SetFeintToIntc1 (peid, 1);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FEINT at TestFeintIntcApi:5B>\n");
            }
        }

        cube::DestroyCoreModule ();
    }
}


void TestEiintIntcApi (void)
{
    { // Test for common G4MH1.x
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        // case1 : Request EIINT
        {
            for (uint32_t channel = 0; channel < 32; channel++) {
                cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
                err = cforestif->SetInterruptToIntc1 (peid, channel);
                if (err != cube::API_E_OK) {
                    printf ( "<Error: Request EIINT at TestEiintIntcApi:1 channel=%u>\n", channel);
                }
            }
        }

        // case2 : Request EIINT with wrong PEID
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc1 (peid+1, 0);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request EIINT at TestEiintIntcApi:2>\n");
            }
        }

        // case3 : Request EIINT with wrong channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc1 (peid, 32);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request EIINT at TestEiintIntcApi:3>\n");
            }
        }

        // case4 : Request EIINT 2times for same channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc1 (peid, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestEiintIntcApi:4A>\n");
            }
            err = cforestif->SetInterruptToIntc1 (peid, 0);
            if (err != cube::API_E_MULTI_INTREQ) {
                printf ( "<Error: Request EIINT at TestEiintIntcApi:4B>\n");
            }
        }

        // case5 : Request EIINT 2times for different channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc1 (peid, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestEiintIntcApi:5A>\n");
            }
            err = cforestif->SetInterruptToIntc1 (peid, 1);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestEiintIntcApi:5B>\n");
            }
        }

        cube::DestroyCoreModule ();
    }
}


void TestEiintIntc2Api (void)
{
    { // Test for common G4MH1.x
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        // case1 : Request EIINT
        {
            for (uint32_t channel = 32; channel < 2048; channel++) {
                cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
                err = cforestif->SetInterruptToIntc2 (channel);
                if (err != cube::API_E_OK) {
                    printf ( "<Error: Request EIINT at TestEiintIntc2Api:1 channel=%u>\n", channel);
                }
            }
        }

        // case2 : Request EIINT with wrong channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc2 (31);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request EIINT at TestEiintIntc2Api:2>\n");
            }
        }

        // case3 : Request EIINT with wrong channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc2 (2048);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request EIINT at TestEiintIntc2Api:3>\n");
            }
        }

        // case4 : Request EIINT 2times for same channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc2 (1000);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestEiintIntc2Api:4A>\n");
            }
            err = cforestif->SetInterruptToIntc2 (1000);
            if (err != cube::API_E_MULTI_INTREQ) {
                printf ( "<Error: Request EIINT at TestEiintIntc2Api:4B>\n");
            }
        }

        // case5 : Request EIINT 2times for different channel
        {
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);
            err = cforestif->SetInterruptToIntc2 (1000);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestEiintIntc2Api:5A>\n");
            }
            err = cforestif->SetInterruptToIntc2 (1001);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestEiintIntc2Api:5B>\n");
            }
        }

        cube::DestroyCoreModule ();
    }
}


void TestPseudoInt (void)
{
    { // Test for common G4MH1.x
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        // case1: Cause EIINT
        for (uint32_t channel = 0; channel < 2048; channel++) {
            for (uint32_t priority = 0; priority < 16; priority++) {
                // Initialize
                cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

                // Request EIINT
                err = cforestif->ReqPseudoEiint (peid, channel, priority, false);
                if (err != cube::API_E_OK) {
                    printf ( "<Error: Request EIINT at TestPseudoInt:1A channel=%u>\n", channel);
                }

                // Release interruption masks. Write ID=NP=0.
                const CReg REG_PSW = 0x105;
                const uint32_t data = 0x0;
                err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
                if (err != cube::API_E_OK) {
                    printf ( "<Error: PSW write at TestPseudoInt:1B channel=%u>\n", channel);
                }

                // Accept EIINT
                cube::ExtTraceInfo* ext_trace_info;
                TraceInfo* p_trace;
                err = cforestif->StepExecute (&p_trace, &ext_trace_info);
                if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + channel))) {
                     printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:1C channel=%u, priority=%u>\n", p_trace->exp_cause, channel, priority);
                }
            }
        }

        { // case2
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request EIINT with incorrect PEID
            err = cforestif->ReqPseudoEiint (peid+1, 0, 0, false);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request EIINT at TestPseudoInt:2 channel=%u>\n", 0);
            }
        }

        { // case3
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request EIINT with incorrect channel
            err = cforestif->ReqPseudoEiint (peid, 2048, 0, false);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request EIINT at TestPseudoInt:3 channel=%u>\n", 2048);
            }
        }

        { // case4
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request EIINT with incorrect priority
            err = cforestif->ReqPseudoEiint (peid, 0, 16, false);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request EIINT at TestPseudoInt:4 channel=%u>\n", 0);
            }
        }

        { // case5
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request EIINT same channel 2times (error at 2nd request)
            err = cforestif->ReqPseudoEiint (peid, 0, 15, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:5A channel=%u>\n", 0);
            }
            err = cforestif->ReqPseudoEiint (peid, 0, 0, false);
            if (err != cube::API_E_MULTI_INTREQ) {
                printf ( "<Error: Request EIINT at TestPseudoInt:5B channel=%u>\n", 0);
            }
        }

        { // case6
            // Request EIINT different channel 2times (accept both with priority order) different priority
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            uint32_t higher_ch  = 1;
            uint32_t higher_pri = 14;
            uint32_t lower_ch  = 0;
            uint32_t lower_pri = 15;

            // Request EIINT different channel 2times (accept both with priority order) different priority
            err = cforestif->ReqPseudoEiint (peid, lower_ch, lower_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:6A channel=%u>\n", lower_ch);
            }
            err = cforestif->ReqPseudoEiint (peid, higher_ch, higher_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:6B channel=%u>\n", higher_ch);
            }

            // Release interruption masks. Write ID=NP=0.
            const CReg REG_PSW = 0x105;
            const uint32_t data = 0x0;
            err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:6C>\n");
            }

            // Don't update ISPR Release interruption masks. INTCFG.ISPC=1.
            const uint32_t REG_INTCFG = 0x14D;
            const uint32_t intcfg_data = 0x1;
            err = cforestif->RegWriteDebug (peid, REG_INTCFG, &intcfg_data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:6E>\n");
            }

            // Accept EIINT
            cube::ExtTraceInfo* ext_trace_info;
            TraceInfo* p_trace;
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + higher_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:6D channel=%u, priority=%u>\n", p_trace->exp_cause, higher_ch, higher_pri);
            }

            // Release interruption masks. Write ID=NP=0.
            err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:6E>\n");
            }

            // Accept EIINT
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + lower_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:6F channel=%u, priority=%u>\n", p_trace->exp_cause, lower_ch, lower_pri);
            }
        }

        { // case7
            // Request EIINT different channel 2times (accept both with priority order) same priority
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            uint32_t higher_ch  = 0;
            uint32_t higher_pri = 15;
            uint32_t lower_ch  = 1;
            uint32_t lower_pri = 15;

            // Request EIINT different channel 2times (accept both with priority order)
            err = cforestif->ReqPseudoEiint (peid, lower_ch, lower_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:7A channel=%u>\n", lower_ch);
            }
            err = cforestif->ReqPseudoEiint (peid, higher_ch, higher_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:7B channel=%u>\n", higher_ch);
            }

            // Release interruption masks. Write ID=NP=0.
            const CReg REG_PSW = 0x105;
            const uint32_t data = 0x0;
            err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:7C>\n");
            }

            // Don't update ISPR Release interruption masks. INTCFG.ISPC=1.
            const uint32_t REG_INTCFG = 0x14D;
            const uint32_t intcfg_data = 0x1;
            err = cforestif->RegWriteDebug (peid, REG_INTCFG, &intcfg_data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:6E>\n");
            }

            // Accept EIINT
            cube::ExtTraceInfo* ext_trace_info;
            TraceInfo* p_trace;
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + higher_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:7D channel=%u, priority=%u>\n", p_trace->exp_cause, higher_ch, higher_pri);
            }

            // Release interruption masks. Write ID=NP=0.
            err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:7E>\n");
            }

            // Accept EIINT
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + lower_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:7F channel=%u, priority=%u>\n", p_trace->exp_cause, lower_ch, lower_pri);
            }
        }

        cube::DestroyCoreModule ();
    }

    {
        // case1: Cause FEINT
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        for (uint32_t channel = 0; channel < 16; channel++) {
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request FEINT
            err = cforestif->ReqPseudoFeint (peid, channel);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FEINT at TestPseudoInt:1A channel=%u>\n", channel);
            }

            // Release interruption masks. Write ID=NP=0.
            const CReg REG_PSW = 0x105;
            const uint32_t data = 0x0;
            err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:1B channel=%u>\n", channel);
            }

            // Accept FEINT
            cube::ExtTraceInfo* ext_trace_info;
            TraceInfo* p_trace;
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0xF0 + channel))) {
                 printf ( "<Error: Accept FEINT (cause=%x) at TestPseudoInt:1C channel=%u>\n", p_trace->exp_cause, channel);
            }
        }

        {  // case2
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request FEINT with incorrect PEID
            err = cforestif->ReqPseudoFeint (peid + 1, 15);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request FEINT at TestPseudoInt:2 channel=%u>\n", 15);
            }
        }

        { // case3
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request FEINT with incorrect channel
            err = cforestif->ReqPseudoFeint (peid, 16);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request FEINT at TestPseudoInt:3 channel=%u>\n", 16);
            }
        }

        { // case4
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request FEINT 2times (error at 2nd request)
            err = cforestif->ReqPseudoFeint (peid, 15);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FEINT at TestPseudoInt:4A channel=%u>\n", 15);
            }
            err = cforestif->ReqPseudoFeint (peid, 14);
            if (err != cube::API_E_MULTI_INTREQ) {
                printf ( "<Error: Request FEINT at TestPseudoInt:4B channel=%u>\n", 14);
            }
        }

        cube::DestroyCoreModule ();
    }

    {
        // case1: Cause FENMI
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        {
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request FENMI
            err = cforestif->ReqPseudoFenmi (peid);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FENMI at TestPseudoInt:1A>\n");
            }

            // Accept FENMI
            cube::ExtTraceInfo* ext_trace_info;
            TraceInfo* p_trace;
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != 0xe0)) {
                 printf ( "<Error: Accept FENMI (cause=%x) at TestPseudoInt:1B>\n", p_trace->exp_cause);
            }
        }

        { // case2
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request FENMI with incorrect PEID
            err = cforestif->ReqPseudoFenmi (peid + 1);
            if (err != cube::API_E_PARAM) {
                printf ( "<Error: Request FENMI at TestPseudoInt:2>\n");
            }
        }

        { // case3
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            // Request FENMI 2times (error at 2nd request)
            err = cforestif->ReqPseudoFenmi (peid);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request FENMI at TestPseudoInt:3A>\n");
            }
            err = cforestif->ReqPseudoFenmi (peid);
            if (err != cube::API_E_MULTI_INTREQ) {
                printf ( "<Error: Request FENMI at TestPseudoInt:3B>\n");
            }
        }

        cube::DestroyCoreModule ();
    }

    { //Common1
        // Test for common G4MH1.x with 2PE
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 2);

        {//case1 Request EIINT different channel 2times (accept both for each PE)
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            uint32_t pe0_ch  = 0;
            uint32_t pe0_pri = 15;
            uint32_t pe1_ch  = 1;
            uint32_t pe1_pri = 15;

            // Request EIINT
            err = cforestif->ReqPseudoEiint (peid, pe0_ch, pe0_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:C11A channel=%u>\n", pe0_ch);
            }
            err = cforestif->ReqPseudoEiint (peid+1, pe1_ch, pe1_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:C11B channel=%u>\n", pe1_ch);
            }

            // Release interruption masks. Write ID=NP=0.
            const CReg REG_PSW = 0x105;
            const uint32_t data = 0x0;
            err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:C11C>\n");
            }
            err = cforestif->RegWriteDebug (peid+1, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:C11D>\n");
            }

            // Stop PE1
            err = cforestif->SetModeFrequency (peid+1, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Unexpected error at C11E>\n");
            }

            // Accept EIINT @ PE0
            cube::ExtTraceInfo* ext_trace_info;
            TraceInfo* p_trace;
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + pe0_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:C11F channel=%u, priority=%u>\n", p_trace->exp_cause, pe0_ch, pe0_pri);
            }

            // Stop PE0
            err = cforestif->SetModeFrequency (peid, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Unexpected error at C11G>\n");
            }
            // Start PE1
            err = cforestif->SetModeFrequency (peid+1, 1);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Unexpected error at C11H>\n");
            }

            // Accept EIINT @ PE1
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + pe1_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:C11I channel=%u, priority=%u>\n", p_trace->exp_cause, pe1_ch, pe1_pri);
            }

            // Start PE0 for next test
            err = cforestif->SetModeFrequency (peid, 1);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Unexpected error at C11J>\n");
            }
        }


        {//case2 Request EIINT same channel 2times (accept both for each PE)
            // Initialize
            cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

            uint32_t pe0_ch  = 0;
            uint32_t pe0_pri = 15;
            uint32_t pe1_ch  = 0;
            uint32_t pe1_pri = 15;

            // Request EIINT
            err = cforestif->ReqPseudoEiint (peid, pe0_ch, pe0_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:C12A channel=%u>\n", pe0_ch);
            }
            err = cforestif->ReqPseudoEiint (peid+1, pe1_ch, pe1_pri, false);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Request EIINT at TestPseudoInt:C12B channel=%u>\n", pe1_ch);
            }

            // Release interruption masks. Write ID=NP=0.
            const CReg REG_PSW = 0x105;
            const uint32_t data = 0x0;
            err = cforestif->RegWriteDebug (peid, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:C12C>\n");
            }
            err = cforestif->RegWriteDebug (peid+1, REG_PSW, &data);
            if (err != cube::API_E_OK) {
                printf ( "<Error: PSW write at TestPseudoInt:C12D>\n");
            }

            // Stop PE1
            err = cforestif->SetModeFrequency (peid+1, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Unexpected error at C12E>\n");
            }

            // Accept EIINT @ PE0
            cube::ExtTraceInfo* ext_trace_info;
            TraceInfo* p_trace;
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + pe0_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:C12F channel=%u, priority=%u>\n", p_trace->exp_cause, pe0_ch, pe0_pri);
            }

            // Stop PE0
            err = cforestif->SetModeFrequency (peid, 0);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Unexpected error at C12G>\n");
            }
            // Start PE1
            err = cforestif->SetModeFrequency (peid+1, 1);
            if (err != cube::API_E_OK) {
                printf ( "<Error: Unexpected error at C12H>\n");
            }

            // Accept EIINT @ PE1
            err = cforestif->StepExecute (&p_trace, &ext_trace_info);
            if ((err != cube::API_E_OK) || (p_trace->exp_cause != (0x1000 + pe1_ch))) {
                 printf ( "<Error: Accept EIINT (cause=%x) at TestPseudoInt:C12I channel=%u, priority=%u>\n", p_trace->exp_cause, pe1_ch, pe1_pri);
            }
        }

        cube::DestroyCoreModule ();
    }

    { // Test for G4MH1.0
        // PseudoAPI returns API_E_OK on G4MH1.0
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH, 1);

        // Initialize
        cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

        err = cforestif->ReqPseudoEiint (peid, 0, 0, false);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error in ReqPseudoEiint G4MH10 at TestPseudoInt>\n");
        }

        err = cforestif->ReqPseudoFenmi (peid);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error in ReqPseudoFenmi G4MH10 at TestPseudoInt>\n");
        }

        err = cforestif->ReqPseudoFeint (peid, 15);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error in ReqPseudoFeint G4MH10 at TestPseudoInt>\n");
        }
        cube::DestroyCoreModule ();
    }

    { // Test for G4MH1.1
        // PseudoAPI returns API_E_OK on G4MH1.1
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH11, 1);

        // Initialize
        cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

        err = cforestif->ReqPseudoEiint (peid, 0, 0, false);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error in ReqPseudoEiint G4MH11 at TestPseudoInt>\n");
        }

        err = cforestif->ReqPseudoFenmi (peid);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error in ReqPseudoFenmi G4MH11 at TestPseudoInt>\n");
        }

        err = cforestif->ReqPseudoFeint (peid, 15);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error in ReqPseudoFeint G4MH11 at TestPseudoInt>\n");
        }
        cube::DestroyCoreModule ();
    }

    { // Test for G4MH2.0
        // PseudoAPI returns API_E_PARAM on G4MH2.0
        uint32_t clid = 0;
        uint32_t peid = 0;
        cube::CForestIF* cforestif = GeneratePE (clid, peid, cube::G4MH20, 1);

        // Initialize
        cube::APIErrorCode err = cforestif->HardReset (cube::ALL_PE, 0);

        err = cforestif->ReqPseudoEiint (peid, 0, 0, false);
        if (err != cube::API_E_PARAM) {
            printf ( "<Error: Unexpected error in ReqPseudoEiint G4MH20 at TestPseudoInt>\n");
        }

        err = cforestif->ReqPseudoFenmi (peid);
        if (err != cube::API_E_PARAM) {
            printf ( "<Error: Unexpected error in ReqPseudoFenmi G4MH20 at TestPseudoInt>\n");
        }

        err = cforestif->ReqPseudoFeint (peid, 15);
        if (err != cube::API_E_PARAM) {
            printf ( "<Error: Unexpected error in ReqPseudoFeint G4MH20 at TestPseudoInt>\n");
        }
        cube::DestroyCoreModule ();
    }
}


int main (int argc, char *argv[])
{
    const uint32_t g4mh_peid = 0;
    const uint32_t mpu_area_num = 24;
    const uint32_t clid = 0; 

    cube::CForestIF* cforestif = cube::MakeCoreModule (L"test path");

    const CReg REG_SPID   = 0x120;
    const CReg REG_RBASE  = 0x122;
    const CReg REG_PEID   = 0x140;
    uint32_t reg_data;

    uint32_t addr;
    uint32_t data;

    cube::APIErrorCode err;
    err = cforestif->SetLocalIOROffsetAddr (0xfffee000, 0xfffeffff, -0x00002000);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr1>\n");
    }
    err = cforestif->SetLocalIOROffsetAddr (0xfffee000, 0xfffeffff, -0x00002000);
    if (err != cube::API_E_MULTI_ALLOC) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr2a>\n");
    }
    err = cforestif->SetLocalIOROffsetAddr (0xfffee000, 0xfffeffff, 0);
    if (err != cube::API_E_MULTI_ALLOC) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr2b>\n");
    }
    err = cforestif->SetLocalIOROffsetAddr (0xfffeffff, 0xfffe0000, 0);
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at SetLocalIOROffsetAddr3>\n");
    }
    err = cforestif->SetIORBaseAddr (cube::BaseAddr_INTC1, 0x00FFFEEA00ULL);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42A>\n");
    }
    err = cforestif->SetIORBaseAddr (cube::BaseAddr_INTC2, 0x00FFF80000ULL);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42B>\n");
    }
#if TEST_G3KH
#else
    err = cforestif->SetIORBaseAddr (cube::BaseAddr_MECNT, 0x00FFFEEC00UL);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42F>\n");
    }
#endif
    err = cforestif->SetIORBaseAddr (cube::BaseAddr_IPIR, 0x00FFFEEC80UL);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42G>\n");
    }

#ifdef ENABLE_BARRIER_CSP
    err = cforestif->SetIORBaseAddr (cube::BaseAddr_BAR, 0x00FFFB8000UL);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42H>\n");
    }
#endif // #ifdef ENABLE_BARRIER_CSP

    err = cforestif->AddPEInfo (clid, g4mh_peid, cube::G4MH);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 0>\n");
    }

    err = cforestif->AddPEInfo (clid, g4mh_peid + 1, cube::G4MH11);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 1>\n");
    }

    err = cforestif->InstantiatePE ();
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 2>\n");
    }

    reg_data = 0x0003;
    err = cforestif->SetPEInfo(g4mh_peid, cube::NC_SPID, reg_data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 4A>\n");
    }

    err = cforestif->SetModeFrequency (g4mh_peid + 1, 0);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 4A01>\n");
    }

    err = cforestif->HardReset (cube::ALL_PE, 0);
    err = cforestif->RegReadDebug (g4mh_peid, REG_SPID, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 4A1>\n");
    } else if (data != reg_data) {
        printf ( "<Error: Unexpected error at 4A2:%08x>\n", data);
    }

    reg_data = (0x12 << 9) | 0x1;
    err = cforestif->SetPEInfo(g4mh_peid, cube::NC_RBASE, reg_data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 4B>\n");
    }
    err = cforestif->HardReset (cube::ALL_PE, 0);
    err = cforestif->RegReadDebug (g4mh_peid, REG_RBASE, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 4B1>\n");
    } else if (data != reg_data) {
          printf ( "<Error: Unexpected error at 4B2:%08x>\n", data);
    }

    err = cforestif->HardReset (cube::ALL_PE, 0);
    reg_data = 0x00000000;
    err = cforestif->SetPEInfo(g4mh_peid, cube::NC_PEID, reg_data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 4D>\n");
    }
    err = cforestif->HardReset (cube::ALL_PE, 0);
    err = cforestif->RegReadDebug (g4mh_peid, REG_PEID, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 4D1>\n");
    } else if (data != reg_data) {
        printf ( "<Error: Unexpected error at 4D2:%08x>\n", data);
    }

    err = cforestif->SetPEInfo(g4mh_peid, cube::MPU_EXIST, true);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6A>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::MPU_EXIST, false);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6B>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::FPU_EXIST, true);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6C>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::FPU_EXIST, false);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6D>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::FXU_EXIST, true);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6E>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::FXU_EXIST, false);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6F>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::SNOOZE_COUNT, 10);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6G>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::MPU_REGION_NUMBER, mpu_area_num);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 5>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::NC_ICCTRL, true);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6P>\n");
    }
    err = cforestif->SetPEInfo(g4mh_peid, cube::NC_ICCTRL, false);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 6Q>\n");
    }
    uint32_t pipe_depth;
    err = cforestif->GetPEInfo (g4mh_peid, cube::PIPELINE_STAGE_NUMBER, &pipe_depth);
    if ((err != cube::API_E_OK) || (pipe_depth != 11)) {
        printf ( "<Error: Unexpected error at 6T>\n");
    }

    err = cforestif->HardReset (cube::ALL_PE, 0);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 7A>\n");
    }
    err = cforestif->HardReset (cube::S_ATTR_PE, 0);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 7B>\n");
    }
    err = cforestif->SetDebugMode (cube::DBG_RW);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 16A>\n");
    }

    TestDefaultMemMap (cforestif);
    TestDefaultMemMapG4MH (cforestif);

    // Check TraceInfo
    TraceInfo* p_trace_pre;
    cforestif->GetTraceInfo (&p_trace_pre);
    PhysAddr  next_inst_addr  = p_trace_pre->next_inst_addr;
    uint32_t  next_pe_id = p_trace_pre->next_pe_id;
    cube::ExtTraceInfo* ext_trace_info;

    TraceInfo* p_trace;
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 16B1>\n");
    } else {
        if (   (next_inst_addr != p_trace->inst_addr)
            || (next_pe_id != p_trace->pe_id) ) {
            printf ( "<Error: Unexpected error at 16B2>\n");
            printf ( "<PreExec: next PEId=%x, "
                    "addr=%08x>\n",
                next_pe_id,
                next_inst_addr);
            printf ( "<PostExec: now PEId=%x, "
                    "addr=%08x>\n",
                p_trace->pe_id,
                p_trace->inst_addr);
        }
    }
    uint32_t  first_peid = p_trace->pe_id;

    next_inst_addr  = p_trace->next_inst_addr;
    next_pe_id = p_trace->next_pe_id;
    cforestif->GetTraceInfo (&p_trace);
    if (   (next_inst_addr != p_trace->next_inst_addr)
        || (next_pe_id != p_trace->next_pe_id) ) {
        printf ( "<Error: Unexpected error at 16B3>\n");
        printf ( "<PostExec: next PEId=%x, "
                "addr=%08x>\n",
            next_pe_id,
            next_inst_addr);
        printf ( "<PostExec: next PEId=%x, "
                "addr=%08x>\n",
            p_trace->next_pe_id,
            p_trace->next_inst_addr);
    }

    err = cforestif->SetDebugMode (cube::DBG_ON);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 16C>\n");
    }

    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 16D>\n");
    } else {
        if (   (next_inst_addr != p_trace->inst_addr)
            || (next_pe_id != p_trace->pe_id) ) {
            printf ( "<Error: Unexpected error at 16D1>\n");
            printf ( "<PreExec: next PEId=%x, "
                    "addr=%08x>\n",
                next_pe_id,
                next_inst_addr);
            printf ( "<PostExec: now PEId=%x, "
                    "addr=%08x>\n",
                p_trace->pe_id,
                p_trace->inst_addr);
        }
    }
    uint32_t  second_peid = p_trace->pe_id;
    // 現在は１TCだけを起動しているので、他TCに実行が移ったら異常
    if (   (first_peid != second_peid) ) {
        printf ( "<Error: Unexpected error at 16E>\n");
    }

    err = cforestif->SetDebugMode (cube::DBG_OFF);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 16F>\n");
    }
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);

    addr = 0x00001000;
    data = 0x12345678;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_NOT_MAPPED) {
        printf ( "<Error: Unexpected error at 19A>\n");
    }

    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_NOT_MAPPED) {
        printf ( "<Error: Unexpected error at 19B>\n");
    }

    PhysAddr physaddr = 0x00001000;
    err = cforestif->PMemWriteDebug (physaddr, data, 4);
    if (err != cube::API_E_NOT_MAPPED) {
        printf ( "<Error: Unexpected error at 19C>\n");
    }

    err = cforestif->PMemReadDebug (physaddr, 4, &data);
    if (err != cube::API_E_NOT_MAPPED) {
        printf ( "<Error: Unexpected error at 19D>\n");
    }

    cube::MemoryBlock mem_block;

    mem_block.bank_size = NULL;
    mem_block.bank_size_count = 1;

    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.mblock_fetch_latency   = 0xFFFE;
    mem_block.mblock_read_latency    = 0xFFFD;
    mem_block.mblock_write_latency   = 0xFFFC;
    mem_block.cl_fetch_latency   = 0xFFFB;
    mem_block.cl_read_latency    = 0xFFFA;
    mem_block.cl_write_latency   = 0xFFF9;
    mem_block.exo_cl_fetch_latency   = 0xFFF8;
    mem_block.exo_cl_read_latency    = 0xFFF7;
    mem_block.exo_cl_write_latency   = 0xFFF6;
    mem_block.cl_id         = 0;
    mem_block.pe_id         = g4mh_peid;
    mem_block.start_address = 0x00000000;
    mem_block.end_address   = 0x01FFFFFF;
    mem_block.mblock_area   = cube::MI_CF_FROM;
    mem_block.offset        = 0x0;
    mem_block.num_outstanding = 0x2;
    mem_block.repeat_rate   = 0x4;
    err = cforestif->SetMemBlock(0, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 25>\n");
    }
    mem_block.start_address = 0x02000000;
    mem_block.end_address   = 0x0200FFFF;
    mem_block.mblock_restrict_type = MRT_WRITE_ENABLE;
    err = cforestif->SetMemBlock(0, mem_block); // duplicated id
    if (err != cube::API_E_MULTI_ALLOC) {
        printf ( "<Error: Unexpected error at 26A>\n");
    }
    mem_block.mblock_restrict_type = MRT_WRITE_ENABLE;
    err = cforestif->SetMemBlock(1, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 26B>\n");
    }
    mem_block.start_address = 0x02000000; // overrap address
    mem_block.end_address   = 0x0201FFFF;
    mem_block.mblock_restrict_type = MRT_FETCH_ENABLE;
    err = cforestif->SetMemBlock(2, mem_block);
    if (err != cube::API_E_MULTI_ALLOC) {
        printf ( "<Error: Unexpected error at 27A>\n");
    }
    mem_block.start_address = 0x02010000;
    mem_block.end_address   = 0x0201FFFF;
    mem_block.mblock_restrict_type = MRT_FETCH_ENABLE;
    err = cforestif->SetMemBlock(2, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 27B>\n");
    }
    mem_block.start_address = 0x02020000;
    mem_block.end_address   = 0x0202FFFF;
    mem_block.mblock_restrict_type = MRT_READ_ENABLE;
    err = cforestif->SetMemBlock(3, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 28>\n");
    }
    err = cforestif->GetMemBlock (0, &mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29>\n");
    }

    if (
        (mem_block.mblock_restrict_type != (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE))
        || (mem_block.mblock_fetch_latency  != 0xFFFE)
        || (mem_block.mblock_read_latency  != 0xFFFD)
        || (mem_block.mblock_write_latency   != 0xFFFC)
        || (mem_block.cl_fetch_latency   != 0xFFFB)
        || (mem_block.cl_read_latency   != 0xFFFA)
        || (mem_block.cl_write_latency   != 0xFFF9)
        || (mem_block.exo_cl_fetch_latency   != 0xFFF8)
        || (mem_block.exo_cl_read_latency   != 0xFFF7)
        || (mem_block.exo_cl_write_latency   != 0xFFF6)
        || (mem_block.cl_id                  != 0x0)
        || (mem_block.pe_id                  != g4mh_peid)
        || (mem_block.start_address != 0x00000000)
        || (mem_block.end_address   != 0x01FFFFFF)
        || (mem_block.mblock_area   != cube::MI_CF_FROM)
        || (mem_block.offset   != 0x0)
        || (mem_block.num_outstanding != 0x2)
        || (mem_block.repeat_rate != 0x4)
    ) {
        printf ( "<Error: Unexpected error at 29B>\n");
    }

    addr = 0x00001000;
    data = 0x12345678;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 19>\n");
    }
    data = 0;
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 20>\n");
    }
    if (data != 0x12345678){
        printf ( "<Error: Unexpected error at 20>\n");
    }
    data = 0x12345678;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 21>\n");
    }
    data = 0;
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 22>\n");
    }
    if (data != 0x12345678){
        printf ( "<Error: Unexpected error at 22>\n");
    }
    data = 0x12345678;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 23>\n");
    }
    data = 0;
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 24>\n");
    }
    if (data != 0x12345678){
        printf ( "<Error: Unexpected error at 24>\n");
    }
    PhysAddr paddr = 0;
    data = 0x12345699;
    err = cforestif->PMemWriteDebug (paddr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 23phys>\n");
    }
    data = 0;
    err = cforestif->PMemReadDebug (paddr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 24phys>\n");
    }
    if (data != 0x12345699){
        printf ( "<Error: Unexpected error at 24phys>\n");
    }

    // test self/mirror ram
    // LocalRAM
    // Body  : 0xFEA00000 - 0xFEA1FFFF (self + (-0x00200000) * peid)
    // Mirror: 0xFEA20000 - 0xFEBFFFFF (body + (-0x00200000) * peid + 0x20000 * n)
    // SELF        : 0xFEC00000 - 0xFEC1FFFF
    // SELF Mirror : 0xFEC20000 - 0xFEDFFFFF (self + 0x20000 * n)
    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.start_address = 0xFEC00000 - (g4mh_peid + 1) * 0x00200000;
    mem_block.end_address   = 0xFEC1FFFF - (g4mh_peid + 1) * 0x00200000;
    mem_block.mblock_area   = cube::MI_CF_LRAM;
    mem_block.offset        = 0x0;
    err = cforestif->SetMemBlock(4, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29C1>\n");
    }
    mem_block.start_address = 0xFEC00000;
    mem_block.end_address   = 0xFEC3FFFF;
    mem_block.mblock_area   = cube::MI_CF_SELF;
    mem_block.offset        = -0x00200000;
    err = cforestif->SetMemBlock(5, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29C2>\n");
    }
    // Write to self
    addr = 0xFEC00000;
    data = 0xBEEFBEEF;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29D>\n");
    }
    data = 0;
    // Read from self
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29E>\n");
    }
    if (data != 0xBEEFBEEF){
        printf ( "<Error: Unexpected error at 29F>\n");
    }
    // Read from body
    addr = 0xFEC00000 - g4mh_peid * 0x00200000;
    data = 0;
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29G>\n");
    }
    if (data != 0xBEEFBEEF){
        printf ( "<Error: Unexpected error at 29H>\n");
    }

    // test ior
    mem_block.start_address = 0x80000000;
    mem_block.end_address   = 0x8000FFFF;
    mem_block.mblock_area   = cube::MI_CF_IOR;
    mem_block.offset        = 0x00000000;
    mem_block.mblock_read_latency    = 0x1;
    mem_block.mblock_write_latency   = 0x2;
    mem_block.exo_cl_read_latency    = 0x3;
    mem_block.exo_cl_write_latency   = 0x4;
    err = cforestif->SetMemBlock(7, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29M>\n");
    }
    // Write
    addr = 0x80000000;
    data = 0xBEEFBEEF;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29N>\n");
    }
    data = 0;
    // Read
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29O>\n");
    }

    // test allocated ior after delete ior area
    err = cforestif->DeleteMemBlock(7);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29P>\n");
    }
    // Write
    addr = 0x80000000;
    data = 0xDEADBEEF;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_NOT_MAPPED) {
        printf ( "<Error: Unexpected error at 29Q>\n");
    }
    data = 0;
    // Read
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_NOT_MAPPED) {
        printf ( "<Error: Unexpected error at 29R>\n");
    }

    // test local ior
    mem_block.start_address = 0x80000000;
    mem_block.end_address   = 0x8000FFFF;
    mem_block.mblock_area   = cube::MI_CF_LOCAL_IOR;
    mem_block.offset        = 0x00000000;
    mem_block.mblock_read_latency    = 0x5;
    mem_block.mblock_write_latency   = 0x6;
    mem_block.exo_cl_read_latency    = 0x7;
    mem_block.exo_cl_write_latency   = 0x8;
    err = cforestif->SetMemBlock(8, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29S>\n");
    }

    // Write
    addr = 0x80000000;
    data = 0xDEADBEEF;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29T>\n");
    }
    data = 0;
    // Read
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29U>\n");
    } else if (data != 0xDEADBEEF) {
        printf ( "<Error: Unexpected error at 29V>\n");
    }

    // test local ior2
    mem_block.start_address = 0xFFFEC000;
    mem_block.end_address   = 0xFFFEDFFF;
    mem_block.mblock_area   = cube::MI_CF_LOCAL_IOR;
    mem_block.offset        = 0x00000000;
    err = cforestif->SetMemBlock(12, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29W>\n");
    }
    // Write
    addr = 0xFFFEDFF0;
    data = 0xDEADBEEF;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29X>\n");
    }
    data = 0;
    // Read
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 29Y>\n");
    } else if (data != 0xDEADBEEF) {
        printf ( "<Error: Unexpected error at 29Z>\n");
    }

    data = 0x12345678;
    err = cforestif->RegWriteDebug (g4mh_peid, PC, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 30>\n");
    }
    err = cforestif->RegReadDebug (g4mh_peid, PC, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 30A>\n");
    }
    if (data != 0x12345678){
        printf ( "<Error: Unexpected error at 30B>\n");
    }

    // SR_RBASE = 0x122
    data = 0x12345000;
    err = cforestif->RegWriteDebug (g4mh_peid, 0x122, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 31>\n");
    }
    err = cforestif->RegReadDebug (g4mh_peid, 0x122, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 31A>\n");
    }
    if (data != 0x2401){ // specified by SetPeInfo.
        printf ( "<Error: Unexpected error at 31B>\n");
    }

    // SR_EIPC = 0x100
    data = 0x12346000;
    err = cforestif->RegWriteDebug (g4mh_peid, 0x100, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 32>\n");
    }
    err = cforestif->RegReadDebug (g4mh_peid, 0x100, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 33>\n");
    }
    if (data != 0x12346000){
        printf ( "<Error: Unexpected error at 33>\n");
    }

    data = 0x1234568F;
    err = cforestif->RegWriteDebug (g4mh_peid, PC, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 30>\n");
    }
    err = cforestif->RegReadDebug (g4mh_peid, PC, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 35>\n");
    }
    if (data != 0x1234568E){ // PC[0]=0固定
        printf ( "<Error: Unexpected error at 35>\n");
    }

    struct cube::CacheSpec cachespec;
    cachespec.size = cube::CACHE_SIZE_64K;
    cachespec.way  = cube::CACHE_WAY_4;
    cachespec.ecc  = cube::ECC_EXIST;
    err = cforestif->SetICacheSpec (g4mh_peid, cachespec);
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at 36A>\n");
    }

    cachespec.size = cube::CACHE_SIZE_1K;
    err = cforestif->SetICacheSpec (g4mh_peid, cachespec);
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at 36B>\n");
    }

    cachespec.size = cube::CACHE_SIZE_32K;
    err = cforestif->SetICacheSpec (g4mh_peid, cachespec);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 36C>\n");
    }

    cachespec.size = cube::CACHE_SIZE_8K;
    err = cforestif->SetICacheSpec (g4mh_peid, cachespec);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 36D>\n");
    }

    cachespec.size = cube::CACHE_SIZE_16K;
    err = cforestif->SetICacheSpec (g4mh_peid, cachespec);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 36E>\n");
    }

    struct cube::CacheSpec cachespec_get = {};
    err = cforestif->GetICacheSpec (g4mh_peid,  &cachespec_get);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 37>\n");
    }
    if (  (cachespec_get.size != cachespec.size)
       || (cachespec_get.way  != cachespec.way)
       || (cachespec_get.ecc  != cachespec.ecc) ) {
        printf ( "<Error: Unexpected error at 37B>\n");
    }

    // Check hit/miss count is incremented
    err = cforestif->HardReset (cube::ALL_PE, 0);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38A>\n");
    }
    cube::CacheInfo cacheinfo;
    err = cforestif->GetICacheInfo (g4mh_peid, &cacheinfo);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38B>\n");
    } else if (  (cacheinfo.Hit_count != 0)
              || (cacheinfo.Miss_count != 0) ) {
        printf ( "<Error: Unexpected error at 38C>\n");
    }
    // Enable IC
    CReg SR_ICCTRL = 0x198;
    data = 0x1;
    err = cforestif->RegWriteDebug (g4mh_peid, SR_ICCTRL, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38D>\n");
    }
    // Cause IC miss
    data = 0;
    err = cforestif->RegWriteDebug (g4mh_peid, PC, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38J>\n");
    }
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38E>\n");
    }
    err = cforestif->GetICacheInfo (g4mh_peid, &cacheinfo);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38F>\n");
    } else if (  (cacheinfo.Hit_count != 0)
              || (cacheinfo.Miss_count != 1) ) {
        printf ( "<Error: Unexpected error at 38G>\n");
    }
    // Cause IC miss (To cause IC hit, fetch different line)
    data = 0x100;
    err = cforestif->RegWriteDebug (g4mh_peid, PC, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38H>\n");
    }
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38I>\n");
    }
    // Cause IC hit
    data = 0;
    err = cforestif->RegWriteDebug (g4mh_peid, PC, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38J>\n");
    }
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38K>\n");
    }
    err = cforestif->GetICacheInfo (g4mh_peid, &cacheinfo);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38L>\n");
    } else if (  (cacheinfo.Hit_count != 1)
              || (cacheinfo.Miss_count != 2) ) {
        printf ( "<Error: Unexpected error at 38M>\n");
    }

    // Check IC size = 0
    cachespec.size = cube::CACHE_SIZE_0;
    cachespec.way  = cube::CACHE_WAY_0;
    cachespec.ecc  = cube::ECC_EXIST;
    err = cforestif->SetICacheSpec (g4mh_peid, cachespec);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38N>\n");
    }
    err = cforestif->HardReset (cube::ALL_PE, 0);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38O>\n");
    }
    data = 0x1;
    err = cforestif->RegWriteDebug (g4mh_peid, SR_ICCTRL, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38P>\n");
    }
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 38Q>\n");
    }

    CReg R_NONE = -1;
    CReg SR_MPAT15 = 0x1FE;
    for (CReg sregno = R_NONE + 1; sregno < SR_MPAT15; ++sregno) {
        SRegTest (cforestif, g4mh_peid, sregno);
    }

    // Invalid peid reg access
    for (CReg sregno = R_NONE + 1; sregno < SR_MPAT15; ++sregno) {
        SRegTest (cforestif, 2, sregno);
    }
    for (CReg sregno = R_NONE + 1; sregno < SR_MPAT15; ++sregno) {
        SRegTest (cforestif, 4, sregno);
    }
    for (CReg sregno = R_NONE + 1; sregno < SR_MPAT15; ++sregno) {
        SRegTest (cforestif, 8, sregno);
    }

    uint32_t channel;
    for (channel = 0; channel < 32; channel++) {
        err = cforestif->SetInterruptToIntc1 (g4mh_peid, channel);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error at 43A:channel%u>\n", channel);
        }
    }
    for (channel = 32; channel < 35; channel++) {
        physaddr = 0xFFF82000;
        data     = 0x0;
        err = cforestif->PMemWriteDebug (physaddr + 4 * channel, data, 4);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error at 43B:channel%u>\n", channel);
        }

        err = cforestif->SetInterruptToIntc2 (channel);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error at 43C:channel%u>\n", channel);
        }
    }

    err = cforestif->HardReset (cube::ALL_PE, 0);

    for (channel = 0; channel < 32; channel++) {
        err = cforestif->ReqPseudoEiint (g4mh_peid, channel, 10, false);
        if (err != cube::API_E_OK) {
            printf ( "<Error: Unexpected error at 43D>\n");
        }
    }

    err = cforestif->ReqPseudoFenmi (g4mh_peid);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 43E>\n");
    }

    err = cforestif->ReqPseudoFeint (g4mh_peid, 16);
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at 43F1>\n");
    }

    err = cforestif->ReqPseudoFeint (g4mh_peid, 15);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 43F2>\n");
    }

    err = cforestif->ReqPseudoSyserr (g4mh_peid, 0x10);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 43G>\n");
    }

    err = cforestif->ReqPseudoOperandSyserr(g4mh_peid, 0x10, false, 0);

    // FENMI
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if ((err != cube::API_E_OK) || (p_trace->exp_cause != 0xe0)) {
        printf ( "<Error: Unexpected error at 43H>\n");
    }

    //SYSERR(NM)
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if ((err != cube::API_E_OK) || (p_trace->exp_cause != 0x10)) {
        printf ( "<Error: Unexpected error at 43I>\n");
    }

    // No exception
    err = cforestif->StepExecute (&p_trace, &ext_trace_info);
    if ((err != cube::API_E_OK) || (p_trace->exp_cause != 0x0)) {
        printf ( "<Error: Unexpected error at 43J>\n");
    }

    // Check access disable
    // Set Body
    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE |
                                      MRT_FETCH_ENABLE | MRT_SELF_ONLY);
    mem_block.start_address = 0x10010000;
    mem_block.end_address   = 0x100FFFFF;
    mem_block.mblock_area   = cube::MI_CF_CRAM;
    mem_block.offset        = 0x0;
    err = cforestif->SetMemBlock (9, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44A>\n");
    }
    // Set Self
    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE);
    mem_block.start_address = 0x10000000;
    mem_block.end_address   = 0x1000FFFF;
    mem_block.mblock_area   = cube::MI_CF_SELF;
    mem_block.offset        = 0x10000;
    err = cforestif->SetMemBlock (10, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44B>\n");
    }
    // Access from Self
    addr = 0x10000000;
    data = 0x12345678;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44C>\n");
    }
    data = 0;
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44D>\n");
    }
    if (data != 0x12345678){
        printf ( "<Error: Unexpected error at 44E>\n");
    }
    // Access from virtual addr of Body (Can't access)
    addr = 0x10010000;
    data = 0x87654321;
    err = cforestif->VMemWriteDebug (g4mh_peid, addr, data, 4);
    if (err == cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44F>\n");
    }
    data = 0;
    err = cforestif->VMemReadDebug (g4mh_peid, addr, 4, &data);
    if (err == cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44G>\n");
    }
    if (data == 0x87654321){
        printf ( "<Error: Unexpected error at 44H>\n");
    }
    // Access from physical addr of Body (Can't access)
    addr = 0x10010000;
    data = 0x87654322;
    err = cforestif->PMemWriteDebug (addr, data, 4);
    if (err == cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44I>\n");
    }
    data = 0;
    err = cforestif->PMemReadDebug (addr, 4, &data);
    if (err == cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44J>\n");
    }
    if (data == 0x87654322){
        printf ( "<Error: Unexpected error at 44K>\n");
    }

    // Set MRT_OWN_PE_ONLY
    mem_block.mblock_restrict_type = (MRT_READ_ENABLE | MRT_WRITE_ENABLE |
                                      MRT_FETCH_ENABLE | MRT_OWN_PE_ONLY);
    mem_block.start_address = 0x10200000;
    mem_block.end_address   = 0x102FFFFF;
    mem_block.mblock_area   = cube::MI_CF_CRAM;
    mem_block.pe_id = g4mh_peid;
    mem_block.offset        = 0x0;
    err = cforestif->SetMemBlock (11, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44L>\n");
    }
    // Access from Own PE
    addr = 0x10200000;
    data = 0x12345678;
    err = cforestif->VMemWriteDebug (1, addr, data, 4);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44M>\n");
    }
    data = 0;
    err = cforestif->VMemReadDebug (1, addr, 4, &data);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 44N>\n");
    }
    if (data != 0x12345678){
        printf ( "<Error: Unexpected error at 44O>\n");
    }

    // test local ior (4KB check)
    mem_block.start_address = 0xfffea000;
    mem_block.end_address   = 0xfffebfff;
    mem_block.mblock_area   = cube::MI_CF_LOCAL_IOR;
    mem_block.offset        = 0x00000000;
    mem_block.pe_id         = g4mh_peid;
    err = cforestif->SetMemBlock(0x15, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 45A>\n");
    }
    mem_block.start_address = 0xfffea000;
    mem_block.end_address   = 0xfffebfff;
    mem_block.mblock_area   = cube::MI_CF_LOCAL_IOR;
    mem_block.offset        = 0x00000000;
    mem_block.pe_id         = g4mh_peid;
    err = cforestif->SetMemBlock(0x16, mem_block);
    if (err != cube::API_E_MULTI_ALLOC) {
        printf ( "<Error: Unexpected error at 45B>\n");
    }
    mem_block.start_address = 0xfffe8000;
    mem_block.end_address   = 0xfffe9fff;
    mem_block.mblock_area   = cube::MI_CF_LOCAL_IOR;
    mem_block.offset        = 0x00000000;
    mem_block.pe_id         = g4mh_peid + 1;
    err = cforestif->SetMemBlock(0x17, mem_block);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 45C>\n");
    }

    mem_block.start_address = 0x10300000;
    mem_block.end_address   = 0x103FFFFF;
    mem_block.mblock_area   = cube::MI_CF_CRAM;
    mem_block.pe_id         = g4mh_peid + 2; // invalid id
    err = cforestif->SetMemBlock(0x18, mem_block);
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at 45D>\n");
    }

    err = cforestif->SetFileOut (L"test.txt");
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 16A2>\n");
    }

    // check G3M enable
    err = cforestif->SetModeFrequency (g4mh_peid, 1);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 46A>\n");
    }
    err = cforestif->GetTraceInfo (&p_trace);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 46C>\n");
    }
    err = cforestif->GetPeTraceInfo (g4mh_peid, &p_trace);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 46D>\n");
    }
    err = cforestif->GetPeTraceInfo (g4mh_peid + 2, &p_trace); // invalid peid
    if (err != cube::API_E_PARAM) {
        printf ( "<Error: Unexpected error at 46F>\n");
    }

    // check G3M disable
    err = cforestif->SetModeFrequency (g4mh_peid, 0);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 46G>\n");
    }
    err = cforestif->GetTraceInfo (&p_trace);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 46I>\n");
    }
    err = cforestif->GetPeTraceInfo (g4mh_peid, &p_trace);
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 46J>\n");
    }

    err = cube::DestroyCoreModule ();
    if (err != cube::API_E_OK) {
        printf ( "<Error: Unexpected error at 42>\n");
    }

    TestPseudoInt ();
    TestEiintIntcApi ();
    TestFeintIntcApi ();
    TestFenmiIntcApi ();
    TestEiintIntc2Api ();

    exit(EXIT_NORMAL);
}
