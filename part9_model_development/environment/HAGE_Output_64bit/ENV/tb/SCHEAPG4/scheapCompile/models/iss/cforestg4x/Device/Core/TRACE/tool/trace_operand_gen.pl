#!/usr/bin/env perl

# (c) 2011,2012,2013 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.

use POSIX qw( ceil );
use Getopt::Long;
use IO::Handle;

my $bom = "\x{feff}"; #BOM
my $opt_with_bom = 0;
my $opt_crlf = 0;

GetOptions('bom|withbom' => \$opt_with_bom, 'crlf' => \$opt_crlf);

###############################################################
#
#  MicroArchDBの読み込み
#
###############################################################
open(INPUT, "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";

@line = ();
@inst = ();
@func = ();

$title = <INPUT>;
chomp ($title);
@line = split(/[\t]/, $title);
$c = 0;
while ($cname = shift (@line)) {
    $columnname{$cname} = $c;
    $c++;
}

#命令No 0 にはダミーを登録
push (@inst, 0);
push (@func, 0);

while (<INPUT>) {
    chomp ($_);
    @line = split(/\t/, $_);
    if ($_ =~ /^\s*$/) { next; }

    # 非公開命令はスキップ
    if ($line[$columnname{'Publicity'}] =~ m/H/) { next; }

    push (@inst, "$line[$columnname{'Operand Format'}]");
    push (@func, "$line[$columnname{'Library Function Name'}]");
}
close (INPUT);

###############################################################
#
#  オペランド表示用の置換元文字列を抽出
#
###############################################################
@inst_op = ();
foreach (@inst) {
    $inst_line = $_;
    while ($inst_line =~ s/([A-Z]+\[[0-9\.:]+\](<<[0-9]+)?(\+[0-9]+)?)//) {
        $inst_op_tmp = $1;
        $new_flag = 1;
        foreach (@inst_op) {
            if ("$_" eq "$inst_op_tmp") {
                $new_flag = 0;
                last;
            }
        }
        if ($new_flag == 1) {
            push (@inst_op, "$inst_op_tmp");
        }
    }
}
@inst_op = reverse sort @inst_op;

###############################################################
#
#  オペランド表示用の置換種類一覧のenumを生成
#
###############################################################
@operand_list = ();
@operand_enum = ();
$idx = 0;
push (@operand_enum, "    enum ReplaceType {");
foreach (@inst_op) {
    $operand = $_;
    $operand =~ s/<<(\d+)/_S$1/g;
    $operand =~ s/\+(\d+)/_P$1/g;
    $operand =~ s/[\[\.:]/_/g;
    $operand =~ s/\]//g;
    push (@operand_list, "$operand");
    push (@operand_enum, "        $operand,");
    $idx++;
}
push (@operand_enum, "\n        TOTAL_REPLACE_NUM");
push (@operand_enum, "    };\n");


###############################################################
#
#  TracePrintIFのコンストラクタ生成
#
###############################################################
@operand_info = ();
push (@operand_info, "TracePrintOperand::TracePrintOperand (void)");
push (@operand_info, "{");
foreach (@func) {
    $func_name = $_;
    $func_left  = "m_inst_func[INST_ID_V_${func_name}]";
    $func_right = "&TracePrintOperand::Conv${func_name};";

    $func_tmp = sprintf ("    %-35s = %s", $func_left, $func_right);
    push (@operand_info, "$func_tmp");
}
push (@operand_info, "");

@func_conv_head = ();
$idx = 0;
foreach (@operand_list) {
    $op_def = $_;
    $operand = $inst_op[$idx];
    if ($operand =~ /([A-Z]+)\[((?:\d+(?::\d+)?\.)?)((?:\d+(?::\d+)?\.)?)(\d+)((?::\d+)?)\]((?:<<\d+)?)((?:\+\d+)?)/) {
        $cate    = $1;
        $code_h  = $2;
        $code_m  = $3;
        $code_ll = $4;
        $code_lr = $5;
        $shift   = $6;
        $offset  = $7;

        # 中位のビットが存在するか
        if ($code_m =~ /(\d+)((?::\d+)?)\./) {
            $code_ml = $1;
            $code_mr = $2;
            # 中位のビットが存在し、かつ上位のビットが存在した場合
            if ($code_h =~ /(\d+)((?::\d+)?)\./) {
                $code_hl = $1;
                $code_hr = $2;
            } else {
                $code_hl = "";
                $code_hr = "";
            }
        } else {
            # 中位のビットが存在せず、上位のビットが存在した場合
            if ($code_h =~ /(\d+)((?::\d+)?)\./) {
                # 上位ビットを中位ビットとして認識させる
                $code_ml = $1;
                $code_mr = $2;
                # 上位ビットはなくなる
                $code_hl = "";
                $code_hr = "";
            } else {
                # 中位のビットも上位のビットもない
                # 下位のビットのみ
                $code_ml = "";
                $code_mr = "";
                $code_hl = "";
                $code_hr = "";
            }
        }

        $code_lr =~ s/://g;
        $code_mr =~ s/://g;
        $code_hr =~ s/://g;
        $shift   =~ s/<//g;
        $offset  =~ s/\+//g;

        if ($code_hl ne "") {
            $seg = "true";
        } else {
            $code_hl = -1;
            $seg = "false";
        }

        if ($code_ml ne "") {
            $mseg = "true";
        } else {
            $code_ml = -1;
            $mseg = "false";
        }

        if ($code_hr eq "") { $code_hr = $code_hl; }
        if ($code_mr eq "") { $code_mr = $code_ml; }
        if ($code_lr eq "") { $code_lr = $code_ll; }
        if ($shift eq "")   { $shift = 0; }
        if ($offset eq "")  { $offset = 0; }
        $sig = $code_ll - $code_lr + 1;
        if ($code_hl != -1) {
            $sig += ($code_hl - $code_hr + 1);
        }
        if ($code_ml != -1) {
            $sig += ($code_ml - $code_mr + 1);
        }
        $sig += $shift;
        $sig += ceil(log($offset + 1))
    } else {
        print "error: Illegal operation $operand\n";
        exit (1);
    }

    $info_tmp = sprintf ("    SetReplaceInfoValue ( %-20s, \"%s\",", $op_def, $operand);
    push (@operand_info, "$info_tmp");
    $info_tmp = sprintf ("                          %2d, %2d, %2d, %2d, %2d, %2d, %5s, %5s, %2d, %2d, %2d);"
                         , $code_hl, $code_hr, $code_ml, $code_mr, $code_ll, $code_lr, $seg, $mseg, $shift, $offset ,$sig);
    push (@operand_info, "$info_tmp");

    ## 置換関数生成用の配列を生成 ##
    if    ("$cate" eq "SR") {
        $func_conv_head_tmp = "    ReplaceSR   (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "VR") {
        $func_conv_head_tmp = "    ReplaceVReg (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "WR") {
        $func_conv_head_tmp = "    ReplaceWReg (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "R") {
        $func_conv_head_tmp = "    ReplaceReg  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "BD") {
        $func_conv_head_tmp = "    ReplaceBcond (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "D") {
        $func_conv_head_tmp = "    ReplaceDec  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "I") {
        $func_conv_head_tmp = "    ReplaceDec  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "E") {
        $func_conv_head_tmp = "    ReplaceEntry (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "L") {
        $func_conv_head_tmp = "    ReplaceList (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "SEL") {
        $func_conv_head_tmp = "    ReplaceSel  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "C") {
        $func_conv_head_tmp = "    ReplaceCnd  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "FCO") {
        $func_conv_head_tmp = "    ReplaceFCO  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "BRA") {
        $func_conv_head_tmp = "    ReplaceBra  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "W") {
        $func_conv_head_tmp = "    ReplaceWidth (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "P") {
        $func_conv_head_tmp = "    ReplacePos  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "CA") {
        $func_conv_head_tmp = "    ReplaceCacheOp  (inst_op, inst_code, inst_pc, ";
    } elsif ("$cate" eq "PO") {
        $func_conv_head_tmp = "    ReplacePrefOp  (inst_op, inst_code, inst_pc, ";
    } else {
        $func_conv_head_tmp = "    ReplaceHexU (inst_op, inst_code, inst_pc, ";
    }
    push (@func_conv_head, "$func_conv_head_tmp");

    $idx++;
}
push (@operand_info, "}");


###############################################################
#
#  置換関数の生成
#
###############################################################
@conv_func = ();
@func_prot = ();
@cmp_lst = ();

foreach (@inst_op) {
    $cmp = $_;
    $cmp =~ s/\[/\\\[/g;
    $cmp =~ s/\]/\\\]/g;
    $cmp =~ s/\./\\\./g;
    $cmp =~ s/\</\\\</g;
    $cmp =~ s/\+/\\\+/g;
    push (@cmp_lst, "$cmp");
}

$idx = 0;
foreach (@inst) {
    $inst_line = $_;
    $inst_func_tmp = sprintf ("void TracePrintOperand::Conv%s (std::string* inst_op, uint64_t inst_code,", $func[$idx]);
    push (@conv_func, "$inst_func_tmp");
    $inst_func_tmp = sprintf ("                                              uint32_t inst_pc) const");
    push (@conv_func, "$inst_func_tmp");
    push (@conv_func, "{");

    $inst_func_tmp = sprintf ("    void %-20s (std::string* inst_op, uint64_t inst_code,", "Conv${func[$idx]}");
    push (@func_prot, "$inst_func_tmp");
    $inst_func_tmp = sprintf ("                               uint32_t inst_pc) const;");
    push (@func_prot, "$inst_func_tmp");

    $lp = 0;
    foreach (@cmp_lst) {
        $cmp = $_;
        if ($inst_line =~ /${cmp}(([^<])|$)/) {
            $inst_func_tmp = sprintf ("%s%s);", $func_conv_head[$lp], $operand_list[$lp]);
            push (@conv_func, "$inst_func_tmp");
        }
        $cmp = $func_conv_head[$lp];
        $lp++;
    }
    push (@conv_func, "}\n");

    $idx++;
}
push (@func_prot, "");


###############################################################
#
#  trace_operand.cppの生成
#
###############################################################
open (FW_C, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "trace_operand_gen.cpp") || die "Can't open trace_operand_gen.cpp: $!\n";
@op_cpp = ();

printf (FW_C "/*\n");
printf (FW_C " * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
printf (FW_C " * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
printf (FW_C " * This program must be used solely for the purpose for which\n");
printf (FW_C " * it was furnished by Renesas Electronics Corporation. No part of this\n");
printf (FW_C " * program may be reproduced or disclosed to others, in any\n");
printf (FW_C " * form, without the prior written permission of Renesas Electronics\n");
printf (FW_C " * Corporation.\n");
printf (FW_C " */\n");
printf (FW_C "\n");

push (@op_cpp, "#include <string>");
push (@op_cpp, "#include <sstream>");
push (@op_cpp, "#include \"./config.h\"");
push (@op_cpp, "#include \"./forest_common.h\"");
push (@op_cpp, "#include \"./trace_info.h\"");
push (@op_cpp, "#include \"./trace_operand.h\"");
push (@op_cpp, "#include \"./micro_arch_db.h\"");
push (@op_cpp, "#include \"./regid.h\"");
push (@op_cpp, "#include \"./sregfile.h\"\n\n");

foreach (@op_cpp) {
    print FW_C "$_\n";
}
foreach (@conv_func) {
    print FW_C "$_\n";
}
foreach (@operand_info) {
    print FW_C "$_\n";
}
IO::Handle::flush(FW_C);
eval { IO::Handle::sync(FW_C); };
close (FW_C);


###############################################################
#
#  trace_operand.hの生成
#
###############################################################
open (FW_H, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "trace_operand.h") || die "Can't open trace_operand.h: $!\n";
@op_header_h = ();

printf (FW_H "/*\n");
printf (FW_H " * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
printf (FW_H " * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
printf (FW_H " * This program must be used solely for the purpose for which\n");
printf (FW_H " * it was furnished by Renesas Electronics Corporation. No part of this\n");
printf (FW_H " * program may be reproduced or disclosed to others, in any\n");
printf (FW_H " * form, without the prior written permission of Renesas Electronics\n");
printf (FW_H " * Corporation.\n");
printf (FW_H " */\n");
printf (FW_H "\n");

push (@op_header_h, "#pragma once");

push (@op_header_h, "#include <string>");
push (@op_header_h, "#include \"./inst_id_list.h\"\n");

push (@op_header_h, "class TracePrintOperand {");
push (@op_header_h, "public:");
push (@op_header_h, "    void PrintOperand (uint32_t inst_id, uint64_t inst_code,");
push (@op_header_h, "                       uint32_t inst_pc, std::string* inst_mnem) const;\n");

push (@op_header_h, "    TracePrintOperand (void);\n");

push (@op_header_h, "private:");

@op_header_f = ();

push (@op_header_f, "    uint32_t NumFromCode (uint64_t inst_code, ReplaceType id) const;\n");

push (@op_header_f, "    void ReplaceReg     (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceVReg    (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceWReg    (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceHexU    (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceDec     (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceEntry   (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceList    (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceSR      (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceSel     (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceCnd     (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceFCO     (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceCacheOp (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplacePrefOp  (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;");
push (@op_header_f, "    void ReplaceBcond   (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;\n");
push (@op_header_f, "    void ReplaceBra     (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;\n");
push (@op_header_f, "    void ReplaceWidth   (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;\n");
push (@op_header_f, "    void ReplacePos     (std::string* inst_op, uint64_t inst_code,");
push (@op_header_f, "                         uint32_t inst_pc, ReplaceType id) const;\n");

push (@op_header_f, "    inline uint32_t OperandExtract (uint64_t x, uint32_t l, uint32_t r) const {");
push (@op_header_f, "        return static_cast<uint32_t> ((x >> r) & ~(~0x0ULL << (l - r + 1)));");
push (@op_header_f, "    }\n");

push (@op_header_f, "    void (TracePrintOperand::*m_inst_func[TOTAL_INST_NUM+1]) (");
push (@op_header_f, "              std::string* inst_op, uint64_t inst_code, uint32_t inst_pc) const;\n");

push (@op_header_f, "    class ReplaceInfo {");
push (@op_header_f, "    public:");
push (@op_header_f, "        std::string m_rep;");
push (@op_header_f, "        uint32_t    m_code_ll;");
push (@op_header_f, "        uint32_t    m_code_lr;");
push (@op_header_f, "        uint32_t    m_code_ml;");
push (@op_header_f, "        uint32_t    m_code_mr;");
push (@op_header_f, "        uint32_t    m_code_hl;");
push (@op_header_f, "        uint32_t    m_code_hr;");
push (@op_header_f, "        bool        m_hseg;");
push (@op_header_f, "        bool        m_mseg;");
push (@op_header_f, "        uint32_t    m_shift;");
push (@op_header_f, "        uint32_t    m_offset;");
push (@op_header_f, "        uint32_t    m_sig;");
push (@op_header_f, "    };\n");

push (@op_header_f, "    ReplaceInfo replace_info[TOTAL_REPLACE_NUM];");
push (@op_header_f, "    void SetReplaceInfoValue (ReplaceType indx, const std::string& rep,");
push (@op_header_f, "                              int32_t code_hl, int32_t code_hr,");
push (@op_header_f, "                              int32_t code_ml, int32_t code_mr,");
push (@op_header_f, "                              int32_t code_ll, int32_t code_lr,");
push (@op_header_f, "                              bool hseg, bool mseg, uint32_t shift, uint32_t offset,");
push (@op_header_f, "                              uint32_t sig);");
push (@op_header_f, "};\n");

foreach (@op_header_h) {
    print FW_H "$_\n";
}
foreach (@operand_enum) {
    print FW_H "$_\n";
}
foreach (@func_prot) {
    print FW_H "$_\n";
}
foreach (@op_header_f) {
    print FW_H "$_\n";
}
IO::Handle::flush(FW_H);
eval { IO::Handle::sync(FW_H); };
close (FW_H);
