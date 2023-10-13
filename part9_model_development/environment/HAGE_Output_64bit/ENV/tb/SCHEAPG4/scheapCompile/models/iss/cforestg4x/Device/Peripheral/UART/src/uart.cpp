/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifdef MSVC
#include <conio.h>
#endif // MSVC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#ifndef MSVC
#include <unistd.h>
#endif // MSVC

#include <functional>
#include <algorithm>
#include <iostream>

#include "./config.h"
#include "./forest_utility.h"
#include "./intc2.h"
#include "./forest_message.h"
#include "./fsbus.h"
#include "./uart.h"


/*!
 * @brief Writes ASIM.CL bit. : ASIM.CLビットライト関数
 * @param cl Data to write. : 書込みデータ
 */
void TRegASIM::SetCL (RegData cl)
{
    if (m_bit.m_cl == cl) return;

    m_bit.m_cl = cl & 0x01U;

    UartChannel* u_ch = m_uart->GetUartChannel (m_chid);
    if (cl == 1) u_ch->SetTransMask (0xFF);
    else         u_ch->SetTransMask (0x7F);
}


/*!
 * @brief Writes ASIM.RXE bit. : ASIM.RXEビットライト関数
 * @param rxe Data to write. : 書込みデータ
 */
void TRegASIM::SetRXE (RegData rxe)
{
    if (m_bit.m_rxe == rxe) return;

    m_bit.m_rxe = rxe & 0x01U;

    UartChannel* u_ch = m_uart->GetUartChannel (m_chid);
    if (rxe == 1) {
        if (u_ch->GetClientSockfd () != Uart::UART_UNUSED_SOCKET) {
            u_ch->SetIsReceiveEnable (1);
        }
    } else {
        u_ch->RegInitReceive ();
    }
}


/*!
 * @brief Writes ASIM.TXE bit. : ASIM.TXEビットライト関数
 * @param txe Data to write. : 書込みデータ
 */
void TRegASIM::SetTXE (RegData txe)
{
    if (m_bit.m_txe == txe) return;

    m_bit.m_txe = txe & 0x01U;

    UartChannel* u_ch = m_uart->GetUartChannel (m_chid);
    if (txe == 1) {
        if (u_ch->GetClientSockfd () != Uart::UART_UNUSED_SOCKET) {
            u_ch->SetIsTransEnable (1);
        }
    } else {
        u_ch->RegInitTransmit ();
    }
}


/*!
 * @brief Writes ASIM.POWER bit. : ASIM.POWERビットライト関数
 * @param power Data to write. : 書込みデータ
 */
void TRegASIM::SetPOWER (RegData power)
{
    if (m_bit.m_power == power) return;

    m_bit.m_power = power & 0x01U;

    UartChannel* u_ch = m_uart->GetUartChannel (m_chid);
    if (power == 0) {
        u_ch->RegInit ();
    }

    m_uart->m_fsbus->RemoveCyclicHandler (m_uart);
    for (uint32_t i = 0; i < Uart::N_CH_UART; i++) {
        if (m_uart->GetASIMPtr (i)->GetPOWER () == true) {
            m_uart->m_fsbus->AddCyclicHandler (m_uart);
            break;
        }
    }
}


/*!
 * @brief Writes ASIM. : ASIMライト関数
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Access size. : アクセスサイズ
 * @return SysErrCause
 */
SysErrCause TRegASIM::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    SetPOWER (((*data) >> 7) & 0x01U);
    SetTXE   (((*data) >> 6) & 0x01U);
    SetRXE   (((*data) >> 5) & 0x01U);
    SetPS    (((*data) >> 3) & 0x03U);
    SetCL    (((*data) >> 2) & 0x01U);
    SetSL    (((*data) >> 1) & 0x01U);
    SetISRM  (((*data) >> 0) & 0x01U);

    return SYSERR_NONE;
}


/*!
 * @brief Writes ASIS. : ASISライト関数
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Access size. : アクセスサイズ
 * @return SysErrCause
 */
SysErrCause TRegASIS::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    uint32_t pe  = ((*data) >> 2) & 0x01U;
    uint32_t fe  = ((*data) >> 1) & 0x01U;
    uint32_t ove = ((*data) >> 0) & 0x01U;
    if (pe == 1)  SetPE (0);
    if (fe == 1)  SetFE (0);
    if (ove == 1) SetOVE (0);

    return SYSERR_NONE;
}


/*!
 * @brief Writes TXB. : TXBライト関数
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Access size. : アクセスサイズ
 * @return SysErrCause
 */
SysErrCause TRegTXB::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    UartChannel* u_ch = m_uart->GetUartChannel (m_chid);

    if (u_ch->TrASIM()->GetTXE () == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Write to TXB in ASIM.TXE=0>\n");
        return SYSERR_NONE;
    }

    if (u_ch->TrASIF()->GetTXBF () == true) {
        CfMsg::DPrint (MSG_ERR, "<Error: Write to TXB in ASIF.TXBF=1>\n");
        return SYSERR_NONE;
    }

    m_data.m_data8 = (*data) & u_ch->GetTransMask ();

    if (u_ch->TrASIF()->GetTXSF () == true) {
        u_ch->TrASIF()->SetTXBF (1);
    } else {
        u_ch->SetTXSF (m_data.m_data8);
        u_ch->TrASIF()->SetTXSF (1);
    }

    return SYSERR_NONE;
}


/*!
 * @brief Instantiates UART register. : UARTレジスタのインスタンス化
 * @param -
 */
void TRegUart::InitTRegMap (void)
{
    PhysAddr uart_addr = m_uart->GetUartAddr ();

    for (uint32_t i = 0; i < Uart::N_CH_UART; i++) {
        PhysAddr asim_addr = uart_addr + ASIM_OFFSET + 0x40 * i;
        PhysAddr rxb_addr  = uart_addr + RXB_OFFSET  + 0x40 * i;
        PhysAddr asis_addr = uart_addr + ASIS_OFFSET + 0x40 * i;
        PhysAddr txb_addr  = uart_addr + TXB_OFFSET  + 0x40 * i;
        PhysAddr asif_addr = uart_addr + ASIF_OFFSET + 0x40 * i;
        PhysAddr cksr_addr = uart_addr + CKSR_OFFSET + 0x40 * i;
        PhysAddr brgc_addr = uart_addr + BRGC_OFFSET + 0x40 * i;
        PhysAddr rts_addr  = uart_addr + RTS_OFFSET  + 0x40 * i;
        PhysAddr cts_addr  = uart_addr + CTS_OFFSET  + 0x40 * i;

        TRegASIM* asim = new TRegASIM (SIZE_8BIT, SIZE_8BIT, asim_addr, i, m_uart);
        TRegRXB*  rxb  = new TRegRXB  (SIZE_8BIT, SIZE_8BIT, rxb_addr,  i, m_uart);
        TRegASIS* asis = new TRegASIS (SIZE_8BIT, SIZE_8BIT, asis_addr, i, m_uart);
        TRegTXB*  txb  = new TRegTXB  (SIZE_8BIT, SIZE_8BIT, txb_addr,  i, m_uart);
        TRegASIF* asif = new TRegASIF (SIZE_8BIT, SIZE_8BIT, asif_addr, i, m_uart);
        TRegCKSR* cksr = new TRegCKSR (SIZE_8BIT, SIZE_8BIT, cksr_addr, i, m_uart);
        TRegBRGC* brgc = new TRegBRGC (SIZE_8BIT, SIZE_8BIT, brgc_addr, i, m_uart);
        TRegRTS*  rts  = new TRegRTS  (SIZE_8BIT, SIZE_8BIT, rts_addr,  i, m_uart);
        TRegCTS*  cts  = new TRegCTS  (SIZE_8BIT, SIZE_8BIT, cts_addr,  i, m_uart);

        m_trf.insert (TRegMap::value_type (asim_addr, asim));
        m_trf.insert (TRegMap::value_type (rxb_addr,  rxb));
        m_trf.insert (TRegMap::value_type (asis_addr, asis));
        m_trf.insert (TRegMap::value_type (txb_addr,  txb));
        m_trf.insert (TRegMap::value_type (asif_addr, asif));
        m_trf.insert (TRegMap::value_type (cksr_addr, cksr));
        m_trf.insert (TRegMap::value_type (brgc_addr, brgc));
        m_trf.insert (TRegMap::value_type (rts_addr,  rts));
        m_trf.insert (TRegMap::value_type (cts_addr,  cts));
    }
}


/*!
 * @brief Main function in UART. : メイン関数
 * @param -
 */
void Uart::CyclicHandler (void)
{
    if (unlikely (--m_scan_counter == 0)) {
        m_scan_counter = DEFAULT_SCAN_COUNT;
        ReceiveSerial ();
    }

    if (unlikely (--m_transmit_counter == 0)) {
        m_transmit_counter = DEFAULT_TRANSMIT_COUNT;
        TransmitSerial ();
    }
}


/*!
 * @brief Gets the pointer to ASIM register class. : ASIMレジスタクラスポインタの取得
 * @param chid Channel number. : チャネル番号
 * @return TRegASIM*
 */
TRegASIM* Uart::GetASIMPtr (uint32_t chid) const
{
    PhysAddr asim_addr = m_uart_base_addr + TRegUart::ASIM_OFFSET + 0x40 * chid;
    return static_cast<TRegASIM*> (m_trfs->GetTrPtr (asim_addr));
}


/*!
 * @brief Gets the pointer to RXB register class. : RXBレジスタクラスポインタの取得
 * @param chid channel number. : チャネル番号
 * @return TRegRXB*
 */
TRegRXB* Uart::GetRXBPtr (uint32_t chid) const
{
    PhysAddr rxb_addr = m_uart_base_addr + TRegUart::RXB_OFFSET + 0x40 * chid;
    return static_cast<TRegRXB*> (m_trfs->GetTrPtr (rxb_addr));
}


/*!
 * @brief Gets the pointer to ASIS register class. : ASISレジスタクラスポインタの取得
 * @param chid Channel number. : チャネル番号
 * @return TRegASIS*
 */
TRegASIS* Uart::GetASISPtr (uint32_t chid) const
{
    PhysAddr asis_addr = m_uart_base_addr + TRegUart::ASIS_OFFSET + 0x40 * chid;
    return static_cast<TRegASIS*> (m_trfs->GetTrPtr (asis_addr));
}


/*!
 * @brief Gets the pointer to TXB register class. : TXBレジスタクラスポインタの取得
 * @param chid Channel number. : チャネル番号
 * @return TRegTXB*
 */
TRegTXB* Uart::GetTXBPtr (uint32_t chid) const
{
    PhysAddr txb_addr = m_uart_base_addr + TRegUart::TXB_OFFSET + 0x40 * chid;
    return static_cast<TRegTXB*> (m_trfs->GetTrPtr (txb_addr));
}


/*!
 * @brief Gets the pointer to ASIF register class. : ASIFレジスタクラスポインタの取得
 * @param chid Channel number. : チャネル番号
 * @return TRegASIF*
 */
TRegASIF* Uart::GetASIFPtr (uint32_t chid) const
{
    PhysAddr asif_addr = m_uart_base_addr + TRegUart::ASIF_OFFSET + 0x40 * chid;
    return static_cast<TRegASIF*> (m_trfs->GetTrPtr (asif_addr));
}


/*!
 * @brief Receives UART. : UART受信
 * @param -
 */
void Uart::ReceiveSerial (void)
{
    fd_set rfds;

    FD_ZERO (&rfds);
    UartChannelVec::const_iterator it = m_ch_active.begin ();
    while (it != m_ch_active.end ()) {
        if ((*it)->IsReceiveEnable ()) {
            FD_SET ((*it)->GetClientSockfd (), &rfds);
        }
        ++ it;
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int32_t retval = select (m_nfds, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
        //perror ("Select failed!");
    } else {
        it = m_ch_active.begin ();
        while (it != m_ch_active.end ()) {
            if ((*it)->IsReceiveEnable ()) {
                // socketから受信
                if (FD_ISSET ((*it)->GetClientSockfd (), &rfds) > 0) {
                    // 受信
                    uint8_t recv_data;
                    ssize_t msglen =
                            read ((*it)->GetClientSockfd (), &recv_data, sizeof (recv_data));
                    if (msglen == -1) {
                        // 受信失敗
                        //perror("read");
                    } else if (msglen != 0) {
                        // メッセージの受信に成功
                        ReceiveData (*it, recv_data);
                        // 割り込み
                        ReqReceiveIntToIntc2 (*it);
                        // 次も入力がある可能性が高いのでカウンタ間隔を狭める
                        m_scan_counter = SEQUENTIAL_SCAN_COUNT;
                    } else {
                        // Peer側からソケットが切断された
                        (*it)->CloseSocketByPeer ();
                        UpdateChannelVector ();
                        UpdateNFDS ();
                    }
                }
            }
            ++ it;
        }
    }
}


/*!
 * @brief Transmits UART. : UART送信
 * @param -
 */
void Uart::TransmitSerial (void)
{
    fd_set rfds;

    FD_ZERO (&rfds);
    UartChannelVec::const_iterator it = m_ch_active.begin ();
    while (it != m_ch_active.end ()) {
        if ((*it)->IsTransEnable ()) {
            FD_SET ((*it)->GetClientSockfd (), &rfds);
        }
        ++ it;
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    select (m_nfds, NULL, &rfds, NULL, &tv);

    it = m_ch_active.begin ();
    while (it != m_ch_active.end ()) {
        if ((*it)->IsTransEnable ()) {
            // 未送信データがあれば送信
            if ((*it)->TrASIF()->GetTXSF () == true) {
                // 出力バッファの内容を送信
                if (FD_ISSET ((*it)->GetClientSockfd (), &rfds) > 0) {
                    // 送信
                    uint8_t trns_data = static_cast<uint8_t> ((*it)->GetTXSF() & 0xFF);
                    ssize_t msglen =
                            write ((*it)->GetClientSockfd (), &trns_data, sizeof (trns_data));
                    if (msglen == -1) {
                        // 送信失敗
                        //perror("wriet");
                    } else if (msglen != 0) {
                        // メッセージの送信に成功
                        TransmitData (*it);
                        // 1フレーム分のデータを送信したら送信完了割り込み
                        ReqTransmitIntToIntc2 (*it);
                    } else {
                        // Peer側からソケットが切断された
                        (*it)->CloseSocketByPeer ();
                        UpdateChannelVector ();
                        UpdateNFDS ();
                    }
                }

                // 連続送信の可能性が高いので間隔を狭める
                m_transmit_counter = SEQUENTIAL_TRANSMIT_COUNT;
            }
        }
        ++ it;
    }
}


/*!
 * @brief Sends received data to RXB. : 受信データをRXBへ転送
 * @param u_ch The pointer to UART channel. : UARTチャネルポインタ
 * @param data Received Data. : 受信データ
 */
void Uart::ReceiveData (UartChannel* u_ch, uint8_t data)
{
    // RXBを未読み出しなら、オーバーランエラー
    if (u_ch->TrRXB()->IsRXBRead () == false) {
        // オーバーランエラー : 受信バッファを更新しない
        u_ch->TrASIS()->SetOVE (1);
    } else {
        // 正常受信
        u_ch->TrRXB()->SetRXB (data & u_ch->GetTransMask ());
    }
    // RXBレジスタRead済みフラグを未readに設定
    u_ch->TrRXB()->SetIsRXBRead (0);
}


/*!
 * @brief Sends data to transmit to the internal buffer. : 送信データを内部バッファへ転送
 * @param u_ch The pointer to UART channel. : UARTチャネルポインタ
 */
void Uart::TransmitData (UartChannel* u_ch)
{
    u_ch->TrASIF()->SetTXSF (0);

    // TXBに有効なデータがあればTXSFに転送
    if (u_ch->TrASIF()->GetTXBF () == true) {
        u_ch->TrASIF()->SetTXSF (1);
        u_ch->TrASIF()->SetTXBF (0);
        u_ch->SetTXSF (u_ch->TrTXB()->GetTXB ());
    }
}


/*!
 * @brief Sends the interrupt request for the completion of recieving. : 受信完了割込み要求
 * @param u_ch The pointer to UART channel. : UARTチャネルポインタ
 */
void Uart::ReqReceiveIntToIntc2 (UartChannel* u_ch)
{
    TRegASIS* asis = u_ch->TrASIS();
    if (asis->GetPE() || asis->GetFE() || asis->GetOVE()) {
        // 受信エラー発生
        if (u_ch->TrASIM()->GetISRM () == true) {
            // 受信完了割り込みを発生
            m_intc2->ReqIntByPeripheral (Uart::INTSR + u_ch->GetCHID());
        } else {
            // 受信エラー割り込みを発生
            m_intc2->ReqIntByPeripheral (Uart::INTSRE + u_ch->GetCHID());
        }
    } else {
        // 受信完了割り込みを発生
        m_intc2->ReqIntByPeripheral (Uart::INTSR + u_ch->GetCHID());
    }
}


/*!
 * @brief Sends the interrupt request for the completion of transmitting. : 送信完了割込み要求
 * @param u_ch The pointer to UART channel. : UARTチャネルポインタ
 */
void Uart::ReqTransmitIntToIntc2 (UartChannel* u_ch)
{
    m_intc2->ReqIntByPeripheral (Uart::INTST + u_ch->GetCHID());
}


/*!
 * @brief Updates the maximum value for a file descriptor. : ファイルディスクリプタの最大値を更新
 * @param -
 */
void Uart::UpdateNFDS (void)
{
    m_nfds = -1;
    for (uint32_t i = 0; i < m_ch_active.size (); i++) {
        m_nfds = std::max (m_ch_active[i]->GetClientSockfd (), m_nfds);
    }
    m_nfds++;
}


/*!
 * @brief Generates the vector of the channels which have the socket already connected. : 接続済みのソケットを持つチャネルのベクタ生成
 * @param -
 */
void Uart::UpdateChannelVector(void)
{
    UartChannelVec::const_iterator it = m_ch_all.begin ();
    while (it != m_ch_all.end ()) {
        if ((*it)->GetClientSockfd () != UART_UNUSED_SOCKET) {
            m_ch_active.push_back (*it);
        }
        ++ it;
    }
}


/*!
 * @brief Constructs UART channel. : UARTチャネルコンストラクタ
 * @param chid Channel number. : チャネル番号
 * @param intc2 The pointer to INTC2. : INTC2ポインタ
 * @param uart The pointer to UART. : UARTポインタ
 */
UartChannel::UartChannel (uint32_t chid, CIntc2* intc2, Uart* uart)
                         : m_chid (chid), m_intc2 (intc2)
{
    m_asim = uart->GetASIMPtr (chid);
    m_rxb  = uart->GetRXBPtr  (chid);
    m_asis = uart->GetASISPtr (chid);
    m_txb  = uart->GetTXBPtr  (chid);
    m_asif = uart->GetASIFPtr (chid);
}


/*!
 * @brief Invalidates UART channel. : UARTチャネル無効化
 * @param -
 */
void UartChannel::CloseSocketByPeer (void)
{
    // Peer側からソケットが切断された
    close (m_client_sockfd);

    m_client_sockfd = Uart::UART_UNUSED_SOCKET;
    m_receive_enable = false;
    m_transmit_enable = false;
}


/*!
 * @brief Writes to the register. : レジスタライト関数
 * @param sideband Sideband. : サイドバンド
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Uart::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->Write (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Reads from the register. : レジスタリード関数
 * @param sideband Sideband. : サイドバンド
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Uart::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Writes to the register with debugging. : レジスタデバックライト関数
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Uart::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->WriteBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Reads from the register with debugging. : レジスタデバックリード関数
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Uart::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Resets the circuit of receive. : 受信回路リセット
 * @param -
 */
void UartChannel::RegInitReceive (void)
{
    m_rxb->Reset ();
    m_asis->Reset ();
    m_receive_enable = false;

    // CPUへの割り込み要求を解除
    m_intc2->CancelIntByPeripheral (Uart::INTSRE + m_chid);
    m_intc2->CancelIntByPeripheral (Uart::INTSR + m_chid);
}


/*!
 * @brief Resets the circuit of transmit. : 送信回路リセット
 * @param -
 */
void UartChannel::RegInitTransmit (void)
{
    m_asif->Reset ();
    m_transmit_enable = false;

    // CPUへの割り込み要求を解除
    m_intc2->CancelIntByPeripheral (Uart::INTST + m_chid);
}


/*!
 * @brief Resets UART register. : UARTレジスタリセット
 * @param -
 */
void UartChannel::RegInit (void)
{
    RegInitReceive ();
    RegInitTransmit ();

    m_asim->Reset ();
    m_txb->Reset ();
    m_trans_data_mask = 0x7F;
    m_txsf = 0xFF;
}


/*!
 * @brief Initializes UART. : UART初期化
 * @param -
 */
void Uart::UartInit (void)
{
    m_fsbus->RemoveCyclicHandler (this);
    m_scan_counter = DEFAULT_SCAN_COUNT;
    m_transmit_counter = DEFAULT_TRANSMIT_COUNT;
}


/*!
 * @brief Resets UART. : UARTリセット
 * @param -
 */
void Uart::HardReset (void)
{
    UartInit ();
    std::for_each (m_ch_all.begin (), m_ch_all.end (),
                   std::mem_fun (&UartChannel::RegInit));
}


/*!
 * @brief Creates UART Channel. : UARTチャネル生成
 * @param -
 */
void Uart::CreateChannel (void)
{
    for (uint32_t i = 0; i < N_CH_UART; i++) {
        UartChannel* ch = new UartChannel (i, m_intc2, this);
        ch->SetClientSockfd (UART_UNUSED_SOCKET);
        m_ch_all.push_back (ch);
    }
}


/*!
 * @brief Deletes UART channel. : UARTチャネル削除
 * @param -
 */
void Uart::DeleteChannel (void)
{
    UartChannelVec::const_iterator it = m_ch_all.begin ();
    while (it != m_ch_all.end ()) {
        delete (*it);
        ++ it;
    }
}


/*!
 * @brief Connects the socket. : ソケット接続
 * @param port Port number. : ポート番号
 * @param ch_num The number of the channels to connect. : 接続チャネル数
 */
void Uart::ConnectSocket (uint16_t port, int32_t ch_num)
{
    // ソケットの作成
    int server_sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        CfMsg::DPrint (MSG_ERR, "<Error: Socket for server failed!>\n");
        exit (-1);
    }

    // ソケットオプションの設定
    // ポート番号の再利用を許可。
    // 同じポート番号をタイムアウトまで再利用できなくなる事を防ぐため。
    const int sock_opt = 1;
    if (setsockopt (server_sockfd, SOL_SOCKET, SO_REUSEADDR, &sock_opt,
                    sizeof (sock_opt)) == -1) {
        CfMsg::DPrint (MSG_ERR, "<Error: error at setsockopt.>\n");
        exit (1);
    }

    // ソケットに名前をつける
    struct sockaddr_in server_address;
    bzero (reinterpret_cast<char *> (&server_address), sizeof (server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl (INADDR_ANY);
    server_address.sin_port = htons (port); // Ignore conversion warning because it comes from htons code.
    socklen_t server_len = sizeof (server_address);

    // ソケットにアドレスをバインド
    void *server_addr_temp = &server_address;
    if (bind (server_sockfd,
              static_cast<struct sockaddr*> (server_addr_temp), server_len) < 0) {
        CfMsg::DPrint (MSG_ERR, "<Error: Bind failed!>\n");
        exit (-1);
    }

    // 接続キューを作成
    if (listen (server_sockfd, ch_num) ) {
        CfMsg::DPrint (MSG_ERR, "<Error: Listen failed!>\n");
        exit (1);
    }

    // ホスト名の取得と表示
    char hostname[256];
    char localhost[] = "localhost";
    if (gethostname (hostname, 255) != 0) {
        strcpy (hostname, localhost);
    } else {
        // 終端は保証されていないので保険として実施
        hostname[255] = '\0';
    }

    CfMsg::DPrint (MSG_ERR, "Plaese connect %d terminals to %s:%d\n", ch_num, hostname, port);
    CfMsg::DPrint (MSG_ERR, "<example command> telnet %s %d>\n", hostname, port);

    // 接続確立
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof (client_address);
    for (int32_t i = 0; i < ch_num; i++) {
        CfMsg::DPrint (MSG_ERR, "Wait for connecting.\n");
        fflush (NULL);
        void *client_addr_temp = &client_address;
        m_ch_all[i]->SetClientSockfd (accept (server_sockfd,
                                              static_cast<struct sockaddr*> (client_addr_temp),
                                              &client_len));
        if (m_ch_all[i]->GetClientSockfd () < 0) {
            CfMsg::DPrint (MSG_ERR, "<Error: Client accept failed!>\n");
            exit (-1);
        }

        CfMsg::DPrint (MSG_ERR, "Terminal %d has been connected.\n", i);
    }
    close (server_sockfd);

    CfMsg::DPrint (MSG_ERR, "Finished connecting.\n");
    fflush (NULL);

    UpdateChannelVector ();
    UpdateNFDS ();
}


/*!
 * @brief Disconnects the socket. : ソケット切断
 * @param -
 */
void Uart::DisconnectSocket (void)
{
    for (uint32_t i = 0; i < m_ch_active.size (); i++) {
        if (m_ch_active[i]->GetClientSockfd () != UART_UNUSED_SOCKET) {
            close (m_ch_active[i]->GetClientSockfd ());
            m_ch_active[i]->SetClientSockfd (UART_UNUSED_SOCKET);
        }
    }
}


/*!
 * @brief Constructs UART. : UARTコンストラクタ
 * @param fsbus The pointer to FsBus. : FsBusポインタ
 * @param intc2 The pointer to INTC2. : INTC2ポインタ
 * @param addr Base address of UART. : UARTベースアドレス
 */
Uart::Uart (FsBus* fsbus, CIntc2* intc2, PhysAddr addr)
        : m_fsbus (fsbus), m_intc2 (intc2), m_uart_base_addr (addr)
{
    m_trfs = new TRegUart (this);

    UartInit ();
    CreateChannel ();

    // レジスタアクセスのためにレジスタを全チャネル分作成するが、
    // ソケットに接続するのはコマンドラインで指定されたチャンネル数だけ
    uint32_t max_ch_num = N_CH_UART;
    int32_t ch_num = std::min (p_forest_util->GetUartChannelNum (), max_ch_num);

    ConnectSocket (p_forest_util->GetUartSocketPort (), ch_num);
}


/*!
 * @brief Destructs UART. : UARTデストラクタ
 * @param -
 */
Uart::~Uart ()
{
    // Socket通信の終了
    DisconnectSocket();

    delete m_trfs;

    // 各チャンネルのインスタンス削除
    DeleteChannel ();
}
