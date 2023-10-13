/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string.h>
#include <sstream>
#include <iomanip>
#include <string>
#include "./config.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./trace_print.h"
#include "./micro_arch_db.h"
#include "./sregfile.h"
#include "./forest_message.h"


/*!
 * @brief Generates the strings to print the operand. : オペランド表示用文字列の生成
 * @param inst_id The Instruction number. : 命令番号
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param inst_mnem The pointer to the variable of the strings to print operand. : オペランド表示用文字列の格納変数ポンタ
 */
void TracePrintOperand::PrintOperand (uint32_t inst_id, uint64_t inst_code,
                                      uint32_t inst_pc, std::string* inst_mnem) const
{
    std::string inst_op = MicroArchDB::GetOperandFormat (inst_id);
    (this->*m_inst_func[inst_id])(&inst_op, inst_code, inst_pc);
    *inst_mnem = inst_op;
    return;
}

/*!
 * @brief Sets the values to the variables of ReplaceInfo class. : ReplaceInfoクラス変数の値設定
 * @param indx The type number to replace. : 置換対象のタイプ番号
 * @param rep The strings to replace. : 置換対象の文字列
 * @param code_hl The position to slice off Higher bits(Higher end). : 上位側のビット切り出し位置（上位端）
 * @param code_hr The position to slice off Higher bits(Lower end). : 上位側のビット切り出し位置（下位端）
 * @param code_ml The position to slice off Middle bits(Higher end). : 中位側のビット切り出し位置（上位端）
 * @param code_mr The position to slice off Middle bits(Lower end). : 中位側のビット切り出し位置（下位端）
 * @param code_ll The position to slice off Lower bits(Higher end). : 下位側のビット切り出し位置（上位端）
 * @param code_lr The position to slice off Lower bits(Lower end). : 下位側のビット切り出し位置（下位端）
 * @param h_seg The existance of Higher bits. : 上位側bitの有無 (true:有, false:無)
 * @param m_seg The existance of Middle bits. : 中位側bitの有無 (true:有, false:無)
 * @param shift The shift number of the sliced bits. : 切り出したビットのシフト数
 * @param offset The offset to add to the sliced bits. : 切り出したビットに加算するオフセット
 * @param sig The position of the sign bit. : signビットの位置
 */
void TracePrintOperand::SetReplaceInfoValue (ReplaceType indx, const std::string& rep,
                                             int32_t code_hl, int32_t code_hr,
                                             int32_t code_ml, int32_t code_mr,
                                             int32_t code_ll, int32_t code_lr,
                                             bool h_seg, bool m_seg,
                                             uint32_t shift, uint32_t offset,
                                             uint32_t sig)
{
    replace_info[indx].m_rep     = (std::string)rep;
    replace_info[indx].m_code_hl = code_hl;
    replace_info[indx].m_code_hr = code_hr;
    replace_info[indx].m_code_ml = code_ml;
    replace_info[indx].m_code_mr = code_mr;
    replace_info[indx].m_code_ll = code_ll;
    replace_info[indx].m_code_lr = code_lr;
    replace_info[indx].m_hseg    = h_seg;
    replace_info[indx].m_mseg    = m_seg;
    replace_info[indx].m_shift   = shift;
    replace_info[indx].m_offset  = offset;
    replace_info[indx].m_sig     = sig;
}

/*!
 * @brief Slices off the numbers(the register number, etc.) from the instruction code. : 命令コードからレジスタ番号などの数値を切り出す
 * @param inst_code The instruction code. : 命令コード
 * @param id The type number to replace. : 置換対象のタイプ番号
 * @return The sliced number : 切り出した数値
 */
uint32_t TracePrintOperand::NumFromCode (uint64_t inst_code, ReplaceType id) const
{
    // 下位側のビット切り出し
    int32_t code_ll = replace_info[id].m_code_ll;
    int32_t code_lr = replace_info[id].m_code_lr;
    if ( unlikely ((code_ll < 0) || (code_lr < 0))) {
        CfMsg::DPrint (MSG_ERR, "<Internal Error: Register bit position is minus>\n");
    }
    uint32_t rep_num_ui = OperandExtract (inst_code, code_ll, code_lr);

    // 中位側のビット切り出し
    int32_t code_ml = replace_info[id].m_code_ml;
    int32_t code_mr = replace_info[id].m_code_mr;
    // 中位側のビットの有無を判定
    if (replace_info[id].m_mseg == true) {
        if ( unlikely ((code_ml < 0) || (code_mr < 0))) {
            CfMsg::DPrint (MSG_ERR, "<Internal Error: Register bit position is minus>\n");
        }
        uint32_t rep_num_ui_m = OperandExtract (inst_code, code_ml, code_mr);
        rep_num_ui_m <<= (code_ll - code_lr +1);
        rep_num_ui |= rep_num_ui_m;
    }

    // 上位側のビットの有無を判定
    if (replace_info[id].m_hseg == true) {
        // 上位側のビット切り出し
        int32_t code_hl = replace_info[id].m_code_hl;
        int32_t code_hr = replace_info[id].m_code_hr;
        if ( unlikely ((code_hl < 0) || (code_hr < 0))) {
            CfMsg::DPrint (MSG_ERR, "<Internal Error: Register bit position is minus>\n");
        }
        uint32_t rep_num_ui_h = OperandExtract (inst_code, code_hl, code_hr);
        rep_num_ui_h <<= (code_ml - code_mr + 1 + code_ll - code_lr +1);
        rep_num_ui |= rep_num_ui_h;
    }
    rep_num_ui <<= replace_info[id].m_shift;
    rep_num_ui += replace_info[id].m_offset;
    return (rep_num_ui);
}

/*!
 * @brief Replaces the number of general-purpose register. : 汎用レジスタ番号を置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceReg (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        std::stringstream ss;
        ss << "r" << NumFromCode (inst_code, id);
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces the number of vector register. : ベクトルレジスタ番号を置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceVReg (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        std::stringstream ss;
        ss << "vr" << NumFromCode (inst_code, id);
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}


/*!
 * @brief Replaces the number of vector register.(4 words) : ベクトルレジスタ番号を置換(4 words)
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceWReg (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        std::stringstream ss;
        ss << "wr" << NumFromCode (inst_code, id);
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}


/*!
 * @brief Replaces the number of imm,disp,etc.(for unsigned) : imm,dispなどの数値を置換 (unsigned用)
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceHexU (std::string* inst_op, uint64_t inst_code,
                                     uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は16進数)
        std::stringstream ss;
        ss << "0x" << std::hex << NumFromCode (inst_code, id);
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}


/*!
 * @brief Replaces the number of eh-rt.(for unsigned) :  eh-rtの数値を置換 (unsigned用)
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceEntry (std::string* inst_op, uint64_t inst_code,
                                     uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        std::stringstream ss;
        ss << "e" << NumFromCode (inst_code, id);
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}


/*!
 * @brief Replaces the number of imm,disp,etc.(for signed) : imm,dispなどの数値を置換 (signed用)
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceDec (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        uint32_t rep_num_ui = NumFromCode (inst_code, id);
        // 符号拡張
        uint32_t msb = 0x01U << (replace_info[id].m_sig - 1);
        int32_t rep_num_i = (rep_num_ui ^ msb) - msb;
        std::stringstream ss;
        ss << rep_num_i;
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces the number of the List. : Listの数値を置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceList (std::string* inst_op, uint64_t inst_code,
                                     uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        uint64_t rep_num_ui = (inst_code & 0x01U);
        rep_num_ui |= ((inst_code >> 20) & 0xFFEU);
        std::stringstream ss;
        ss << "0x" << std::hex << std::setfill('0') << std::setw(3) << rep_num_ui;
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces the number of SR. : SRの数値を置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceSR (std::string* inst_op, uint64_t inst_code,
                                   uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        std::stringstream ss;
        ss << "sr" << NumFromCode (inst_code, id);
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces the number of SEL. : SELの数値を置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceSel (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は10進数)
        std::stringstream ss;
        ss << "sel" << NumFromCode (inst_code, id);
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces cond. : condを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceCnd (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (条件の文字列)
        uint32_t rep_num_ui = NumFromCode (inst_code, id);
        std::stringstream ss;
        switch (rep_num_ui) {
            case 0x0: ss << "V";     break;
            case 0x1: ss << "C/L";   break;
            case 0x2: ss << "Z";     break;
            case 0x3: ss << "NH";    break;
            case 0x4: ss << "S/N";   break;
            case 0x5: ss << "T";     break;
            case 0x6: ss << "LT";    break;
            case 0x7: ss << "LE";    break;
            case 0x8: ss << "NV";    break;
            case 0x9: ss << "NC/NL"; break;
            case 0xA: ss << "NZ";    break;
            case 0xb: ss << "H";     break;
            case 0xc: ss << "NS/P";  break;
            case 0xd: ss << "SA";    break;
            case 0xe: ss << "GE";    break;
            case 0xf: ss << "GT";    break;
            default:
                // 上記以外は16進数の数値で表示
                ss << "0x" << std::hex << rep_num_ui;
                break;
        }
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces fcond. : fcondを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceFCO (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (条件の文字列)
        uint32_t rep_num_ui = NumFromCode (inst_code, id);
        std::stringstream ss;
        switch (rep_num_ui) {
            case 0x0: ss << "F";    break;
            case 0x1: ss << "UN";   break;
            case 0x2: ss << "EQ";   break;
            case 0x3: ss << "UEQ";  break;
            case 0x4: ss << "OLT";  break;
            case 0x5: ss << "ULT";  break;
            case 0x6: ss << "OLE";  break;
            case 0x7: ss << "ULE";  break;
            case 0x8: ss << "SF";   break;
            case 0x9: ss << "NGLE"; break;
            case 0xa: ss << "SEQ";  break;
            case 0xb: ss << "NGL";  break;
            case 0xc: ss << "LT";   break;
            case 0xd: ss << "NGE";  break;
            case 0xe: ss << "LE";   break;
            case 0xf: ss << "NGT";  break;
            default:
                // 上記以外は16進数の数値で表示
                ss << "0x" << std::hex << rep_num_ui;
                break;
        }
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces bcond. : bcondを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceBcond (std::string* inst_op, uint64_t inst_code,
                                      uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        std::stringstream ss;
        // 置換後文字列の生成 (符号拡張disp + PC を 16進数)
        uint32_t rep_num_ui = NumFromCode (inst_code, id);
        uint32_t msb = 0x01U << (replace_info[id].m_sig - 1);
        int32_t rep_num_i = (rep_num_ui ^ msb) - msb;
        rep_num_ui = inst_pc + rep_num_i;
        std::stringstream ss_op;
        ss_op << "0x" << std::hex << rep_num_ui;

        // 置換後文字列の生成 (条件の文字列)
        uint32_t rep_cnd = OperandExtract (inst_code, 3, 0);
        std::string s_mnem;
        switch (rep_cnd) {
            case 0x0: s_mnem = "bv";  break;
            case 0x1: s_mnem = "bl";  break;
            case 0x2: s_mnem = "be";  break;
            case 0x3: s_mnem = "bnh"; break;
            case 0x4: s_mnem = "bn";  break;
            case 0x5: s_mnem = "br";  break;
            case 0x6: s_mnem = "blt"; break;
            case 0x7: s_mnem = "ble"; break;
            case 0x8: s_mnem = "bnv"; break;
            case 0x9: s_mnem = "bnl"; break;
            case 0xa: s_mnem = "bne"; break;
            case 0xb: s_mnem = "bh";  break;
            case 0xc: s_mnem = "bp";  break;
            case 0xd: s_mnem = "bsa"; break;
            case 0xe: s_mnem = "bge"; break;
            case 0xf: s_mnem = "bgt"; break;
            default:  s_mnem = "rep"; break;
        }
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss_op.str());
        inst_op->replace(0, 3, s_mnem);
    }
}

/*!
 * @brief Replaces disp of the branch instruction. : 分岐命令のdispを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceBra (std::string* inst_op, uint64_t inst_code,
                                      uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (符号拡張disp + PC を 16進数)
        uint32_t rep_num_ui = NumFromCode (inst_code, id);
        uint32_t msb = 0x01U << (replace_info[id].m_sig - 1);
        int32_t rep_num_i = (rep_num_ui ^ msb) - msb;
        rep_num_ui = inst_pc + rep_num_i;
        std::stringstream ss;
        ss << "0x" << std::hex << rep_num_ui;
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}


/*!
 * @brief Replaces width. : widthを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceWidth (std::string* inst_op, uint64_t inst_code,
                                      uint32_t inst_pc, ReplaceType id) const
{
    // @@pos/widthのディスアセンブル方法は再考する

    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は16進数)
        uint32_t msb = NumFromCode (inst_code, id);
        uint32_t lsb = NumFromCode (inst_code, P_27_19_17);

        uint32_t ofst_type = (inst_code >> 21) & 0x3;

        switch (ofst_type) {
        case 0 : msb += 0x10; lsb += 0x10; break;
        case 1 : msb += 0x10; break;
        default : break;
        }

        uint32_t width = msb - lsb + 1;
        std::stringstream ss;
        ss << "0x" << std::hex << width;
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}


/*!
 * @brief Replaces pos. : posを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplacePos (std::string* inst_op, uint64_t inst_code,
                                    uint32_t inst_pc, ReplaceType id) const
{
    // @@pos/widthのディスアセンブル方法は再考する

    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (数値は16進数)
        uint32_t lsb = NumFromCode (inst_code, id);

        if (((inst_code >> 21) & 0x3) == 0) {
            lsb += 0x10;
        }

        std::stringstream ss;
        ss << "0x" << std::hex << lsb;
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}


/*!
 * @brief Replaces cacheop. : cacheopを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplaceCacheOp (std::string* inst_op, uint64_t inst_code,
                                        uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (条件の文字列)
        uint32_t rep_num_ui = NumFromCode (inst_code, id);
        std::stringstream ss;
        switch (rep_num_ui) {
            case 0x00: ss << "CHBII";   break;
            case 0x20: ss << "CIBII";   break;
            case 0x40: ss << "CFALI";   break;
            case 0x60: ss << "CISTI";   break;
            case 0x61: ss << "CILDI";   break;
            default:
                // 上記以外は16進数の数値で表示
                ss << "0x" << std::hex << rep_num_ui;
                break;
        }
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}

/*!
 * @brief Replaces prefop. : prefopを置換
 * @param inst_op The pointer to the strings after replacement. : 置換後の文字列を格納する変数のポインタ
 * @param inst_code The instruction code. : 命令コード
 * @param inst_pc The program counter. : プログラムカウンタ値
 * @param id The type number to replace. : 置換対象のタイプ番号
 */
void TracePrintOperand::ReplacePrefOp (std::string* inst_op, uint64_t inst_code,
                                       uint32_t inst_pc, ReplaceType id) const
{
    // 置換対象文字列の位置検索
    std::string::size_type pos = inst_op->find (replace_info[id].m_rep);
    if (pos != std::string::npos) {
        // 置換後文字列の生成 (条件の文字列)
        uint32_t rep_num_ui = NumFromCode (inst_code, id);
        std::stringstream ss;
        switch (rep_num_ui) {
            case 0x00: ss << "PREFI";   break;
            default:
                // 上記以外は16進数の数値で表示
                ss << "0x" << std::hex << rep_num_ui;
                break;
        }
        // 対象文字列の置換
        inst_op->replace(pos, replace_info[id].m_rep.length(), ss.str());
    }
}
