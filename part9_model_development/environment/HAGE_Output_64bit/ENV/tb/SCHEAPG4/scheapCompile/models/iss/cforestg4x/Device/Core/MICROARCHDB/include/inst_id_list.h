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

#define  INST_ID_V_0                   0
#define  INST_ID_V_ADD                 1
#define  INST_ID_V_ADD_I5              2
#define  INST_ID_V_ADD_I16             3
#define  INST_ID_V_ADF                 4
#define  INST_ID_V_AND                 5
#define  INST_ID_V_ANDI                6
#define  INST_ID_V_BCOND_D9            7
#define  INST_ID_V_BCOND_D17           8
#define  INST_ID_V_BINSL               9
#define  INST_ID_V_BINSM               10
#define  INST_ID_V_BINSU               11
#define  INST_ID_V_BSH                 12
#define  INST_ID_V_BSW                 13
#define  INST_ID_V_CACHE               14
#define  INST_ID_V_CALLT               15
#define  INST_ID_V_CAXI                16
#define  INST_ID_V_CLR1_B3             17
#define  INST_ID_V_CLR1                18
#define  INST_ID_V_CMOV_I5             19
#define  INST_ID_V_CMOV                20
#define  INST_ID_V_CMP_I5              21
#define  INST_ID_V_CMP                 22
#define  INST_ID_V_CTRET               23
#define  INST_ID_V_DI                  24
#define  INST_ID_V_DISPOSE             25
#define  INST_ID_V_DISPOSE_R           26
#define  INST_ID_V_DIV                 27
#define  INST_ID_V_DIVH_R2             28
#define  INST_ID_V_DIVH                29
#define  INST_ID_V_DIVHU               30
#define  INST_ID_V_DIVQ                31
#define  INST_ID_V_DIVU                32
#define  INST_ID_V_DIVQU               33
#define  INST_ID_V_EI                  34
#define  INST_ID_V_EIRET               35
#define  INST_ID_V_FERET               36
#define  INST_ID_V_FETRAP              37
#define  INST_ID_V_HALT                38
#define  INST_ID_V_HSH                 39
#define  INST_ID_V_HSW                 40
#define  INST_ID_V_HVTRAP              41
#define  INST_ID_V_JARL                42
#define  INST_ID_V_JARL_D22            43
#define  INST_ID_V_JARL_D32            44
#define  INST_ID_V_JMP                 45
#define  INST_ID_V_JMP_D32             46
#define  INST_ID_V_JR_D22              47
#define  INST_ID_V_JR_D32              48
#define  INST_ID_V_LDB_D16             49
#define  INST_ID_V_LDB_D23             50
#define  INST_ID_V_LDBU_D16            51
#define  INST_ID_V_LDBU_D23            52
#define  INST_ID_V_LDDW_D23            53
#define  INST_ID_V_LDH_D16             54
#define  INST_ID_V_LDH_D23             55
#define  INST_ID_V_LDHU_D16            56
#define  INST_ID_V_LDHU_D23            57
#define  INST_ID_V_LDW_D16             58
#define  INST_ID_V_LDW_D23             59
#define  INST_ID_V_LDLW                60
#define  INST_ID_V_LDSR                61
#define  INST_ID_V_LOOP                62
#define  INST_ID_V_MAC                 63
#define  INST_ID_V_MACU                64
#define  INST_ID_V_MOV_I32             65
#define  INST_ID_V_MOV_I5              66
#define  INST_ID_V_MOV                 67
#define  INST_ID_V_MOVEA               68
#define  INST_ID_V_MOVHI               69
#define  INST_ID_V_MUL_I9              70
#define  INST_ID_V_MUL                 71
#define  INST_ID_V_MULH_I5             72
#define  INST_ID_V_MULH                73
#define  INST_ID_V_MULHI               74
#define  INST_ID_V_MULU_I9             75
#define  INST_ID_V_MULU                76
#define  INST_ID_V_NOP                 77
#define  INST_ID_V_NOT                 78
#define  INST_ID_V_NOT1_B3             79
#define  INST_ID_V_NOT1                80
#define  INST_ID_V_OR                  81
#define  INST_ID_V_ORI                 82
#define  INST_ID_V_POPSP               83
#define  INST_ID_V_PREF                84
#define  INST_ID_V_PREPARE             85
#define  INST_ID_V_PREPARE_SP          86
#define  INST_ID_V_PREPARE_LO16        87
#define  INST_ID_V_PREPARE_HI16        88
#define  INST_ID_V_PREPARE_I32         89
#define  INST_ID_V_PUSHSP              90
#define  INST_ID_V_RIE                 91
#define  INST_ID_V_RIE_I9              92
#define  INST_ID_V_ROTL_I5             93
#define  INST_ID_V_ROTL_R3             94
#define  INST_ID_V_SAR_I5              95
#define  INST_ID_V_SAR                 96
#define  INST_ID_V_SAR_R3              97
#define  INST_ID_V_SASF                98
#define  INST_ID_V_SATADD_I5           99
#define  INST_ID_V_SATADD              100
#define  INST_ID_V_SATADD_R3           101
#define  INST_ID_V_SATSUB              102
#define  INST_ID_V_SATSUB_R3           103
#define  INST_ID_V_SATSUBI             104
#define  INST_ID_V_SATSUBR             105
#define  INST_ID_V_SBF                 106
#define  INST_ID_V_SCH0L               107
#define  INST_ID_V_SCH0R               108
#define  INST_ID_V_SCH1L               109
#define  INST_ID_V_SCH1R               110
#define  INST_ID_V_SET1_B3             111
#define  INST_ID_V_SET1                112
#define  INST_ID_V_SETF                113
#define  INST_ID_V_SHL_I5              114
#define  INST_ID_V_SHL                 115
#define  INST_ID_V_SHL_R3              116
#define  INST_ID_V_SHR_I5              117
#define  INST_ID_V_SHR                 118
#define  INST_ID_V_SHR_R3              119
#define  INST_ID_V_SLDB                120
#define  INST_ID_V_SLDBU               121
#define  INST_ID_V_SLDH                122
#define  INST_ID_V_SLDHU               123
#define  INST_ID_V_SLDW                124
#define  INST_ID_V_SNOOZE              125
#define  INST_ID_V_SSTB                126
#define  INST_ID_V_SSTH                127
#define  INST_ID_V_SSTW                128
#define  INST_ID_V_STB_D16             129
#define  INST_ID_V_STB_D23             130
#define  INST_ID_V_STDW_D23            131
#define  INST_ID_V_STH_D16             132
#define  INST_ID_V_STH_D23             133
#define  INST_ID_V_STW_D16             134
#define  INST_ID_V_STW_D23             135
#define  INST_ID_V_STCW                136
#define  INST_ID_V_STSR                137
#define  INST_ID_V_SUB                 138
#define  INST_ID_V_SUBR                139
#define  INST_ID_V_SWITCH              140
#define  INST_ID_V_SXB                 141
#define  INST_ID_V_SXH                 142
#define  INST_ID_V_SYNCE               143
#define  INST_ID_V_SYNCI               144
#define  INST_ID_V_SYNCM               145
#define  INST_ID_V_SYNCP               146
#define  INST_ID_V_SYSCALL             147
#define  INST_ID_V_TRAP                148
#define  INST_ID_V_TST                 149
#define  INST_ID_V_TST1_B3             150
#define  INST_ID_V_TST1                151
#define  INST_ID_V_XOR                 152
#define  INST_ID_V_XORI                153
#define  INST_ID_V_ZXB                 154
#define  INST_ID_V_ZXH                 155
#define  INST_ID_V_DBCP                156
#define  INST_ID_V_DBPUSH              157
#define  INST_ID_V_DBRET               158
#define  INST_ID_V_DBTAG               159
#define  INST_ID_V_DBTRAP              160
#define  INST_ID_V_ABSFD               161
#define  INST_ID_V_ABSFS               162
#define  INST_ID_V_ADDFD               163
#define  INST_ID_V_ADDFS               164
#define  INST_ID_V_CEILFDL             165
#define  INST_ID_V_CEILFDUL            166
#define  INST_ID_V_CEILFDUW            167
#define  INST_ID_V_CEILFDW             168
#define  INST_ID_V_CEILFSL             169
#define  INST_ID_V_CEILFSUL            170
#define  INST_ID_V_CEILFSUW            171
#define  INST_ID_V_CEILFSW             172
#define  INST_ID_V_CMOVFD              173
#define  INST_ID_V_CMOVFS              174
#define  INST_ID_V_CMPFD               175
#define  INST_ID_V_CMPFS               176
#define  INST_ID_V_CVTFDL              177
#define  INST_ID_V_CVTFDS              178
#define  INST_ID_V_CVTFDUL             179
#define  INST_ID_V_CVTFDUW             180
#define  INST_ID_V_CVTFDW              181
#define  INST_ID_V_CVTFLD              182
#define  INST_ID_V_CVTFLS              183
#define  INST_ID_V_CVTFSD              184
#define  INST_ID_V_CVTFSL              185
#define  INST_ID_V_CVTFSUL             186
#define  INST_ID_V_CVTFSUW             187
#define  INST_ID_V_CVTFSW              188
#define  INST_ID_V_CVTFULD             189
#define  INST_ID_V_CVTFULS             190
#define  INST_ID_V_CVTFUWD             191
#define  INST_ID_V_CVTFUWS             192
#define  INST_ID_V_CVTFWD              193
#define  INST_ID_V_CVTFWS              194
#define  INST_ID_V_DIVFD               195
#define  INST_ID_V_DIVFS               196
#define  INST_ID_V_FLOORFDL            197
#define  INST_ID_V_FLOORFDUL           198
#define  INST_ID_V_FLOORFDUW           199
#define  INST_ID_V_FLOORFDW            200
#define  INST_ID_V_FLOORFSL            201
#define  INST_ID_V_FLOORFSUL           202
#define  INST_ID_V_FLOORFSUW           203
#define  INST_ID_V_FLOORFSW            204
#define  INST_ID_V_MAXFD               205
#define  INST_ID_V_MAXFS               206
#define  INST_ID_V_MINFD               207
#define  INST_ID_V_MINFS               208
#define  INST_ID_V_MULFD               209
#define  INST_ID_V_MULFS               210
#define  INST_ID_V_NEGFD               211
#define  INST_ID_V_NEGFS               212
#define  INST_ID_V_RECIPFD             213
#define  INST_ID_V_RECIPFS             214
#define  INST_ID_V_ROUNDFDL            215
#define  INST_ID_V_ROUNDFDUL           216
#define  INST_ID_V_ROUNDFDUW           217
#define  INST_ID_V_ROUNDFDW            218
#define  INST_ID_V_ROUNDFSL            219
#define  INST_ID_V_ROUNDFSUL           220
#define  INST_ID_V_ROUNDFSUW           221
#define  INST_ID_V_ROUNDFSW            222
#define  INST_ID_V_RSQRTFD             223
#define  INST_ID_V_RSQRTFS             224
#define  INST_ID_V_SQRTFD              225
#define  INST_ID_V_SQRTFS              226
#define  INST_ID_V_SUBFD               227
#define  INST_ID_V_SUBFS               228
#define  INST_ID_V_TRFSR               229
#define  INST_ID_V_TRNCFDL             230
#define  INST_ID_V_TRNCFDUL            231
#define  INST_ID_V_TRNCFDUW            232
#define  INST_ID_V_TRNCFDW             233
#define  INST_ID_V_TRNCFSL             234
#define  INST_ID_V_TRNCFSUL            235
#define  INST_ID_V_TRNCFSUW            236
#define  INST_ID_V_TRNCFSW             237
#define  INST_ID_V_FMAFS               238
#define  INST_ID_V_FMSFS               239
#define  INST_ID_V_FNMAFS              240
#define  INST_ID_V_FNMSFS              241
#define  INST_ID_V_CVTFHS              242
#define  INST_ID_V_CVTFSH              243
#define  INST_ID_V_BR                  244
#define  INST_ID_V_CMPFS4              245
#define  INST_ID_V_ROUNDFSW4           246
#define  INST_ID_V_ROUNDFSUW4          247
#define  INST_ID_V_TRNCFSW4            248
#define  INST_ID_V_TRNCFSUW4           249
#define  INST_ID_V_CEILFSW4            250
#define  INST_ID_V_CEILFSUW4           251
#define  INST_ID_V_FLOORFSW4           252
#define  INST_ID_V_FLOORFSUW4          253
#define  INST_ID_V_CVTFSW4             254
#define  INST_ID_V_CVTFSUW4            255
#define  INST_ID_V_CVTFWS4             256
#define  INST_ID_V_CVTFUWS4            257
#define  INST_ID_V_CVTFHS4             258
#define  INST_ID_V_CVTFSH4             259
#define  INST_ID_V_ABSFS4              260
#define  INST_ID_V_NEGFS4              261
#define  INST_ID_V_SQRTFS4             262
#define  INST_ID_V_RECIPFS4            263
#define  INST_ID_V_RSQRTFS4            264
#define  INST_ID_V_FLPVS4              265
#define  INST_ID_V_MOVVW4              266
#define  INST_ID_V_TRFSRVW4            267
#define  INST_ID_V_ADDFS4              268
#define  INST_ID_V_SUBFS4              269
#define  INST_ID_V_MULFS4              270
#define  INST_ID_V_MAXFS4              271
#define  INST_ID_V_MINFS4              272
#define  INST_ID_V_DIVFS4              273
#define  INST_ID_V_ADDRFS4             274
#define  INST_ID_V_SUBRFS4             275
#define  INST_ID_V_MULRFS4             276
#define  INST_ID_V_MAXRFS4             277
#define  INST_ID_V_MINRFS4             278
#define  INST_ID_V_ADDXFS4             279
#define  INST_ID_V_SUBXFS4             280
#define  INST_ID_V_MULXFS4             281
#define  INST_ID_V_ADDSUBFS4           282
#define  INST_ID_V_SUBADDFS4           283
#define  INST_ID_V_ADDSUBXFS4          284
#define  INST_ID_V_SUBADDXFS4          285
#define  INST_ID_V_ADDSUBNFS4          286
#define  INST_ID_V_SUBADDNFS4          287
#define  INST_ID_V_ADDSUBNXFS4         288
#define  INST_ID_V_SUBADDNXFS4         289
#define  INST_ID_V_FMAFS4              290
#define  INST_ID_V_FMSFS4              291
#define  INST_ID_V_FNMAFS4             292
#define  INST_ID_V_FNMSFS4             293
#define  INST_ID_V_LDVW_D16            294
#define  INST_ID_V_STVW_D16            295
#define  INST_ID_V_LDVQW_D16           296
#define  INST_ID_V_STVQW_D16           297
#define  INST_ID_V_LDVDW_D16           298
#define  INST_ID_V_STVDW_D16           299
#define  INST_ID_V_LDVZH4_D16          300
#define  INST_ID_V_STVZH4_D16          301
#define  INST_ID_V_CMOVFW4             302
#define  INST_ID_V_SHFLVW4             303
#define  INST_ID_V_CLIP_B              304
#define  INST_ID_V_CLIP_BU             305
#define  INST_ID_V_CLIP_H              306
#define  INST_ID_V_CLIP_HU             307
#define  INST_ID_V_LDLBU               308
#define  INST_ID_V_LDB_INC             309
#define  INST_ID_V_LDBU_INC            310
#define  INST_ID_V_LDB_DEC             311
#define  INST_ID_V_LDBU_DEC            312
#define  INST_ID_V_STCB                313
#define  INST_ID_V_STB_INC             314
#define  INST_ID_V_STB_DEC             315
#define  INST_ID_V_LDLHU               316
#define  INST_ID_V_LDH_INC             317
#define  INST_ID_V_LDHU_INC            318
#define  INST_ID_V_LDH_DEC             319
#define  INST_ID_V_LDHU_DEC            320
#define  INST_ID_V_STCH                321
#define  INST_ID_V_STH_INC             322
#define  INST_ID_V_STH_DEC             323
#define  INST_ID_V_LDW_INC             324
#define  INST_ID_V_LDW_DEC             325
#define  INST_ID_V_STW_INC             326
#define  INST_ID_V_STW_DEC             327
#define  INST_ID_V_RESBANK             328
#define  INST_ID_V_CLL                 329
#define  INST_ID_V_STM_GSR             330
#define  INST_ID_V_STM_MP              331
#define  INST_ID_V_LDM_GSR             332
#define  INST_ID_V_LDM_MP              333

#define TOTAL_INST_NUM 333

