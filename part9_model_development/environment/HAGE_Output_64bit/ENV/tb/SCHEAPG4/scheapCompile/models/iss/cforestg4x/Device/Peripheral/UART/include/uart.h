/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./treg.h"

class Uart;
class UartChannel;


/*!
  @brief UART register base class. :  UARTレジスタベースクラス
 */
class TRegUartBase : public TRegDataBase<TRegData8> {
public:
    TRegUartBase (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
                  TRegMask reg_mask, uint32_t chid, Uart* uart)
     : TRegDataBase<TRegData8> (reg_size, mini_access_size, reg_addr, reg_mask),
       m_chid (chid), m_uart (uart) {}

protected:
    uint32_t m_chid;
    Uart* m_uart;
};


/*!
  @brief ASIM register class. : ASIMレジスタクラス
 */
class TRegASIM : public TRegUartBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_isrm  :1;
            RegData m_sl    :1;
            RegData m_cl    :1;
            RegData m_ps    :2;
            RegData m_rxe   :1;
            RegData m_txe   :1;
            RegData m_power :1;
        } m_bit;
    };

public:
    static const uint32_t ASIM_MASK = 0x000000FFU;

    void SetISRM (RegData isrm) { m_bit.m_isrm = isrm & 0x01U; }
    void SetSL (RegData sl) { m_bit.m_sl = sl & 0x01U; }
    void SetCL (RegData cl);
    void SetPS (RegData ps) { m_bit.m_ps = ps & 0x03U; }
    void SetRXE (RegData rxe);
    void SetTXE (RegData txe);
    void SetPOWER (RegData power);
    RegData GetISRM (void) const { return m_bit.m_isrm; }
    RegData GetSL (void) const { return m_bit.m_sl; }
    RegData GetCL (void) const { return m_bit.m_cl; }
    RegData GetPS (void) const { return m_bit.m_ps; }
    RegData GetRXE (void) const { return m_bit.m_rxe; }
    RegData GetTXE (void) const { return m_bit.m_txe; }
    RegData GetPOWER (void) const { return m_bit.m_power; }

    void Reset (void) { m_data.m_data8 = 0x01U; }

    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    const char* GetName (void) const { return "ASIM"; }

    TRegASIM (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
              uint32_t chid, Uart* uart)
     : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (ASIM_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief RXB register class. : RXBレジスタクラス
 */
class TRegRXB : public TRegUartBase {
private:
    TRegData8 m_data;
    mutable bool m_is_read; // RXBレジスタRead済みフラグ

public:
    static const uint32_t RXB_MASK = 0x000000FFU;

    void SetRXB (RegData rxb) { m_data.m_data8 = rxb & 0xFFU; }
    void SetIsRXBRead (bool is_read) { m_is_read = is_read; }
    RegData GetRXB (void) const { return m_data.m_data8; }
    bool IsRXBRead (void) const { return m_is_read; }

    void Reset (void) {
        m_data.m_data8 = 0xFFU;
        m_is_read = true;
    }

    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const {
        *data = m_data.m_data8;
        m_is_read = true;
        return SYSERR_NONE;
    }

    const char* GetName (void) const { return "RXB"; }

    TRegRXB (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
             uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (RXB_MASK), chid, uart) {
        m_treg = &m_data;
        m_is_read = true;
    }
};


/*!
  @brief ASIS register class. : ASISレジスタクラス
 */
class TRegASIS : public TRegUartBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_ove :1;
            RegData m_fe  :1;
            RegData m_pe  :1;
            RegData       :5;
        } m_bit;
    };

public:
    static const uint32_t ASIS_MASK = 0x000000FFU;

    void SetOVE (RegData ove) { m_bit.m_ove = ove & 0x01U; }
    void SetFE (RegData fe) { m_bit.m_fe = fe & 0x01U; }
    void SetPE (RegData pe) { m_bit.m_pe = pe & 0x01U; }
    RegData GetOVE (void) const { return m_bit.m_ove; }
    RegData GetFE (void) const { return m_bit.m_fe; }
    RegData GetPE (void) const { return m_bit.m_pe; }

    void Reset (void) { m_data.m_data8 = 0x00U; }

    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    const char* GetName (void) const { return "ASIS"; }

    TRegASIS (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
              uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (ASIS_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief TXB register class. : TXBレジスタクラス
 */
class TRegTXB : public TRegUartBase {
private:
    TRegData8 m_data;

public:
    static const uint32_t TXB_MASK = 0x000000FFU;

    void SetTXB (RegData txb) { m_data.m_data8 = txb & 0xFFU; }
    RegData GetTXB (void) const { return m_data.m_data8; }

    void Reset (void) { m_data.m_data8 = 0xFFU; }

    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);

    const char* GetName (void) const { return "TXB"; }

    TRegTXB (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
             uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (TXB_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief ASIF register class. : ASIFレジスタクラス
 */
class TRegASIF : public TRegUartBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_txsf :1;
            RegData m_txbf :1;
            RegData        :6;
        } m_bit;
    };

public:
    static const uint32_t ASIF_MASK = 0x00000000U;

    void SetTXSF (RegData txsf) { m_bit.m_txsf = txsf & 0x01U; }
    void SetTXBF (RegData txbf) { m_bit.m_txbf = txbf & 0x01U; }
    RegData GetTXSF (void) const { return m_bit.m_txsf; }
    RegData GetTXBF (void) const { return m_bit.m_txbf; }

    void Reset (void) { m_data.m_data8 = 0x00U; }

    const char* GetName (void) const { return "ASIS"; }

    TRegASIF (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
              uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (ASIF_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief CKSR register class. : CKSRレジスタクラス
 */
class TRegCKSR : public TRegUartBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_tps :4;
            RegData       :4;
        } m_bit;
    };

public:
    static const uint32_t CKSR_MASK = 0x0000000FU;

    void SetTPS (RegData tps) { m_bit.m_tps = tps & 0x0FU; }
    RegData GetTPS (void) const { return m_bit.m_tps; }

    void Reset (void) { m_data.m_data8 = 0x00U; }

    const char* GetName (void) const { return "CKSR"; }

    TRegCKSR (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
              uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (CKSR_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief BRGC register class. : BRGCレジスタクラス
 */
class TRegBRGC : public TRegUartBase {
private:
    TRegData8 m_data;

public:
    static const uint32_t BRGC_MASK = 0x000000FFU;

    void SetBRGC (RegData brgc) { m_data.m_data8 = brgc & 0xFFU; }
    RegData GetBRGC (void) const { return m_data.m_data8; }

    void Reset (void) { m_data.m_data8 = 0xFFU; }

    const char* GetName (void) const { return "BRGC"; }

    TRegBRGC (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
              uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (BRGC_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief RTS register class. : RTSレジスタクラス
 */
class TRegRTS : public TRegUartBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_rts :1;
            RegData       :7;
        } m_bit;
    };

public:
    static const uint32_t RTS_MASK = 0x00000001U;

    void SetRTS (RegData rts) { m_bit.m_rts = rts & 0x01U; }
    RegData GetRTS (void) const { return m_bit.m_rts; }

    void Reset (void) { m_data.m_data8 = 0x00U; }

    const char* GetName (void) const { return "RTS"; }

    TRegRTS (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
             uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (RTS_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief CTS register class. : CTSレジスタクラス
 */
class TRegCTS : public TRegUartBase {
private:
    union {
        TRegData8 m_data;
        struct {
            RegData m_cts :1;
            RegData       :7;
        } m_bit;
    };

public:
    static const uint32_t CTS_MASK = 0x00000000U;

    void SetCTS (RegData cts) { m_bit.m_cts = cts & 0x01U; }
    RegData GetCTS (void) const { return m_bit.m_cts; }

    void Reset (void) { m_data.m_data8 = 0x00U; }

    const char* GetName (void) const { return "CTS"; }

    TRegCTS (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr,
             uint32_t chid, Uart* uart)
    : TRegUartBase (reg_size, mini_access_size, reg_addr, TRegMask (CTS_MASK), chid, uart) {
        m_treg = &m_data;
    }
};


/*!
  @brief UART register class. : UARTレジスタクラス
 */
class TRegUart : public TRegModuleVc {
public:
    explicit TRegUart (Uart* uart) : m_uart (uart) { InitTRegMap (); }
    ~TRegUart () { DelTRegMap (); }

    static const PhysAddr ASIM_OFFSET = 0x0000;
    static const PhysAddr RXB_OFFSET  = 0x0004;
    static const PhysAddr ASIS_OFFSET = 0x0008;
    static const PhysAddr TXB_OFFSET  = 0x000C;
    static const PhysAddr ASIF_OFFSET = 0x0010;
    static const PhysAddr CKSR_OFFSET = 0x0014;
    static const PhysAddr BRGC_OFFSET = 0x0018;
    static const PhysAddr RTS_OFFSET  = 0x001C;
    static const PhysAddr CTS_OFFSET  = 0x0020;

private:
    Uart* m_uart;
    void InitTRegMap (void);
};


/*!
  @brief UART class. : UARTクラス
 */
class Uart : public CModuleVc {
public:
    // ソケット制御
    static const uint32_t DEFAULT_SCAN_COUNT        = 30000;
    static const uint32_t DEFAULT_TRANSMIT_COUNT    = 30000;
    static const uint32_t SEQUENTIAL_SCAN_COUNT     = 3000;
    static const uint32_t SEQUENTIAL_TRANSMIT_COUNT = 3000;
    static const int32_t UART_UNUSED_SOCKET         = -1;
    static const uint32_t N_CH_UART = 8;

    static const uint32_t INTSR  = 96;  // UART0受信完了割込み チャネル番号
    static const uint32_t INTSRE = 104; // UART0受信エラー割込み チャネル番号
    static const uint32_t INTST  = 112; // UART0送信完了割込み チャネル番号

    static const PhysAddr UART_AREA_SIZE = 0x000001000ULL;

    FsBus* m_fsbus;

    Uart (FsBus* fsbus, CIntc2* intc2, PhysAddr base_addr);
    ~Uart ();

    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause IsAccPermited (SideBand sideband, PhysAddr addr, ByteSize size,
                               MemAccessCommand cmd) { return SYSERR_NONE; }

    void HardReset (void);
    void CyclicHandler (void);
    PhysAddr GetUartAddr (void) const { return m_uart_base_addr; }

    UartChannel* GetUartChannel (uint32_t chid) { return m_ch_all[chid]; }

    TRegASIM* GetASIMPtr (uint32_t chid) const;
    TRegRXB*  GetRXBPtr  (uint32_t chid) const;
    TRegASIS* GetASISPtr (uint32_t chid) const;
    TRegTXB*  GetTXBPtr  (uint32_t chid) const;
    TRegASIF* GetASIFPtr (uint32_t chid) const;
    void ConnectSocket (uint16_t port, int32_t num);
private:
    CIntc2*  m_intc2;
    PhysAddr m_uart_base_addr;
    TRegModuleVc* m_trfs;

    uint32_t m_scan_counter;
    uint32_t m_transmit_counter;
    int32_t  m_nfds;

    typedef std::vector<UartChannel*> UartChannelVec;
    UartChannelVec m_ch_all;
    UartChannelVec m_ch_active;

    void UartInit (void);
    void CreateChannel (void);
    void DeleteChannel (void);
    void ReceiveSerial (void);
    void TransmitSerial (void);
    void ReceiveData (UartChannel* u_ch, uint8_t data);
    void TransmitData (UartChannel* u_ch);
    void ReqReceiveIntToIntc2 (UartChannel* u_ch);
    void ReqTransmitIntToIntc2 (UartChannel* u_ch);
    void UpdateChannelVector (void);
    void UpdateNFDS (void);
    //void ConnectSocket (int port, int32_t num);
    void DisconnectSocket (void);
};


/*!
  @brief UART channel class. : UARTチャネルクラス
 */
class UartChannel {
public:
    UartChannel (uint32_t chid, CIntc2* intc2, Uart* uart);
    ~UartChannel () {}

    void CloseSocketByPeer (void);
    void RegInitReceive (void);
    void RegInitTransmit (void);
    void RegInit (void);

    TRegASIM* TrASIM (void) const { return m_asim; }
    TRegRXB*  TrRXB (void)  const { return m_rxb; }
    TRegASIS* TrASIS (void) const { return m_asis; }
    TRegTXB*  TrTXB (void)  const { return m_txb; }
    TRegASIF* TrASIF (void) const { return m_asif; }

    void SetCHID (uint32_t chid) { m_chid = chid; }
    uint32_t GetCHID (void) const { return m_chid; }

    void SetTXSF (uint32_t txsf) { m_txsf = txsf; }
    uint32_t GetTXSF (void) const { return m_txsf; }

    void SetTransMask (uint8_t mask) { m_trans_data_mask = mask; }
    uint8_t GetTransMask (void) const { return m_trans_data_mask; }

    void SetClientSockfd (int32_t sockfd) { m_client_sockfd = sockfd; }
    int32_t GetClientSockfd (void) const { return m_client_sockfd; }

    void SetIsTransEnable (bool enable) { m_transmit_enable = enable; }
    bool IsTransEnable (void) const { return m_transmit_enable; }

    void SetIsReceiveEnable (bool enable) { m_receive_enable = enable; }
    bool IsReceiveEnable (void) const { return m_receive_enable; }

private:
    TRegASIM* m_asim;
    TRegRXB*  m_rxb;
    TRegASIS* m_asis;
    TRegTXB*  m_txb;
    TRegASIF* m_asif;
    uint32_t  m_txsf; // TXB内部バッファ

    uint32_t m_chid;
    CIntc2*  m_intc2;
    uint8_t  m_trans_data_mask;
    int32_t  m_client_sockfd;

    bool m_transmit_enable;
    bool m_receive_enable;
};
