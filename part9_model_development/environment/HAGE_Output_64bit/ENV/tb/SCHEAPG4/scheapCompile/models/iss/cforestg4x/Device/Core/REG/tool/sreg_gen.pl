#!/usr/bin/perl

# (c) 2011,2012,2013,2017 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.
use Getopt::Long;
use IO::Handle;

my $bom          = "\x{feff}";    #BOM
my $opt_with_bom = 0;
my $opt_crlf     = 0;

GetOptions('bom|withbom' => \$opt_with_bom, 'crlf' => \$opt_crlf);

open(INPUT_REG, "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";
open(INPUT_BIT, "<$ARGV[1]") || die "Can't open $ARGV[1]: $!\n";
open(OUTPUT_H, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[2]")
  || die "Can't open $ARGV[2]: $!\n";
open(OUTPUT_MOCK, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[3]")
  || die "Can't open $ARGV[3]: $!\n";

# システムレジスタ情報ファイルを入力
# To get PRIV anb Copro info.
my %srnames = ();
while (<INPUT_REG>)
{

    # Skip comment lines and empty lines.
    if (($_ =~ /^[ |\t]*\#/) or ($_ =~ /^[ |\t]*\n/))
    {
        next;
    }

    chomp;

    # selid,regid,reg_class_name,reg_name,res_type,priv,id,bankid,base
    my ($group, $id, $class, $name, $res_type, $priv, $lid, $bankid, $base) =
      split(/,/, $_, 9);

    # Hash by Class name as key
    $srnames{$class} = [$res_type, $priv, $lid, $bankid, $base, $group];
}

my $SR_RESTYPE = 0;
my $SR_PRIV    = 1;
my $SR_ID      = 2;
my $SR_BANKID  = 3;
my $SR_BASE    = 4;
my $SR_GROUP   = 5;

close(INPUT_REG);

# システムレジスタビット情報ファイルを入力
my %srbits = ();
while (<INPUT_BIT>)
{

    # Skip comment lines and empty lines.
    if (($_ =~ /^[ |\t]*\#/) or ($_ =~ /^[ |\t]*\n/))
    {
        next;
    }

    chomp;
    $line = $_;

    # reg_class_name,bit_name,pos,width,rd,wr,init
    my ($sreg, $bit, $pos, $width, $attr_rd, $attr_wr, $init) =
      split(/,/, $_, 8);

    # Get copro info from register info
    $virtualres = 0;
    $res_type   = $srnames{$sreg}[$SR_RESTYPE];
    if ($res_type =~ /F/)
    {
        $copro = "FPU";
    }
    elsif ($res_type =~ /S/)
    {
        $copro = "SIMD";
    }
    elsif ($res_type =~ /V/)
    {
        $copro      = "-";
        $virtualres = 1;
    }
    elsif ($res_type =~ /-/)
    {
        $copro = "-";
    }
    else
    {
        die "Unknown resource info in $ARGV[0] for $line at $.\n";
    }

    # Get privilege info from register info
    $pr = $srnames{$sreg}[$SR_PRIV];
    if ($pr =~ /D/)
    {
        if ($virtualres == 1)
        {
            $priv = "DBHV";
        }
        else
        {
            $priv = "DBSV";
        }
    }
    elsif ($pr =~ /H/)
    {
        $priv = "HV";
    }
    elsif ($pr =~ /S/)
    {
        $priv = "SV";
    }
    elsif ($pr =~ /U/)
    {
        $priv = "UM";
    }
    else
    {
        die "Unknown priv info in $ARGV[0] for $line at $.\n";
    }

    $base  = $srnames{$sreg}[$SR_BASE];
    $group = $srnames{$sreg}[$SR_GROUP];
    $bankid = $srnames{$sreg}[$SR_BANKID];

    # print $sreg . ":" . $base . ":" . $group . "\n";
    # システムレジスタ名とビット名をキーとした多次元連想配列
    $srbits{$sreg}{$bit} = [
                            $pos,   $width, $attr_rd, $attr_wr,
                            $init,  $priv,  $copro,   $base,
                            $group, $virtualres, $bankid
                           ];
}

# システムレジスタビット連想配列の添え字
my $POS       = 0;
my $WIDTH     = 1;
my $ATTR_RD   = 2;
my $ATTR_WR   = 3;
my $INIT      = 4;
my $PRIV      = 5;
my $COPRO     = 6;
my $BASE_NAME = 7;
my $GROUP_ID  = 8;
my $IS_VRES   = 9;
my $BANK_ID   = 10;

close(INPUT_BIT);

select(OUTPUT_H);

# プロローグ
{
    printf ${bom} if $opt_with_bom;
    print "/*
 * (c) 2011,2012,2013,2017 Renesas Electronics Corporation RENESAS
 * ELECTRONICS CONFIDENTIAL AND PROPRIETARY This program must be
 * used solely for the purpose for which it was furnished by
 * Renesas Electronics Corporation. No part of this program may be
 * reproduced or disclosed to others, in any form, without the;
 * prior written permission of Renesas Electronics Corporation.
 */

#pragma once

#include <string>
#include <sstream>
#include \"forest_message.h\"
#include \"sreg_vm_base.h\"
#include \"proc_element_base.h\"

";
}

# システムレジスタ インターフェースクラス宣言
foreach $sk (sort keys %srbits)
{
    my @bits = sort { $srbits{$sk}{$b}[$POS] <=> $srbits{$sk}{$a}[$POS] }
      keys %{$srbits{$sk}};
    $base_name = $srbits{$sk}{$bits[0]}[$BASE_NAME];
    if ($base_name eq "-")
    {
        $base_class = "SRegBasicImpl";
    }
    else
    {
        $base_class = "SRegBase" . $base_name;
    }

    # Class name of Host registers have prefix "HM". To prevent virtual register name.
    $group = $srbits{$sk}{$bits[0]}[$GROUP_ID];
    if (($base_name ne "-") and ($group != 9))
    {
        $reg_prefix = "HM";
    }
    else
    {
        $reg_prefix = "";
    }

    # check virtualized function
    my $vartualres = 0;
    if ($srbits{$sk}{$bits[0]}[$IS_VRES] == 1)
    {

        # Valid if HVCFG.HEV=1.
        # If invalid, treated as undefined
        $vartualres = 1;
    }

    print "/*!
  \@brief $sk register interface class
 */
class SReg${reg_prefix}${sk}_IF {
";

    my $has_id = 0;

    # bit variables
    foreach $b (@bits)
    {
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 0)
        {
            $has_id = 1;    # ビット幅0は CForest内部ID
        }
    }

    print "public:\n";

    # bit access functions
    my $wr_sp = 0;
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1)
        {
            $argtype = "bool";
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 64)
        {
            $argtype = "RegData64";
        }
        else
        {
            $argtype = "RegData";
        }
        $attr_wr = $srbits{$sk}{$b}[$ATTR_WR];
        if ($attr_wr =~ m/T/)
        {
            $wr_sp = 1
              ; # レジスタライト時に特殊処理が必要なレジスタ
        }
        print "  virtual void Set${b}(${argtype} ${l_b}) = 0;\n";
    }
    my $rd_sp = 0;
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1)
        {
            $argtype = "bool";
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 64)
        {
            $argtype = "RegData64";
        }
        else
        {
            $argtype = "RegData";
        }
        $attr_rd = $srbits{$sk}{$b}[$ATTR_RD];
        if ($attr_rd =~ m/T/)
        {

            # レジスタリード時に特殊処理が必要なレジスタ
            $rd_sp = 1;
        }
        print "  virtual ${argtype} Get${b}(void) const = 0;\n";
    }

    # reset functions
    my $init_sp = 0;
    foreach $b (@bits)
    {
        $init = $srbits{$sk}{$b}[$INIT];
        if ($init =~ m/S/)
        {
            $init_sp = 1
              ; # 特別な初期化処理が必要なレジスタ: 自動生成対象外
        }
    }
    print "  virtual void Reset(void) = 0;\n";

    # write functions
    print
      "  virtual void Write(RegData data, PrivLevel priv, ContextId ctxt) = 0;\n";

    # read functions
    print "  virtual RegData Read(PrivLevel priv, ContextId ctxt) const = 0;\n";

    foreach $b (@bits)
    {

        # privilege level
        $pr = $srbits{$sk}{$b}[$PRIV];
        if ($pr =~ /DBHV/)
        {
            $priv_level = "PRIV_PERMIT_DBHV";
            $pie_level  = "PRIV_PIE_DBHV";
        }
        elsif ($pr =~ /DBSV/)
        {
            $priv_level = "PRIV_PERMIT_DBSV";
            $pie_level  = "PRIV_PIE_DBSV";
        }
        elsif ($pr =~ /DBUM/)
        {
            $priv_level = "PRIV_PERMIT_DBUM";
            $pie_level  = "PRIV_PIE_DBUM";
        }
        elsif ($pr =~ /HV/)
        {
            $priv_level = "PRIV_PERMIT_HV";
            $pie_level  = "PRIV_PIE_HV";
        }
        elsif ($pr =~ /SV/)
        {
            $priv_level = "PRIV_PERMIT_SV";
            $pie_level  = "PRIV_PIE_SV";
        }
        elsif ($pr =~ /UM/)
        {
            $priv_level = "PRIV_PERMIT_UM";
            $pie_level  = "PRIV_PIE_UM";
        }

        # copro mode
        $cp = $srbits{$sk}{$b}[$COPRO];
        if ($cp =~ /FPU/)
        {
            $copro_type = "COPRO_FPU";
        }
        elsif ($cp =~ /SIMD/)
        {
            $copro_type = "COPRO_SIMD";
        }
        else
        {
            $copro_type = "COPRO_NO";
        }
    }

    # コプロタイプ
    if ($copro_type ne "COPRO_NO")
    {
        print "  virtual CoproType GetCoproType(void) const = 0;\n";
    }

    # システムレジスタ名
    print "  virtual std::string GetName(void) const = 0;\n";

    # System register ID
    if ($has_id == 1)
    {
        if ($sk =~ /_N$/)
        {
            $reg_name = $`;
        }
        else
        {
            $reg_name = $sk;
        }
        if (($sk =~ /PMCTRL/) or ($sk =~ /PMCOUNT/))
        {
            print "  virtual CfSReg GetCfSRegId(void) const = 0;\n";
        }
        else
        {
            print "  virtual CfSReg GetCfSRegId(void) const = 0;\n";
        }
    }
    else
    {
        print "  virtual CfSReg GetCfSRegId(void) const = 0;\n";
    }

    # Destructor
    print "  virtual ~SReg${reg_prefix}${sk}_IF() {}\n";
    print "};\n";
    print "\n";
}

# システムレジスタ クラス宣言
foreach $sk (sort keys %srbits)
{
    my @bits = sort { $srbits{$sk}{$b}[$POS] <=> $srbits{$sk}{$a}[$POS] }
      keys %{$srbits{$sk}};
    $base_name = $srbits{$sk}{$bits[0]}[$BASE_NAME];
    if ($base_name eq "-")
    {
        $base_class = "SRegBasicImpl";
    }
    else
    {
        $base_class = "SRegBase" . $base_name;
    }

    # Class name of Host registers have prefix "HM". To prevent virtual register name.
    $group = $srbits{$sk}{$bits[0]}[$GROUP_ID];
    if (($base_name ne "-") and ($group != 9))
    {
        $reg_prefix = "HM";
    }
    else
    {
        $reg_prefix = "";
    }

    # check virtualized function
    my $vartualres = 0;
    if ($srbits{$sk}{$bits[0]}[$IS_VRES] == 1)
    {
        $vartualres =
          1;    # Valid if HVCFG.HEV=1. If invalid, treated as undefined
    }

    print "/*!
  \@brief $sk register class
 */
class SReg${reg_prefix}${sk} : public ${base_class}, SReg${reg_prefix}${sk}_IF {
private:
";

    my $has_id = 0;

    # bit variables
    foreach $b (@bits)
    {
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 0)
        {
            $has_id = 1;    # ビット幅0は CForest内部ID
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 1)
        {
            print "  bool    m_${l_b};\n";
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 64)
        {
            print "  RegData64 m_${l_b};\n";
        }
        else
        {
            print "  RegData m_${l_b};\n";
        }
    }

    my $has_bankid = 0;

    # Check existance of bankid from bit variables
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$BANK_ID] ne "-")
        {
            $has_bankid = 1;
        }
    }

    # CForest variables
    if ($has_id == 1)
    {
        print "  uint32_t m_id;\n";
    }
    if ($has_bankid == 1)
    {
        print "  uint32_t m_bank_id;\n";
    }
    print "\n";

    print "public:\n";

    # bit access functions
    my $wr_sp = 0;
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1)
        {
            $argtype = "bool";
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 64)
        {
            $argtype = "RegData64";
        }
        else
        {
            $argtype = "RegData";
        }
        $attr_wr = $srbits{$sk}{$b}[$ATTR_WR];
        if ($attr_wr =~ m/T/)
        {
            $wr_sp = 1
              ; # レジスタライト時に特殊処理が必要なレジスタ
        }
        if ($attr_wr =~ m/S/)
        {
            print "  void Set${b}(${argtype} ${l_b});\n"
              ;    # 特別な書込み処理が必要なレジスタ
        }
        else
        {
            print "  void Set${b}(${argtype} ${l_b}) { m_${l_b} = ${l_b}; }\n";
        }
    }
    my $rd_sp = 0;
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1)
        {
            $argtype = "bool";
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 64)
        {
            $argtype = "RegData64";
        }
        else
        {
            $argtype = "RegData";
        }
        $attr_rd = $srbits{$sk}{$b}[$ATTR_RD];
        if ($attr_rd =~ m/T/)
        {

            $rd_sp = 1
              ; # レジスタリード時に特殊処理が必要なレジスタ

        }
        if ($attr_rd =~ m/S/)
        {
            print "  ${argtype} Get${b}(void) const;\n"
              ;    # 特別な読込み処理が必要なレジスタ
        }
        else
        {
            print "  ${argtype} Get${b}(void) const { return m_${l_b}; }\n";
        }
    }
    print "\n";

    # reset functions
    my $init_sp = 0;
    foreach $b (@bits)
    {
        $init = $srbits{$sk}{$b}[$INIT];
        if ($init =~ m/S/)
        {
            $init_sp = 1
              ; # 特別な初期化処理が必要なレジスタ: 自動生成対象外
        }
    }
    if ($init_sp == 1)
    {
        print "  void Reset(void);\n";
    }
    else
    {
        print "  void Reset(void) {\n";
        foreach $b (@bits)
        {
            if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
            my $l_b  = lc $b;
            my $init = $srbits{$sk}{$b}[$INIT];
            if ($init !~ m/-/)
            {
                print "     m_${l_b} = 0x${init}U;\n";
            }
        }
        print "  }\n";
    }
    print "\n";

    # write functions
    print "  void Write(RegData data, PrivLevel priv, ContextId ctxt)";
    if ($wr_sp == 1)
    {
        print ";\n";
    }
    else
    {

        if ($vartualres == 0)
        {
            print " {
        if (IsPrivileged(priv) == false) return;\n";
        }
        else
        {
            print " {
        if (m_pe->IsHvEnabled() == false) return;\n";
        }

        foreach $b (@bits)
        {
            if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
            $attr_wr = $srbits{$sk}{$b}[$ATTR_WR];
            $mask =
              sprintf("RegData (0x%08xU)", 2**$srbits{$sk}{$b}[$WIDTH] - 1);
            if ($attr_wr =~ m/W/)
            {
                print
                  "     Set${b} ((data >> $srbits{$sk}{$b}[$POS]) & $mask);\n";
            }
        }
        print "  }\n";
    }
    print "\n";

    # read functions
    print "  RegData Read(PrivLevel priv, ContextId ctxt) const";
    if ($rd_sp == 1)
    {
        print ";\n";
    }
    else
    {
        if ($vartualres == 0)
        {
            print " {
        if (IsPrivileged (priv) == false) return 0;\n";
        }
        else
        {
            print " {
        if (m_pe->IsHvEnabled () == false) return 0;\n";
        }
        print "     RegData ret = 0x0";
        foreach $b (@bits)
        {
            if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
            $attr_rd = $srbits{$sk}{$b}[$ATTR_RD];
            if ($attr_rd =~ m/R/)
            {
                print "\n            | (Get${b} () << $srbits{$sk}{$b}[$POS])";
            }
        }
        print ";\n";
        print "     return ret;\n";
        print "  }\n";
    }
    print "\n";

    foreach $b (@bits)
    {

        # privilege level
        $pr = $srbits{$sk}{$b}[$PRIV];
        if ($pr =~ /DBHV/)
        {
            $priv_level = "PRIV_PERMIT_DBHV";
            $pie_level  = "PRIV_PIE_DBHV";
        }
        elsif ($pr =~ /DBSV/)
        {
            $priv_level = "PRIV_PERMIT_DBSV";
            $pie_level  = "PRIV_PIE_DBSV";
        }
        elsif ($pr =~ /DBUM/)
        {
            $priv_level = "PRIV_PERMIT_DBUM";
            $pie_level  = "PRIV_PIE_DBUM";
        }
        elsif ($pr =~ /HV/)
        {
            $priv_level = "PRIV_PERMIT_HV";
            $pie_level  = "PRIV_PIE_HV";
        }
        elsif ($pr =~ /SV/)
        {
            $priv_level = "PRIV_PERMIT_SV";
            $pie_level  = "PRIV_PIE_SV";
        }
        elsif ($pr =~ /UM/)
        {
            $priv_level = "PRIV_PERMIT_UM";
            $pie_level  = "PRIV_PIE_UM";
        }

        # copro mode
        $cp = $srbits{$sk}{$b}[$COPRO];
        if ($cp =~ /FPU/)
        {
            $copro_type = "COPRO_FPU";
        }
        elsif ($cp =~ /SIMD/)
        {
            $copro_type = "COPRO_SIMD";
        }
        else
        {
            $copro_type = "COPRO_NO";
        }
    }

    # コプロタイプ
    if ($copro_type ne "COPRO_NO")
    {
        print
          "  CoproType GetCoproType (void) const { return ${copro_type}; }\n";
    }

    # システムレジスタ名
    if ($has_id == 1)
    {
        if ($sk =~ /_N$/)
        {
            $reg_name = $`;
        }
        else
        {
            $reg_name = $sk;
        }

        print "  std::string GetName(void) const {\n";
        print "     std::ostringstream oss;\n";
        print "     oss << \"${reg_name}\" << m_id;\n";
        if ($has_bankid == 1) {
        print "     if (m_bank_id != 0) {\n";
        print "         oss << \"_B\" << m_bank_id;\n";
        print "     }\n";
        }
        print "     return oss.str();\n";
        print "  }\n";
    }
    else
    {
        print "  std::string GetName(void) const { return \"${sk}\"; }\n";
    }
    print "\n";

    # System register ID
    if ($has_id == 1)
    {
        if ($sk =~ /_N$/)
        {
            $reg_name = $`;
        }
        else
        {
            $reg_name = $sk;
        }
        if (($sk =~ /PMCTRL/) or ($sk =~ /PMCOUNT/))
        {
            print
              "  CfSReg GetCfSRegId(void) const { return (m_pe->GetCoreVersion() >= CORE_VER_20)\n";
            print
              "                                                ? static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_${reg_name}0_G4MH20) + m_id)\n";
            print
              "                                                : static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_${reg_name}0_G4X1X) + (m_id << 1)); }\n";
        }
        elsif ($has_bankid == 1)
        {
            print "  CfSReg GetCfSRegId(void) const {\n";
            print "      if (m_bank_id == 0) {\n";
            print "          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_${reg_name}0) + m_id);\n";
            print "      } else if (m_bank_id == 1) {\n";
            print "          return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_${reg_name}0_B1) + m_id);\n";
            print "      } else {\n";
            print "          printf (\"<Internal Error: Unsupported bankid in ${reg_name}>\");\n";
            print "          return CF_SR_DUMMY;\n";
            print "      }\n";
            print "  }\n";
        }
        else
        {
            print
              "  CfSReg GetCfSRegId(void) const { return static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_${reg_name}0) + m_id); }\n";
        }
    }
    else
    {
        print
          "  CfSReg GetCfSRegId(void) const { return CF_SR_${reg_prefix}${sk}; }\n";
    }
    print "\n";

    # Constructor
    if ($has_id == 1)
    {
        if ($has_bankid == 1)
        {
            print
              "  SReg${reg_prefix}${sk}(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) :\n";
        }
        else
        {
            print
              "  SReg${reg_prefix}${sk}(ContextId ctid, ProcElementBase* pe, uint32_t id) :\n";
        }
    }
    else
    {
        print
          "  SReg${reg_prefix}${sk}(ContextId ctid, ProcElementBase* pe) :\n";
    }
    print "     ${base_class} (ctid, ${priv_level}, ${pie_level}, pe)";
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        print ",\n        m_${l_b} (0)";
    }
    if ($has_id == 1)
    {
        print ",\n        m_id (id)";
    }
    if ($has_bankid == 1)
    {
        print ",\n        m_bank_id (bank_id)";
    }
    print " {}\n";
    print "};\n";
    print "\n";
}

# エピローグ
{
    print "/*!
  @brief DUMMY class
 */
class SRegDUMMY : public SRegBasicImpl {
public:
  void Reset(void) {}
  void Write(RegData data, PrivLevel priv, ContextId ctxt) {
    CfMsg::DPrint (MSG_WAR,
                   \"--:--:--:-: <Warning: invalid SR access : write ignored>\\n\");
  }
  RegData Read(PrivLevel priv, ContextId ctxt) const {
    CfMsg::DPrint (MSG_WAR,
                   \"--:--:--:-: <Warning: invalid SR access : read 0x0>\\n\");
    return 0x0;
  }
  std::string GetName(void) const { return \"invalid SR\"; }
  CfSReg GetCfSRegId(void) const { return CF_SR_DUMMY; };
  SRegDUMMY() : SRegBasicImpl(0, PRIV_PERMIT_SV, PRIV_PIE_SV, NULL) {}
};
";

    print "/*!
  @brief DUMMY class (Cause PIE always for direct access to GMxxx registers on Guest mode)
 */
class SRegDUMMYHV : public SRegBasicImpl {
public:
  void Reset (void) {}
  void Write (RegData data, PrivLevel priv, ContextId ctxt);
  RegData Read (PrivLevel priv, ContextId ctxt) const;
  std::string GetName (void) const { return \"GMxxx SR\"; }
  CfSReg GetCfSRegId (void) const { return CF_SR_DUMMY; };
  explicit SRegDUMMYHV (ProcElementBase* pe) : SRegBasicImpl (0, PRIV_PERMIT_HV, PRIV_PIE_HV, pe) {}
};
";
}
IO::Handle::flush(OUTPUT_H);
eval { IO::Handle::sync(OUTPUT_H); };
close(OUTPUT_H);

select(OUTPUT_MOCK);

# プロローグ
{
    printf ${bom} if $opt_with_bom;
    print "/*
 * (c) 2011,2012,2013,2017 Renesas Electronics Corporation RENESAS
 * ELECTRONICS CONFIDENTIAL AND PROPRIETARY This program must be
 * used solely for the purpose for which it was furnished by
 * Renesas Electronics Corporation. No part of this program may be
 * reproduced or disclosed to others, in any form, without the;
 * prior written permission of Renesas Electronics Corporation.
 */

#pragma once

#include <string>
#include <sstream>

#include \"sreg.h\"
#include \"sreg_vm_base.h\"
#include \"proc_element_base.h\"
#include \"gmock/gmock.h\"
";
}

# システムレジスタ MOCKクラス宣言
foreach $sk (sort keys %srbits)
{
    my @bits = sort { $srbits{$sk}{$b}[$POS] <=> $srbits{$sk}{$a}[$POS] }
      keys %{$srbits{$sk}};
    $base_name = $srbits{$sk}{$bits[0]}[$BASE_NAME];
    if ($base_name eq "-")
    {
        $base_class = "SRegBasicImpl";
    }
    else
    {
        $base_class = "SRegBase" . $base_name;
    }

    # Class name of Host registers have prefix "HM". To prevent virtual register name.
    $group = $srbits{$sk}{$bits[0]}[$GROUP_ID];
    if (($base_name ne "-") and ($group != 9))
    {
        $reg_prefix = "HM";
    }
    else
    {
        $reg_prefix = "";
    }

    # check virtualized function
    my $vartualres = 0;
    if ($srbits{$sk}{$bits[0]}[$IS_VRES] == 1)
    {
        $vartualres =
          1;    # Valid if HVCFG.HEV=1. If invalid, treated as undefined
    }

    print "
/*!
  \@brief $sk register mock class
 */
class MockSReg${reg_prefix}${sk} : public SReg${reg_prefix}${sk} {
";

    my $has_id = 0;

    # bit variables
    foreach $b (@bits)
    {
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 0)
        {
            $has_id = 1;    # ビット幅0は CForest内部ID
        }
    }

    my $has_bankid = 0;

    # Check existance of bankid from bit variables
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$BANK_ID] ne "-")
        {
            $has_bankid = 1;
        }
    }

    # CForest variables
    if ($has_id == 1)
    {
        print "  uint32_t m_id;\n";
    }
    if ($has_bankid == 1)
    {
        print "  uint32_t m_bank_id;\n";
    }
    print "\n";

    print "public:\n";

    # bit access functions
    my $wr_sp = 0;
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1)
        {
            $argtype = "bool";
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 64)
        {
            $argtype = "RegData64";
        }
        else
        {
            $argtype = "RegData";
        }
        $attr_wr = $srbits{$sk}{$b}[$ATTR_WR];
        if ($attr_wr =~ m/T/)
        {
            $wr_sp = 1
              ; # レジスタライト時に特殊処理が必要なレジスタ
        }
        printf("  MOCK_METHOD1(Set${b}, void(${argtype} ${l_b}));\n");
    }
    my $rd_sp = 0;
    foreach $b (@bits)
    {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1)
        {
            $argtype = "bool";
        }
        elsif ($srbits{$sk}{$b}[$WIDTH] == 64)
        {
            $argtype = "RegData64";
        }
        else
        {
            $argtype = "RegData";
        }
        $attr_rd = $srbits{$sk}{$b}[$ATTR_RD];
        if ($attr_rd =~ m/T/)
        {

            $rd_sp = 1
              ; # レジスタリード時に特殊処理が必要なレジスタ

        }
        printf("  MOCK_CONST_METHOD0(Get${b}, ${argtype}(void));\n");
    }
    print "\n";

    # reset functions
    print "  MOCK_METHOD0(Reset, void(void));\n";
    print "\n";

    # write functions
    print
      "  MOCK_METHOD3(Write, void(RegData data, PrivLevel priv, ContextId ctxt));\n";
    print
      "  MOCK_METHOD1(WriteBody, void(RegData data));\n";

    # read functions
    print
      "  MOCK_CONST_METHOD2(Read, RegData(PrivLevel priv, ContextId ctxt));\n";
    print
      "  MOCK_CONST_METHOD0(ReadBody, RegData(void));\n";

    foreach $b (@bits)
    {

        # privilege level
        $pr = $srbits{$sk}{$b}[$PRIV];
        if ($pr =~ /DBHV/)
        {
            $priv_level = "PRIV_PERMIT_DBHV";
            $pie_level  = "PRIV_PIE_DBHV";
        }
        elsif ($pr =~ /DBSV/)
        {
            $priv_level = "PRIV_PERMIT_DBSV";
            $pie_level  = "PRIV_PIE_DBSV";
        }
        elsif ($pr =~ /DBUM/)
        {
            $priv_level = "PRIV_PERMIT_DBUM";
            $pie_level  = "PRIV_PIE_DBUM";
        }
        elsif ($pr =~ /HV/)
        {
            $priv_level = "PRIV_PERMIT_HV";
            $pie_level  = "PRIV_PIE_HV";
        }
        elsif ($pr =~ /SV/)
        {
            $priv_level = "PRIV_PERMIT_SV";
            $pie_level  = "PRIV_PIE_SV";
        }
        elsif ($pr =~ /UM/)
        {
            $priv_level = "PRIV_PERMIT_UM";
            $pie_level  = "PRIV_PIE_UM";
        }

        # copro mode
        $cp = $srbits{$sk}{$b}[$COPRO];
        if ($cp =~ /FPU/)
        {
            $copro_type = "COPRO_FPU";
        }
        elsif ($cp =~ /SIMD/)
        {
            $copro_type = "COPRO_SIMD";
        }
        else
        {
            $copro_type = "COPRO_NO";
        }
    }

    # コプロタイプ
    if ($copro_type ne "COPRO_NO")
    {
        print "  MOCK_METHOD0(GetCoproType, CoproType(void));\n";
    }

    # システムレジスタ名
    if ($has_id == 1)
    {
        if ($sk =~ /_N$/)
        {
            $reg_name = $`;
        }
        else
        {
            $reg_name = $sk;
        }

        print "  std::string GetName(void) const {\n";
        print "     std::ostringstream oss;\n";
        print "     oss << \"${reg_name}\" << m_id;\n";
        if ($has_bankid == 1) {
        print "     if (m_bank_id != 0) {\n";
        print "         oss << \"_B\" << m_bank_id;\n";
        print "     }\n";
        }
        print "     return oss.str();\n";
        print "  }\n";
    }
    else
    {
        print "  MOCK_CONST_METHOD0(GetName, std::string(void));\n";
    }
    print "\n";

    # System register ID
    if ($has_id == 1)
    {
        if ($sk =~ /_N$/)
        {
            $reg_name = $`;
        }
        else
        {
            $reg_name = $sk;
        }
        if (($sk =~ /PMCTRL/) or ($sk =~ /PMCOUNT/))
        {
            print "  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));\n";
        }
        else
        {
            print "  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));\n";
        }
    }
    else
    {
        print "  MOCK_CONST_METHOD0(GetCfSRegId, CfSReg(void));\n";
    }
    print "\n";

    if ($has_id == 1)
    {
        if ($has_bankid == 1)
        {
            print
              "  MockSReg${reg_prefix}${sk}(ContextId ctid, ProcElementBase* pe, uint32_t id, uint32_t bank_id) : SReg${reg_prefix}${sk}(ctid, pe, id, bank_id) {};\n";
        }
        else
        {
            print
              "  MockSReg${reg_prefix}${sk}(ContextId ctid, ProcElementBase* pe, uint32_t id) : SReg${reg_prefix}${sk}(ctid, pe, id) {};\n";
        }
    }
    else
    {
        print
          "  MockSReg${reg_prefix}${sk}(ContextId ctid, ProcElementBase* pe) : SReg${reg_prefix}${sk}(ctid, pe) {};\n";
    }
    print "};\n";

    #--------------------------
    # bit access functions
    my $wr_sp = 0;
    foreach $b (@bits) {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1) {
            $argtype = "bool";
        } elsif ($srbits{$sk}{$b}[$WIDTH] == 64) {
            $argtype = "RegData64";
        } else {
            $argtype = "RegData";
        }
        $attr_wr = $srbits{$sk}{$b}[$ATTR_WR];
        if ($attr_wr =~ m/T/) {
            # レジスタライト時に特殊処理が必要なレジスタ
            $wr_sp = 1;
        }
        if ($attr_wr =~ m/S/) {
            # 特別な書込み処理が必要なレジスタ
            printf ("void SReg${reg_prefix}${sk}::Set${b}(${argtype} ${l_b}) {};\n");
        }
    }
    my $rd_sp = 0;
    foreach $b (@bits) {
        if ($srbits{$sk}{$b}[$WIDTH] == 0) { next; }
        my $l_b = lc $b;
        if ($srbits{$sk}{$b}[$WIDTH] == 1) {
            $argtype = "bool";
        } elsif ($srbits{$sk}{$b}[$WIDTH] == 64) {
            $argtype = "RegData64";
        } else {
            $argtype = "RegData";
        }
        $attr_rd = $srbits{$sk}{$b}[$ATTR_RD];
        if ($attr_rd =~ m/T/) {
            # レジスタリード時に特殊処理が必要なレジスタ
            $rd_sp = 1;
        }
        if ($attr_rd =~ m/S/) {
            print "${argtype} SReg${reg_prefix}${sk}::Get${b}(void) const { return static_cast<${argtype}>(0); };\n"; # 特別な読込み処理が必要なレジスタ
        }
    }
    # reset functions
    my $init_sp = 0;
    foreach $b (@bits)
    {
        $init = $srbits{$sk}{$b}[$INIT];
        if ($init =~ m/S/)
        {
            $init_sp = 1
              ; # 特別な初期化処理が必要なレジスタ: 自動生成対象外
        }
    }    # reset functions
    if ($init_sp == 1) {
        print "void SReg${reg_prefix}${sk}::Reset(void) {};\n";
    }
    # write functions
    if ($wr_sp == 1) {
        print "void SReg${reg_prefix}${sk}::Write(RegData data, PrivLevel priv, ContextId ctxt) {};\n";
    }
    # read functions
    if ($rd_sp == 1) {
        print "RegData SReg${reg_prefix}${sk}::Read(PrivLevel priv, ContextId ctxt) const { return static_cast<RegData>(0); };\n";
    }
    print "\n";
}
IO::Handle::flush(OUTPUT_MOCK);
eval { IO::Handle::sync(OUTPUT_MOCK); };
close(OUTPUT_MOCK);
