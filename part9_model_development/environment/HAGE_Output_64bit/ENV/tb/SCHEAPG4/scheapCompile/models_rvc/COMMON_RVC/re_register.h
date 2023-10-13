/*************************************************************************
 *
 *  re_register.h
 *
 * Copyright(c) 2010-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
*************************************************************************/
/// @re_register.h
/// @brief General register class
/// $Id: re_register.h v2016_09_21 $
/// $Date$
/// $Revison$
/// $Author$

#ifndef __RE_REGISTER_H
#define __RE_REGISTER_H
#include "reg_super.h"
#include <vector>
#include <map>

namespace vpcl
{

class re_register;

enum eAccessMode   { emNotAccessMode=0, emAccD=1, emAccR=2, emAccW=3, emAccW0=4,
                     emAccW1=5, emAccRW=6, emAccRBW0=7, emAccW0S1=8,
                     emAccRW0=9, emAccRW1=0xA,                          // to support ACC_RD|ACC_W0, ACC_RD|ACC_W1
                     emAccW1S0=0x11, emAccWBR=0x13, emAccRBW1=0x12, emAccRBW0S1=0x14, emAccRBW=0x21,// new added
                     emAccRBW1S0=0x15,emAccW0BR=0x16, emAccW1BR=0x17, emAccW0S1BR=0x18, emAccW1S0BR=0x19 // new added
                     , emAccRW1S0=0x22 // added (phase 5)
                     , emAccW1T=0x23, emAccW0T=0x24, emAccRW1T=0x25, emAccRW0T=0x26 // new added 2015.Feb.11
                     , emAccWS0=0x27, emAccWS1=0x28, emAccRWS0=0x29, emAccRWS1=0x2A // new added 2015.Feb.11
                     , emAccRW0S1=0x2B, emAccRS0=0x2C, emAccRS0W=0x2D, emAccRS0W0=0x2E, emAccRS0W1=0x2F  // new added 2015.Feb.11
                     , emAccRS1=0x30, emAccRS1W=0x31, emAccRS1W0=0x32, emAccRS1W1=0x33 // new added 2015.Feb.11
                     , ACC_WRITE_ZERO_ONLY=0xB, ACC_WRITE_ONE_ONLY=0xC                      // legacy
                     , ACC_WRITE_ONLY=0xD, ACC_READ_ONLY=0xE, ACC_READ_WRITE=0xF            // legacy
                     , ACC_RD=0x10, ACC_W0=0x20, ACC_W1=0x40, ACC_RW=0x70, NOT_ENTRY=0xFF}; // legacy

enum eWritableType { emValueError=0, emValueList=1, emValueRange=2, emValueAny=3};

enum eAccessStatus { emNotAccessed=0, emReadStatus=1, emWrittenStatus=2, emWritten0Status=3, emWritten1Status=4};

enum eSupportMode  { SPP_ENABLE, SPP_DISABLE};

enum eMsgType      { emFatalLevel   = 0,
                     emErrorLevel   = 1,
                     emWarningLevel = 2,
                     emInfoLevel    = 4};

#define REG_SIZE  32
#define MAX_OWNER 16
#define N_SPP_MODE 49

/// each bit control class
class bit_info
{

public:
  unsigned int  mEndAddr;                  ///< end bit
  unsigned int  mStartAddr;                ///< start bit
  std::string   mBitName;                  ///< bit name
  unsigned int  mInitVal;                  ///< initial value
  int           mMode[MAX_OWNER];          ///< write/read permission mode
  eSupportMode  mSpp;                      ///< support target feature or not
  eWritableType mWritableType;             ///< type of writable value
  std::vector<unsigned int> mWritableList; ///< list of writable value
  eAccessStatus mAccessStatus;             ///< previous accessing status
  bit_info     *next;                      ///< next bit's pointer
  

  /// bit setting constructor
  /// @return none
  bit_info(
    unsigned int addr,                     ///< [in] register address
    re_register *parent                    ///< [in] register poiter
  );

  /// bit access by assignment operator
  /// @return none
  bit_info &operator = (
    unsigned int data                      ///< [in] value assigned to bit
  );

  /// bit access by assignment operator   // phase 2 added
  /// @return none
  bit_info &operator = (
    bit_info org                          ///< [in] assigned bit info
  );

  /// bit access by reference operator
  /// @return the value of bit
  operator unsigned int ();

  // multi master
  /// set access mode for additional owner
  /// @return none
  void add_condition(
    unsigned int owner_num,                ///< [in] additional owner's ID
    int mode                               ///< [in] access mode
  );

  /// set access owner
  /// @return none
  void access_owner(
    unsigned int owner_num                 ///< [in] owner's ID
  );

  // other public methods
  /// get bit name
  /// @return bit name
  std::string name();

  /// control to put or not put error message
  /// @return none
  void surpress_message(
    bool control                           ///< [in] control flag whether putting error message
  );
  
  // get value of bit                              
  /// @return the value of bit as unsigned int type
  unsigned int read(                           
  )const;                                          

private:
  // set value for bit
  /// set value for bit without invalid checking
  /// @return none
  void SetValue(
    const unsigned int data                ///< [in] setting data for bit
  );

  /// set value for bit with invalid checking
  /// @return true/false if writing is successful/unsuccessful
  bool WriteValue(
    const unsigned int value               ///< [in] written data for bit
  );

  unsigned int  mAddr;                     ///< register address
  re_register  *mParent;                   ///< re_register pointer
  bool          mSurpressMessage;          ///< surpress error message
  unsigned int  mModeNum;                  ///< access owner number
};

/// general register class
class re_register
{

public:
  // register setting
  /// default constructor
  /// @return none
  re_register();

  /// constructor with an input object
  /// @return none
  re_register(
    const re_register &                    ///< [in] register object
  );

  /// destructor
  /// @return none
  ~re_register();

  /// constructor with register set
  /// @return none
  re_register(
    unsigned int  addr,                    ///< [in] register address
    reg_super    *parent,                  ///< [in] reg_super pointer to use call-back funtions
    std::string   reg_name = "unknown",    ///< [in] register name
    const char   *mod_name = "unknown"     ///< [in] module name
  );

  /// copy operator
  /// @return register object
  re_register &operator=(
    const re_register &                    ///< [in] register object
  );

  /// specify register setting
  /// @return none
  void set(
    unsigned int  addr,                   ///< [in] register address
    reg_super    *parent,                 ///< [in] reg_super pointer to use call-back funtions
    std::string   reg_name,               ///< [in] register name
    const char   *mod_name                ///< [in] module name
  );

  // bit setting
  /// specify bit information multi
  /// @return none
  void operator ()(
    unsigned int  eaddr,                  ///< [in] end address
    unsigned int  saddr,                  ///< [in] start address
    std::string   name,                   ///< [in] bit name
    unsigned int  init_val,               ///< [in] initial value
    int           mode,                   ///< [in] access mode
    eSupportMode  spp = SPP_ENABLE,       ///< [in] supported mode
    bool          overlap = false         ///< [in] overlap (Added Mar.17.2015)
  );

  /// specify bit information single
  /// @return none
  void operator ()(
    unsigned int  eaddr,                  ///< [in] start/end address
    std::string   name,                   ///< [in] bit name
    unsigned int  init_val,               ///< [in] initial value
    int           mode,                   ///< [in] access mode
    eSupportMode  spp = SPP_ENABLE        ///< [in] supported mode
  );

  // bit setting supported for enhancement
  /// specify bit information multi
  /// @return none
  void operator ()(
    unsigned int  eaddr,                  ///< [in] end address
    unsigned int  saddr,                  ///< [in] start address
    std::string   name,                   ///< [in] bit name
    unsigned int  init_val = 0,           ///< [in] initial value
    std::string   mode = "R|W",           ///< [in] access mode
    eSupportMode  spp = SPP_ENABLE,       ///< [in] supported mode
    std::string   writable_list = "",     ///< [in] writable value list
    bool          overlap = false         ///< [in] overlap (Added Mar.17.2015)
  );

  /// specify bit information single
  /// @return none
  void operator ()(
    unsigned int  eaddr,                  ///< [in] start/end address
    std::string   name,                   ///< [in] bit name
    unsigned int  init_val = 0,           ///< [in] initial value
    std::string   mode = "R|W",           ///< [in] access mode
    eSupportMode  spp = SPP_ENABLE,       ///< [in] supported mode
    std::string   writable_list = ""      ///< [in] writable value list
  );

  // register access
  /// assignment operator
  /// @return true/false if assignment is successful/unsuccessful
  re_register &operator = (
    unsigned int data                     ///< [in] value assigned to register
  );

  /// writing with access limitation
  /// @return true/false if writing is successful/unsuccessful
  bool write(
    unsigned int data                     ///< [in] written value to register
  );

  /// reference operator
  /// @return the value of register
  operator unsigned int ();

  /// read with access limitation
  /// @return the value of register
  unsigned int read();

  /// logical and assign operator
  /// @return true/false if assignment is successful/unsuccessful
  re_register &operator &= (
    unsigned int data                     ///< [in] value assigned to register
  );

  /// logical or assign operator
  /// @return true/false if assignment is successful/unsuccessful
  re_register &operator |= (
    unsigned int data                     ///< [in] value assigned to register
  );

  /// increment operator
  /// @return register object
  re_register &operator ++ (
    int dummy                             ///< [in] dummy argument
  );

  /// decrement operator
  /// @return register object
  re_register &operator -- (
    int dummy                             ///< [in] dummy argument
  );

  /// n-increment operator
  /// @return register object
  re_register &operator += (
    unsigned int data                     ///< [in] added value
  );

  /// n-decrement operator
  /// @return register object
  re_register &operator -= (
    unsigned int data                     ///< [in] subtracted value
  );

  // bit access
  /// bit reference
  /// @return bit_info object
  bit_info &operator[](
    const char * name                     ///< [in] bit name
  );

  /// specified area access
  /// @return the value of specified bit
  unsigned int operator[](
    unsigned int addr                     ///< [in] bit position
  );

  // multi master
  /// set access owner
  /// @return none
  void access_owner(
    unsigned int num                      ///< [in] access owner ID
  );

  // other
  /// initialize register value
  /// @return none
  void reset();

  /// get register name
  /// @return register name
  std::string name();

  /// get register address
  /// @return register address
  unsigned int addr();

  /// dump debug message with specified level
  /// @return none
  void _message( 
    eMsgType type,                        ///< [in] output message level
    std::string fmt=""                    ///< [in] message content
    ,...
  );

  /// get access mode from input setting
  /// @return an integer number that indicates access mode
  int GetAccessMode(
    int mode                              ///< [in] access mode
  );

  reg_super    *mParent;                  ///< target model pointer to access cb function

  /// initialize message level debug
  /// @return none
  void InitMsgLevel();

  ///Set mMessageLevel
  /// @return none
  void SetMessageLevel(
    std::map<std::string, bool> DbgMsgLevel
  );

  ///Get mMessageLevel
  /// @return none
  std::string GetMessageLevel(
  );
  void UpdateBitInitValue (std::string bit_name, unsigned int value);
  bit_info     *mBitInfo;                 ///< end point of bit list

  void EnableReset(bool is_active); 
  void get_fileline(std::string filename, unsigned int line_number);

  std::map<std::string, bool> mMessageLevel;///< set debug message level
private:
  bool mIsReset; // current reset status
  std::string mFileName;
  unsigned int mLineNum;

  /// check error occurs when defining register
  /// @return true/false if error occurs/not occurs
  bool CheckError(
    unsigned int eaddr,                   ///< [in] end address
    unsigned int saddr,                   ///< [in] start address
    std::string bit_name,                 ///< [in] bit name
    bool         overlap                  ///< [in] overlap (Added Mar.17.2015)
  );

  /// get the access mode from input string
  /// @return the access mode of register or bit
  eAccessMode GetAccessMode(
    const std::string mode                ///< [in] access mode
  );

  /// get writable value type
  /// @return the type of writable value
  eWritableType GetWritableType(
    const std::string writable_list,      ///< [in] writable value list
    std::vector<unsigned int> &val_list   ///< [out] list of writable value after spliting
  );

  /// check an input string is a number or not
  /// @return true/false if it is a number/not a number
  bool IsNumber(
    const std::string num_str             ///< [in] number string
  );

  /// check an input string is a digit string or not
  /// @return true/false if it is a digit string
  bool IsDigitStr(
    const int type,                       ///< [in] type of number string (binary, decimal, hexa...)
    const std::string dig_str             ///< [in] digital string
  );

  /// convert a string to an unsigned integer number
  /// @return the value of converted string
  unsigned int Str2Int(
    const std::string num_str             ///< [in] number string
  );

  /// get the number value from an input number string
  /// @return the value of number string
  unsigned int GetValue(
    const int type,                       ///< [in] type of number string (binary, decimal, hexa...)
    const std::string dig_str             ///< [in] digital string
  );

  /// split a input string into many substrings by delimiter
  /// @return none
  void Split(
    const std::string writable_list,      ///< [in] writable value list
    const char delimiter,                 ///< [in] delimiter to split value list
    std::vector<std::string> &str_list    ///< [out] list of number string after spliting
  );

  /// create bit from a set of bit information
  /// @return none
  void CreateBit(
    unsigned int  eaddr,                  ///< [in] end address
    unsigned int  saddr,                  ///< [in] start address
    std::string   bit_name,               ///< [in] bit name
    unsigned int  init_val,               ///< [in] initial value
    int           mode,                   ///< [in] access mode
    eSupportMode  spp,                    ///< [in] supported mode
    eWritableType value_type,             ///< [in] writable value type
    std::vector<unsigned int> writable_list ///< [in] writable value list
  );

  unsigned int  mAddr;                    ///< specified register's address
  std::string   mModName;                 ///< model name
  std::string   mRegName;                 ///< register name
  unsigned int  mEntried;                 ///< entried area check
  unsigned int  mAccessNum;               ///< access owner number
  bool          mInitialized;             ///< flag whether register is initialized or not
};

} //namespace
#endif//__RE_REGISTER_H
