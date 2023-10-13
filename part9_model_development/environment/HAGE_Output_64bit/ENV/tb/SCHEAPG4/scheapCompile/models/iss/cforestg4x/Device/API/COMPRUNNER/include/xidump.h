#if defined(COMP_RUNNER)
#include "./svdpi.h"
#endif // defined(COMP_RUNNER)

#include <vector>
#include "./forest_common.h"

#pragma once


/** \file xidump.h
 * \brief CompRunner XDump function header file.
 *
 * This file contains the class, variable in XDump function.
 */


class ForestUtil;

/**
 * \class XDUMP_ARITH
 *
 * \brief XDump for arithmetic class.
 *
 */
class XDUMP_ARITH {
public:
    /**
     * \var uint32_t m_insn_id
     * \brief instruction id for using XDump for arithmetic.
     */
    uint32_t m_insn_id;
    /**
     * \var int32_t m_diff
     * \brief different limitation for using XDump for arithmetic.
     */
    int32_t m_diff;

public:
    /**
     * \fn XDUMP_ARITH(uint32_t insn_id = 0, int32_t diff = 0)
     * \brief Create XDUMP_ARITH object with instruction ID and different info.
     *
     * @param[in]  insn_id                 instruction ID.
     * @param[in]  diff                    different info.
     * @return None.
     */
    XDUMP_ARITH(uint32_t insn_id = 0, int32_t diff = 0)
    : m_insn_id(insn_id), m_diff(diff) {}

    ~XDUMP_ARITH() {}
};

#if defined(COMP_RUNNER_G3KH)
class XDUMP_FPU_MIN {
public:
    uint32_t m_insn_id;

public:
    XDUMP_FPU_MIN (uint32_t insn_id = 0)
    : m_insn_id(insn_id) {}

    ~XDUMP_FPU_MIN () {};
};
#endif // defined(COMP_RUNNER_G3KH)

/**
 * \class XDUMP_ADDR
 *
 * \brief XDump for Address store by xdump register.
 *
 */
class XDUMP_ADDR {
public:
    /**
     * \var PhysAddr m_start_addr
     * \brief start address of xdump address store by xdump register.
     */
    PhysAddr m_start_addr;

    /**
     * \var ByteSize m_end_addr
     * \brief end address of xdump address store by xdump register.
     */
    PhysAddr m_end_addr;

    /**
     * \var uint32_t m_store_type
     * \brief store type: word, hword, byte
     */
    uint32_t m_store_type;

    /**
     * \var CReg m_xdump_regno
     * \brief xdump register number.
     */
    CReg m_xdump_regno;

    /**
     * \var uint32_t m_nblk_id
     * \brief xdump non-blocking buffer id
     */
    uint32_t m_nblk_id;


    /**
     * \var uint32_t m_xdump_mask
     * \brief xdump mask data
     */
    uint32_t m_xdump_mask;

public:
    /**
     * \fn XDUMP_ADDR(PhysAddr start_addr = 0, PhysAddr end_addr=0, uint32_t store_type=0 , CReg xdump_reg=0, uint32_t nblk_id=0, uint32_t xdump_mask=0)
     * \brief Create XDUMP_ADDR object.
     *
     * @param[in]  start_addr              start address store.
     * @param[in]  end_addr                end address store.
     * @param[in]  store type              Store size.
     * @param[in]  xdump_reg               Xdump register number.
     * @param[in]  nblk_id                 non blocking buffer id.
     * @param[in]  xdump_mask              Xdump mask value.
     * @return None.
     */
    XDUMP_ADDR(PhysAddr start_addr = 0, PhysAddr end_addr=0, uint32_t store_type=0 , CReg xdump_reg=0,
               uint32_t nblk_id=0, uint32_t xdump_mask=0)
    : m_start_addr(start_addr), m_end_addr(end_addr), m_store_type(store_type), m_xdump_regno(xdump_reg),
      m_nblk_id(nblk_id), m_xdump_mask(xdump_mask) {}

    ~XDUMP_ADDR() {}
};

/**
 * \class XDUMP_REG
 *
 * \brief XDump for register class.
 *
 */
class XDUMP_REG {
public:
    /**
     * \var uint64_t m_start_addr
     * \brief start address for using XDump register.
     */
    uint64_t m_start_addr;
    /**
     * \var uint64_t m_end_addr
     * \brief end address for using XDump register.
     */
    uint64_t m_end_addr;

public:
    /**
     * \fn XDUMP_REG(uint64_t start_addr = 0, uint64_t end_addr = 0)
     * \brief Create XDUMP_REG object with start and end address.
     *
     * @param[in]  start_addr              start address.
     * @param[in]  end_addr                end address.
     * @return None.
     */
    XDUMP_REG(uint64_t start_addr = 0, uint64_t end_addr = 0)
    : m_start_addr(start_addr), m_end_addr(end_addr) {}

    ~XDUMP_REG() {}
};


/**
 * \class XDUMP_FLAG
 *
 * \brief XDump for flag class.
 */
class XDUMP_FLAG {
public:
    /**
     * \var uint64_t m_start_addr
     * \brief start address for using XDump flag.
     */
    uint64_t m_start_addr;
    /**
     * \var uint64_t m_end_addr
     * \brief end address for using XDump flag.
     */
    uint64_t m_end_addr;

public:
    /**
     * \fn XDUMP_FLAG(uint64_t start_addr = 0, uint64_t end_addr = 0)
     * \brief Create XDUMP_FLAG object with start and end address.
     *
     * @param[in]  start_addr              start address.
     * @param[in]  end_addr                end address.
     * @return None.
     */
    XDUMP_FLAG(uint64_t start_addr = 0, uint64_t end_addr = 0)
    : m_start_addr(start_addr), m_end_addr(end_addr) {}

    ~XDUMP_FLAG() {}
};


/**
 * \class XDUMP_DATA
 *
 * \brief XDump for register class.
 *
 */
class XDUMP_DATA {
public:
    /**
     * \var uint64_t m_start_addr
     * \brief start address for using XDump data.
     */
    uint64_t m_start_addr;
    /**
     * \var uint64_t m_end_addr
     * \brief end address for using XDump data.
     */
    uint64_t m_end_addr;
    /**
     * \var uint64_t m_mask
     * \brief dumping bits for using XDump data.
     */
    uint64_t m_mask;

public:
    /**
     * \fn XDUMP_DATA(uint64_t start_addr = 0, uint64_t end_addr = 0, uint64_t mask = 0)
     * \brief Create XDUMP_DATA object with start and end address.
     *
     * @param[in]  start_addr              start address.
     * @param[in]  end_addr                end address.
     * @param[in]  mask                    dumping bits.
     * @return None.
     */
    XDUMP_DATA(uint64_t start_addr = 0, uint64_t end_addr = 0, uint64_t mask = 0)
    : m_start_addr(start_addr), m_end_addr(end_addr), m_mask(mask) {}

    ~XDUMP_DATA() {}
};

/**
 * \class XDUMP_SYSREG
 *
 * \brief XDump for system register class.
 *
 */
class XDUMP_SYSREG {
public:
    /**
     * \var uint32_t m_reg_id
     * \brief reg_id of Xdump system register
     */
    uint32_t m_reg_id;
    /**
     * \var uint32_t m_sel_id
     * \brief sel_id of Xdump system register
     */
    uint32_t m_sel_id;
    /**
     * \var uint64_t m_mask
     * \brief dumping bits for using XDump system register.
     */
    uint64_t m_mask;

public:
    /**
     * \fn XDUMP_SYSREG(uint32_t reg_id = 0, uint32_t sel_id = 0, uint64_t mask = 0)
     * \brief Create XDUMP_SYSREG object with reg_id, sel_id and mask value.
     *
     * @param[in]  reg_id                reg_id.
     * @param[in]  sel_id                sel_id.
     * @param[in]  mask                  mask value.
     * @return None.
     */
    XDUMP_SYSREG(uint32_t reg_id = 0, uint32_t sel_id = 0, uint64_t mask = 0)
    : m_reg_id(reg_id), m_sel_id(sel_id), m_mask(mask) {}

    ~XDUMP_SYSREG() {}
};

/**
 * \class XDUMP
 *
 * \brief XDump operantion class.
 */
class XDUMP {
public:
    /**
     * \parameter Parameter for new class Xdump Address updated by Xdump register
     * \brief parameter.
     */
    static const uint32_t DEFAULT_MASK= 0x0U;
    static const uint32_t BYTE_MASK= 0xFFU;
    static const uint32_t HWORD_MASK= 0xFFFFU;
    static const uint32_t WORD_MASK= 0xFFFFFFFFU;
    static const uint32_t WORD_ALIGN_MASK= 0xFFFFFFFCU;

    static const uint32_t BYTE_SHIFT_MASK= 0x3U;
    static const uint32_t HWORD_SHIFT_MASK= 0x1U;

    static const bool IS_XDUMP_ADDR= true;
    static const bool NOT_XDUMP_ADDR= false;

    static const uint32_t END_ADDR_OFFSET = 0x3U;

    static const uint32_t BYTE_SIZE = 8;
    static const uint32_t HWORD_SIZE = 16;
    static const uint32_t UNUSED_VAL  = 0x00000000;

    /**
     * \var ForestUtil *m_forest_util
     * \brief Pointer to ForestUtil object.
     */
    ForestUtil *m_forest_util;
    /**
     * \var typedef std::vector<XDUMP_ARITH*> XDumpArithVec
     * \brief Vector of XDump for arithmetic members.
     */
    typedef std::vector<XDUMP_ARITH*> XDumpArithVec;
#if defined(COMP_RUNNER_G3KH)
    typedef std::vector<XDUMP_FPU_MIN*> XDumpFpuMinVec;
#endif // defined(COMP_RUNNER_G3KH)
    /**
     * \var typedef std::vector<XDUMP_REG*> XDumpRegVec;
     * \brief Vector of XDump for register members.
     */
    typedef std::vector<XDUMP_REG*> XDumpRegVec;
    /** 
     * \var typedef std::vectore<XDUMP_ADDR*> XDumpAddrVec
     * \brief Vector of XDump for Address store by Xdump register members.
     */
    typedef std::vector<XDUMP_ADDR*> XDumpAddrVec;
    /**
     * \var typedef std::vector<XDUMP_FLAG*> XDumpFlagVec;
     * \brief Vector of XDump for flag members.
     */
    typedef std::vector<XDUMP_FLAG*> XDumpFlagVec;
    /**
     * \var typedef std::vector<XDUMP_DATA*> XDumpDataVec;
     * \brief Vector of XDump for data.
     */
    typedef std::vector<XDUMP_DATA*> XDumpDataVec;
    /**
     * \var typedef std::vector<XDUMP_SYSREG*> XDumpSysregVec;
     * \brief Vector of XDump for system register.
     */
    typedef std::vector<XDUMP_SYSREG*> XDumpSysregVec;
    /**
     * \var XDumpArithVec m_xdump_arith
     * \brief Variable of XDump for arithmetic.
     *
     */
    XDumpArithVec m_xdump_arith;
    /**
     * \var XDumpAddrVec m_xdump_addr
     * \brief Variable of XDump Address store by Xdump register.
     *
     */
    XDumpAddrVec m_xdump_addr;
    /**
     * \var XDumpRegVec m_xdump_reg
     * \brief Variable of XDump for register.
     *
     */
    XDumpRegVec m_xdump_reg;
    /**
     * \var XDumpFlagVec m_xdump_flag
     * \brief Variable of XDump for flag.
     *
     */
    XDumpFlagVec m_xdump_flag;
    /**
     * \var XDumpDataVec m_xdump_data
     * \brief Variable of XDump for data.
     *
     */
    XDumpDataVec m_xdump_data;
    /**
     * \var XDumpSysregVec m_xdump_sysreg
     * \brief Variable of XDump for system register.
     *
     */
    XDumpSysregVec m_xdump_sysreg;
#if defined(COMP_RUNNER_G3KH)
    XDumpFpuMinVec m_xdump_fpumin;
    XDUMP_FPU_MIN XDumpFpuMinEle;
#endif // defined(COMP_RUNNER_G3KH)
    /**
     * \var XDUMP_ARITH XDumpArithEle
     * \brief Variable of XDump for arithmetic member.
     *
     */
    XDUMP_ARITH XDumpArithEle;

    /**
     * \var XDUMP_DATA XDumpDataEle
     * \brief Variable of XDump for arithmetic member.
     *
     */
    XDUMP_DATA XDumpDataEle;
    /**
     * \var XDUMP_ADDR XDumpAddrEle
     * \brief Variable of XDump Address store by Xdump register member.
     *
     */
    XDUMP_ADDR XDumpAddrEle;
    /**
     * \fn bool IsXDumpAddrEmpty ()
     * \brief Check whether there is any Xdump Address store by Xdump register member.
     *
     * @return checking status.
     */
    bool IsXDumpAddrEmpty ();
    /**
     * \fn bool IsUpdateXDumpAddr (CReg regnum, uint32_t nblk_id)
     * \brief Check whether whether there is any Xdump address update by given register/nonblk-id
     *        if yes, then paste Xdump address info to XdumpAddrEle.
     *
     * @param[in]  reg_num                 register ID.
     * @param[in]  nblk_id                 nonblocking buffer ID.
     * @return checking status.
     */
    bool IsUpdateXDumpAddr (CReg regnum, uint32_t nblk_id);
    /**
     * \fn bool IsXDumpAddr (uint32_t load_addr)
     * \brief Check whether given address is Xdump address updated by Xdump register.
     *
     * @param[in]  load_addr               Given address.
     * @return checking status.
     */
    bool IsXDumpAddr (uint32_t load_addr);
    /**
     * \fn bool AddXDumpAddr (uint32_t store_addr, ByteSize store_size, CReg regnum, uint32_t nblk_id)
     * \brief Add Xdump address updated by Xdump register information.
     *
     * @param[in]  store_addr              Xdump address.
     * @param[in]  store_size              Store size.
     * @param[in]  regnum                  register number.
     * @param[in]  nblk_id                 nonblocking bufffer ID.
     * @return Adding status.
     */
    bool AddXDumpAddr (uint32_t store_addr, ByteSize store_size, CReg regnum, uint32_t nblk_id);
    /**
     * \fn void DelXDumpAddr (CReg regnum, uint32_t nblk_id)
     * \brief Delete Xdump address member by given register num and nonblocking ID
     *
     * @param[in]  regnum                  Register number.
     * @param[in]  nblk_id                 nonblocking bufffer ID.
     */
    void DelXDumpAddr (CReg regnum, uint32_t nblk_id);
    /**
     * \fn void DelXDumpAddr (PhysAddr store_addr, ByteSize store_size);
     * \brief Delete Xdump address member by given store address and store size
     *
     * @param[in]  store_addr              Xdump address.
     * @param[in]  store_size              Store size.
     * @return checking status.
     */
    void DelXDumpAddr (PhysAddr store_addr, ByteSize store_size);
    /**
     * \var XDUMP_SYSREG XDumpSysregEle
     * \brief Variable of XDump for System Register member.
     *
     */
    XDUMP_SYSREG XDumpSysregEle;

    /**
     * \fn bool IsXDumpArith (uint32_t insn_id)
     * \brief Check instruction ID belongs to XDump arithmetic or not.
     *
     * @param[in]  insn_id                 instruction ID.
     * @return checking status.
     */
    bool IsXDumpArith (uint32_t insn_id);
#if defined(COMP_RUNNER_G3KH)
    bool IsXDumpFpuMin (uint32_t insn_id);
#endif // defined(COMP_RUNNER_G3KH)
    /**
     * \fn bool IsXDumpReg(uint32_t load_addr)
     * \brief Check load address belongs to XDump register function or not.
     *
     * @param[in]  load_addr               load address.
     * @return checking status.
     */
    bool IsXDumpReg(uint32_t load_addr);
    /**
     * \fn bool IsXDumpFlag(uint32_t load_addr)
     * \brief Check load address belongs to XDump flag function or not.
     *
     * @param[in]  load_addr               load address.
     * @return checking status.
     */
    bool IsXDumpFlag(uint32_t load_addr);
    /**
     * \fn bool IsXDumpData(uint32_t load_addr)
     * \brief Check load address belongs to XDump data function or not.
     *
     * @param[in]  load_addr               load address.
     * @return checking status.
     */
    bool IsXDumpData(uint32_t load_addr);
    /**
     * \fn bool IsXDumpSysreg(CReg regid)
     * \brief Check System register belongs to XDump System register function or not.
     *
     * @param[in]  regid               System register id.
     * @return checking status.
     */
    bool IsXDumpSysreg(CReg regid);

public:
    /**
     * \fn explicit XDUMP(ForestUtil *forest_util)
     * \brief Create XDUMP object.
     *
     * @param[in]  forest_util               Pointer to ForestUtil class.
     * @return None..
     */
    explicit XDUMP(ForestUtil *forest_util)
    : m_forest_util(forest_util) {}

    ~XDUMP() {}
};


/**
 * \class IDUMP
 *
 * \brief IDUMP operantion class.
 */
class IDUMP {
public:
    /**
     * \var ForestUtil *m_forest_util
     * \brief Pointer to ForestUtil object.
     */
    ForestUtil *m_forest_util;
    /**
     * \var typedef enum _INT_TYPE
     * \brief Interruption type enumeration. This type is not used in CompRunner G4MHv2.
     *
     */
    //! @brief Variable to _INT_TYPE type.
    typedef enum _INT_TYPE {
        brk_async_ld_stdw_nm_ack = 0x01,
        brk_async_rmw_nm_ack,
        eh_relay_brk_nm_ack,
        dbnmi_nt_ack,
        dbint_nt_ack,
        rmint_nt_ack,
        cpu_fenmiack_1l_nt,
        syserr_pin_ack_nt,
        cpu_feintack_1l_nt,
        cpu_intack_1l_nt,
        ehu_ack_fpi_fpu_nt,
        syserr_if_dtec, // 0x0c

        brk_async_ld_stdw_vm_ack = 0x11,
        brk_async_rmw_vm_ack,
        eh_relay_brk_vm_ack,
        dbnmi_t0_ack,
        dbint_t0_ack,
        rmint_t0_ack,
        cpu_feintack_1l_t0,
        cpu_intack_1l_t0,
        ehu_ack_fpi_fpu_t0,

        ucpop_dtec = 0x20,
        pie_dtec,
        mae_dtec,
        mip_dtec,
        idex_hvtrap_inst,
        idex_fetrap_inst,
        syserr_re_exe_dtec
    } _INT_TYPE;

public:
#if defined(COMP_RUNNER_G3KH)
    void SetFPIState(uint32_t pe_id, ContextId tc_id, bool fpi_nc) ;
    void SetINTState(uint32_t pe_id, ContextId tc_id, bool int_nc) ;
    void SetINTState(uint32_t pe_id, ContextId tc_id, bool int_nc, uint32_t pri, uint32_t ch);
    void SetFEINTState(uint32_t pe_id, ContextId tc_id, bool int_nc) ;
    void SetSYSERRState(uint32_t pe_id, ContextId tc_id, bool int_nc);
    void SetSYSERRIFState(uint32_t pe_id, ContextId tc_id, bool int_nc);
    bool IDumpValidate(uint32_t type, std::ostringstream *oss, uint32_t pri, uint32_t ch);
    void IDumpProcessing(uint32_t type, uint32_t pe_id, ContextId tc_id, uint32_t pri, uint32_t ch);
#else // defined(COMP_RUNNER_G3KH)
    //! @brief Validate the valid of Interruption type. This function is not used in CompRunner G4MHv2.
    bool IDumpValidate(uint32_t type, std::ostringstream *oss);
    // void IDumpProcessing(uint32_t type, uint32_t pe_id, ContextId tc_id);
#endif // defined(COMP_RUNNER_G3KH)

public:
    /**
     * \fn explicit IDUMP(ForestUtil *forest_util)
     * \brief Create IDUMP object.
     *
     * @param[in]  forest_util               Pointer to ForestUtil class.
     * @return None..
     */
    explicit IDUMP(ForestUtil *forest_util)
    : m_forest_util(forest_util) {}
    ~IDUMP() {}
};


/**
 * \class XdumpInfo
 *
 * \brief XDump information class.
 */
class XdumpInfo {
 public:
    //! @brief Create XdumpInfo with full of contents.
    XdumpInfo (uint32_t rtl_pe, uint32_t rtl_pc, uint32_t rtl_id,
               int rtl_ldmiss, int rtl_nblkld, int rtl_commit_last,
               svLogicVecVal* rtl_nblk_id,
               uint32_t rtl_wren, svLogicVecVal* rtl_wreg, svLogicVecVal* rtl_value,
               uint32_t rtl_flag_wren, svLogicVecVal* rtl_flags)
        : m_isNblkWrite (0), m_rtl_pe(rtl_pe), m_rtl_pc(rtl_pc), m_rtl_id(rtl_id),
        m_rtl_wren(rtl_wren),
        m_rtl_ldmiss (rtl_ldmiss), m_rtl_nblkld (rtl_nblkld), m_rtl_commit_last (rtl_commit_last),
        m_rtl_nblk_id (rtl_nblk_id),
        m_rtl_wreg(rtl_wreg), m_rtl_value(rtl_value),
        m_rtl_flag_wren(rtl_flag_wren), m_rtl_flags(rtl_flags) {}

    //! @brief Create XdumpInfo with simple contents.
    XdumpInfo (uint32_t rtl_pe, uint32_t rtl_refdone,
               svLogicVecVal* rtl_wr_id,
               svLogicVecVal* rtl_wraddr, svLogicVecVal* rtl_wrdata)
        : m_isNblkWrite (1), m_rtl_pe (rtl_pe), m_rtl_refdone (rtl_refdone),
        m_rtl_wr_id (rtl_wr_id), m_rtl_wreg(rtl_wraddr), m_rtl_value(rtl_wrdata) {}

    ~XdumpInfo (void) {}
    /**
     * \fn uint32_t GetPEID ()
     * \brief Get PEID.
     *
     * @param  -
     *
     * @return PEID.
     */
    uint32_t GetPEID () { return m_rtl_pe; }
    /**
     * \fn uint32_t GetPC ()
     * \brief Get PC value.
     *
     * @param  -
     *
     * @return PC value.
     */
    uint32_t GetPC () { return m_rtl_pc; }
    /**
     * \fn uint32_t GetID ()
     * \brief Get RTL ID..
     *
     * @param  -
     *
     * @return RTL ID.
     */
    uint32_t GetID () { return m_rtl_id; }
    /**
     * \fn uint32_t GetLdMiss ()
     * \brief Get LdMiss.
     *
     * @param  -
     *
     * @return LdMiss.
     */
    uint32_t GetLdMiss () { return m_rtl_ldmiss; }
    /**
     * \fn uint32_t GetNblkLd ()
     * \brief Get Non-blocking load.
     *
     * @param  -
     *
     * @return Non-blocking load.
     */
    uint32_t GetNblkLd () { return m_rtl_nblkld; }
    /**
     * \fn uint32_t GetCommitLast ()
     * \brief last commit.
     *
     * @param  -
     *
     * @return last commit.
     */
    uint32_t GetCommitLast () { return m_rtl_commit_last; }
    /**
     * \fn uint32_t GetNblkId ()
     * \brief Get Non-blocking ID.
     *
     * @param  -
     *
     * @return Non-blocking ID.
     */
    svLogicVecVal* GetNblkId () { return m_rtl_nblk_id; }
    /**
     * \fn uint32_t GetWrEn ()
     * \brief Get Write enable status.
     *
     * @param  -
     *
     * @return Write status.
     */
    uint32_t        GetWrEn () { return m_rtl_wren; }
    /**
     * \fn uint32_t GetWReg ()
     * \brief Get register written index.
     *
     * @param  -
     *
     * @return register index.
     */
    svLogicVecVal * GetWReg () { return m_rtl_wreg; }
    /**
     * \fn uint32_t GetWValue ()
     * \brief Get written data.
     *
     * @param  -
     *
     * @return data.
     */
    svLogicVecVal * GetWValue () { return m_rtl_value; }
    /**
     * \fn uint32_t GetFlagEn ()
     * \brief Get Flag enable.
     *
     * @param  -
     *
     * @return flag status.
     */
    uint32_t GetFlagEn () { return m_rtl_flag_wren; }
    /**
     * \fn uint32_t GetFlags ()
     * \brief Get flag values.
     *
     * @param  -
     *
     * @return flag values.
     */
    svLogicVecVal * GetFlags () { return m_rtl_flags; }
    /**
     * \fn uint32_t GetRefDone ()
     * \brief Get refdone status.
     *
     * @param  -
     *
     * @return refdone status.
     */
    uint32_t GetRefDone () { return m_rtl_refdone; }
    /**
     * \fn uint32_t GetWrId ()
     * \brief Get written ID.
     *
     * @param  -
     *
     * @return written ID.
     */
    svLogicVecVal * GetWrId () { return m_rtl_wr_id; }
    /**
     * \fn uint32_t IsNblkWrite ()
     * \brief Check non-blocking status.
     *
     * @param  -
     *
     * @return non-blocking status.
     */
    bool IsNblkWrite () { return m_isNblkWrite; }

 private:
    bool           m_isNblkWrite;
    uint32_t       m_rtl_pe;
    uint32_t       m_rtl_pc;
    uint32_t       m_rtl_refdone;
    uint32_t       m_rtl_id;
    uint32_t       m_rtl_wren;
    uint32_t       m_rtl_ldmiss;
    uint32_t       m_rtl_nblkld;
    uint32_t       m_rtl_commit_last;
    svLogicVecVal* m_rtl_nblk_id;
    svLogicVecVal* m_rtl_wr_id;
    svLogicVecVal* m_rtl_wreg;
    svLogicVecVal* m_rtl_value;
    uint32_t       m_rtl_flag_wren;
    svLogicVecVal* m_rtl_flags;
};
