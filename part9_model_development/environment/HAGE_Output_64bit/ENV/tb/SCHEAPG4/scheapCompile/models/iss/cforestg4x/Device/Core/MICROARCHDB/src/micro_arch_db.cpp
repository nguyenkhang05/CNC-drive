/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./forest_common.h"
#include "./micro_arch_db.h"
#include "./inst_id_list.h"
#include "./forest_message.h"


/*!
 * @brief  Gets the instruction ID for the Opcode.
 * @param  opcode Opcode(Operation code).
 * @return Instruction ID.
 * @retval INST_ID_V_0 If the opcode is not registered.
 * @note
 * If it's failed to fetch, it's guaranteed that opcode must be 0x0000.\n
 * Then no need to catch this error, because it will be NOP if it's decoded.
 * @sa MemInstFetch
 */
int32_t MicroArchDB::GetInstID (uint32_t opcode)
{
    // テーブル全体でのインデックス
    // V850の最初のデコードキーは DECKEY_B (bit10-5)
    uint32_t table_index = BitExtract (opcode, 10, 5);
    // ブロックの先頭
    int32_t table_pos = m_code_table[table_index].pos;

    while (table_pos != -1) {
        // 次のデコード位置を取得
        uint32_t index;
        DecodeKey deckey = m_code_table[table_index].deckey;
        switch (deckey) {
        case DECKEY_A:  index =  BitExtract (opcode, 15, 11); break;
        case DECKEY_B:  index =  BitExtract (opcode, 10,  5); break;
        case DECKEY_C:  index =  BitExtract (opcode,  4,  0); break;
        case DECKEY_D:  index =  BitExtract (opcode, 31, 27); break;
        case DECKEY_E:  index =  BitExtract (opcode, 26, 21); break;
        case DECKEY_F:  index =  BitExtract (opcode, 20, 16); break;
        case DECKEY_J:  index =  BitExtract (opcode, 26, 22); break;
        case DECKEY_K:  index =  BitExtract (opcode, 21, 16); break;
        case DECKEY_S:  index =  BitExtract (opcode, 15, 11); break;
        case DECKEY_T:  index =  BitExtract (opcode, 15, 11); break;
        case DECKEY_U:  index =  BitExtract (opcode, 21, 16); break;
        case DECKEY_V:  index =  BitExtract (opcode, 21, 16); break;
        case DECKEY_W:  index =  BitExtract (opcode, 21, 16); break;
        case DECKEY_A0: index = (BitExtract (opcode, 15, 11) == 0x0) ? 0 : 1; break;
        case DECKEY_B0: index = (BitExtract (opcode, 10,  5) == 0x0) ? 0 : 1; break;
        case DECKEY_C0: index = (BitExtract (opcode,  4,  0) == 0x0) ? 0 : 1; break;
        case DECKEY_D0: index = (BitExtract (opcode, 31, 27) == 0x0) ? 0 : 1 + (BitExtract (opcode, 27, 27) << 1); break;
        case DECKEY_E0: index = (BitExtract (opcode, 26, 21) == 0x0) ? 0 : 1; break;
        case DECKEY_F0: index = (BitExtract (opcode, 20, 16) == 0x0) ? 0 : 1; break;
        default:
            // 非到達コード
            CfMsg::DPrint (MSG_ERR, "<Internal Error: Unreachable code>\n");
            return INST_ID_V_0;
            break;
        }

        // 次のブロックの先頭位置を取得
        table_index = table_pos + index;
        table_pos = m_code_table[table_index].pos;
    }

    // デコード終了
    return m_code_table[table_index].id;
}
