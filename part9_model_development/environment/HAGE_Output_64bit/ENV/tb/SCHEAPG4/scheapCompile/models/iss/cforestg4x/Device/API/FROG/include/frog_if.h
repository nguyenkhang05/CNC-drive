/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */


/** \file frog_if.h
 * \brief FROG interface header file.
 *
 * This file contains the interface of API common functions for FROG tool.
 * FROG tool will use these functions to connect to CForest simulator.
 * Thus, FROG tool can understand the current status in CForest and generate code step by step for its sequence.
 *
 */


#if ! defined(__FROG_IF_H__)
#define __FROG_IF_H__

#ifdef _MSC_VER
    #define DLLEXPORT   __declspec(dllexport)
#else // _MSC_VER
/*!
  \def DLLEXPORT
  Define DLLEXPORT value.
*/
    #define DLLEXPORT
#endif // _MSC_VER

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
typedef char                int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef long                int32_t;
typedef unsigned long       uint32_t;
typedef long long           int64_t;
typedef unsigned long long  uint64_t;
#else
/* gcc or VC10 */
#include <stdint.h>
#endif
/**
 * \brief Register data type for FROG API functions
 *
 */
typedef __uint128_t FrogRegData;


/**
 * \var enum RegType
 * \brief Type of register in FROG I/F.
 *
 */
enum RegType {
    FROG_PC = 0,   /* PC reg                 */
    FROG_GR = 1,   /* General register       */
    FROG_VR = 2,   /* Vector register        */
    FROG_WR = 3,   /* Wide register (FXU)    */
    FROG_SR = 4    /* System register        */
};


/**
 * \class FrogRegPro
 *
 * \brief Register Profile class - Contain the register information.
 *
 * - @ref m_reg_name  Register name
 * - @ref m_sel_id    Selection ID in register
 * - @ref m_reg_id    Register ID in register
 * - @ref m_context   Context ID of register:
 *                                     * "N"  = Native;
 *                                     * "NT" = Native, Thread, Not share;
 *                                     * "T"  = Native, Thread, Share;
 *                                     * "V"  = Virtual;
 *                                     * "NV" = Native, Virtual Not share;
 *                                     * "VS" = Native, Virtual, Share;
 * - @ref m_privilege Privilege of register:
 *                                     * "U" = User.
 *                                     * "D" = Debug.
 *                                     * "HV" = Hypervisor.
 *                                     * "SV" = Supervisor, "C" Coprocessor
 * - @ref m_type      Type of register. See \"enum @ref RegType\" for details
 * - @ref m_creg      Register ID (for FROG I/F Internal)
 */

class FrogRegPro {
public:
    /**
     * \var m_reg_name
     * \brief Register name.
     *
     */
    std::string m_reg_name;
    /**
     * \var m_sel_id
     * \brief Selection ID in register.
     *
     */
    uint32_t    m_sel_id;
    /**
     * \var m_reg_id
     * \brief Register ID In register.
     *
     */
    uint32_t    m_reg_id;
    /**
     * \var m_context
     * \brief Context ID of register.
     *
     */
    std::string m_context;
    /**
     * \var m_privilege
     * \brief Privilege of register.
     *
     */
    std::string m_privilege;
    /**
     * \var m_type
     * \brief Type of register.
     *
     */
    uint32_t    m_type;
    /**
     * \var m_creg
     * \brief Register ID (for FROG I/F internal).
     *
     */
    uint32_t    m_creg;

    FrogRegPro () {};
    /**
     * \fn FrogRegPro (std::string reg_name, uint32_t sel_id, uint32_t reg_id, std::string context, std::string privilege, uint32_t type, uint32_t creg)
     * \brief Initialize the register profile object with full info.
     *
     * This function is used to create a new register project object with full of information.
     *
     * @param[in] reg_name  register name.
     * @param[in] sel_id    selection ID of register.
     * @param[in] reg_id    register ID of register.
     * @param[in] context   context ID of register.
     * @param[in] privilege privilege of register.
     * @param[in] type      type of register
     * @param[in] creg      register ID (for FROG I/F internal).
     *
     * @return None
     */
    FrogRegPro (std::string reg_name, uint32_t sel_id, uint32_t reg_id, std::string context, std::string privilege, uint32_t type, uint32_t creg)
        : m_reg_name(reg_name), m_sel_id(sel_id), m_reg_id(reg_id), m_context (context), m_privilege (privilege), m_type (type), m_creg (creg) {}
    ~FrogRegPro () {}
};

/* GCS I/F Retrun code */
/*!
  \def GCSIF_SUCCESS
  Define GCSIF_SUCCESS index.
*/
/*!
  \def GCSIF_ERR_NOT_INITIALIZED
  Define GCSIF_ERR_NOT_INITIALIZED index.
*/
/*!
  \def GCSIF_ERR_STEP
  Define GCSIF_ERR_STEP index.
*/
/*!
  \def GCSIF_ERR_INVALID_TCID
  Define GCSIF_ERR_INVALID_TCID index.
*/
/*!
  \def GCSIF_ERR_INVALID_REGISTER_NAME
  Define GCSIF_ERR_INVALID_REGISTER_NAME index.
*/
/*!
  \def GCSIF_ERR_INVALID_OTHER_PARAM
  Define GCSIF_ERR_INVALID_OTHER_PARAM index.
*/
/*!
  \def GCSIF_ERR_INVALID_CONTEXT
  Define GCSIF_ERR_INVALID_CONTEXT index.
*/
/*!
  \def GCSIF_ERR_INVALID_VCID
  Define GCSIF_ERR_INVALID_VCID index.
*/
/*!
  \def GCSIF_ERR_THREAD_NOT_LINKED
  Define GCSIF_ERR_THREAD_NOT_LINKED index.
*/
/*!
  \def GCSIF_ERR_THREAD_IS_HALT
  Define GCSIF_ERR_THREAD_IS_HALT index.
*/
/*!
  \def GCSIF_ERR_THREAD_CANT_EXEC
  Define GCSIF_ERR_THREAD_CANT_EXEC index.
*/
/*!
  \def GCSIF_ERR_ISS_MEMORY
  Define GCSIF_ERR_ISS_MEMORY index.
*/
/*!
  \def GCSIF_ERR_LLBIT_LOCK
  Define GCSIF_ERR_LLBIT_LOCK index.
*/
/*!
  \def GCSIF_ERR_LLBIT_MAX
  Define GCSIF_ERR_LLBIT_MAX index.
*/
/*!
  \def GCSIF_WAR_DUPLICATED_EVENT
  Define GCSIF_WAR_DUPLICATED_EVENT index.
*/
/*!
  \def GCSIF_ERR_INVALID_EVENT
  Define GCSIF_ERR_INVALID_EVENT index.
*/
/*!
  \def GCSIF_ERR_NOT_RECEIVED
  Define GCSIF_ERR_NOT_RECEIVED index.
*/
/*!
  \def GCSIF_ERR_MMU_DISABLED
  Define GCSIF_ERR_MMU_DISABLED index.
*/
/*!
  \def GCSIF_ERR_ALLOCATE_MEMORY
  Define GCSIF_ERR_ALLOCATE_MEMORY index.
*/
/*!
  \def GCSIF_ERR_INTERNAL
  Define GCSIF_ERR_INTERNAL index.
*/

#define      GCSIF_SUCCESS                      (0x00)   /* No error. */
#define      GCSIF_ERR_NOT_INITIALIZED          (0x01)   /* Simulator is uninitialized. */
#define      GCSIF_ERR_STEP                     (0x02)   /* Simulator returned some error on execute operation. */
#define      GCSIF_ERR_INVALID_TCID             (0x10)   /* Thread ID is wrong. TCID should be 0~63. */
#define      GCSIF_ERR_INVALID_REGISTER_NAME    (0x11)   /* Parameter name of register is wrong */
#define      GCSIF_ERR_INVALID_OTHER_PARAM      (0x12)   /* Some parameter is wrong. */
#define      GCSIF_ERR_INVALID_CONTEXT          (0x13)   /* This register mismatch with context.*/
#define      GCSIF_ERR_INVALID_VCID             (0x14)   /* VCID is wrong. VCID should be 0~7. */
#define      GCSIF_ERR_THREAD_NOT_LINKED        (0x20)   /* This thread is not assigned to any virtual machine */
#define      GCSIF_ERR_THREAD_IS_HALT           (0x21)   /* This thread is on halt. */
#define      GCSIF_ERR_THREAD_CANT_EXEC         (0x22)   /* This thread is stalling for any reason. */
#define      GCSIF_ERR_ISS_MEMORY               (0x50)   /* Error occurred on memmory access. */
#define      GCSIF_ERR_LLBIT_LOCK               (0x51)   /* Another thread is locking this address. */
#define      GCSIF_ERR_LLBIT_MAX                (0x52)   /* This thread is locking another address. */
#define      GCSIF_WAR_DUPLICATED_EVENT         (0x60)   /* Existing event is overwritten. */
#define      GCSIF_ERR_INVALID_EVENT            (0x61)   /* Parameter of event is wrong. */
#define      GCSIF_ERR_NOT_RECEIVED             (0x62)   /* Request isn’t accepted. */
#define      GCSIF_ERR_MMU_DISABLED             (0x90)   /* Simulator is unavailable for MMU. (-> set in py-file). */
#define      GCSIF_ERR_ALLOCATE_MEMORY          (0xF0)   /* Insufficient memory to run this application. */
#define      GCSIF_ERR_INTERNAL                 (0xFF)   /* Unexpected internal error occurred.*/

/* GCS I/F Retrun value */
static const uint32_t FROG_ACCESS_TYPE_READ       = 0x0;
static const uint32_t FROG_ACCESS_TYPE_WRITE      = 0x1;
static const uint32_t FROG_ACCESS_BUS_RW          = 0x2;
static const uint32_t FROG_ACCESS_BUS_FETCH       = 0x3;
static const uint32_t FROG_ACCESS_BUS_API         = 0x4;
static const uint32_t FROG_LOAD_ACTION_LOAD       = 0x5;
static const uint32_t FROG_LOAD_ACTION_FETCH      = 0x6;
static const uint32_t FROG_TLB_READ_ENTRY         = 0x7;
static const uint32_t FROG_TLB_WRITE_ENTRY        = 0x8;
static const uint32_t FROG_LLBIT_CREATE           = 0x9;
static const uint32_t FROG_LLBIT_DELETE           = 0xA;

/**
 * Miscellaneous - Notify the comment (callback) with execution information
 *
 * @param[in] comment Comment Execution information: ptid, pc, opecode, psw, reg, store addr/data, mnemonic, cforest behavior
 */
typedef void (*GCS_CB_COMMENT)(const std::string &comment);


/**
 * LLbit Interface - Notify the llbit updating (callback) with create or delete event.
 *
 * @param[in] pe_id   pe is updated
 * @param[in] address address is stored in llbit
 * @param[in] size    size is stored in llbit
 * @param[in] isNC    true:Native thread, false:HT#0-63
 * @param[in] ptid    Thread ID(0-63).
 * @param[in] status  "C" = New link created, "D" = Disappear link.
 */
typedef void (*GCS_CB_LLBIT)(uint32_t pe_id, uint64_t address, uint32_t size, bool isNC, uint32_t ptid, const uint32_t status);


/**
 * Memory Interface - Notify the memory accessing (callback) with read or write operation.
 *
 * @param[in] address     Physical memory address at which to begin update
 * @param[in] size        Number of bytes to update ( 1 => 8-bit, 2 => 16-bit, 4 => 32-bit)
 * @param[in] value       Value of bytes to update memory
 * @param[in] prev_value  Prev Value of bytes to update memory
 * @param[in] access_type 0: read, 1: write
 * @param[in] access_bus  2: R/W Data , 3: INST_RAM  , 4: API
 * @param[in] load_action 5: Load ,6: Fetch
 */
typedef void (*GCS_CB_MEMORY_EX)(uint64_t address, uint32_t size,
              uint64_t value, uint64_t prev_value,
              const uint32_t access_type, const uint32_t access_bus,
              const uint32_t load_action);

/**
 * Register Interface - Notify the register update (callback) on Native context
 *
 * @param[in] register_name Name of register to update
 * @param[in] value         Value to update register
 * @param[in] prevalue      Prev Value to update register
 */
typedef void (*GCS_CB_NC_REG_EX)(const std::string register_name, FrogRegData value, FrogRegData prev_value);

/**
 * Register Interface - Notify the register update (callback) on Virtual context
 *
 * @param[in] register_name Name of register to update
 * @param[in] value         Value to update register
 * @param[in] pre_value     Prev Value to update register
 * @param[in] vcid          Machine ID (0-7)
 */
typedef void (*GCS_CB_VC_REG_EX)(const std::string register_name, FrogRegData value, FrogRegData prev_value, uint32_t vcid);

/**
 * Register Interface - Notify the register update (callback) on Thread context
 *
 * @param[in] register_name Name of register to update
 * @param[in] value         Value to update register
 * @param[in] pre_value     Prev Value to update register
 * @param[in] tcid          Thread ID(0-63)
 */
typedef void (*GCS_CB_TC_REG_EX)(const std::string register_name, FrogRegData value, FrogRegData prev_value, uint32_t tcid);

/**
 * TLB Interface - Notify the entry accessing (callback) for RTG.FROG
 *
 * @param[in] index       Index of entry (0-63)
 * @param[in] telo0       Value of TELO0 register.
 * @param[in] telo1       Value of TELO1 register.
 * @param[in] tehi0       Value of TEHI0 register.
 * @param[in] tehi1       Value of TEHI1 register.
 * @param[in] pre_telo0   Previous value of TELO0 register.
 * @param[in] pre_telo1   Previous value of TELO1 register.
 * @param[in] pre_tehi0   Previous value of TEHI0 register.
 * @param[in] pre_tehi1   Previous value of TEHI1 register
 * @param[in] access_type 7 = read entry, 8 = write entry.
 */
typedef void (*GCS_CB_TLB_EX)(uint32_t index, uint32_t telo0, uint32_t telo1,
              uint32_t tehi0, uint32_t tehi1, uint32_t telo0_prev,
              uint32_t telo1_prev, uint32_t tehi0_prev,
              uint32_t tehi1_prev, uint32_t access_type);

/**
 * \struct GCS_CB_INFO_EX_DUMMY
 * \brief Define structure for callback information.
 *
 * - \ref fpC            : Comment callback address.
 * - \ref fpExM          : Memory event callback address Extend.
 * - \ref fpExNR         : Register(Native Context) update callback address Extend.
 * - \ref fpExVR         : Register(Virtual Machine Context) update callback address Extend.
 * - \ref fpExTR         : Register(Thread Context) update callback address Extend.
 * - \ref fpL            : LLbit event callback address.
 * - \ref fpExTLB        : TLB event callback address.
 */
/**
 * \var typedef struct GCS_CB_INFO_EX_DUMMY GCS_CB_INFO_EX
 * \brief Shortcut definition for callback information.
 * \brief The return value for call-back functions.
 *
 * \ref GCS_CB_INFO_EX_DUMMY
 */

typedef struct GCS_CB_INFO_EX_DUMMY{
    /**
     * \var GCS_CB_COMMENT         fpC
     * \brief Comment callback address.
     *
     */
   GCS_CB_COMMENT         fpC;
   /**
    * \var GCS_CB_MEMORY_EX       fpExM
    * \brief Memory event callback address Extend.
    *
    */
   GCS_CB_MEMORY_EX       fpExM;
   /**
    * \var GCS_CB_NC_REG_EX       fpExNR
    * \brief Register(Native Context) update callback address Extend
    *
    */
   GCS_CB_NC_REG_EX       fpExNR;
   /**
    * \var GCS_CB_VC_REG_EX       fpExVR
    * \brief Register(Virtual Machine Context) update callback address Extend
    *
    */
   GCS_CB_VC_REG_EX       fpExVR;
   /**
    * \var GCS_CB_TC_REG_EX       fpExTR
    * \brief Register(Thread Context) update callback address Extend.
    *
    */
   GCS_CB_TC_REG_EX       fpExTR;
   /**
    * \var GCS_CB_LLBIT           fpL
    * \brief LLbit event callback address
    *
    */
   GCS_CB_LLBIT           fpL;
   /**
    * \var GCS_CB_TLB_EX          fpExTLB
    * \brief TLB event callback address.
    *
    */
   GCS_CB_TLB_EX          fpExTLB;
} GCS_CB_INFO_EX;

/**
 * \struct GCS_HW_INFO_EX_DUMMY
 * \brief Define structure for emulator status.
 *
 * - \ref vmnum           : Number of virtual machine (0 – 8).
 * - \ref threadnum       : Number of hardware thread (0 – 64).
 * - \ref mpnum           : Number of memory protection area.
 * - \ref tlbnum          : Number of TLB entry.
 * - \ref bpnum           : Number of HW break point channel.
 * - \ref smallpage       : MMU Page size.
 */
/**
 * \var typedef struct GCS_HW_INFO_EX_DUMMY GCS_HW_INFO_EX
 * \brief Shortcut definition for emulator status.
 *
 * \ref GCS_HW_INFO_EX_DUMMY
 */
typedef struct GCS_HW_INFO_EX_DUMMY {
   /**
    * \var uint32_t      vmnum
    * \brief Number of virtual machine (0 – 8).
    *
    */
   uint32_t      vmnum;
   /**
    * \var uint32_t      threadnum
    * \brief Number of hardware thread (0 – 64).
    *
    */
   uint32_t      threadnum;
   /**
    * \var uint32_t      mpnum
    * \brief Number of memory protection area.
    *
    */
   uint32_t      mpnum;
   /**
    * \var uint32_t      tlbnum
    * \brief Number of TLB entry
    *
    */
   uint32_t      tlbnum;
   /**
    * \var uint32_t      bpnum
    * \brief Number of HW break point channel.
    *
    */
   uint32_t      bpnum;
   /**
    * \var uint32_t      smallpage
    * \brief MMU Page size.
    *
    */
   uint32_t      smallpage;
} GCS_HW_INFO_EX;


/**
 * General Interface - Setup
 *
 * @param[in]  debug_level 0: ERR, 1: WAR, 2: EXP, 3: INF
 * @param[in]  pCbInfo     Function pointer of callback
 * @param[in]  inifilepath Simulator initial setting.
 * @param[out] pHwInfo     Simulator initial setting.
 * @return Error code
 *         GCSIF_ERR_ALLOCATE_MEMORY       : fail to allocate memory
 *         GCSIF_SUCCESS                   : successful
 */
extern uint32_t gcs_initialize_simulator_ex(uint32_t debug_level, GCS_CB_INFO_EX* pCbInfo,
                                       const std::string &inifilepath, GCS_HW_INFO_EX * pHwInfo);


/**
 * General Interface - Reset
 *
 * @return Error code
 *         GCSIF_ERR_ALLOCATE_MEMORY       : fail to allocate memory
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_reset_simulator( void );


/**
 * General Interface - Step
 *
 * @param[in] ptid Physical Thread ID (0,1,2,3,4,5,6,7)
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_TCID          : Thread id is 0-63
 *         GCSIF_ERR_THREAD_IS_HALT        : Thread is halt or suspend.
 *         GCSIF_ERR_INTERNAL              : Unknown error.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_step_simulator( uint32_t ptid );


/**
 *   Register(Native Context) Write
 *
 * @param[in] name  register name
 * @param[in] value value
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_set_nc_register(const std::string &name, FrogRegData  value);


/**
 *   Register(Native Context) Read
 *
 * @param[in]  name  register name
 * @param[out] value value
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_get_nc_register(const std::string &name, FrogRegData* value);

/**
 *   Register(Virtual machine Context) Write
 *
 * @param[in] name  register name
 * @param[in] value value
 * @param[in] vcid  Machine ID (0-7)
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_VCID          : VCID is wrong. vcid should be 0 to 7.
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register
 *         GCSIF_ERR_INVALID_CONTEXT       : Invalid context
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_set_vc_register(const std::string &name, FrogRegData  value, uint32_t vcid);


/**
 *   Register(Virtual machine Context) Read
 *
 * @param[in]  name  register name
 * @param[out] value value
 * @param[in]  vcid  Machine ID (0-7)
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_VCID          : VCID is wrong. vcid should be 0 to 7.
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register
 *         GCSIF_ERR_INVALID_CONTEXT       : Invalid context
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_get_vc_register(const std::string &name, FrogRegData* value, uint32_t vcid);


/**
 *   Register(Thread Context) Write
 *
 * @param[in] name  register name
 * @param[in] value value
 * @param[in] tcid  Thread ID(0-63)
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_TCID          : TCID is wrong. vcid should be 0 to 63.
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register
 *         GCSIF_ERR_INVALID_CONTEXT       : Invalid context
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_set_tc_register(const std::string &name, FrogRegData value, uint32_t tcid);


/**
 *   Register(Thread Context) Read
 *
 * @param[in]  name  register name
 * @param[out] value value
 * @param[in]  tcid  Thread ID(0-63)
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_TCID          : TCID is wrong. vcid should be 0 to 63.
 *         GCSIF_ERR_INVALID_REGISTER_NAME : Unknown register
 *         GCSIF_ERR_INVALID_CONTEXT       : Invalid context
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_get_tc_register(const std::string &name, FrogRegData* value, uint32_t tcid);


/**
 * Set TLB entry
 *
 * @param[in] index Index of tlb entry
 * @param[in] telo0 Value of TELO0
 * @param[in] telo1 Value of TELO1
 * @param[in] tehi0 Value of TEHI0
 * @param[in] tehi1 Value of TEHI1
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_OTHER_PARAM   : Parameter "index" should be 0~63.
 *         GCSIF_ERR_MMU_DISABLED          : MMU function is disabled.
 *         GCSIF_ERR_INTERNAL              : Unknown error.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_write_simulator_tlb(uint32_t index, uint32_t telo0, uint32_t telo1, uint32_t tehi0, uint32_t tehi1);


/**
 * Get TLB entry
 *
 * @param[in]  index Index of tlb entry
 * @param[out] telo0 Value to get TELO0 (Not returend if telo0 is null)
 * @param[out] telo1 Value to get TELO1 (Not returend if telo1 is null)
 * @param[out] tehi0 Value to get TEHI0 (Not returend if tehi0 is null)
 * @param[out] tehi1 Value to get TEHI1 (Not returend if tehi1 is null)
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_OTHER_PARAM   : Parameter "index" should be 0~63.
 *         GCSIF_ERR_MMU_DISABLED          : MMU function is disabled.
 *         GCSIF_ERR_INTERNAL              : Unknown error.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_read_simulator_tlb (uint32_t index, uint32_t* telo0, uint32_t* telo1, uint32_t* tehi0, uint32_t* tehi1);


/**
 * Miscellaneous - GetVersion
 *
 * @param[out] version Simulator version
 */
extern void gcs_get_simulator_version (std::string &version);


/**
 * Memory Interface - Memory Read
 *
 * @param[in]  address Physical memory address at which to begin read
 * @param[in]  size    Number of bytes to read ( 16-bit access => 2, 32-bit access => 4 )
 * @param[out] bytes   Contents of memory read ( string format ex."(0x01,0x02,0x03)" )
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_OTHER_PARAM   : bytes is null, size is zero.
 *         GCSIF_ERR_INTERNAL              : Unknown error.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t   gcs_read_simulator_memory( uint64_t address, uint32_t size, std::string &bytes );


/**
 * Memory Interface - Memory Write
 *
 * @param[in] address Physical memory address at which to begin write
 * @param[in] size    Number of bytes to write
 * @param[in] bytes   Value of bytes to write
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_OTHER_PARAM   : bytes is null, size is zero, data mismatch with size.
 *         GCSIF_ERR_INTERNAL              : Unknown error.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t   gcs_write_simulator_memory( uint64_t address, uint32_t size, const std::string &bytes );


/**
 * LLbit Interface - Add link
 *
 * @param[in] address memory address (4byte align)
 * @param[in] isNc    link owner (true:NativeThread, false:HT#0-63)
 * @param[in] ptid    Thread id
 * @param[in] size    link size (1/2/4/8)
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_INTERNAL              : Unknown error.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t   gcs_create_memory_link( uint64_t address, bool isNc, uint32_t ptid, uint32_t size);


/**
 * LLbit Interface - Remove link on address
 *
 * @param[in] address Physical memory address at which to remove link
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t   gcs_remove_memory_link_addr( uint64_t address );


/**
 * Miscellaneous - Disable CForest Simulator
 *
 * @param[in] disable for reserve
 * @return Error code
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_disable_simulator_stop( uint32_t disable );


/**
 * CForest status - Get current mode.
 *
 * @param[out] mode 1:Native, 0:Virtual
 * @return Error code
 *         GCSIF_ERR_NOT_INITIALIZED       : Simulator is not initialized.
 *         GCSIF_ERR_INVALID_OTHER_PARAM   : mode is null.
 *         GCSIF_SUCCESS                   : Successful
 */
extern uint32_t gcs_is_simulator_native(uint32_t* mode);

/**
 * Register profile - Get register information from CForest
 *
 * @param[in]  reg_name Register name
 * @param[out] out      Return register profile in FROG. See FrogRegPro for details.
 * @return false       : Can't find the register profile
 *         true        : Found the register profile
 */
extern bool gcs_get_register_profile (const std::string &reg_name, FrogRegPro *out);

/**
 * Register name - Get register name
 *
 * @param[in]  selID SelId
 * @param[in]  regID RegId
 * @param[out] name  Register name
 * @return false       : Can't find the register name
 *         true        : Found the register name
 */
extern bool gcs_get_sreg_name(uint32_t selID, uint32_t regID, std::string &name);

/**
 *   This function checks the available of FXU function in python script file.
 *
 * @return true       : set_has_fxu("on") is put in python script file.
 *         false      : set_has_fxu("on") is not put in python script file.
 */
extern bool gcs_check_fxu_function (void );


/**
 *   This function checks the available of FPU function in python script file.
 *
 * @return true       : set_has_fpu("on") is put in python script file.
 *         false      : set_has_fpu("on") is not put in python script file.
 */
extern bool gcs_check_fpu_function (void );

/**
 *   This function requests EIINT or EITBL to CPU directly (not to INTC).
 *
 * @param[in] channel  the channel of EIINT/EITBL (0-2047)
 * @param[in] priority priority of EIINT (0-15)
 * @param[in] eitb     0=EIINT ，1=EITBL
 * @param[in] is_gm    0=Req to Host，1=Req to Guest
 * @param[in] gpid     0=GPID for GMEIINT/BGEIINT.
 * @param[in] is_bgint When is_gm=1, 0=GMEIINT，1=BGEIINT
 * @param[in] isNC     distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid     Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_INVALID_EVENT         : Unsupported event.
 *         GCSIF_WAR_DUPLICATED_EVENT      : Overwite exis event.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_eiint_pe (uint32_t channel, uint32_t priority, bool eitb, bool is_gm, uint32_t gpid, bool is_bgint, int isNC, unsigned int ptid);

/**
 *   This function requests SYSERR to CPU directly.
 *
 * @param[in] cause the cause code of SYSERR (0x10-0x1F)
 * @param[in] isNC  distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid  Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_INVALID_EVENT         : Unsupported event.
 *         GCSIF_WAR_DUPLICATED_EVENT      : Overwite exis event.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_syserr_pe (uint32_t cause, int isNC, unsigned int ptid);

/**
 *   This function requests SYSERR (by host mode or by guest mode) to CPU directly.
 *
 * @param[in] cause the cause code of SYSERR (0x10-0x1F)
 * @param[in] isNC  distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid  Thread id
 * @param[in] is_gm 0=Syserr caused by Host mode, 1=Syserr caused by Guest mode
 * @param[in] gpid  target GPID (0x0-0x7. When is_gm =0, gpid should be 0.)
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_INVALID_EVENT         : Unsupported event.
 *         GCSIF_WAR_DUPRICATED_EVENT      : Overwite exis event.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_syserr_mode_pe (uint32_t cause, int isNC, unsigned int ptid, bool is_gm, uint32_t gpid);

/**
 *   This function requests RLB to CPU.
 *
 * @param[in] source_peid peid which requests RLB
 * @param[in] isNC        distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid        Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_NOT_RECEIVED          : EVENT is existed already.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_rlb_pe (uint32_t source_peid, int isNC, unsigned int ptid);


/**
 *   This function requests DBNMI to CPU.
 *
 * @param[in] isNC distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_NOT_RECEIVED          : EVENT is existed already.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_dbnmi_pe (int isNC, unsigned int ptid);


/**
 *   This function requests DBINT to CPU.
 *
 * @param[in] channel the channel of DBINT (0-15)
 * @param[in] isNC    distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid    Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_NOT_RECEIVED          : EVENT is existed already.
 *         GCSIF_ERR_INVALID_EVENT         : channel is invalid value.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_dbint_pe (uint32_t channel, int isNC, unsigned int ptid);


/**
 *   This function requests RMINT to CPU.
 *
 * @param[in] isNC distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_NOT_RECEIVED          : EVENT is existed already.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_rmint_pe (int isNC, unsigned int ptid);


/**
 *   This function requests FENMI to CPU.
 *
 * @param[in] isNC distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_NOT_RECEIVED          : EVENT is existed already.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_fenmi_pe (int isNC, unsigned int ptid);


/**
 *   This function requests FEINT to CPU.
 *
 * @param[in] channel  the channel of FEINT (0-15)
 * @param[in] is_gm    0=Req to Host，1=Req to Guest
 * @param[in] gpid     0=GPID for GMFEINT/BGFEINT.
 * @param[in] is_bgint When is_gm=1, 0=GMFEINT，1=BGFEINT
 * @param[in] isNC     distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid     Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_NOT_RECEIVED          : EVENT is existed already.
 *         GCSIF_ERR_INVALID_EVENT         : channel is invalid value.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_feint_pe (uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint, int isNC, unsigned int ptid);


/**
 *   This function requests for disabling the assertion of interruption types to CPU.
 *
 * @param[in] isNC     distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid     Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_NOT_RECEIVED          : EVENT is existed already.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_req_cpu_stop_pe(int isNC, unsigned int ptid);


/**
 *   This function requests for reseting PE from G4MH2.1 core type..
 *
 * @param[in] peid     peid will be reset.
 * @param[in] isNC     distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid     Thread id
 * @return Error code
 *         GCSIF_ERR_NOT_RECEIVED          : Can't reset PE.
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_reset_pe(unsigned int peid, int isNC, unsigned int ptid);


/**
 *   This function clear the async event requests to CPU.
 *
 * @param[in] event event name. feint, fenmi, eiint, eitbl, dbint, syserr, rmint, dbnmi, rlb
 * @param[in] isNC  distinguish NT from HT#0, if tcid is 0 (true:NativeThread, false:TC#n)
 * @param[in] ptid  Thread id
 * @return Error code
 *         GCSIF_ERR_INVALID_TCID          : ptid is invalid value.
 *         GCSIF_ERR_INVALID_OTHER_PARAM   : Unknown event
 *         GCSIF_SUCCESS                   : Successful
 */
extern int gcs_clear_req_pe (const std::string &event, int isNC, unsigned int ptid);

/**
 * This function returns existance of exception/interruption in the previous executing instruction.
 *
 * \param -
 * @return true : Exception or interruption occured in previous execution.
 *         false : Exception and interruption didn't occur in previous execution.
 */
extern bool gcs_get_exception (void);


/**
 * This function will load srec file to CForest simulator.
 *
 * @param[in] filename name of srec file
 * @return true         : Loading srec file successfully.
 *         false        : Exception and interruption didn't occur in previous execution.
 */
extern bool gcs_internal_LoadSrec (const std::string filename);
#endif /* __FROG_IF_H__ */
