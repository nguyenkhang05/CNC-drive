/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "./config.h"
#include "./frog_internals.h"
#include "./frog_if.h"
#include "./forest_utility.h"


/** \file frog_internals.cpp
 * \brief FROG internal functions source file.
 *
 * This file contains the implementation of internal functions for FROG interface.
 * FROG interface will use these functions to connect API functions together, also connect to CForest.
 *
 */


extern ForestUtil * m_forest_util;
extern CfrogRegWriteRecord* g_reg_record;

// SR ID is 0x100 + (selID << 5) + regID
static const CReg SR_PSW = 0x105;
static const CReg SR_GMPSW = 0x225; // selID=9, regID=5
static const CReg SR_SESR = 0x10C;
static const CReg SR_PSWH = 0x10F;  // selID=0, regID=15
static const CReg SR_HVCFG = 0x130; // selID=1, regID=16

void ToUpperCopy (const std::string &in_str, std::string *out_str)
{
    std::transform(in_str.begin(), in_str.end(), std::back_inserter(*out_str), ::toupper);
}


uint32_t gcs_internal_use_gmpsw (bool* is_use)
{
    if (m_forest_util == NULL) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }

    uint32_t pe_id = m_forest_util->GetPeLastId();

    if (m_forest_util->IsValidPeId (pe_id) == false) {
        // invalid PEID
        return GCSIF_ERR_INVALID_OTHER_PARAM;
    }
    bool use_gmpsw;
    if ((m_forest_util->GetCoreType (pe_id) == CORE_G4MH) && (m_forest_util->GetCoreVersion (pe_id) >= CORE_VER_20)) {
        uint64_t hvcfg_data = 0;
        if ( m_forest_util->RegReadDebug(pe_id, SR_HVCFG, &hvcfg_data) == false ) {
            return GCSIF_ERR_INVALID_OTHER_PARAM;
        }
        if ((hvcfg_data & 0x1) == 0x1) {
            uint64_t pswh_data = 0;
            if ( m_forest_util->RegReadDebug(pe_id, SR_PSWH, &pswh_data) == false ) {
                return GCSIF_ERR_INVALID_OTHER_PARAM;
            }
            if (((pswh_data >> 31) & 0x1) == 0x0) {
                use_gmpsw = false; // Host mode. Use PSW.
            } else {
                use_gmpsw = true; // Guest mode. Use GMPSW.
            }
        } else {
            use_gmpsw = false; // HV is disabled. Use PSW.
        }
    } else {
        use_gmpsw = false; // HV is disabled in before G4MH2.0. Use PSW.
    }
    *is_use = use_gmpsw;
    return GCSIF_SUCCESS;
}


uint32_t gcs_internal_get_psw_on_current_mode (uint32_t* val)
{
    uint64_t psw_data64 = 0x0;
    uint32_t psw_data;

    if (m_forest_util == NULL) {
        return GCSIF_ERR_NOT_INITIALIZED;
    }

    uint32_t pe_id = m_forest_util->GetPeLastId();

    if (m_forest_util->IsValidPeId (pe_id) == false) {
        // invalid PEID
        return GCSIF_ERR_INVALID_OTHER_PARAM;
    }
    bool use_gmpsw = false;
    uint32_t err = gcs_internal_use_gmpsw (&use_gmpsw);
    if (err != GCSIF_SUCCESS) {
        return err;
    }
    if (use_gmpsw) {
        if ( m_forest_util->RegReadDebug(pe_id, SR_GMPSW, &psw_data64) == false ) {
            return GCSIF_ERR_INVALID_OTHER_PARAM;
        }
    } else {
        if ( m_forest_util->RegReadDebug(pe_id, SR_PSW, &psw_data64) == false ) {
            return GCSIF_ERR_INVALID_OTHER_PARAM;
        }
    }
    psw_data = static_cast<uint32_t>(psw_data64 & 0xFFFFFFFFU);
	*val = psw_data;
    return GCSIF_SUCCESS;
}



std::string gcs_internal_string_trim(const std::string& str)
{
    std::string::const_iterator head, tail;

    if (str.length() < 1)
    {
        return std::string("");
    }
    head = str.begin();
    tail = str.end();

    while (isspace(*head))
    {
        ++head;
    }
    if (head != tail)
    {
        do
        {
            --tail;
        }
        while (isspace(*tail));
        ++tail;
    }

    return std::string(head, tail);
}

int gcs_internal_make_string_data(int* size, const char* data, char** ret)
{
    static std::vector<char> array;
    std::stringstream ss;

    /* init */
    array.clear();
    ss.str("");
    ss.clear();

    /* convert */
    {
        std::string strdata(data);

        /* "0x"���� */
        std::string::size_type pos;
        while ((pos = strdata.find("0x", 0, 2)) != std::string::npos)
        {
            strdata.replace(pos, 2, " ");
        }
        while ((pos = strdata.find("0X", 0, 2)) != std::string::npos)
        {
            strdata.replace(pos, 2, " ");
        }
        while ((pos = strdata.find(",", 0, 1)) != std::string::npos)
        {
            strdata.replace(pos, 2, " ");
        }
        ss << gcs_internal_string_trim(strdata);
    }

    while (!ss.eof())
    {
        uint32_t u;
        std::string hexstr;

        ss >> hexstr;
        if (sscanf(hexstr.c_str(), "%02X", &u) != 1)
        {
            return -1;
        }
        array.push_back((char)u);
    }
    if (ss.fail())
    {
        return -1;
    }

    *size    = array.size();
    *ret    = (char*)&array[0];

    return 0;
}


int gcs_internal_make_data_string(int size, char* data, char** ret)
{
   std::stringstream ss;
   static std::string retstr;

   if (size > 256) {
      return -1;
   }
   ss.str("");
   ss.clear();
   ss << "(";
   for (int i = 0; i < size; i++) {
      ss << "0x" << std::hex << std::setw(2) << std::setfill('0') << (data[i] & 0xFF);
      if (i < (size - 1)) {
         ss << ",";
      }
   }
   ss << ")";
   retstr = ss.str();
   *ret = (char*)retstr.c_str();

   return 0;
}


/**
 * Internal Interface - Push history of register writing to internal record
 *
 * @param[in] peid     PEID of accessed register
 * @param[in] tcid     TCID of accessed register
 * @param[in] reg_name Name of accessed register.
 * @return void
 */
void gcs_internal_record_reg_write (uint32_t peid, int32_t tcid, const std::string &reg_name)
{
    g_reg_record->push (std::make_tuple (peid, tcid, reg_name));
}


/**
 * Internal Interface - Pop history of register writing from internal record
 *
 * \param -
 * @return RegTupleType (std::tuple<uint32_t peid, int32_t tcid, std::string regname>)
 * @retval If record is empty, tuple (0,0,"") will be returned.
 */
RegTupleType gcs_internal_get_reg_write (void)
{
    if (g_reg_record->IsEmpty ()) return std::make_tuple (0U,0,"");
    return g_reg_record->pop ();
}


/**
 * Internal Interface - Check IsEmpty register write info in internal record
 *
 * \param -
 * @return bool IsEmpty
 */
bool gcs_internal_IsEmpty_reg_write (void)
{
    return g_reg_record->IsEmpty ();
}
