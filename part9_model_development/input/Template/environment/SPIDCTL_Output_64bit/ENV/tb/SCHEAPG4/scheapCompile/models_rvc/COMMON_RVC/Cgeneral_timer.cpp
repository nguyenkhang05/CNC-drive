// -----------------------------------------------------------------------------
// $Id: Cgeneral_timer.cpp,v 1.1.1.1 2018/06/18 08:55:02 chuonghoangle Exp $
//
// Copyright(c) 2008, 2012 Renesas Electronics Corporation
// Copyright(c) 2008, 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "math.h"
#include "dbglib.h"
#include "Cgeneral_timer.h"

// ============================================================ //
// Name    : compareBigNumber                                   //
// Type    : private method                                     //
// Function: compare two big numbers                            //
// Input   : num1 - first number                                //
//           num2 - second number                               //
//           bit_length - length in bit                         //
// Output  : None                                               //
// Return  : int value.                                         //
//             < 0: num1 < num2                                 //
//             = 0: num1 = num2                                 //
//             > 0: num1 > num2                                 //
// Date    : 11/26/08                                           //
// ============================================================ //
int Cgeneral_timer::compareBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length)
{
    int result = 0;     // store the comparison result
    // validate bit_length
    if (bit_length != 0 && num1 != NULL && num2 != NULL) {
        // calculate temporary length
        unsigned int tmp_len = bit_length/_BITS_PER_UINT_;
        unsigned int index = (bit_length % _BITS_PER_UINT_ == 0) ? tmp_len : tmp_len + 1; // store the comparison index

        unsigned int first_comp = 0;
        unsigned int second_comp = 0;
        unsigned int remain_len = 0;
        unsigned int mask = 0;

        while (result == 0 && index > 0) {
            index--;

            if (tmp_len-1 != index) {
                first_comp = num1[index];
                second_comp = num2[index];
            }
            else {
                remain_len = bit_length - ((unsigned int) tmp_len * _BITS_PER_UINT_);
                remain_len = (remain_len == 0) ? _BITS_PER_UINT_ : remain_len;
                mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - remain_len);
                first_comp = num1[index]; first_comp &= mask;
                second_comp = num2[index]; second_comp &= mask;
            }


            if (first_comp > second_comp) {
                result = 1;
            }
            else if (first_comp < second_comp) {
                result = -1;
            }
            else {
                result = 0;
            }
        }
    }
    return result;
}

// ============================================================ //
// Name    : copyBigNumber                                      //
// Type    : private method                                     //
// Function: copy value of big numbers                          //
// Input   : num1 - source number                               //
//           bit_length - length in bit                         //
// Output  : num2 - destination number                          //
// Return  : None                                               //
// Date    : 11/27/08                                           //
// ============================================================ //
void Cgeneral_timer::copyBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length)
{
    // validate bit_length
    if (bit_length != 0 && num1 != NULL && num2 != NULL) {
        // calculate temporary length
        unsigned int tmp_len = bit_length/_BITS_PER_UINT_;
        unsigned int index = 0; // store the copying index
 
        // Just copy 32-bit block (1 unsigned-int size) if enough.
        // Keep the remaining bits (if having) for latter copy.
        while (index != tmp_len) {
            num2[index] = num1[index];
            index++;
        }
 
        // Copy remaining bits if having
        // reuse the tmp_len to store the number of remaining bits
        tmp_len = bit_length - (tmp_len * _BITS_PER_UINT_);
        if (tmp_len != 0) {
            unsigned int mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - tmp_len);
            num2[index] = num1[index] & mask;
        }
    }
}

// ============================================================ //
// Name    : resetBigNumber                                     //
// Type    : private method                                     //
// Function: reset value of big numbers                         //
//                based on counting direction                   //
// Input   : num - source number                                //
//           bit_length - length in bit                         //
//           direction - counting direction                     //
// Output  : num - destination number                           //
// Return  : None                                               //
// Date    : 11/27/08                                           //
// ============================================================ //
void Cgeneral_timer::resetBigNumber (unsigned int* num, unsigned int bit_length, counter_direction direction)
{
    // validate bit_length
    if (bit_length != 0 && num != NULL) {
        unsigned int tmp_len = bit_length/_BITS_PER_UINT_;
        unsigned int index = bit_length - (tmp_len * _BITS_PER_UINT_);
        unsigned int mask = (index != 0) ? _MAX_UINT_ >> (_BITS_PER_UINT_ - index) : _MAX_UINT_;
        unsigned int resetVal = (direction == emCountUp) ? 0 : _MAX_UINT_;

        index = 0; // store the resetting index
        while (index != tmp_len) {
            num[index] = resetVal;
            index++;
        }

        tmp_len = bit_length - (tmp_len * _BITS_PER_UINT_);
        if (tmp_len != 0) {
            num[index] = resetVal & mask;
        }
    }
}

// ============================================================ //
// Name    : converBigNumberToHexStr                            //
// Type    : private method                                     //
// Function: convert big number to hex string                   //
// Input   : num - big number                                   //
//           bit_length - bit-length of input number            //
// Output  : str - string                                       //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::converBigNumberToHexStr (unsigned int *num, char *str, unsigned int bit_length)
{
    if (bit_length != 0 && num != NULL && str != NULL) {
        // calculate temporary length
        unsigned int tmp_len;
        tmp_len = (bit_length % _BITS_PER_UINT_ == 0) ? bit_length/_BITS_PER_UINT_ : (bit_length/_BITS_PER_UINT_) + 1;
 
        unsigned int index = tmp_len - 1;
        bool flag = true;
        while (flag) {
            sprintf (&str[(tmp_len - index - 1) * _CHARS_PER_UINT_], "%08X", num[index]);
            flag = (index == 0) ? false : true;
            index--;
        }
    }
}

// ============================================================ //
// Name    : resizeBigNumber                                    //
// Type    : private method                                     //
// Function: changing size of a big number                      //
// Input   : num - big number                                   //
//           old_bit_length - old bit-length of input number    //
//           new_bit_length - new bit-length of input number    //
// Output  : num - new big number                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::resizeBigNumber (unsigned int **num, unsigned int old_bit_length, unsigned int new_bit_length)
{
    if (old_bit_length == new_bit_length) {
        return;
    }

    if (num != NULL) {
        unsigned int tmp_len;   // store the length according to new size
        unsigned int index;     // store the processing index
        if (old_bit_length > new_bit_length) {
            // calculate the last index
            index = new_bit_length / _BITS_PER_UINT_;
            // calculate the new length
            tmp_len = new_bit_length - (index * _BITS_PER_UINT_);
            if (tmp_len != 0) {
                unsigned int mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - tmp_len);
                (*num)[index] = (*num)[index] & mask;
            }
        }
        else {
            // calculate new length
            tmp_len = (new_bit_length % _BITS_PER_UINT_ == 0) ? new_bit_length/_BITS_PER_UINT_ : (new_bit_length/_BITS_PER_UINT_) + 1;
            // calculate last old index
            index = (old_bit_length % _BITS_PER_UINT_ == 0) ? old_bit_length/_BITS_PER_UINT_ : (old_bit_length/_BITS_PER_UINT_) + 1;
 
            unsigned int *tmp = NULL;
            if (*num != NULL) {
                if (tmp_len != index) {
                    tmp = *num;
                    *num = new unsigned int [tmp_len];
                    copyBigNumber (tmp, *num, old_bit_length);
                }
            }
            else {
                *num = new unsigned int [tmp_len];
                index = 0;
            }
 
            if (*num == NULL) {
                DBG_MSG ("Error: Cannot allocate more memory to resize the big number!\n");
                *num = tmp;
            }
            else {
                while (index != tmp_len) {
                    (*num)[index] = 0;
                    index ++;
                }
 
                if (tmp != NULL) {
                    delete [] tmp;
                }
            }
        }
    }
}

// ============================================================ //
// Name    : setCounterDirection                                //
// Type    : protected method                                   //
// Function: set the value of direction of counting             //
// Input   : value - counter direction                          //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::setCounterDirection (counter_direction value)
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot set counter direction in slave timer!\n");
        return;
    }

    if (value != emCountUp
            && value != emCountDown) {
        DBG_MSG ("Error: Set invalid value to counter direction!\n");
    }
    else {
        updateCNTValue();
        direction = value;
    }
}

// ============================================================ //
// Name    : setCmpMatchOutType                                 //
// Type    : protected method                                   //
// Function: set compare match output type (0, 1, or emToggle)    //
// Input   : value - type of output                             //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::setCmpMatchOutType (cmp_match_out_type value)
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot set compare match output type in slave timer!\n");
        return;
    }

    if (value != emLowActive
            && value != emHighActive
            && value != emToggle) {
        DBG_MSG ("Error: Set invalid value to compare match output type!\n");
    }
    else {
        cmpMatchOutType = value;
    }
}

// ============================================================ //
// Name    : getCounterDirection                                //
// Type    : protected method                                   //
// Function: get the value of direction of counting             //
// Input   : None                                               //
// Output  : None                                               //
// Return  : counter_direction value                            //
// Date    : 12/01/08                                           //
// ============================================================ //
Cgeneral_timer::counter_direction Cgeneral_timer::getCounterDirection ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Warning: getCounterDirection is called from a slave timer! The one of master is called instead.\n");

        Cgeneral_timer* head = cascadedList.front();
        
        if (head != NULL) {
            return head -> getCounterDirection();
        }
    }

    return direction;
}

// ============================================================ //
// Name    : getCmpMatchOutType                                 //
// Type    : protected method                                   //
// Function: get compare match output type (0, 1, or emToggle)    //
// Input   : None                                               //
// Output  : None                                               //
// Return  : cmp_match_out_type value                           //
// Date    : 12/01/08                                           //
// ============================================================ //
Cgeneral_timer::cmp_match_out_type Cgeneral_timer::getCmpMatchOutType ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Warning: getCmpMatchOutType is called from a slave timer! The one of master is called instead.\n");

        Cgeneral_timer* head = cascadedList.front();
        
        if (head != NULL) {
            return head -> getCmpMatchOutType();
        }
    }

    return cmpMatchOutType;
}

// ============================================================ //
// Name    : setCNT                                             //
// Type    : protected method                                   //
// Function: set the value of gtCNT (less or equal 32 bits)       //
// Input   : value - value of gtCNT                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::setCNT (unsigned int value)
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot set value to gtCNT in slave timer!\n");
        return;
    }

    if (gtCNT != NULL) {
        if (numOfCascadedItems > _NUM_1_) {
            DBG_MSG ("Warning: setCNT is called in cascaded Master! Try to use setBigValueCNT instead.\n");
            DBG_MSG ("Warning: Just low 32-bit value of Master's gtCNT is set.\n");
        }
        else if (length > 1) {
            DBG_MSG ("Warning: setCNT is called but current size of counter is bigger than 32 bits.\n");
            DBG_MSG ("Warning: Just low 32-bit of gtCNT is set.\n");
        }

        Cgeneral_timer* head = cascadedList.front();
        
        if (head != NULL) {
            head -> updateCNTValue();
        }

        unsigned int shift_len = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _BITS_PER_UINT_ : gtCR.gtCMS;
        unsigned int mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - shift_len);
        gtCNT[0] = value & mask;
    }
}

// ============================================================ //
// Name    : setCOR                                             //
// Type    : protected method                                   //
// Function: set the value of gtCOR (less or equal 32 bits)       //
// Input   : value - value of gtCOR                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::setCOR (unsigned int value)
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot set value to gtCOR in slave timer!\n");
        return;
    }

    if (gtCOR != NULL) {
        if (numOfCascadedItems > _NUM_1_) {
            DBG_MSG ("Warning: setCOR is called in cascaded Master! Try to use setBigValueCOR instead.\n");
            DBG_MSG ("Warning: Just low 32-bit value of Master's gtCOR is set.\n");
        }
        else if (length > 1) {
            DBG_MSG ("Warning: setCOR is called but current size of counter is bigger than 32 bits.\n");
            DBG_MSG ("Warning: Just low 32-bit of gtCOR is set.\n");
        }

        Cgeneral_timer* head = cascadedList.front();

        if (head != NULL) {
            head -> updateCNTValue();
        }

        unsigned int shift_len = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _BITS_PER_UINT_ : gtCR.gtCMS;
        unsigned int mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - shift_len);
        gtCOR[0] = value & mask;
    }
}

// ============================================================ //
// Name    : clearPR                                            //
// Type    : protected method                                   //
// Function: clear the value of PR                              //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::clearPR ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot clear PR in slave timer!\n");
        return;
    }

    list <Cgeneral_timer*>::iterator listIter;
    listIter = cascadedList.begin();
    unsigned int index = 0;
    unsigned int main_index = 0;
    Cgeneral_timer *timer_ptr = *listIter;

    while (main_index < numOfCascadedItems) {
        timer_ptr = *listIter;
        if (timer_ptr != NULL && timer_ptr -> gtPR != NULL) {
            while (index < length) {
                timer_ptr -> gtPR[index] = 0;
                index++;
            }
        }
        index = 0;
        main_index++;
        listIter++;
    }
}

// ============================================================ //
// Name    : getCNT                                             //
// Type    : protected method                                   //
// Function: get the 32-bit value of gtCNT                        //
// Input   : None                                               //
// Output  : None                                               //
// Return  : unsigned int value - 32-bit value of gtCNT           //
// Date    : 12/01/08                                           //
// ============================================================ //
unsigned int Cgeneral_timer::getCNT ()
{
    if (gtCNT != NULL) {
        if (length > 1) {
            DBG_MSG ("Warning: getCNT is called but current size of counter is bigger than 32 bits!\n");
            DBG_MSG ("Warning: Just return the value of low 32 bits of gtCNT.\n");
        }

        Cgeneral_timer* head = cascadedList.front();
        sc_time current_time = sc_time_stamp();
        sc_time clock_period(counting_period, time_unit);
        unsigned int returnCNT = 0;
        unsigned int shift_len = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _BITS_PER_UINT_ : gtCR.gtCMS;
        unsigned int mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - shift_len);
        if (head != NULL) {
            if (mSupportDecoupling) {
                if (mLocalAccessTime.to_double() < (current_time + mLocalTime).to_double()) {
                    mLocalAccessTime = current_time + mLocalTime;
                }
                else {
                    DBG_MSG ("Warning: The CNT value is accessed at the past time.!\n");
                }
                // Adjust the start_time and mLocalTime for the first quantum
                if (current_time.to_double() == start_time_tmp.to_double()) {
                    // Adjust the start_time variable
                    if (mLocalTime.to_double() >= wait_to_start) {
                        // update start time value
                        start_time = start_time_tmp + sc_time(wait_to_start, time_unit);
                        // Adjust local time value
                        mLocalTime = mLocalTime - sc_time(wait_to_start,time_unit);
                    }
                    else {
                        // The counter is not started
                        return (gtCNT[0] & mask);
                    }
                }
                // Operation for processing temporal decoupling
                head -> updateCNTValue();
                unsigned int dif_val = (unsigned int)((mLocalTime - start_local_time)/clock_period);
                dif_val = dif_val & mask;
                if (gtCR.gtCMM == emOneShot)
                {
                    // Handle for one-shot operation 
                    if (gtCR.gtSTR == true) {
                        if ((gtCR.gtCCLR == emInFactor)||(gtCR.gtCCLR == emBothInExFactor)) {
                            if (direction == emCountDown) {
                                returnCNT = ((gtCNT[0] - dif_val) <= getCOR()) ? mask : (gtCNT[0] - dif_val);
                            } 
                            else {
                                returnCNT = ((gtCNT[0] + dif_val) >= getCOR()) ? 0x0 : (gtCNT[0] + dif_val);
                            }
                        }
                        else {
                            if (direction == emCountDown) {
                                returnCNT = (gtCNT[0]  < dif_val) ? mask : (gtCNT[0] - dif_val);
                            } 
                            else {
                                returnCNT = ((mask - gtCNT[0]) < dif_val) ? 0x0 : (gtCNT[0] + dif_val);
                            }
                        }
                    }
                    else {
                        returnCNT = gtCNT[0];
                    }
                }
                else {
                    // Handle for free-run operation
                    if (gtCR.gtSTR == true) {
                        if ((gtCR.gtCCLR == emInFactor)||(gtCR.gtCCLR == emBothInExFactor)) {
                            if (direction == emCountDown) {
                                returnCNT = ((gtCNT[0] - dif_val) <= getCOR()) ? mask : (gtCNT[0] - dif_val);
                            } 
                            else {
                                returnCNT = ((gtCNT[0] + dif_val) >= getCOR()) ? 0x0 : (gtCNT[0] + dif_val);
                            }
                        }
                        else {
                            returnCNT = (direction == emCountDown) ? (gtCNT[0] - dif_val) : (gtCNT[0] + dif_val);
                        }
                    }
                    else {
                        returnCNT = gtCNT[0];
                    }
                }
            }
            else {
                head -> updateCNTValue();
                returnCNT = gtCNT[0];
            }
        }
        return (returnCNT & mask);
    }

    return 0;
}

// ============================================================ //
// Name    : getCOR                                             //
// Type    : protected method                                   //
// Function: get the 32-bit value of gtCOR                        //
// Input   : None                                               //
// Output  : None                                               //
// Return  : unsigned int value - 32-bit value of gtCOR           //
// Date    : 12/01/08                                           //
// ============================================================ //
unsigned int Cgeneral_timer::getCOR ()
{
    if (gtCOR != NULL) {
        if (length > 1) {
            DBG_MSG ("Warning: getCOR is called but current size of counter is bigger than 32 bits!\n");
            DBG_MSG ("Warning: Just return the value of low 32 bits of gtCOR.\n");
        }

        unsigned int shift_len = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _BITS_PER_UINT_ : gtCR.gtCMS;
        unsigned int mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - shift_len);
        return (gtCOR[0] & mask);
    }

    return 0;
}

// ============================================================ //
// Name    : getPR                                              //
// Type    : protected method                                   //
// Function: get the 32-bit value of PR                         //
// Input   : None                                               //
// Output  : None                                               //
// Return  : unsigned int value - 32-bit value of PR            //
// Date    : 12/01/08                                           //
// ============================================================ //
unsigned int Cgeneral_timer::getPR ()
{
    if (gtPR != NULL) {
        if (length > 1) {
            DBG_MSG ("Warning: getPR is called but current size of counter is bigger than 32 bits!\n");
            DBG_MSG ("Warning: Just return the value of low 32 bits of gtPR.\n");
        }

        unsigned int shift_len = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _BITS_PER_UINT_ : gtCR.gtCMS;
        unsigned int mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - shift_len);
        return (gtPR[0] & mask);
    }

    return 0;
}

// ============================================================ //
// Name    : clearAllFlag                                       //
// Type    : protected method                                   //
// Function: clear all flags of gtSR                              //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::clearAllFlag ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot clear flags of gtSR in slave timer!\n");
        return;
    }

    gtSR.gtCMF = false;
    gtSR.gtOVF = false;
    gtSR.gtUNF = false;
    gtSR.gtICF = false;
    gtSR.gtNumOfCMF = 0;
    gtSR.gtNumOfOVF = 0;
    gtSR.gtNumOfUNF = 0;
    gtSR.gtNumOfICF = 0;
}

// ============================================================ //
// Name    : clearCMF                                           //
// Type    : protected method                                   //
// Function: clear compare match flag of gtSR                     //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::clearCMF ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot clear CMF of gtSR in slave timer!\n");
        return;
    }

    gtSR.gtCMF = false;
}

// ============================================================ //
// Name    : clearOVF                                           //
// Type    : protected method                                   //
// Function: clear overflow flag of gtSR                          //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::clearOVF ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot clear OVF of gtSR in slave timer!\n");
        return;
    }

    gtSR.gtOVF = false;
}

// ============================================================ //
// Name    : clearUNF                                           //
// Type    : protected method                                   //
// Function: clear underflow flag of gtSR                         //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::clearUNF ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot clear UNF of gtSR in slave timer!\n");
        return;
    }

    gtSR.gtUNF = false;
}

// ============================================================ //
// Name    : clearICF                                           //
// Type    : protected method                                   //
// Function: clear input capture flag of gtSR                     //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::clearICF ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot clear ICF of gtSR in slave timer!\n");
        return;
    }

    gtSR.gtICF = false;
}

// ============================================================ //
// Name    : getSR                                              //
// Type    : protected method                                   //
// Function: get the status value                               //
// Input   : None                                               //
// Output  : None                                               //
// Return  : StatusRegister value                               //
// Date    : 12/01/08                                           //
// ============================================================ //
Cgeneral_timer::StatusRegister Cgeneral_timer::getSR ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Warning: getSR is called from a slave timer! The one of master is called instead.\n");

        Cgeneral_timer* head = cascadedList.front();
        
        if (head != NULL) {
            return head -> getSR();
        }
    }

    return gtSR;
}

// ============================================================ //
// Name    : setCmpMatchOutVal                                  //
// Type    : protected method                                   //
// Function: set current compare match output value             //
// Input   : value - input value                                //
// Output  : None                                               //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::setCmpMatchOutVal (bool value)
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Cannot set value of compare match output in slave timer!\n");
        return;
    }

    cmpMatchOut = value;
}

// ============================================================ //
// Name    : getCmpMatchOutVal                                  //
// Type    : protected method                                   //
// Function: get current compare match output value             //
// Input   : None                                               //
// Output  : None                                               //
// Return  : bool value - current compare match output value    //
// Date    : 12/01/08                                           //
// ============================================================ //
bool Cgeneral_timer::getCmpMatchOutVal ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Warning: getCmpMatchOutVal is called from a slave timer! The one of master is called instead.\n");

        Cgeneral_timer* head = cascadedList.front();
        
        if (head != NULL) {
            return head -> getCmpMatchOutVal();
        }
    }

    return cmpMatchOut;
}

// ============================================================ //
// Name    : dumpProfile                                        //
// Type    : protected method                                   //
// Function: get profile following input type                   //   
// Input   : type - input type of profile                       //
//           *fp - file used to store dumped information        //
// Output  : FILE *fp - file used to store dumped information   //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::dumpProfile (unsigned int type, FILE *fp)
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Warning: dumpProfile is called from a slave timer! The one of master is called instead.\n");
        Cgeneral_timer *head = cascadedList.front();
        if (head != NULL) {
            head -> dumpProfile(type, fp);
        }

        return;
    }   

    if (fp != NULL) {
        // check the input type
        if (type == _NUM_1_) {
            dumpProfileType1 (fp);
        }
        else if (type == _NUM_2_) {
            dumpProfileType2 (fp);
        }
        else {
            DBG_MSG ("Error: Invalid type of profile! Type 1 is used.\n");
            dumpProfileType1 (fp);
        }
    }
}

// ============================================================ //
// Name    : dumpProfileType1                                   //
// Type    : private method                                     //
// Function: get profile following input type                   //   
// Input   : *fp - file used to store dumped information        //
// Output  : FILE *fp - file used to store dumped information   //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::dumpProfileType1 (FILE *fp)
{
    char *tmp_str = new char [length * _CHARS_PER_UINT_];
    char cnt_str[_NUM_255_];
    char cor_str[_NUM_255_];
    char pr_str[_NUM_255_];

    Cgeneral_timer* head = cascadedList.front();
    list <Cgeneral_timer*>::iterator listIter;
    listIter = cascadedList.begin();
    Cgeneral_timer *timer = *listIter;

    unsigned int index = 0;
    updateCNTValue();

    if (head != NULL && fp != NULL && tmp_str != NULL) {
        fprintf (fp, "[%s] Profile type 1\n", this->name());
        fprintf (fp, "=================================================\n");
        fprintf (fp, "- Number of overflow: %d (times)\n", head -> gtSR.gtNumOfOVF);
        fprintf (fp, "- Number of underflow: %d (times)\n", head -> gtSR.gtNumOfUNF);
        fprintf (fp, "- Number of compare match: %d (times)\n", head -> gtSR.gtNumOfCMF);
        fprintf (fp, "- Number of input capture: %d (times)\n", head -> gtSR.gtNumOfICF);
        fprintf (fp, "- Registers' values\n");
        
        sprintf (cnt_str, "    + CNT:\n");
        sprintf (cor_str, "    + COR:\n");
        sprintf (pr_str, "    + PR:\n");
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                converBigNumberToHexStr(timer -> gtCNT, tmp_str, gtCR.gtCMS);
                sprintf (&cnt_str[strlen(cnt_str)], "        # %s.CNT = 0x%s\n", timer -> name(), tmp_str);
                converBigNumberToHexStr(timer -> gtCOR, tmp_str, gtCR.gtCMS);
                sprintf (&cor_str[strlen(cor_str)], "        # %s.COR = 0x%s\n", timer -> name(), tmp_str);
                converBigNumberToHexStr(timer -> gtPR, tmp_str, gtCR.gtCMS);
                sprintf (&pr_str[strlen(pr_str)], "        # %s.PR  = 0x%s\n", timer -> name(), tmp_str);
            }

            index++;
            listIter++;
        }

        fprintf (fp, "%s", cnt_str);
        fprintf (fp, "%s", cor_str);
        fprintf (fp, "%s", pr_str);

        fprintf (fp, "    + gtCR\n");
        fprintf (fp, "       . CMIE = %d, OVIE = %d, UNIE = %d, ICPE = %d\n",
                     head -> gtCR.gtCMIE, head -> gtCR.gtOVIE, head -> gtCR.gtUNIE, head -> gtCR.gtICPE);
        fprintf (fp, "       . CKS = %d, CKEG = %d\n", head -> gtCR.gtCKS, head -> gtCR.gtCKEG);
        fprintf (fp, "       . IE = %d, CCLR = %d\n", head -> gtCR.gtIE, head -> gtCR.gtCCLR);
        fprintf (fp, "       . CMS = %d, CMM = %d\n", head -> gtCR.gtCMS, head -> gtCR.gtCMM);
        fprintf (fp, "       . STR = %d\n", head -> gtCR.gtSTR);
        fprintf (fp, "    + gtSR: ");
        fprintf (fp, "CMF = %d, OVF = %d, UNF = %d, ICF = %d\n",
                     head -> gtSR.gtCMF, head -> gtSR.gtOVF, head -> gtSR.gtUNF, head -> gtSR.gtICF);
    }
    delete [] tmp_str;
}

// ============================================================ //
// Name    : dumpProfileType2                                   //
// Type    : private method                                     //
// Function: get profile following input type                   //   
// Input   : *fp - file used to store dumped information        //
// Output  : FILE *fp - file used to store dumped information   //
// Return  : None                                               //
// Date    : 12/01/08                                           //
// ============================================================ //
void Cgeneral_timer::dumpProfileType2 (FILE *fp)
{
    char cor_str[_NUM_255_];
    char pr_str[_NUM_255_];

    Cgeneral_timer* head = cascadedList.front();
    list <Cgeneral_timer*>::iterator listIter;
    listIter = cascadedList.begin();
    Cgeneral_timer *timer = *listIter;
    unsigned int index = 0;

    char *tmp_str = new char [length * _CHARS_PER_UINT_];

    if (fp != NULL && tmp_str != NULL && head != NULL) {
        fprintf (fp, "[%s] Profile type 2\n", this->name());
        fprintf (fp, "=================================================\n");
        sprintf (cor_str, "- Compare match value: \n");
        sprintf (pr_str, "- Input capture value: \n");
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                converBigNumberToHexStr(timer -> gtCOR, tmp_str, gtCR.gtCMS);
                sprintf (&cor_str[strlen(cor_str)], "    + %s.COR = 0x%s\n", timer -> name(), tmp_str);
                converBigNumberToHexStr(timer -> gtPR, tmp_str, gtCR.gtCMS);
                sprintf (&pr_str[strlen(pr_str)], "    + %s.PR  = 0x%s\n", timer -> name(), tmp_str);
            }

            index++;
            listIter++;
        }

        fprintf (fp, "%s", cor_str);
        fprintf (fp, "- Compare match ouput value = 0x%x\n", head -> cmpMatchOut);
        fprintf (fp, "%s", pr_str);
    }
    delete [] tmp_str;
}

// ============================================================ //
// Name     : getCR                                             //
// Type     : protected method                                  //
// Function : Get control register value                        //
// Input    : None                                              //
// Output   : None                                              //
// Return   : A struct contains all members' values of gtCR       //
// Date     : 11/28/08                                          //
// ============================================================ //
Cgeneral_timer::ControlRegister Cgeneral_timer::getCR()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Warning: getCR is called from a slave timer! The one of master is called instead.\n");

        Cgeneral_timer* head = cascadedList.front();
        
        if (head != NULL) {
            return head -> getCR();
        }
    }

    return gtCR;
}

// ============================================================ //
// Name     : setCR                                             //
// Type     : protected method                                  //
// Function : set new value for control register                //
// Input    : a struct of new control register                  //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setCR(ControlRegister *value)
{
    if (value == NULL) {
        DBG_MSG ("Error: setCR is called with NULL value argument!\n");
    }

    if (isCascadeMaster == true){
        gtCR.gtCMIE = value->gtCMIE;
        gtCR.gtOVIE = value->gtOVIE;
        gtCR.gtICPE = value->gtICPE;
        gtCR.gtUNIE = value->gtUNIE;
        setCKS(value->gtCKS);
        setCKEG(value->gtCKEG);
        gtCR.gtIE  = value->gtIE;
        setCCLR(value->gtCCLR);
        setCMS(value->gtCMS);
        setCMM(value->gtCMM);
        setSTR(value->gtSTR);
    }
    else{
        DBG_MSG ("Error: Cannot set new value for gtCR of slave timer!\n");
    }
}

// ============================================================ //
// Name     : setCMIE                                           //
// Type     : protected method                                  //
// Function : set new value for CMIE bit of control register    //
// Input    : the new value for CMIE bit                        //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setCMIE(bool value)
{
    if (isCascadeMaster == true){
        gtCR.gtCMIE = value;
    }
    else{
        DBG_MSG ("Error: Cannot set new value for CMIE bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setOVIE                                           //
// Type     : protected method                                  //
// Function : set new value for OVIE bit of control register    //
// Input    : the new value for OVIE bit                        //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setOVIE(bool value)
{
    if (isCascadeMaster == true){
        gtCR.gtOVIE = value;
    }
    else{
        DBG_MSG ("Error: Cannot set new value for OVIE bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setICPE                                           //
// Type     : protected method                                  //
// Function : set new value for ICPE bit of control register    //
// Input    : the new value for ICPE bit                        //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setICPE(bool value)
{
    if (isCascadeMaster == true){
        gtCR.gtICPE = value;
    }
    else{
        DBG_MSG ("Error: Cannot set new value for ICPE bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setUNIE                                           //
// Type     : protected method                                  //
// Function : set new value for UNIE bit of control register    //
// Input    : the new value for UNIE bit                        //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setUNIE(bool value)
{
    if (isCascadeMaster == true){
        gtCR.gtUNIE = value;
    }
    else{
        DBG_MSG ("Error: Cannot set new value for UNIE bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setSTR                                            //
// Type     : protected method                                  //
// Function : set new value for STR bit of control register     //
// Input    : the new value for STR bit                         //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setSTR(bool value)
{
    if (isCascadeMaster == true){
        if(value){
            start_time = sc_time_stamp();
            isCMatSTR = false;
        }
        updateCNTValue ();
        gtCR.gtSTR = value;
        StartStop();
    }
    else{
        DBG_MSG ("Error: Cannot set new value for STR bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setIE                                             //
// Type     : protected method                                  //
// Function : set new value for IE bit of control register      //
// Input    : the new value for IE bit                          //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setIE(bool value)
{
    if (isCascadeMaster == true){
        gtCR.gtIE = value;
    }
    else{
        DBG_MSG ("Error: Cannot set new value for IE bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setCKS                                            //
// Type     : protected method                                  //
// Function : set new value for CKS  bit of control register    //
// Input    : the new value for CKS  bit                        //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 11/28/08                                          //
// ============================================================ //
void Cgeneral_timer::setCKS(unsigned int value)
{
    if (isCascadeMaster == true){
        if (value == 0) {
            DBG_MSG ("Error: Cannot set 0 as the new value for CKS bit of gtCR!\n");
            return;
        }
        gtCR.gtCKS = value;
        calcCountingPeriod ();
    }
    else{
        DBG_MSG ("Error: Cannot set new value for CKS bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setCKEG                                           //
// Type     : protected method                                  //
// Function : set new value for CKEG  bit of control register   //
// Input    : the new value for CKEG  bit                       //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/01/08                                          //
// ============================================================ //
void Cgeneral_timer::setCKEG(detect_edge_mode value)
{
    if (isCascadeMaster == true){
        if (value != emRising
                && value != emFalling
                && value != emBothEdge){
            DBG_MSG ("Error: Set invalid value for CKEG bit of gtCR!\n");
        }
        else{
            gtCR.gtCKEG = value;
            calcCountingPeriod();
        }
    }
    else{
        DBG_MSG ("Error: Cannot set new value for CKEG bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setCCLR                                           //
// Type     : protected method                                  //
// Function : set new value for CCLR bit of control register    //
// Input    : the new value for CCLR bit                        //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/01/08                                          //
// ============================================================ //
void Cgeneral_timer::setCCLR(counter_clear_type value)
{
    if (isCascadeMaster == true){
        if (value != emProhibit
                && value != emInFactor
                && value != emExFactor
                && value != emBothInExFactor){
            DBG_MSG ("Error: Set invalid value for CCLR bit of gtCR!\n");
        }
        else {
            gtCR.gtCCLR = value;
        }
    }
    else{
        DBG_MSG ("Error: Cannot set new value for CCLR bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setCMS                                            //
// Type     : protected method                                  //
// Function : set new value for CMS bit of control register     //
// Input    : the new value for CMS bit                         //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/01/08                                          //
// ============================================================ //
void Cgeneral_timer::setCMS(unsigned int value)
{
    if (isCascadeMaster == true){
        if (value == 0) {
            DBG_MSG ("Error: Cannot set 0 as the new value for CMS bit of gtCR!\n");
            return;
        }

        list <Cgeneral_timer*>::iterator listIter;
        listIter = cascadedList.begin();
        Cgeneral_timer *timer = *listIter;
        unsigned int index = 0;
 
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                resizeBigNumber (&(timer -> gtCNT), timer -> gtCR.gtCMS, value);
                resizeBigNumber (&(timer -> gtCOR), timer -> gtCR.gtCMS, value);
                resizeBigNumber (&(timer -> gtPR), timer -> gtCR.gtCMS, value);
            
                timer -> length = (value % _BITS_PER_UINT_ == 0) ? value/_BITS_PER_UINT_ : (value/_BITS_PER_UINT_) + 1;
                timer -> gtCR.gtCMS = value;
            }

            index++;
            listIter++;
        }
    }
    else{
        DBG_MSG ("Error: Cannot set new value for CMS bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setCMM                                            //
// Type     : protected method                                  //
// Function : set new value for CMM bit of control register     //
// Input    : the new value for CMM bit                         //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/01/08                                          //
// ============================================================ //
void Cgeneral_timer::setCMM(operation_mode value)
{
    if (isCascadeMaster == true){
        if (value != emFreeRun && value != emOneShot){
            DBG_MSG ("Error: Set invalid value for CMM bit of gtCR!\n");
        }
        else{
            gtCR.gtCMM = value;
        }
    }
    else{
        DBG_MSG ("Error: Cannot set new value for CMM bit of gtCR in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setBigValueCNT                                    //
// Type     : protected method                                  //
// Function : set new value for gtCNT with size over 32 bits    //
// Input    : the array contains new value for gtCNT register   //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/02/08                                          //
// ============================================================ //
void Cgeneral_timer::setBigValueCNT(unsigned int *value)
{
    if (isCascadeMaster == true){
        list <Cgeneral_timer*>::iterator listIter;
        listIter = cascadedList.begin();
        unsigned int index = 0;
        Cgeneral_timer *timer = *listIter;
 
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                copyBigNumber (&value[index * length], timer -> gtCNT, gtCR.gtCMS);
            }

            index++;
            listIter++;
        }
    }
    else{
        DBG_MSG ("Error: Cannot set new big value for gtCNT register in slave timer!\n");
    }
}

// ============================================================ //
// Name     : setBigValueCOR                                    //
// Type     : protected method                                  //
// Function : set new value for gtCNT with size over 32 bits      //
// Input    : the array contains new value for gtCNT register     //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/02/08                                          //
// ============================================================ //
void Cgeneral_timer::setBigValueCOR(unsigned int *value)
{
    if (isCascadeMaster == true){
        list <Cgeneral_timer*>::iterator listIter;
        listIter = cascadedList.begin();
        unsigned int index = 0;
        Cgeneral_timer *timer = *listIter;
 
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                copyBigNumber (&value[index * length], timer -> gtCOR, gtCR.gtCMS);
            }

            index++;
            listIter++;
        }
    }
    else{
        DBG_MSG ("Error: Cannot set new big value for gtCOR register in slave timer!\n");
    }
}

// ============================================================ //
// Name     : getBigValueCNT                                    //
// Type     : protected method                                  //
// Function : get the value of gtCNT with size over 32 bits       //
// Input    : None                                              //
// Output   : the array contains value of gtCNT register          //
// Return   : None                                              //
// Date     : 12/02/08                                          //
// ============================================================ //
void Cgeneral_timer::getBigValueCNT(unsigned int *value)
{
    if (gtCNT != NULL && value != NULL) {
        Cgeneral_timer* head = cascadedList.front();
        
        if (head != NULL) {
            head -> updateCNTValue();
        }

        copyBigNumber (gtCNT, value, gtCR.gtCMS);
    }
}

// ============================================================ //
// Name     : getBigValueCOR                                    //
// Type     : protected method                                  //
// Function : get the value of gtCOR with size over 32 bits       //
// Input    : None                                              //
// Output   : the array contains value of gtCOR register          //
// Return   : None                                              //
// Date     : 12/02/08                                          //
// ============================================================ //
void Cgeneral_timer::getBigValueCOR(unsigned int *value)
{
    if (gtCOR != NULL && value != NULL) {
        copyBigNumber (gtCOR, value, gtCR.gtCMS);
    }
}

// ============================================================ //
// Name     : getBigValuePR                                     //
// Type     : protected method                                  //
// Function : get the value of PR with size over 32 bits        //
// Input    : None                                              //
// Output   : the array contains value of PR register           //
// Return   : None                                              //
// Date     : 12/02/08                                          //
// ============================================================ //
void Cgeneral_timer::getBigValuePR(unsigned int *value)
{
    if (gtPR != NULL && value != NULL) {
        copyBigNumber (gtPR, value, gtCR.gtCMS);
    }
}

// ============================================================ //
// Name     : getCNTOfCascadedList                              //
// Type     : protected method                                  //
// Function : get the value of gtCNT of cascaded list             //
// Input    : None                                              //
// Output   : the array contains value of gtCNT register          //
// Return   : None                                              //
// Date     : 12/12/08                                          //
// ============================================================ //
void Cgeneral_timer::getCNTOfCascadedList(unsigned int *value)
{
    list <Cgeneral_timer*>::iterator listIter;
    listIter = cascadedList.begin();
    Cgeneral_timer *timer = *listIter;
    if (isCascadeMaster == true){
        updateCNTValue ();
        unsigned int index = 0;
 
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                copyBigNumber (timer -> gtCNT, &value[index * length], gtCR.gtCMS);
            }

            index++;
            listIter++;
        }
    }
    else{
        DBG_MSG ("Warning: Cannot get gtCNT register value of cascaded list in slave timer!\n");
        DBG_MSG ("Warning: The one of Master is called instead.\n");
        if (timer != NULL) {
            timer -> getCNTOfCascadedList (value);
        }
    }
}

// ============================================================ //
// Name     : getCOROfCascadedList                              //
// Type     : protected method                                  //
// Function : get the value of gtCOR of cascaded list             //
// Input    : None                                              //
// Output   : the array contains value of gtCOR register          //
// Return   : None                                              //
// Date     : 12/12/08                                          //
// ============================================================ //
void Cgeneral_timer::getCOROfCascadedList(unsigned int *value)
{
    list <Cgeneral_timer*>::iterator listIter;
    listIter = cascadedList.begin();
    Cgeneral_timer *timer = *listIter;
    if (isCascadeMaster == true){
        unsigned int index = 0;
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                copyBigNumber (timer -> gtCOR, &value[index * length], gtCR.gtCMS);
            }

            index++;
            listIter++;
        }
    }
    else{
        DBG_MSG ("Warning: Cannot get gtCOR register value of cascaded list in slave timer!\n");
        DBG_MSG ("Warning: The one of Master is called instead.\n");
        if (timer != NULL) {
            timer -> getCOROfCascadedList (value);
        }
    }
}

// ============================================================ //
// Name     : getPROfCascadedList                              //
// Type     : protected method                                  //
// Function : get the value of PR of cascaded list             //
// Input    : None                                              //
// Output   : the array contains value of PR register          //
// Return   : None                                              //
// Date     : 12/12/08                                          //
// ============================================================ //
void Cgeneral_timer::getPROfCascadedList(unsigned int *value)
{
    list <Cgeneral_timer*>::iterator listIter;
    listIter = cascadedList.begin();
    Cgeneral_timer *timer = *listIter;
    if (isCascadeMaster == true){
        unsigned int index = 0;
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                copyBigNumber (timer -> gtPR, &value[index * length], gtCR.gtCMS);
            }

            index++;
            listIter++;
        }
    }
    else{
        DBG_MSG ("Warning: Cannot get PR register value of cascaded list in slave timer!\n");
        DBG_MSG ("Warning: The one of Master is called instead.\n");
        if (timer != NULL) {
            timer -> getPROfCascadedList (value);
        }
    }
}

// ============================================================ //
// Name     : inputCapture                                      //
// Type     : protected method                                  //
// Function : capture the value of gtCNT to PR                    //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/02/08                                          //
// ============================================================ //
void Cgeneral_timer::inputCapture()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: Input capture cannot be executed in slave timer.\n");
        return;
    }

    if (gtCR.gtIE == true){
        updateCNTValue ();

        list <Cgeneral_timer*>::iterator listIter;
        listIter = cascadedList.begin();
        unsigned int index = 0;
        Cgeneral_timer *timer = *listIter;
 
        while (index < numOfCascadedItems) {
            timer = *listIter;
            if (timer != NULL) {
                copyBigNumber (timer -> gtCNT, timer -> gtPR, gtCR.gtCMS);

                if (gtCR.gtCCLR == emExFactor
                        || gtCR.gtCCLR == emBothInExFactor) {
                    resetBigNumber (timer -> gtCNT, gtCR.gtCMS, direction);
                }
            }

            index++;
            listIter++;
        }

        gtSR.gtNumOfICF++;
        gtSR.gtICF = true;
        interruptNeedCalled = true;
        check_interrupt();
    }
    else{
        DBG_MSG ("Warning: Input capture is disable now!\n");
    }
}

// ============================================================ //
// Name     : cascadeTimer                                      //
// Type     : public method                                     //
// Function : cascade 1 timer to list                           //
// Input    : timer - pointer to cascaded timer                 //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/04/08                                          //
// ============================================================ //
void Cgeneral_timer::cascadeTimer (Cgeneral_timer *timer)
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: cannot cascade another timer to slave timer.\n");
        return;
    }

    if (timer == NULL) {
        DBG_MSG ("Error: cannot cascade a NULL timer to list.\n");
        return;
    }

    timer -> setSTR (false);

    numOfCascadedItems = numOfCascadedItems + (timer -> numOfCascadedItems);
    unsigned int num_bk = timer -> numOfCascadedItems;
    timer -> numOfCascadedItems = _NUM_1_;

    list <Cgeneral_timer*>::iterator listIter;
    list <Cgeneral_timer*>::iterator headIter;
    listIter = timer -> cascadedList.begin();
    headIter = listIter;
    unsigned int index = 0;
    Cgeneral_timer *temp_pointer = *listIter;
 
    while (index < num_bk) {
        temp_pointer = *listIter;
        if (temp_pointer != NULL) {
            temp_pointer -> isCascadeMaster = false;

            resizeBigNumber (&(temp_pointer -> gtCNT), temp_pointer -> gtCR.gtCMS, gtCR.gtCMS);
            resizeBigNumber (&(temp_pointer -> gtCOR), temp_pointer -> gtCR.gtCMS, gtCR.gtCMS);
            resizeBigNumber (&(temp_pointer -> gtPR), temp_pointer -> gtCR.gtCMS, gtCR.gtCMS);

            temp_pointer -> gtCR.gtCMS = gtCR.gtCMS;
            temp_pointer -> length = length;

            cascadedList.push_back(temp_pointer);

            if (listIter != headIter) {
                temp_pointer -> cascadedList.clear();
                temp_pointer -> cascadedList.push_back (this);
            }
        }

        index++;
        listIter++;
    }

    timer -> cascadedList.clear();
    timer -> cascadedList.push_back (this);
}

// ============================================================ //
// Name     : breakCascadedList                                 //
// Type     : public method                                     //
// Function : Break down the cascaded list to single channels   //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/04/08                                          //
// ============================================================ //
void Cgeneral_timer::breakCascadedList ()
{
    if (isCascadeMaster == false) {
        DBG_MSG ("Error: cannot break down cascaded list in slave timer.\n");
        return;
    }

    if (cascadedList.begin() != cascadedList.end()) {
        list <Cgeneral_timer*>::iterator listIter;
        list <Cgeneral_timer*>::iterator headIter;

        listIter = cascadedList.begin();
        headIter = listIter;
        unsigned int index = 0;
        unsigned int num_bk = numOfCascadedItems;
        Cgeneral_timer *temp_pointer = *listIter;
  
        while (index < num_bk) {
            temp_pointer = *listIter;
            if (temp_pointer != NULL) {
                temp_pointer -> isCascadeMaster = true;
                temp_pointer -> numOfCascadedItems = _NUM_1_;

                if (listIter != headIter) {
                    temp_pointer -> cascadedList.clear();
                    temp_pointer -> cascadedList.push_back (temp_pointer);
                }
            }
 
            index++;
            listIter++;
        }
 
        cascadedList.clear();
        cascadedList.push_back (this);
    }
}

// ============================================================ //
// Name     : removeTimerFromList                               //
// Type     : public method                                     //
// Function : Remove this timer from cascaded list              //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/04/08                                          //
// ============================================================ //
void Cgeneral_timer::removeTimerFromList ()
{
    Cgeneral_timer *temp_pointer;
    if (isCascadeMaster == true) {
        if (cascadedList.begin() != cascadedList.end()) {
            list <Cgeneral_timer*>::iterator listIter;
            Cgeneral_timer* newHead;

            listIter = cascadedList.begin();
            listIter++;
            temp_pointer = *listIter;

            if (temp_pointer != NULL) {
                newHead = temp_pointer;
            }
            else {
                return;
            }

            newHead -> numOfCascadedItems = numOfCascadedItems - 1;
            newHead -> cascadedList.clear ();
            // copy all setting to new head
            newHead -> isCascadeMaster = true;
            newHead -> setCR (&gtCR);
            newHead -> setCounterDirection (direction);
            newHead -> setCmpMatchOutType (cmpMatchOutType);
            newHead -> setCmpMatchOutVal (cmpMatchOut);

            unsigned int index = 1;
            while (index < numOfCascadedItems) {
                temp_pointer = *listIter;
                if (temp_pointer != NULL) {
                    newHead -> cascadedList.push_back (temp_pointer);
                    if (temp_pointer != newHead) {
                        temp_pointer -> cascadedList.clear();
                        temp_pointer -> cascadedList.push_back (newHead);
                    }
                }

                index++;
                listIter++;
            }

            cascadedList.clear();
            cascadedList.push_back (this);
            numOfCascadedItems = _NUM_1_;
        }
    }
    else {
        temp_pointer = cascadedList.front();
        if (temp_pointer != NULL) {
            temp_pointer -> cascadedList.remove (this);
            temp_pointer -> numOfCascadedItems--;
        }

        numOfCascadedItems = _NUM_1_;
        cascadedList.clear();
        cascadedList.push_back(this);
        isCascadeMaster = true;
    }
}

// ============================================================ //
// Name     : check_interrupt                                   //
// Type     : private method                                    //
// Function : check whether calling interrupt handling or not   //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/05/08                                          //
// ============================================================ //
void Cgeneral_timer::check_interrupt()
{
    if ((gtCR.gtCMIE == true && gtSR.gtCMF == true) ||
        (gtCR.gtOVIE == true && gtSR.gtOVF == true) ||
        (gtCR.gtUNIE == true && gtSR.gtUNF == true) ||
        (gtCR.gtICPE == true && gtSR.gtICF == true)) {
        if (interruptNeedCalled == true) {
            interruptHandling();
            interruptNeedCalled = false;
        }
    }
    else {
        interruptNeedCalled = false;
    }

}

// ============================================================ //
// Name     : ~Cgeneral_timer                                   //
// Type     : public destructor                                 //
// Function : destructor of the class                           //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// Date     : 12/08/08                                          //
// ============================================================ //
Cgeneral_timer::~Cgeneral_timer ()
{
    delete [] gtCNT;
    delete [] gtCOR;
    delete [] gtPR;
}

//--------------- Virtual Methods --------------------//
void Cgeneral_timer::cmpMatchOutputHandling (bool value)  // Handle compare match output event
{
}

void Cgeneral_timer::interruptHandling ()                 // Handle interrupt event
{
}

//------------------- Added functions ----------------------//
// ============================================================ //
// Name     : StartStop                                         //
// Type     : Private method                                    //
// Function : Determine to start/stop the counter               //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// ============================================================ //
void Cgeneral_timer::StartStop ()
{
    if (callStartStop_time == sc_time_stamp().to_double() && callStartStop_time != 0){
        return;
    }

    if (gtCR.gtSTR == true) {
        start_time_tmp = sc_time_stamp();
        specificEvent.cancel();
        callStartStop_time = sc_time_stamp().to_double();
        wait_to_start = (gtCR.gtCKEG == emFalling) ? clk_period/2 : 0;
        start_local_time = mLocalTime;
        continueCountEvent.notify(wait_to_start, time_unit);
    }
    else {
        continueCountEvent.cancel();
        specificEvent.cancel();
    }
}

// ============================================================ //
// Name     : continueCount_method                              //
// Type     : Private method                                    //
// Function : Determine which event will come                   //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// ============================================================ //
void Cgeneral_timer::continueCount_method ()
{
    if(mIsReset == true) {
        return;
    }
    if (isCascadeMaster == false || gtCR.gtSTR == false) {
        specificEventValue.write(0);
        return;
    }
    else {
        start_time = sc_time_stamp();
        previous_dif_value = 0;
        sc_uint<32> eventValue = 0;

        unsigned int arr_len = this->length * numOfCascadedItems;
        unsigned int temp_bit_len = arr_len * _BITS_PER_UINT_;

        // store the CNT value of cascaded list
        unsigned int *temp_CNT = new unsigned int [arr_len];
        getCNTOfCascadedList (temp_CNT);
        // store the COR value of cascaded list
        unsigned int *temp_arr = new unsigned int [arr_len];
        getCOROfCascadedList (temp_arr);

        unsigned int *cmp_match_time = new unsigned int [arr_len];
        unsigned int *next_event_time;
        unsigned int temp_pos = 0;

        unsigned int i = 0;
        if (direction == emCountUp) {
            if (compareBigNumber (temp_arr, temp_CNT, temp_bit_len) == 0) {
                if (isCMatSTR) {
                    isCMatSTR = false;
                    for (i = 0; i < numOfCascadedItems; i++) {
                        temp_pos = i*length;
                        resetBigNumber (&cmp_match_time[temp_pos], gtCR.gtCMS, emCountDown);
                    }
                } else {
                    isCMatSTR = true;
                    subtractBigNumber (temp_arr, temp_CNT, temp_bit_len, cmp_match_time, 0);
                }
            }
            else {
                subtractBigNumber (temp_arr, temp_CNT, temp_bit_len, cmp_match_time, 0);
            }

            for (i = 0; i < numOfCascadedItems; i++) {
                temp_pos = i*length;
                resetBigNumber (&temp_arr[temp_pos], gtCR.gtCMS, emCountDown);
            }

            //subtractBigNumber (temp_arr, temp_CNT, temp_bit_len, under_over_time, 0);
            subtractBigNumber (temp_arr, temp_CNT, temp_bit_len, temp_CNT, 0);
            eventValue = _GT_OVERFLOW_EVENT_;
        }
        else {
            if (compareBigNumber (temp_arr, temp_CNT, temp_bit_len) == 0) {
                if (isCMatSTR) {
                    isCMatSTR = false;
                    for (i = 0; i < numOfCascadedItems; i++) {
                        temp_pos = i*length;
                        resetBigNumber (&cmp_match_time[temp_pos], gtCR.gtCMS, emCountDown);
                    }
                } else {
                    isCMatSTR = true;
                    subtractBigNumber (temp_CNT, temp_arr, temp_bit_len, cmp_match_time, 0);
                }
            }
            else {
                subtractBigNumber (temp_CNT, temp_arr, temp_bit_len, cmp_match_time, 0);
            }

            resetBigNumber (temp_arr, temp_bit_len, emCountUp);
            subtractBigNumber (temp_CNT, temp_arr, temp_bit_len, temp_CNT, 0);
            eventValue = _GT_UNDERFLOW_EVENT_;
        }

        // compare compare match time and under/overflow time
        for (i = 0; i < numOfCascadedItems; i++) {
            temp_pos = i*length;
            copyBigNumber (&cmp_match_time[temp_pos], &cmp_match_time[temp_pos], gtCR.gtCMS); 
        }
        int cmp_value = compareBigNumber(cmp_match_time, temp_CNT, temp_bit_len);
        if (cmp_value == 0) {
            eventValue = _GT_CMP_MATCH_EVENT_ | eventValue;
            next_event_time = cmp_match_time;
        }
        else if (cmp_value < 0) {
            eventValue = _GT_CMP_MATCH_EVENT_;
            next_event_time = cmp_match_time;
        }
        else {
            next_event_time = temp_CNT;
        }

        // reuse the temp_arr to store 0xFFFFFFFF
        resetBigNumber (temp_arr, temp_bit_len, emCountUp);
        unsigned int remain_bit = _BITS_PER_UINT_;
        unsigned int shift_bit = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _BITS_PER_UINT_ : gtCR.gtCMS;
        for (i = 0; i < arr_len; i++) {
            shift_bit = (shift_bit < remain_bit) ? shift_bit : remain_bit;
            temp_arr [i] = _MAX_UINT_ >> (_BITS_PER_UINT_ - shift_bit);
            remain_bit = remain_bit - shift_bit;
            if (remain_bit <= 0) {
                break;
            }
        }

        cmp_value = compareBigNumber(next_event_time, temp_arr, temp_bit_len);

        double time_to_next_event = 0;
        unsigned int mask = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _BITS_PER_UINT_ : gtCR.gtCMS;
        mask = _MAX_UINT_ >> (_BITS_PER_UINT_ - mask);
        if (cmp_value <= 0) {
            if (gtCR.gtCMS < _BITS_PER_UINT_) {
                i = 0;
                while (i*gtCR.gtCMS <= _BITS_PER_UINT_ && i < arr_len) {
                    time_to_next_event = (double)(next_event_time[i] << (i * gtCR.gtCMS)) + time_to_next_event;
                    i++;
                }
            }
            else {
                time_to_next_event = (double) (next_event_time [0]);
            }
        }
        else {
            time_to_next_event = _MAX_UINT_;
            eventValue = _GT_NO_EVENT_;
        }

        delete [] temp_CNT;
        delete [] temp_arr;
        delete [] cmp_match_time;

        specificEventValue.write(eventValue);
        specificEvent.notify (time_to_next_event * counting_period, time_unit);
    }
}

// ============================================================ //
// Name     : specificEvent_method                              //
// Type     : Private method                                    //
// Function : Handle a specific event                           //
// Input    : None                                              //
// Output   : None                                              //
// Return   : None                                              //
// ============================================================ //
void Cgeneral_timer::specificEvent_method ()
{
    if(mIsReset == true) {
        interruptHandling();
        return;
    }
    if (isCascadeMaster == false) {
        return;
    }

    sc_uint<32> eventValue = specificEventValue.read();

    if ((eventValue & _GT_CMP_MATCH_EVENT_) == _GT_CMP_MATCH_EVENT_){
        // raise compare match flag
        gtSR.gtCMF = true;
        switch (cmpMatchOutType){
            case emLowActive: setCmpMatchOutVal(false); break;
            case emHighActive: setCmpMatchOutVal(true); break;
            case emToggle: setCmpMatchOutVal (cmpMatchOut == false); break;
            default: cmpMatchOut = false; break;
        }

        // call compare match handling
        cmpMatchOutputHandling(cmpMatchOut);

        interruptNeedCalled = true;
        gtSR.gtNumOfCMF++;

        // reset the counter if any
        if (gtCR.gtCMM == emOneShot ||
            (gtCR.gtCMM == emFreeRun && (gtCR.gtCCLR == emInFactor || gtCR.gtCCLR == emBothInExFactor))) {
            resetCNTValue();
        }
        else {
            updateCNTValue();
        }

        if (gtCR.gtCMM == emOneShot){
            gtCR.gtSTR = false;
        }
    }

    if ((eventValue & _GT_OVERFLOW_EVENT_) == _GT_OVERFLOW_EVENT_) {
        gtSR.gtOVF = true;
        gtSR.gtNumOfOVF++;
        interruptNeedCalled = true;
        resetCNTValue();
    }

    if ((eventValue & _GT_UNDERFLOW_EVENT_) == _GT_UNDERFLOW_EVENT_) {
        gtSR.gtUNF = true;
        gtSR.gtNumOfUNF++;
        interruptNeedCalled = true;
        resetCNTValue();
    }

    if (eventValue == _GT_NO_EVENT_) {
        updateCNTValue();
    }

    check_interrupt ();
    continueCountEvent.notify();
}

// ============================================================ //
// Name    : subtractBigNumber                                  //
// Type    : private method                                     //
// Function: subtract two big numbers                           //
// Input   : num1 - first number                                //
//           num2 - second number                               //
//           bit_length - length in bit                         //
//           remember - remember in previous subtraction        //
// Output  : result - store the result of subtraction           //
// Return  : unsigned int value - remember value (0/1)          //
// ============================================================ //
unsigned int Cgeneral_timer::subtractBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length,
                                                unsigned int *result, unsigned int remember)
{
    // validate bit_length
    if (bit_length != 0 && num1 != NULL && num2 != NULL && result != NULL && remember < 2) {
        // calculate temporary length
        unsigned int tmp_len = bit_length/_BITS_PER_UINT_;
        unsigned int index = bit_length - (tmp_len * _BITS_PER_UINT_);
        unsigned int mask = (index != 0) ? _MAX_UINT_ >> (_BITS_PER_UINT_ - index) : _MAX_UINT_;

        unsigned int temp_num1 = 0;
        unsigned int temp_num2 = 0;

        index = 0;
        while (index != tmp_len) {
            temp_num1 = num1[index];
            temp_num2 = num2[index];
            result[index] = temp_num1 - temp_num2 - remember;
            remember = (result[index] >= temp_num1 && temp_num2 != 0) ? 1 : 0;
            index++;
        }

        tmp_len = bit_length - (tmp_len * _BITS_PER_UINT_);
        if (tmp_len != 0) {
            temp_num1 = num1[index];
            temp_num2 = num2[index];
            result[index] = (temp_num1 - temp_num2 - remember) & mask;
            remember = (result[index] >= (temp_num1 & mask) && temp_num2 != 0) ? 1 : 0;
        }
    }
    return remember;
}

// ============================================================ //
// Name    : addBigNumber                                       //
// Type    : private method                                     //
// Function: add two big numbers                                //
// Input   : num1 - first number                                //
//           num2 - second number                               //
//           bit_length - length in bit                         //
//           remember - remember in previous addition           //
// Output  : result - store the result of addition              //
// Return  : unsigned int value - remember value (0/1)          //
// ============================================================ //
unsigned int Cgeneral_timer::addBigNumber (unsigned int *num1, unsigned int *num2, unsigned int bit_length,
                                                unsigned int *result, unsigned int remember)
{
    // validate bit_length
    if (bit_length != 0 && num1 != NULL && num2 != NULL && result != NULL && remember < 2) {
        // calculate temporary length
        unsigned int tmp_len = bit_length/_BITS_PER_UINT_;
        unsigned int index = bit_length - (tmp_len * _BITS_PER_UINT_);
        unsigned int mask = (index != 0) ? _MAX_UINT_ >> (_BITS_PER_UINT_ - index) : _MAX_UINT_;
        unsigned int temp_num1 = 0;
        unsigned int temp_num2 = 0;

        index = 0;
        while (index != tmp_len) {
            temp_num1 = num1[index];
            temp_num2 = num2[index];
            result[index] = temp_num1 + temp_num2 + remember;
            remember = (result[index] <= temp_num1 && temp_num2 != 0) ? 1 : 0;
            index++;
        }

        tmp_len = bit_length - (tmp_len * _BITS_PER_UINT_);
        if (tmp_len != 0) {
            temp_num1 = num1[index];
            temp_num2 = num2[index];
            result[index] = (temp_num1 + temp_num2 + remember) & mask;
            remember = (result[index] <= (temp_num1 & mask) && temp_num2 != 0) ? 1 : 0;
        }
    }
    return remember;
}

// ============================================================ //
// Name    : updateCNTValue                                     //
// Type    : private method                                     //
// Function: update the value of CNT register.                  //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// ============================================================ //
void Cgeneral_timer::updateCNTValue ()
{
    if (isCascadeMaster == false || gtCR.gtSTR == false) {
        return;
    }

    sc_time temp(counting_period, time_unit);
    
    sc_time stop_time = sc_time_stamp();
    if ((mSupportDecoupling)&&(mLocalTime.to_double() >= wait_to_start)&&(stop_time == start_time_tmp)) {
        stop_time = stop_time + sc_time(wait_to_start, time_unit);
    }
    unsigned int dif_value = 0;
    dif_value = (unsigned int)((stop_time - start_time)/temp);

    if (gtCR.gtCKEG == emBothEdge) {
        if ((unsigned int)((stop_time - start_time).to_double()) == ((unsigned int)(((dif_value + 1) * temp).to_double()))) {
            dif_value = dif_value + 1;
        }
    }
    else {
        dif_value = (unsigned int)((stop_time - start_time)/temp);
    }

    unsigned int dif_value_subtract = previous_dif_value;
    previous_dif_value = dif_value;
    
    if (dif_value == 0) {
        return;
    }

    dif_value = dif_value - dif_value_subtract;

    unsigned int remember = 0;
    unsigned int mask = (gtCR.gtCMS >= _BITS_PER_UINT_) ? _MAX_UINT_ : _MAX_UINT_ >> (_BITS_PER_UINT_ - gtCR.gtCMS);
  
    unsigned int *temp_value = new unsigned int [length];
    if (temp_value != NULL) {
        list <Cgeneral_timer*>::iterator listIter;
        listIter = cascadedList.begin();
        unsigned int main_index = 0;
        Cgeneral_timer *timer_ptr = *listIter;
        while (main_index < numOfCascadedItems) { // && remember_flag == true) {
            timer_ptr = *listIter;
            if (timer_ptr != NULL && timer_ptr -> gtCNT != NULL) {
                resetBigNumber (temp_value, gtCR.gtCMS, emCountUp);
                temp_value[0] = (dif_value & mask) >> (main_index * gtCR.gtCMS);
                remember = (direction == emCountUp) ?
                    addBigNumber(timer_ptr -> gtCNT, temp_value, gtCR.gtCMS, timer_ptr -> gtCNT, remember) :
                    subtractBigNumber(timer_ptr -> gtCNT, temp_value, gtCR.gtCMS, timer_ptr -> gtCNT, remember);
 
                mask = (gtCR.gtCMS == _BITS_PER_UINT_) ? 0 : mask << gtCR.gtCMS;
                if (mask == 0 && remember == 0) {
                    break;
                }
            }
            main_index++;
            listIter++;
        }
 
        delete [] temp_value;
    }
}

// ============================================================ //
// Name    : resetCNTValue                                      //
// Type    : private method                                     //
// Function: reset the value of CNT register.                   //
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// ============================================================ //
void Cgeneral_timer::resetCNTValue ()
{
    list <Cgeneral_timer*>::iterator listIter;
    listIter = cascadedList.begin();
    unsigned int index = 0;
    Cgeneral_timer *timer = *listIter;
 
    while (index < numOfCascadedItems) {
        timer = *listIter;
        if (timer != NULL) {
            resetBigNumber (timer -> gtCNT, gtCR.gtCMS, direction);
        }

        index++;
        listIter++;
    }
}

// ============================================================ //
// Name    : setClkCountPeriod                                  //
// Type    : protected method                                   //
// Function: set the clock count period for timer               //
// Input   : clk_period - value of clock period                 //
// Output  : None                                               //
// Return  : None                                               //
// ============================================================ //
void Cgeneral_timer::setClkCountPeriod (double clk_period)
{
    if (isCascadeMaster == true) {
        this->clk_period = clk_period;
        calcCountingPeriod();
    }
}

// ============================================================ //
// Name    : getClkCountPeriod                                  //
// Type    : protected method                                   //
// Function: set the clock count period for timer               //
// Input   : None                                               //
// Output  : clk_period - value of clock period                 //
// Return  : None                                               //
// ============================================================ //
double Cgeneral_timer::getClkCountPeriod ()
{
    if (isCascadeMaster == true) {
        return this->clk_period;
    }

    return _ERR_NUM_;
}

// ============================================================ //
// Name    : setTimeUnit                                        //
// Type    : protected method                                   //
// Function: set the time unit for timer                        //
// Input   : time_unit - time unit of time                      //
// Output  : None                                               //
// Return  : None                                               //
// ============================================================ //
void Cgeneral_timer::setTimeUnit (sc_time_unit time_unit)
{
    if (isCascadeMaster == true) {
        this->time_unit = time_unit;
    }
}

// ============================================================ //
// Name    : getTimeUnit                                        //
// Type    : protected method                                   //
// Function: set the time unit for timer                        //
// Input   : None                                               //
// Output  : time_unit - time unit of time                      //
// Return  : None                                               //
// ============================================================ //
sc_time_unit Cgeneral_timer::getTimeUnit ()
{
    if (isCascadeMaster == true) {
        return this->time_unit;
    }

    return SC_NS;
}

// ============================================================ //
// Name    : setLocalTime                                       //
// Type    : protected method                                   //
// Function: set local offset time into mLocalTime variable     //
// Input   : ltime - local offset time                          //
// Output  : None                                               //
// Return  : None                                               //
// ============================================================ //
void Cgeneral_timer::setLocalTime (const sc_time ltime) 
{
    mLocalTime = ltime;
}

// ================================================================= //
// Name    : enableSupportDecoupling                                 //
// Type    : protected method                                        //
// Function: set input value into mSupportDecoupling variable        //
// Input   : isDecoupling - Enable/Disable temporal decoupling mode  //
// Output  : None                                                    //
// Return  : None                                                    //
// ================================================================= //
void Cgeneral_timer::enableSupportDecoupling (const bool isDecoupling) 
{
    mSupportDecoupling = isDecoupling;
}

// ============================================================ //
// Name    : calcCountingPeriod                                 //
// Type    : private method                                     //
// Function: calculate count period and store in counting_period//
// Input   : None                                               //
// Output  : None                                               //
// Return  : None                                               //
// ============================================================ //
void Cgeneral_timer::calcCountingPeriod () {
    counting_period = clk_period * (double)gtCR.gtCKS;
    if (gtCR.gtCKEG == emBothEdge) {
        counting_period = counting_period / 2;
    }
}

void Cgeneral_timer::EnableReset (bool is_active) {
    mIsReset = is_active ;
    if(mIsReset) {
        continueCountEvent.cancel();
        specificEvent.cancel();
        continueCountEvent.notify();
        specificEvent.notify();
        Reset(false);
    }
}

void Cgeneral_timer::Reset(bool is_constractor)
{
//        clk_period = _GT_DEF_CLK_PERIOD_;
//        time_unit = _GT_DEF_TIME_UNIT_;
        wait_to_start = 0;
        start_time = sc_time(0, time_unit);
        callStartStop_time = 0;

//        isCascadeMaster = true;
//        numOfCascadedItems = _NUM_1_;

//        cascadedList.clear ();
//        cascadedList.push_back (this);

        // reset compare match output type
//        cmpMatchOutType = emLowActive;
        // reset compare match output value
//        cmpMatchOut = true;

        // reset counter direction
//        direction = emCountUp;

        // reset gtCR value
        gtCR.gtCMIE = false;
        gtCR.gtOVIE = false;
        gtCR.gtUNIE = false;
        gtCR.gtICPE = false;
        gtCR.gtCKS  = 1UL;
        gtCR.gtCKEG = emRising;
        gtCR.gtSTR  = false;
        gtCR.gtIE   = false;
        gtCR.gtCMM  = emOneShot;
        gtCR.gtCMS = _BITS_PER_UINT_;
        gtCR.gtCCLR = emProhibit;

        // re-calculate length of arrays based on CMS value
        length = 1UL;
        previous_dif_value = 0;

//        gtCNT = new unsigned int [length];
        if (gtCNT != NULL) {
            gtCNT[0] = 0;
        }

 //       gtCOR = new unsigned int [length];
        if (gtCOR != NULL) {
            gtCOR[0] = _MAX_UINT_;
        }

//        gtPR = new unsigned int [length];
        if (gtPR != NULL) {
            gtPR[0] = 0;
        }

        // reset gtSR value
        clearAllFlag ();
        interruptNeedCalled = false;

        calcCountingPeriod();

}

// vim600: set foldmethod=marker :
