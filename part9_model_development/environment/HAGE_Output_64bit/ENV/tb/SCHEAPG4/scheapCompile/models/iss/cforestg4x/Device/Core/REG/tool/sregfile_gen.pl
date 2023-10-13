#!/usr/bin/perl

# (c) 2011,2012,2013 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.
use Getopt::Long;
use IO::Handle;

my $bom = "\x{feff}"; #BOM
my $opt_with_bom = 0;
my $opt_crlf = 0;

GetOptions('bom|withbom' => \$opt_with_bom, 'crlf' => \$opt_crlf);

open (INPUT_REG,  "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";
open (OUTPUT_H, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[1]") || die "Can't open $ARGV[1]: $!\n";
open (OUTPUT_CPP, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[2]") || die "Can't open $ARGV[2]: $!\n";
open (OUTPUT_MOCK, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[3]") || die "Can't open $ARGV[3]: $!\n";

# システムレジスタ情報ファイルを入力
my %srnames = ();
while (<INPUT_REG>) {
    # Skip comment lines and empty lines.
    if(($_ =~ /^[ |\t]*\#/) or ($_ =~ /^[ |\t]*\n/)){
        next;
    }

    chomp;
    # selid,regid,reg_class_name,reg_name,res_type,priv,id,bankid,base
    my ($group, $id, $class, $name, $res_type, $priv, $lid, $bankid, $base) = split (/,/, $_, 10);

    $arch = "FVS";

    # SelID と RegId をキーとした多次元連想配列
    $srnames{$group}{$id} = [$class, $name, $res_type, $priv, $lid, $bankid, $arch, $base];
}


# システムレジスタ連想配列の添え字
my $CLASS     = 0;
my $NAME      = 1;
my $RESOURCE  = 2;
my $PRIV_V20  = 3;
my $LID       = 4;
my $BANKID    = 5;
my $ARCH      = 6;
my $BASE_NAME = 7;

close (INPUT_REG);


# Hファイル出力
select (OUTPUT_H);
# プロローグ
{
    printf ${bom} if $opt_with_bom;
    printf ("/*\n");
    printf (" * (c) 2011,2012,2013,2017 Renesas Electronics Corporation\n");
    printf (" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf (" * This program must be used solely for the purpose for which\n");
    printf (" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf (" * program may be reproduced or disclosed to others, in any\n");
    printf (" * form, without the prior written permission of Renesas Electronics\n");
    printf (" * Corporation.\n");
    printf (" */\n");
    printf ("\n");

    print "#pragma once


#include \"./forest_common.h\"
#include \"./sregfile_base.h\"
#include \"./sreg.h\"

"
}


# Native Context システムレジスタインターフェースクラス
{
    print "
class NcSRegFileIF {
public:
    virtual ~NcSRegFileIF () {};

";
    foreach $group ( sort {$a <=> $b} keys %srnames ) {
        print "    // System register group $group\n";
        foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
            my $class = $srnames{$group}{$id}[$CLASS];
            my $name  = $srnames{$group}{$id}[$NAME];

            # Class name of Host registers have prefix "HM". To prevent virtual register name.
            $base_name = $srnames{$group}{$id}[$BASE_NAME];
            if (($base_name ne "-") and ($group != 9)) {
                $reg_prefix = "HM";
            } else {
                $reg_prefix = "";
            }

            printf ("    virtual SReg%-12s Sr%-s() const = 0;\n",
                    "${reg_prefix}${class}*", "${reg_prefix}${name}" );
        }
    }
    print "};\n";
    print "\n";
    print "\n";
}

# Native Context システムレジスタクラス
{
    print "
class NcSRegFile : public SRegFileBase, NcSRegFileIF {
public:
    NcSRegFile(ContextId ctxt_id, ProcElementBase* pe, SRegBase* sreg_init);
    ~NcSRegFile();

";
    foreach $group ( sort {$a <=> $b} keys %srnames ) {
        print "    // System register group $group\n";
        foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
            my $class = $srnames{$group}{$id}[$CLASS];
            my $name  = $srnames{$group}{$id}[$NAME];

            # Class name of Host registers have prefix "HM". To prevent virtual register name.
            $base_name = $srnames{$group}{$id}[$BASE_NAME];
            if (($base_name ne "-") and ($group != 9)) {
                $reg_prefix = "HM";
            } else {
                $reg_prefix = "";
            }

            printf ("    SReg%-8s Sr%-8s() const { return static_cast%-15s(m_srf%-16s}\n",
                    "${reg_prefix}${class}*", "${reg_prefix}${name}", "<SReg${reg_prefix}${class}*>", "[CF_SR_${reg_prefix}${name}]);" );
        }
    }
    print "
private:
    friend class ProcElement;
    void InitSRegs (ContextId ctxt_id, ProcElementBase* pe);
";
    print "};\n";
    print "\n";
    print "\n";
}

# 一般用途向け システムレジスタインターフェースクラス
{
    print "
class SRegFileIF {
public:
    virtual ~SRegFileIF() {};

";

    foreach $group ( sort {$a <=> $b} keys %srnames ) {
        print "    // System register group $group\n";
        foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
            my $class = $srnames{$group}{$id}[$CLASS];
            my $name  = $srnames{$group}{$id}[$NAME];

            # Class name of Host registers have prefix "HM". To prevent virtual register name.
            $base_name = $srnames{$group}{$id}[$BASE_NAME];
            if (($base_name ne "-") and ($group != 9)) {
                $reg_prefix = "HM";
                $class = "Base" . $base_name;
                $alias = "V";
            } else {
                $reg_prefix = "";
                $alias = "";
            }

            printf ("    virtual SReg%-12s Sr%s() const = 0;\n", "${class}*", ${alias} . ${name} );
        }
    }
    print "};\n";
}

# 一般用途向け システムレジスタクラス
{
    print "
class SRegFile : public SRegFileBase, SRegFileIF {
public:
    SRegFile (NcSRegFile* nc_srf);
    void LinkSRegs (SRegFileBase* srfile);

";

    foreach $group ( sort {$a <=> $b} keys %srnames ) {
        print "    // System register group $group\n";
        foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
            my $class = $srnames{$group}{$id}[$CLASS];
            my $name  = $srnames{$group}{$id}[$NAME];

            # Class name of Host registers have prefix "HM". To prevent virtual register name.
            $base_name = $srnames{$group}{$id}[$BASE_NAME];
            if (($base_name ne "-") and ($group != 9)) {
                $reg_prefix = "HM";
                $class = "Base" . $base_name;
                $alias = "V";
            } else {
                $reg_prefix = "";
                $alias = "";
            }

            printf ("    SReg%-8s Sr%-8s() const { return static_cast%-15s(m_srf%-16s}\n",
                    "${class}*", ${alias} . ${name}, "<SReg${class}*>", "[CF_SR_${alias}${name}]);" );
        }
    }
    print "};\n";
}
IO::Handle::flush(OUTPUT_H);
eval { IO::Handle::sync(OUTPUT_H); };
close (OUTPUT_H);


# C++ファイル出力

select (OUTPUT_CPP);
# プロローグ
{
    printf ${bom} if $opt_with_bom;
    printf ("/*\n");
    printf (" * (c) 2011,2012,2013,2017 Renesas Electronics Corporation\n");
    printf (" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf (" * This program must be used solely for the purpose for which\n");
    printf (" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf (" * program may be reproduced or disclosed to others, in any\n");
    printf (" * form, without the prior written permission of Renesas Electronics\n");
    printf (" * Corporation.\n");
    printf (" */\n");
    printf ("\n");

    print "#include \"./config.h\"\n";
    print "#include \"./regid.h\"\n";
    print "#include \"./sregfile.h\"\n";
    print "#include \"./proc_element.h\"\n";
    print "\n";
    print "\n";
}


# Native Context システムレジスタ生成関数
{
    print "void NcSRegFile::InitSRegs (ContextId ctxt_id, ProcElementBase* pe)\n";
    print "{\n";
    print "    switch (pe->GetArchClass()) {\n";
    foreach $archtype ('F', 'V', 'S') {
        print "    case E3V5_${archtype}:\n";
        foreach $group ( sort {$a <=> $b} keys %srnames ) {
            print "    // System register group $group\n";
            foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
                if (index ($srnames{$group}{$id}[$ARCH], $archtype) != -1) {
                    my $class = $srnames{$group}{$id}[$CLASS];
                    my $name  = $srnames{$group}{$id}[$NAME];
                    my $lid   = $srnames{$group}{$id}[$LID];
                    my $bankid = $srnames{$group}{$id}[$BANKID];

                    # Class name of Host registers have prefix "HM". To prevent virtual register name.
                    $base_name = $srnames{$group}{$id}[$BASE_NAME];
                    if (($base_name ne "-") and ($group != 9)) {
                        $reg_prefix = "HM";
                    } else {
                        $reg_prefix = "";
                    }

                    if ($lid =~ /-/) {
                        printf ("    m_srf%-16s = new SReg%-7s (ctxt_id, pe);\n",
                                "[CF_SR_${reg_prefix}${name}]", "${reg_prefix}${class}");
                    } else {
                        if ($bankid =~ /-/) {
                            printf ("    m_srf%-16s = new SReg%-7s (ctxt_id, pe, %s);\n",
                                    "[CF_SR_${reg_prefix}${name}]", "${reg_prefix}${class}", $lid);
                        } else {
                            printf ("    m_srf%-16s = new SReg%-7s (ctxt_id, pe, %s, %s);\n",
                                    "[CF_SR_${reg_prefix}${name}]", "${reg_prefix}${class}", $lid, $bankid);
                        }
                    }
                }
            }
        }
        print "    break;\n\n";
    }
    print "    default:\n";
    print "        break;\n";
    print "    }\n";
    print "}\n";
    print "\n";
    print "\n";
}


# (SelID, RegID) と Enum SReg の関連付けテーブル
{
    print "void RegID::InitIdMap (void)\n";
    print "{\n";
    print "    if (m_cfsrid_map.size() != 0) return;\n";
    print "\n";

    foreach $group ( sort {$a <=> $b} keys %srnames ) {
        print "    // System register group $group\n";
        foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
            my $name  = $srnames{$group}{$id}[$NAME];

            # Class name of Host registers have prefix "HM". To prevent virtual register name.
            $base_name = $srnames{$group}{$id}[$BASE_NAME];
            if (($base_name ne "-") and ($group != 9)) {
                $reg_prefix = "HM";
            } else {
                $reg_prefix = "";
            }

            printf ("    AddSRegIdMap (%2d, %2d, CF_SR_%s);\n",
                    $group, $id, $reg_prefix . $name);
        }
    }

    print "}\n";
}
IO::Handle::flush(OUTPUT_CPP);
eval { IO::Handle::sync(OUTPUT_CPP); };
close (OUTPUT_CPP);

# MOCKファイル出力
select (OUTPUT_MOCK);
# プロローグ
{
    printf ${bom} if $opt_with_bom;
    printf ("/*\n");
    printf (" * (c) 2011,2012,2013,2017 Renesas Electronics Corporation\n");
    printf (" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf (" * This program must be used solely for the purpose for which\n");
    printf (" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf (" * program may be reproduced or disclosed to others, in any\n");
    printf (" * form, without the prior written permission of Renesas Electronics\n");
    printf (" * Corporation.\n");
    printf (" */\n");
    printf ("\n");

    print "#pragma once

#include \"./sregfile.h\"

"
}


# Native Context システムレジスタMockクラス
{
    print "
class MockNcSRegFile : public NcSRegFile {
public:
    MockNcSRegFile(ContextId ctxt_id, ProcElementBase* pe, SRegBase* sreg_init)
      : NcSRegFile(ctxt_id, pe, sreg_init) {}

";
    foreach $group ( sort {$a <=> $b} keys %srnames ) {
        print "    // System register group $group\n";
        foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
            my $class = $srnames{$group}{$id}[$CLASS];
            my $name  = $srnames{$group}{$id}[$NAME];

            # Class name of Host registers have prefix "HM". To prevent virtual register name.
            $base_name = $srnames{$group}{$id}[$BASE_NAME];
            if (($base_name ne "-") and ($group != 9)) {
                $reg_prefix = "HM";
            } else {
                $reg_prefix = "";
            }

            printf ("    MOCK_CONST_METHOD0(Sr%-s, SReg%s());\n",
                    "${reg_prefix}${name}","${reg_prefix}${class}*");
        }
    }
    print "};\n";
    print "\n";
    print "\n";
}

# 一般用途向け システムレジスタMockクラス
{
    print "
class MockSRegFile : public SRegFile {
public:
    MockSRegFile (NcSRegFile* nc_srf) : SRegFile (nc_srf) {}
";

    foreach $group ( sort {$a <=> $b} keys %srnames ) {
        print "    // System register group $group\n";
        foreach $id ( sort {$a <=> $b} keys %{$srnames{$group}} ) {
            my $class = $srnames{$group}{$id}[$CLASS];
            my $name  = $srnames{$group}{$id}[$NAME];

            # Class name of Host registers have prefix "HM". To prevent virtual register name.
            $base_name = $srnames{$group}{$id}[$BASE_NAME];
            if (($base_name ne "-") and ($group != 9)) {
                $reg_prefix = "HM";
                $class = "Base" . $base_name;
                $alias = "V";
            } else {
                $reg_prefix = "";
                $alias = "";
            }

            printf ("    MOCK_CONST_METHOD0(Sr%s, SReg%s());\n", ${alias} . ${name}, "${class}*" );
        }
    }
    print "};\n";
}
IO::Handle::flush(OUTPUT_MOCK);
eval { IO::Handle::sync(OUTPUT_MOCK); };
close (OUTPUT_MOCK);
