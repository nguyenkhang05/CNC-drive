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

my $bom          = "\x{feff}";    #BOM
my $opt_with_bom = 0;
my $opt_crlf     = 0;

GetOptions('bom|withbom' => \$opt_with_bom, 'crlf' => \$opt_crlf);

open(INPUT_REG, "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";
open(INPUT_BIT, "<$ARGV[1]") || die "Can't open $ARGV[1]: $!\n";
open(OUTPUT_H, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[2]")
  || die "Can't open $ARGV[2]: $!\n";

# システムレジスタ情報ファイルを入力
my %srnames = ();
while (<INPUT_REG>)
{
    # Skip comment lines and empty lines.
    if (($_ =~ /^[ |\t]*\#/) or ($_ =~ /^[ |\t]*\n/))
    {
        next;
    }

    chomp;

    # selid,regid,reg_class_name,reg_name,res_type,priv,id,bankid, base
    my ($group, $id, $class, $name, $res_type, $priv, $lid, $bankid, $base) =
      split(/,/, $_, 10);

    # SelID と RegId をキーとした連想配列
    $srnames{$group}{$id} = [$name, $base];
}
close(INPUT_REG);

# システムレジスタ連想配列の添え字
my $NAME      = 0;
my $BASE_NAME = 1;

# システムレジスタビット情報ファイルを入力
my %pswbits  = ();
my %fpsrbits = ();
while (<INPUT_BIT>)
{
    # Skip comment lines and empty lines.
    if (($_ =~ /^[ |\t]*\#/) or ($_ =~ /^[ |\t]*\n/))
    {
        next;
    }

    chomp;

    # reg_class_name,bit_name,pos,width,rd,wr,init
    my ($sreg, $bit, $pos, $width, $attr_rd, $attr_wr, $init) =
      split(/,/, $_, 8);

    if ($sreg eq "HMPSW")
    {
        # PSWのビット名を格納
        $pswbits{$pos} = $bit;
    }
    if ($sreg eq "FPSR")
    {
        # FPSRのビット名を格納
        $fpsrbits{$pos} = $bit;
    }
}

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

/*!
  \@brief enum for System register
 */
enum CfSReg {
    CF_SR_DUMMY       = 0x000,  // Guard
";
}

# システムレジスタIDカウンタ
my $srid = 1;

# システムレジスタ 列挙子宣言
foreach $group (sort { $a <=> $b } keys %srnames)
{
    print "\n";
    print "    // System register group $group\n";
    foreach $id (sort { $a <=> $b } keys %{$srnames{$group}})
    {

        # Class name of Host registers have prefix "HM". To prevent virtual register name.
        $base_name = $srnames{$group}{$id}[$BASE_NAME];
        if (($base_name ne "-") and ($group != 9))
        {
            $reg_prefix = "HM";
            $has_alias  = 1;
        }
        else
        {
            $reg_prefix = "";
            $has_alias  = 0;
        }

        printf("    CF_SR_%-10s = 0x%03x,\n",
               $reg_prefix . $srnames{$group}{$id}[$NAME], $srid);
        if ($has_alias == 1)
        {
            printf("    CF_SR_%-10s = CF_SR_%-10s,\n",
                   "V" . $srnames{$group}{$id}[$NAME],
                   $reg_prefix . $srnames{$group}{$id}[$NAME]);
        }
        $srid++;
    }
}

# システムレジスタ ガードID
{
    print "\n";
    print "    // System register guard ID\n";
    printf("    CF_SR_GUARD      = 0x%03x,\n", $srid);
    $srid++;
}

# エピローグ
{
    print "};\n";
}
IO::Handle::flush(OUTPUT_H);
eval { IO::Handle::sync(OUTPUT_H); };
close(OUTPUT_H);
