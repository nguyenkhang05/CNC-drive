	.file "boot.s"

	/* 例外ハンドラ */
	/* ここにRBASEが設定されることを想定（ここが電源投入時の開始アドレス） */
	/* リセット以外の要因はIntHandler関数@int_handler.cに行く */
	.section .reset.text
	.align 1
	.global _ResetVector
	.type	_ResetVector, @function
_ResetVector:
	jr _Bootup
	.REPT 6
	nop
	.ENDR
SYSERR: #010H 010H
	.REPT 8
	nop
	.ENDR
HVTRAP: #020H 020H
    EIRET
	.REPT 6
	nop
	.ENDR
FETRAP: #030H 030H
	.REPT 8
	nop
	.ENDR
TRAP0: #040H 040H
    /* Change from UM to SV mode */
    stsr sr1, r10, 0 /* Read EIPSW */
    xor r11, r10     /* UM=1. r11 is set before TRAP0 exception */
    ldsr r10, sr1, 0 /* PSW */
    EIRET
	.REPT 1
	nop
	.ENDR
TRAP1: #050H 050H
	.REPT 8
	nop
	.ENDR
RIE: #060H 060H
    stsr sr2, r11, 0
    add 4,r11
    ldsr r11, sr2, 0 /* FEPC */
    feret
  	.REPT 1
	nop
	.ENDR
FPPFPI: #070H 070H
    stsr sr6, r4
	.REPT 6
	nop
	.ENDR
UCPOP: #080H 080H
	.REPT 8
	nop
	.ENDR
MIP: #090H 090H
	.REPT 8
	nop
	.ENDR
PIE: #0A0H 0A0H
    stsr sr2, r11, 0
    add 4,r11
    ldsr r11, sr2, 0 /* FEPC */
    feret
  	.REPT 1
	nop
	.ENDR
DEBUG: #0B0H 0B0H
    dbret
	.REPT 6
	nop
	.ENDR
MAE: #0C0H 0C0H
	.REPT 8
	nop
	.ENDR
RFU: #0D0H 0D0H
	.REPT 8
	nop
	.ENDR
FENMI: #0E0H 0E0H
	.REPT 8
	nop
	.ENDR
FEINT: #0F0H 0F0H
	.REPT 8
	nop
	.ENDR
EIINTn0: #（優先度0） 100H
	.REPT 8
	nop
	.ENDR
EIINTn1: #（優先度1） 110H
	.REPT 8
	nop
	.ENDR
EIINTn2: #（優先度2） 120H
	.REPT 8
	nop
	.ENDR
EIINTn3: #（優先度3） 130H
	.REPT 8
	nop
	.ENDR
EIINTn4: #（優先度4） 140H
	.REPT 8
	nop
	.ENDR
EIINTn5: #（優先度5） 150H
	.REPT 8
	nop
	.ENDR
EIINTn6: #（優先度6） 160H
	.REPT 8
	nop
	.ENDR
EIINTn7: #（優先度7） 170H
	.REPT 8
	nop
	.ENDR
EIINTn8: #（優先度8） 180H
	.REPT 8
	nop
	.ENDR
EIINTn9: #（優先度9） 190H
	.REPT 8
	nop
	.ENDR
EIINTn10: #（優先度10） 1A0H
	.REPT 8
	nop
	.ENDR
EIINTn11: #（優先度11） 1B0H
	.REPT 8
	nop
	.ENDR
EIINTn12: #（優先度12） 1C0H
	.REPT 8
	nop
	.ENDR
EIINTn13: #（優先度13） 1D0H
	.REPT 8
	nop
	.ENDR
EIINTn14: #（優先度14） 1E0H
	.REPT 8
	nop
	.ENDR
EIINTn15: #（優先度15） 1F0H
    jr GoHalt
	.REPT 6
	nop
	.ENDR
	.size	_ResetVector, .-_ResetVector

    SIM_BASE = 0xFFFF7F00
    SIM_RESULT = 0xFC
    SIM_STOPIO = 0xFE

	/* ブート処理 */
	/* PE1はGCCのcrt0を実行する */
	/* PE1以外はHALT */
	.align 1
	.extern _SetBasePointer
	.global _Bootup
	.type	_Bootup, @function
_Bootup:
InitRegs:
    mov     r0, r1
    mov     r0, r2
    mov     r0, r3
    mov     r0, r4
    mov     r0, r5
    mov     r0, r6
    mov     r0, r7
    mov     r0, r8
    mov     r0, r9
    mov     r0, r10
    mov     r0, r11
    mov     r0, r12
    mov     r0, r13
    mov     r0, r14
    mov     r0, r15
    mov     r0, r16
    mov     r0, r17
    mov     r0, r18
    mov     r0, r19
    mov     r0, r20
    mov     r0, r21
    mov     r0, r22
    mov     r0, r23
    mov     r0, r24
    mov     r0, r25
    mov     r0, r26
    mov     r0, r27
    mov     r0, r28
    mov     r0, r29
    mov     r0, r30
    mov     r0, r31

    dbtrap

    dbret /* PIE occur on HV mode. */

    /* Check to read regs */
    mov 0xFFFFFFFF, r10
    ldsr r10, sr20, 1 /* HVSB */
    stsr sr20, r11, 1 /* HVSB */
    ldsr r10, sr21, 0 /* SNZCFG */
    stsr sr21, r11, 0 /* SNZCFG */
    stsr sr5, r10, 9 /* GMPSW to */
    ldsr r10, sr5, 9 /* GMPSW */

    /* Change to guest mode */
    stsr sr5, r10, 0 /* PSW to */
    ldsr r10, sr1, 0 /* EIPSW */
    mov 0x80000000, r10
    ldsr r10, sr18, 0 /* EIPSWH */

    jarl .+4,r10 ; add 10,r10
    ldsr r10, sr0, 0 /* EIPC */
    eiret

    /* Guest mode SV here */
    nop
    stsr sr5, r10, 0 /* PSW to */
    ldsr r10, sr5, 0 /* PSW */

    /* Check to read regs */
    mov 0xFFFFFFFF, r10
    ldsr r10, sr20, 1  /*HVSB */
    stsr sr20, r11, 1  /*HVSB */
    ldsr r10, sr21, 0  /*SNZCFG */
    stsr sr21, r11, 0  /*SNZCFG */
    stsr sr5, r10, 9 /* GMPSW to */
    ldsr r10, sr5, 9 /* GMPSW */

    /* Guest mode : Change to UM mode */
    stsr sr5, r10, 0 /* Read PSW */
    mov 0x40000000, r11
    or r11, r10     /* UM=1 */
    ldsr r10, sr5, 0 /* PSW */

    /* Guest mode UM here */
    stsr sr5, r10, 0 /* PSW to */
    ldsr r10, sr5, 0 /* PSW */
    stsr sr5, r10, 9 /* GMPSW to */
    ldsr r10, sr5, 9 /* GMPSW */

    mov 0x40000000, r11 /* mask in trap0 */
    trap 0

    hvtrap 0

    dbtrap
    nop

    dbret /* PIE occur on SV mode. */

    mov     SIM_BASE, r7
    mov     0x00000000, r8
    st.h    r8, SIM_RESULT[r7]
    mov     0x00000010, r8
    st.h    r8, SIM_STOPIO[r7]

GoHalt:
    halt
    br GoHalt
    .size _Bootup, .-_Bootup

    .align 1
    .global _main
    .type _main, @function
_main:
    nop
    .size _main, .-_main

_test_end:
    nop
	.size	_test, .-_test
