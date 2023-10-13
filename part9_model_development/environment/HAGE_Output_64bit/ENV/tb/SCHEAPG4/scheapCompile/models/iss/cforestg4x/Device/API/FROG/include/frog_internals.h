#if ! defined(__FROG_IF_INTERNALS_H__)
#define __FROG_IF_INTERNALS_H__

#include <tuple>
#include <vector>
#include <stack>
#include <string>
#include "./frog_reg_profile.h"


/** \file frog_internals.h
 * \brief FROG internal functions header file.
 *
 * This file contains the declaration of internal functions for FROG interface.
 * FROG interface will use these functions to connect API functions together, also connect to CForest.
 *
 */


/** Internal function for CForest I/F **/
/**
 *   This function eliminate blank space at head and tail of string input..
 *
 * @param[in] str the string need to trim.
 * @return string that eliminated space.
 */
extern std::string gcs_internal_string_trim(const std::string& str);
/**
 *   This function makes string from input data.
 *
 * @param[in]  size size of data needs to make string.
 * @param[in]  data data needs to make string.
 * @param[out] ret  the string recently created.
 * @return string is created or not.
 */
extern int gcs_internal_make_string_data(int* size, const char* data, char** ret);
/**
 *   This function makes string from input data.
 *
 * @param[in]  size size of data needs to make string.
 * @param[in]  data data needs to make string.
 * @param[out] ret  the string recently created.
 * @return string is created or not.
 */
extern int gcs_internal_make_data_string(int size, char* data, char** ret);
/**
 *   This function makes string from input data.
 *
 * @param[in] in_str  size of data needs to make string.
 * @param[in] out_str data needs to make string.
 * @return string is created or not.
 */
extern void ToUpperCopy (const std::string &in_str, std::string *out_str);
/**
 *   This function makes string from input data.
 *
 * @param[in] val size of data needs to make string.
 * @return string is created or not.
 */
extern uint32_t gcs_internal_get_psw_on_current_mode (uint32_t* val);
/**
 *   This function makes string from input data.
 *
 * @param[in] is_use size of data needs to make string.
 * @return string is created or not.
 */
extern uint32_t gcs_internal_use_gmpsw (bool* is_use);

/**
 * \var typedef std::tuple<uint32_t, int32_t, std::string> RegTupleType
 *
 * \brief Register info included peid, tcid, name.
 */
typedef std::tuple<uint32_t, int32_t, std::string> RegTupleType; // peid, tcid, reg_name
/**
 *   This function makes string from input data.
 *
 * @param[in] peid     size of data needs to make string.
 * @param[in] tcid     size of data needs to make string.
 * @param[in] reg_name size of data needs to make string.
 * @return string is created or not.
 */
extern void gcs_internal_record_reg_write (uint32_t peid, int32_t tcid, const std::string &reg_name);
/**
 *   This function makes string from input data.
 *
 * \param -
 * @return string is created or not.
 */
extern RegTupleType gcs_internal_get_reg_write (void);
/**
 *   This function makes string from input data.
 *
 * \param -
 * @return string is created or not.
 */
extern bool gcs_internal_IsEmpty_reg_write (void);


/**
 * \class CfrogRegWriteRecord
 *
 * \brief Register write record class.
 *
 * This class is used to a buffer to contain the register write information.
 * It will have 3 operations: push, pop and IsEmpty.
 *
 */

class CfrogRegWriteRecord {
public:
    /**
     *   This function add new element to register vectors.
     *
     * @param[in] reg_rec new register will be added.
     * @return -
     */
    void push (RegTupleType reg_rec)
    {
        m_reg_vec.push (reg_rec);
    }
    /**
     *   This function remove last element from register vectors.
     *
     * \param -
     * @return last register.
     */
    RegTupleType pop (void)
    {
        RegTupleType ret = m_reg_vec.top ();
        m_reg_vec.pop ();
        return ret;
    }
    /**
     *   This function check register vector is empty or not.
     *
     * \param -
     * @return status of register vector.
     */
    bool IsEmpty (void) const { return m_reg_vec.empty(); }

    CfrogRegWriteRecord () {}
    ~CfrogRegWriteRecord () {}
private:
    std::stack<RegTupleType> m_reg_vec;
};
#endif /* __FROG_IF_INTERNALS_H__ */
