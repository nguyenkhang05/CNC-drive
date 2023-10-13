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

class ForestUtil;
class ForestCmdOpt;

class GdbStub {
public:
    typedef void (*sighandler_t)(int);

    ForestUtil* m_forest_util;
    ForestCmdOpt* m_cmdopt;

    static const uint32_t BREAK_ADDR_MASK = 0xFFFFFFFF;

    /* BUFMAX is Max packet size(including '\0'). */
    static const uint32_t BUFMAX = 1024*3;

    // gコマンドに対するレジスタダンプ制限
    //   知らないレジスタ番号をgコマンドに返すとGDBは終了してしまう。
    //   FSF版GDBはFP(=65)まで、kpit版はSR115(=148)（本当は149)まで対応。
    //   必要ならGDBはpコマンドで追加取得するので、
    //   gコマンドで返すのは最低限の65までに制限する
    static const uint32_t N_G_CMD_LIMIT = 65;

    static const uint32_t N_GDB_REG = 695;
    static const uint32_t LEN_REGNAME = 17;

    // システムレジスタと汎用レジスタのIDが重なっているので、
    // reglist[]内の位置で区別する
    static const CReg GREG_POS = 32;
    static const CReg PC_MIRROR_POS = 64;
    static const CReg R29_MIRROR_POS = 65;
    static const CReg WREG_POS_START = 598;
    static const CReg WREG_POS_END = 629;

    static const CReg reglist[N_GDB_REG];
    static const char regname[N_GDB_REG][LEN_REGNAME];

    // Thread-id/Ht-idの特殊番号
    // 通常はThreadid = htid + 1だが一部特殊扱い
    typedef int32_t ThreadIdType;
    typedef int32_t HtIdType;
    static const HtIdType   GDB_HTID_INIT      =  0;
    static const HtIdType   GDB_HTID_ALL       = -1;
    static const HtIdType   GDB_HTID_ANY       = -2;
    static const HtIdType   GDB_HTID_INVALID   = -3;
    static const ThreadIdType GDB_THREAD_0       =  GDB_HTID_INIT + 1;
    static const ThreadIdType GDB_THREAD_ANY     =  0;
    static const ThreadIdType GDB_THREAD_ALL     = -1;
    static const ThreadIdType GDB_THREAD_INVALID = -2;

    enum ThSelType {
        SEL_ALL_TH,
        SEL_SPECIFIC_TH
    };

    bool    m_quit;     /* If 1: quit(disconnect) GDB */
    ThreadIdType m_run_thread;
    ThreadIdType m_debug_thread;
    HtIdType m_qthreadinfo_next_htid;
    HtIdType m_break_htid;

    class VcontArg {
    public:
        bool step;
        int32_t htid;
        int32_t signal;
    };

    enum EndianType {
        ENDIAN_BIG,
        ENDIAN_LITTLE
    };
    EndianType GetEndian (void) const;

    HtIdType GdbCheckValidHtid(HtIdType htid) const;
    ThreadIdType GdbGetThreadidFromHtid(HtIdType htid) const;
    HtIdType GdbGetHtidFromThreadid(ThreadIdType threadid) const;
    int32_t CharToHex (char ch) const;
    uint32_t ChangeEndianForGDB (int32_t byte_size, uint32_t data) const;
    char GetGdbChar (int client_sockfd);
    size_t GetGdbStr (int client_sockfd, size_t length, char* buff);
    void PutGdbChar (int client_sockfd, char ch);
    char* GetGdbPacket (int client_sockfd, char* cmd_buffer);
    void PutGdbPacket (int client_sockfd, char* cmd_buffer);
    int32_t HexToUintN (char** ptr, uint32_t* intValue, int32_t length) const;
    int32_t HexToUint128 (char** ptr, uint64_t* data_l, uint64_t* data_u) const;
    int32_t HexToUint (char** ptr, uint32_t* intValue) const;
    int32_t HexToInt (char** ptr, int32_t* intValue) const;
    void GdbQuerry (char* inbuf, char* outbuf);
    CReg ChangeRegId (int32_t regno);
    void GdbRegRead (HtIdType htid, char* inbuf, char* outbuf);
    void GdbRegReadAll (HtIdType htid, char* inbuf, char* outbuf);
    void GdbRegWrite (HtIdType htid, char* inbuf, char* outbuf);
    void GdbAllTcPcWrite (RegData data);
    void GdbSetBreakPoint (char* inbuf, char* outbuf);
    void GdbClearBreakPoint (char* inbuf, char* outbuf);
    void GdbMemWrite (HtIdType htid, char* inbuf, char* outbuf);
    void GdbMemWriteBinary (HtIdType htid, char* inbuf, char* outbuf);
    void GdbMemRead (HtIdType htid, char* inbuf, char* outbuf);
    void GdbStepExec(HtIdType htid, char* outbuf);
    void GdbContinueExec(HtIdType htid, char* outbuf);
    void GdbContinue (HtIdType htid, char* inbuf, char* outbuf);
    void GdbStep (HtIdType htid, char* inbuf, char* outbuf);
    void GdbCheckThreadAlive(char* p_inbuf, char* outbuf);
    void GdbThreadSubsequentOperation(char* p_inbuf, char* outbuf);
    int ParseVcontPacket(char** p_inbuf, VcontArg* vcont_arg);
    void GdbVCont(char* p_inbuf, char* outbuf);
    void GdbGetSignalNumber(char* p_inbuf, char* outbuf);
    void GdbCmdExec (int client_sockfd);
    void ConnectGDB (uint16_t port);
    void GdbSetRunThread(ThreadIdType threadid);
    ThreadIdType GdbGetRunThread(void) const;
    void GdbSetDebugThread(ThreadIdType threadid);
    ThreadIdType GdbGetDebugThread(void) const;
    HtIdType GdbGetRunHtid(void) const;
    HtIdType GdbGetDebugHtid(void) const;

    GdbStub (ForestUtil* forest_util, ForestCmdOpt* cmdopt);
    ~GdbStub () {}
};

