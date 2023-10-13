#!/usr/bin/perl

# (c) 2011,2012,2013 Renesas Electronics Corporation
# RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
# This program must be used solely for the purpose for which
# it was furnished by Renesas Electronics Corporation. No part of this
# program may be reproduced or disclosed to others, in any
# form, without the prior written permission of Renesas Electronics
# Corporation.
use Getopt::Long;

my $bom = "\x{feff}"; #BOM
my $opt_with_bom = 0;
my $opt_crlf = 0;

GetOptions('bom|withbom' => \$opt_with_bom, 'crlf' => \$opt_crlf);

open (INPUT_REG_G4MH1x,  "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";
open (INPUT_REG_G4MH2x,  "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";
open (OUTPUT_H, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[1]") || die "Can't open $ARGV[1]: $!\n";
open (OUTPUT_CPP, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[2]") || die "Can't open $ARGV[2]: $!\n";

select (OUTPUT_H);
{
    printf ${bom} if $opt_with_bom;
    printf ("/*\n");
    printf (" * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
    printf (" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf (" * This program must be used solely for the purpose for which\n");
    printf (" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf (" * program may be reproduced or disclosed to others, in any\n");
    printf (" * form, without the prior written permission of Renesas Electronics\n");
    printf (" * Corporation.\n");
    printf (" */\n");
    printf ("\n");
    print "
#if ! defined(__FROG_REG_PROFILE_H__)
#define __FROG_REG_PROFILE_H__

#include <string>
#include <vector>
#include <map>
#include <inttypes.h>
#include \"./forest_common.h\"
#include \"./frog_if.h\"

extern std::map<std::string, FrogRegPro*> frog_reg_map;
extern bool init_FROG_register_profile_G4MH1x ();
extern bool init_FROG_register_profile_G4MH2x ();

#endif /* __FROG_REG_PROFILE_H__ */
"
}
close (OUTPUT_H);

select (OUTPUT_CPP);
{
    printf ${bom} if $opt_with_bom;
    printf ("/*\n");
    printf (" * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
    printf (" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf (" * This program must be used solely for the purpose for which\n");
    printf (" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf (" * program may be reproduced or disclosed to others, in any\n");
    printf (" * form, without the prior written permission of Renesas Electronics\n");
    printf (" * Corporation.\n");
    printf (" */\n");
    printf ("\n");
    print "
#include \"./frog_reg_profile.h\"

std::map <std::string, FrogRegPro*> frog_reg_map;

// Init register profile for FROG Interface in G4MH1.x.
bool init_FROG_register_profile_G4MH1x () {
    //Create program register profile for FROG I/F
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"PC\", new FrogRegPro (\"PC\", 18, 0, \"NT\", \"U\", FROG_PC, PC)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R0\", new FrogRegPro (\"R0\", 16, 0, \"T\", \"U\", FROG_GR, R0)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R1\", new FrogRegPro (\"R1\", 16, 1, \"T\", \"U\", FROG_GR, R1)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R2\", new FrogRegPro (\"R2\", 16, 2, \"T\", \"U\", FROG_GR, R2)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R3\", new FrogRegPro (\"R3\", 16, 3, \"T\", \"U\", FROG_GR, R3)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R4\", new FrogRegPro (\"R4\", 16, 4, \"T\", \"U\", FROG_GR, R4)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R5\", new FrogRegPro (\"R5\", 16, 5, \"T\", \"U\", FROG_GR, R5)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R6\", new FrogRegPro (\"R6\", 16, 6, \"T\", \"U\", FROG_GR, R6)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R7\", new FrogRegPro (\"R7\", 16, 7, \"T\", \"U\", FROG_GR, R7)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R8\", new FrogRegPro (\"R8\", 16, 8, \"T\", \"U\", FROG_GR, R8)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R9\", new FrogRegPro (\"R9\", 16, 9, \"T\", \"U\", FROG_GR, R9)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R10\", new FrogRegPro (\"R10\", 16, 10, \"T\", \"U\", FROG_GR, R10)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R11\", new FrogRegPro (\"R11\", 16, 11, \"T\", \"U\", FROG_GR, R11)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R12\", new FrogRegPro (\"R12\", 16, 12, \"T\", \"U\", FROG_GR, R12)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R13\", new FrogRegPro (\"R13\", 16, 13, \"T\", \"U\", FROG_GR, R13)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R14\", new FrogRegPro (\"R14\", 16, 14, \"T\", \"U\", FROG_GR, R14)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R15\", new FrogRegPro (\"R15\", 16, 15, \"T\", \"U\", FROG_GR, R15)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R16\", new FrogRegPro (\"R16\", 16, 16, \"T\", \"U\", FROG_GR, R16)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R17\", new FrogRegPro (\"R17\", 16, 17, \"T\", \"U\", FROG_GR, R17)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R18\", new FrogRegPro (\"R18\", 16, 18, \"T\", \"U\", FROG_GR, R18)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R19\", new FrogRegPro (\"R19\", 16, 19, \"T\", \"U\", FROG_GR, R19)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R20\", new FrogRegPro (\"R20\", 16, 20, \"T\", \"U\", FROG_GR, R20)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R21\", new FrogRegPro (\"R21\", 16, 21, \"T\", \"U\", FROG_GR, R21)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R22\", new FrogRegPro (\"R22\", 16, 22, \"T\", \"U\", FROG_GR, R22)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R23\", new FrogRegPro (\"R23\", 16, 23, \"T\", \"U\", FROG_GR, R23)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R24\", new FrogRegPro (\"R24\", 16, 24, \"T\", \"U\", FROG_GR, R24)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R25\", new FrogRegPro (\"R25\", 16, 25, \"T\", \"U\", FROG_GR, R25)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R26\", new FrogRegPro (\"R26\", 16, 26, \"T\", \"U\", FROG_GR, R26)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R27\", new FrogRegPro (\"R27\", 16, 27, \"T\", \"U\", FROG_GR, R27)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R28\", new FrogRegPro (\"R28\", 16, 28, \"T\", \"U\", FROG_GR, R28)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R29\", new FrogRegPro (\"R29\", 16, 29, \"T\", \"U\", FROG_GR, R29)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R30\", new FrogRegPro (\"R30\", 16, 30, \"T\", \"U\", FROG_GR, R30)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R31\", new FrogRegPro (\"R31\", 16, 31, \"T\", \"U\", FROG_GR, R31)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"SP\", new FrogRegPro (\"SP\", 16, 3, \"T\", \"U\", FROG_GR, R3)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"EP\", new FrogRegPro (\"EP\", 16, 30, \"T\", \"U\", FROG_GR, R30)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR0\", new FrogRegPro (\"WR0\", 37, 0, \"T\", \"U\", FROG_WR, WR0)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR1\", new FrogRegPro (\"WR1\", 37, 1, \"T\", \"U\", FROG_WR, WR1)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR2\", new FrogRegPro (\"WR2\", 37, 2, \"T\", \"U\", FROG_WR, WR2)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR3\", new FrogRegPro (\"WR3\", 37, 3, \"T\", \"U\", FROG_WR, WR3)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR4\", new FrogRegPro (\"WR4\", 37, 4, \"T\", \"U\", FROG_WR, WR4)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR5\", new FrogRegPro (\"WR5\", 37, 5, \"T\", \"U\", FROG_WR, WR5)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR6\", new FrogRegPro (\"WR6\", 37, 6, \"T\", \"U\", FROG_WR, WR6)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR7\", new FrogRegPro (\"WR7\", 37, 7, \"T\", \"U\", FROG_WR, WR7)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR8\", new FrogRegPro (\"WR8\", 37, 8, \"T\", \"U\", FROG_WR, WR8)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR9\", new FrogRegPro (\"WR9\", 37, 9, \"T\", \"U\", FROG_WR, WR9)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR10\", new FrogRegPro (\"WR10\", 37, 10, \"T\", \"U\", FROG_WR, WR10)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR11\", new FrogRegPro (\"WR11\", 37, 11, \"T\", \"U\", FROG_WR, WR11)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR12\", new FrogRegPro (\"WR12\", 37, 12, \"T\", \"U\", FROG_WR, WR12)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR13\", new FrogRegPro (\"WR13\", 37, 13, \"T\", \"U\", FROG_WR, WR13)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR14\", new FrogRegPro (\"WR14\", 37, 14, \"T\", \"U\", FROG_WR, WR14)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR15\", new FrogRegPro (\"WR15\", 37, 15, \"T\", \"U\", FROG_WR, WR15)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR16\", new FrogRegPro (\"WR16\", 37, 16, \"T\", \"U\", FROG_WR, WR16)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR17\", new FrogRegPro (\"WR17\", 37, 37, \"T\", \"U\", FROG_WR, WR17)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR18\", new FrogRegPro (\"WR18\", 37, 18, \"T\", \"U\", FROG_WR, WR18)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR19\", new FrogRegPro (\"WR19\", 37, 19, \"T\", \"U\", FROG_WR, WR19)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR20\", new FrogRegPro (\"WR20\", 37, 20, \"T\", \"U\", FROG_WR, WR20)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR21\", new FrogRegPro (\"WR21\", 37, 21, \"T\", \"U\", FROG_WR, WR21)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR22\", new FrogRegPro (\"WR22\", 37, 22, \"T\", \"U\", FROG_WR, WR22)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR23\", new FrogRegPro (\"WR23\", 37, 23, \"T\", \"U\", FROG_WR, WR23)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR24\", new FrogRegPro (\"WR24\", 37, 24, \"T\", \"U\", FROG_WR, WR24)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR25\", new FrogRegPro (\"WR25\", 37, 25, \"T\", \"U\", FROG_WR, WR25)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR26\", new FrogRegPro (\"WR26\", 37, 26, \"T\", \"U\", FROG_WR, WR26)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR27\", new FrogRegPro (\"WR27\", 37, 27, \"T\", \"U\", FROG_WR, WR27)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR28\", new FrogRegPro (\"WR28\", 37, 28, \"T\", \"U\", FROG_WR, WR28)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR29\", new FrogRegPro (\"WR29\", 37, 29, \"T\", \"U\", FROG_WR, WR29)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR30\", new FrogRegPro (\"WR30\", 37, 30, \"T\", \"U\", FROG_WR, WR30)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR31\", new FrogRegPro (\"WR31\", 37, 31, \"T\", \"U\", FROG_WR, WR31)));

    //Get system register profile from sysreg_g3m_reg.csv for FROG I/F
"
}

{
    my $creg_G4MH1x = 0;
    while (<INPUT_REG_G4MH1x>) {
        # Skip comment lines and empty lines.
        if(($_ =~ /^[ |\t]*\#/) or ($_ =~ /^[ |\t]*\n/)){
            next;
        }

        chomp;
        # selid,regid,reg_class_name,reg_name,res_type,priv,id, base
        my ($group, $id, $class, $name, $res_type, $priv, $lid, $base) = split (/,/, $_, 9);

        $ctxt = "N";

        # Update priv info based on copro info
        if (($res_type =~ /F/) or ($res_type =~ /S/)) {
            if      ($priv =~ /S/)  { $priv = "SC";
            } elsif ($priv =~ /U/)  { $priv = "C";
            } else { die "Unknown priv info in $ARGV[0] at $.\n";
            }
        }

        # for PSW special
        if ($name eq "PSW") {
            $priv = "SU";
        }

        if ($name =~ /G4X1X/) {
            my @name_part = split(/\_/,$name);
			chomp ($name_part[0]);
			$name = $name_part[0];
        } elsif ($name =~ /G4MH20/) {
            next;
        }

        $creg_G4MH1x = 0x00000100 + $group * 2 ** 5 + $id;
    #    printf  ("    std::string name_%-10s = \"%s\";\n", ${name}, ${name});
    #    printf  ("    std::string ctxt_%-10s = \"%s\";\n", ${name}, ${ctxt});
    #    printf  ("    std::string priv_%-10s = \"%s\";\n", ${name}, ${priv});
    #    printf  ("    frog_reg_map.push_back (name_%-10s, new FrogRegPro (name_%-10s, %d, %d, ctxt_%-10s, priv_%-10s, FROG_SR, 0x%x));\n", ${name}, ${name}, ${group}, ${id}, ${name}, ${name}, $creg_G4MH1x);
        printf  ("    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"%s\", new FrogRegPro (\"%s\", %d, %d, \"%s\", \"%s\", FROG_SR, 0x%x)));\n", ${name}, ${name}, ${group}, ${id}, ${ctxt}, ${priv}, ${creg_G4MH1x});
    }

    {
        printf ("    return true;\n");
        printf ("}\n");
    }
}

{
    print "
// Init register profile for FROG Interface in G4MH2.x.
bool init_FROG_register_profile_G4MH2x () {
    //Create program register profile for FROG I/F
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"PC\", new FrogRegPro (\"PC\", 18, 0, \"NT\", \"U\", FROG_PC, PC)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R0\", new FrogRegPro (\"R0\", 16, 0, \"T\", \"U\", FROG_GR, R0)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R1\", new FrogRegPro (\"R1\", 16, 1, \"T\", \"U\", FROG_GR, R1)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R2\", new FrogRegPro (\"R2\", 16, 2, \"T\", \"U\", FROG_GR, R2)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R3\", new FrogRegPro (\"R3\", 16, 3, \"T\", \"U\", FROG_GR, R3)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R4\", new FrogRegPro (\"R4\", 16, 4, \"T\", \"U\", FROG_GR, R4)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R5\", new FrogRegPro (\"R5\", 16, 5, \"T\", \"U\", FROG_GR, R5)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R6\", new FrogRegPro (\"R6\", 16, 6, \"T\", \"U\", FROG_GR, R6)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R7\", new FrogRegPro (\"R7\", 16, 7, \"T\", \"U\", FROG_GR, R7)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R8\", new FrogRegPro (\"R8\", 16, 8, \"T\", \"U\", FROG_GR, R8)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R9\", new FrogRegPro (\"R9\", 16, 9, \"T\", \"U\", FROG_GR, R9)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R10\", new FrogRegPro (\"R10\", 16, 10, \"T\", \"U\", FROG_GR, R10)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R11\", new FrogRegPro (\"R11\", 16, 11, \"T\", \"U\", FROG_GR, R11)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R12\", new FrogRegPro (\"R12\", 16, 12, \"T\", \"U\", FROG_GR, R12)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R13\", new FrogRegPro (\"R13\", 16, 13, \"T\", \"U\", FROG_GR, R13)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R14\", new FrogRegPro (\"R14\", 16, 14, \"T\", \"U\", FROG_GR, R14)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R15\", new FrogRegPro (\"R15\", 16, 15, \"T\", \"U\", FROG_GR, R15)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R16\", new FrogRegPro (\"R16\", 16, 16, \"T\", \"U\", FROG_GR, R16)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R17\", new FrogRegPro (\"R17\", 16, 17, \"T\", \"U\", FROG_GR, R17)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R18\", new FrogRegPro (\"R18\", 16, 18, \"T\", \"U\", FROG_GR, R18)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R19\", new FrogRegPro (\"R19\", 16, 19, \"T\", \"U\", FROG_GR, R19)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R20\", new FrogRegPro (\"R20\", 16, 20, \"T\", \"U\", FROG_GR, R20)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R21\", new FrogRegPro (\"R21\", 16, 21, \"T\", \"U\", FROG_GR, R21)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R22\", new FrogRegPro (\"R22\", 16, 22, \"T\", \"U\", FROG_GR, R22)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R23\", new FrogRegPro (\"R23\", 16, 23, \"T\", \"U\", FROG_GR, R23)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R24\", new FrogRegPro (\"R24\", 16, 24, \"T\", \"U\", FROG_GR, R24)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R25\", new FrogRegPro (\"R25\", 16, 25, \"T\", \"U\", FROG_GR, R25)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R26\", new FrogRegPro (\"R26\", 16, 26, \"T\", \"U\", FROG_GR, R26)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R27\", new FrogRegPro (\"R27\", 16, 27, \"T\", \"U\", FROG_GR, R27)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R28\", new FrogRegPro (\"R28\", 16, 28, \"T\", \"U\", FROG_GR, R28)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R29\", new FrogRegPro (\"R29\", 16, 29, \"T\", \"U\", FROG_GR, R29)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R30\", new FrogRegPro (\"R30\", 16, 30, \"T\", \"U\", FROG_GR, R30)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"R31\", new FrogRegPro (\"R31\", 16, 31, \"T\", \"U\", FROG_GR, R31)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"SP\", new FrogRegPro (\"SP\", 16, 3, \"T\", \"U\", FROG_GR, R3)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"EP\", new FrogRegPro (\"EP\", 16, 30, \"T\", \"U\", FROG_GR, R30)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR0\", new FrogRegPro (\"WR0\", 37, 0, \"T\", \"U\", FROG_WR, WR0)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR1\", new FrogRegPro (\"WR1\", 37, 1, \"T\", \"U\", FROG_WR, WR1)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR2\", new FrogRegPro (\"WR2\", 37, 2, \"T\", \"U\", FROG_WR, WR2)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR3\", new FrogRegPro (\"WR3\", 37, 3, \"T\", \"U\", FROG_WR, WR3)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR4\", new FrogRegPro (\"WR4\", 37, 4, \"T\", \"U\", FROG_WR, WR4)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR5\", new FrogRegPro (\"WR5\", 37, 5, \"T\", \"U\", FROG_WR, WR5)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR6\", new FrogRegPro (\"WR6\", 37, 6, \"T\", \"U\", FROG_WR, WR6)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR7\", new FrogRegPro (\"WR7\", 37, 7, \"T\", \"U\", FROG_WR, WR7)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR8\", new FrogRegPro (\"WR8\", 37, 8, \"T\", \"U\", FROG_WR, WR8)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR9\", new FrogRegPro (\"WR9\", 37, 9, \"T\", \"U\", FROG_WR, WR9)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR10\", new FrogRegPro (\"WR10\", 37, 10, \"T\", \"U\", FROG_WR, WR10)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR11\", new FrogRegPro (\"WR11\", 37, 11, \"T\", \"U\", FROG_WR, WR11)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR12\", new FrogRegPro (\"WR12\", 37, 12, \"T\", \"U\", FROG_WR, WR12)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR13\", new FrogRegPro (\"WR13\", 37, 13, \"T\", \"U\", FROG_WR, WR13)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR14\", new FrogRegPro (\"WR14\", 37, 14, \"T\", \"U\", FROG_WR, WR14)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR15\", new FrogRegPro (\"WR15\", 37, 15, \"T\", \"U\", FROG_WR, WR15)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR16\", new FrogRegPro (\"WR16\", 37, 16, \"T\", \"U\", FROG_WR, WR16)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR17\", new FrogRegPro (\"WR17\", 37, 37, \"T\", \"U\", FROG_WR, WR17)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR18\", new FrogRegPro (\"WR18\", 37, 18, \"T\", \"U\", FROG_WR, WR18)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR19\", new FrogRegPro (\"WR19\", 37, 19, \"T\", \"U\", FROG_WR, WR19)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR20\", new FrogRegPro (\"WR20\", 37, 20, \"T\", \"U\", FROG_WR, WR20)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR21\", new FrogRegPro (\"WR21\", 37, 21, \"T\", \"U\", FROG_WR, WR21)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR22\", new FrogRegPro (\"WR22\", 37, 22, \"T\", \"U\", FROG_WR, WR22)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR23\", new FrogRegPro (\"WR23\", 37, 23, \"T\", \"U\", FROG_WR, WR23)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR24\", new FrogRegPro (\"WR24\", 37, 24, \"T\", \"U\", FROG_WR, WR24)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR25\", new FrogRegPro (\"WR25\", 37, 25, \"T\", \"U\", FROG_WR, WR25)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR26\", new FrogRegPro (\"WR26\", 37, 26, \"T\", \"U\", FROG_WR, WR26)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR27\", new FrogRegPro (\"WR27\", 37, 27, \"T\", \"U\", FROG_WR, WR27)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR28\", new FrogRegPro (\"WR28\", 37, 28, \"T\", \"U\", FROG_WR, WR28)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR29\", new FrogRegPro (\"WR29\", 37, 29, \"T\", \"U\", FROG_WR, WR29)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR30\", new FrogRegPro (\"WR30\", 37, 30, \"T\", \"U\", FROG_WR, WR30)));
    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"WR31\", new FrogRegPro (\"WR31\", 37, 31, \"T\", \"U\", FROG_WR, WR31)));

    //Get system register profile from sysreg_g3m_reg.csv for FROG I/F
"
}

{
    my $creg_G4MH2x = 0;
    while (<INPUT_REG_G4MH2x>) {
        # Skip comment lines and empty lines.
        if(($_ =~ /^[ |\t]*\#/) or ($_ =~ /^[ |\t]*\n/)){
            next;
        }

        chomp;
        # selid,regid,reg_class_name,reg_name,res_type,priv,id, base
        my ($group, $id, $class, $name, $res_type, $priv, $lid, $base) = split (/,/, $_, 9);

        $ctxt = "N";

        # Update priv info based on copro info
        if (($res_type =~ /F/) or ($res_type =~ /S/)) {
            if      ($priv =~ /S/)  { $priv = "SC";
            } elsif ($priv =~ /U/)  { $priv = "C";
            } else { die "Unknown priv info in $ARGV[0] at $.\n";
            }
        }

        # for PSW special
        if ($name eq "PSW") {
            $priv = "SU";
        }

        if ($name =~ /G4MH20/) {
            my @name_part = split(/\_/,$name);
			chomp ($name_part[0]);
			$name = $name_part[0];
        } elsif ($name =~ /G4X1X/) {
            next;
        }
        $creg_G4MH2x = 0x00000100 + $group * 2 ** 5 + $id;
    #    printf  ("    std::string name_%-10s = \"%s\";\n", ${name}, ${name});
    #    printf  ("    std::string ctxt_%-10s = \"%s\";\n", ${name}, ${ctxt});
    #    printf  ("    std::string priv_%-10s = \"%s\";\n", ${name}, ${priv});
    #    printf  ("    frog_reg_map.push_back (name_%-10s, new FrogRegPro (name_%-10s, %d, %d, ctxt_%-10s, priv_%-10s, FROG_SR, 0x%x));\n", ${name}, ${name}, ${group}, ${id}, ${name}, ${name}, $creg_G4MH2x);
        printf  ("    frog_reg_map.insert (std::map <std::string, FrogRegPro*>::value_type(\"%s\", new FrogRegPro (\"%s\", %d, %d, \"%s\", \"%s\", FROG_SR, 0x%x)));\n", ${name}, ${name}, ${group}, ${id}, ${ctxt}, ${priv}, ${creg_G4MH2x});
    }

    {
        printf ("    return true;\n");
        printf ("}\n");
    }
}
close (INPUT_REG_G4MH2x);
close (INPUT_REG_G4MH1x);
close (OUTPUT_CPP);
