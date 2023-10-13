#! /usr/bin/perl

#----------------------------------------------------------------------
# Status    :
#    $Source: /shsv/sld/ipp/project/2012/rel/201207_prj_m40_SC_HEAP/Work/tungbui/11_Release/02_Final_Version/092712_M40_SYS_CVSROOT/Output/sim/scripts_linux/check_result/check_result.pl,v $
#    $Revision: 1.1.1.1 $
#    $Date  : $
#    $Author: chaunguyen $
#    $Locker:  $
#    $State : $
#    $Id: check_result.pl,v 1.1.1.1 2012/09/27 10:12:21 chaunguyen Exp $
#----------------------------------------------------------------------
#  (C) Copyright 2009   RVC (Renesas Design Vietnam Co., Ltd.)
#  All rights reserved. RVC Confidential Proprietary
#  (C) Copyright 2009   RENESAS Technology Corp.  All rights reserved.
#----------------------------------------------------------------------

use strict;

my $PAT_DIR = "../../pat/";
my $SIM_DIR = "../../sim/";
my $LOG_DIR = "../../log/";
my $RESULT_DIR = "../../results/";
my $SCRIPT_DIR = "../../scripts_linux";
my $TM_PATH_DIR = $PAT_DIR;
my $ADDRESS_MAP_DIR = "..";

my $CAT = "";
my $MODEL = "";              #Added DungNguyen
my $VERSION = "";            #Added DungNguyen
my $TIME_MODE = "";          #Added DungNguyen
my $PAT = "";                #Added nhuvo 20180615

my @TM_PATH     = ();
my @TM_PATH_SEL = ();
my @TM_NAME     = ();
my @TM_LOCATION = ();
my $count;
my @first_item  = ();
my @MISS_LOG = ();
my $miss_log = 0;

my $P_PASS = "";

my @LOG_RESULT   = ();
my @DUMP_RESULT  = ();
my @FINAL_RESULT = ();

my @TM_DATE    = ();
my @TM_CPUTIME = ();
#my @TM_MEMORY  = ();

my $i;
my $dump_str;
my $pc_break_str;
my $pass_fail_str;

my $ENV_BUS_INFO = $ENV{ENV_BUS_INFO};
###################################################################
#                           Main process                          #
###################################################################

&load_argument ();
&load_TM_list ();
&load_p_pass ();

&check_log ();
&check_judgement_script ();
&create_log_file ();
&print_result ();
if ($miss_log == 1) {
    print STDOUT ("\n<WARNING> The log file of TM(s) listed below not found:\n");
    for (my $i=0; $i<=$#TM_PATH ;$i++) {
        if ($MISS_LOG[$i] == 1) {
            print STDOUT ("-- $TM_PATH_SEL[$i]\n");
        }
    }
}
###################################################################
#                    Subroutine declaration                       #
###################################################################

sub print_help {
     print "\nUsage : perl check_result.pl [-c <category>]\n";
     print "Script is used to generate simulation result of run TM(s).\n";
     print "\n";
     print "-c <category>   : TM category wanted to check result.\n\n";
     print "-help           : print help information.\n";
     exit;
}

#   Description   : load argument from command line
sub load_argument {

    $CAT = "/";
    $MODEL = "";
    $VERSION = "";
    $TIME_MODE = "";
    $PAT = "";
    for (my $i=0; $i<=$#ARGV ;$i++) {

        if ($ARGV[$i] =~ /^-c$/i) {
             $i++;
             $CAT  = $ARGV[$i];
        } # category
        elsif ($ARGV[$i] =~ /^-m$/i) {
            $i++;
            $MODEL =  $ARGV[$i];
        } # model
        elsif ($ARGV[$i] =~ /^-v$/i) {
            $i++;
            $VERSION =  $ARGV[$i];
        } # version
        elsif ($ARGV[$i] =~ /^-t$/i) {
            $i++;
            $TIME_MODE =  $ARGV[$i];
        } # timing mode
# Added by nhuvo 20180615
        elsif ($ARGV[$i] =~ /^-p$/i) {
            $i++;
            $PAT =  $ARGV[$i];
        } # pat name
###
        elsif ($ARGV[$i] =~ /^-help$/i) {
             &print_help();
        } # help
    }
}

#   Description   : read "TM_path.txt" to get the TM list
sub load_TM_list {

    my @TM_PATH_SEL_PRE = ""; # previous tm_path  # Added by SonTran Nov-18
    open(TM_PATH_FILE, "$TM_PATH_DIR/TM_path.txt") || die ("Unable to open TM_path.txt.\n");
    @TM_PATH = <TM_PATH_FILE>;
    close(TM_PATH_FILE);

    @TM_PATH_SEL = ();
    $count = 0;
    my $tm_count = 0;# count legal tm  # Added by SonTran Nov-18

    # remove ".*" at the end of the path
    my $i = 0;
    while($i<=$#TM_PATH) {
#        $TM_PATH[$i] =~ s/^.*pat\/(.*)\.[^.]+$/$1/;
        $TM_PATH[$i] =~ s/\.[^.]+$//;
        # Added by SonTran Nov-18#########################################
        # Process to get tm that is not duplicated
        my $indx = 0;
        my $tm_exist = 0;
        while (($indx<$count) && ($tm_exist == 0)) {
            if ($TM_PATH[$count] eq $TM_PATH_SEL_PRE[$indx]) {
                $tm_exist = 1;
            }
            $indx++;
        }
        # search for all TM paths containing category path
        if (($TM_PATH[$i] =~ /$CAT/)&&($tm_exist == 0)) {
            $TM_PATH_SEL[$tm_count] = $TM_PATH[$i];
            print "$TM_PATH_SEL[$tm_count]\n";
            $tm_count++;
        }
        $count++;
        $i++;
        $TM_PATH_SEL_PRE[$tm_count-1] = $TM_PATH_SEL[$tm_count-1];
        ##################################################################
    }

    # if not found
    if ($#TM_PATH_SEL < 0) {
        print "<ERROR> Input argument doesn't match any test-pattern category.\nUse option '-help' for more info.\n";
        exit;
    }

    # if found
    @TM_LOCATION = @TM_PATH_SEL;
    @first_item = ();
    for (my $i=0; $i<=$#TM_PATH_SEL ;$i++) {

        ($TM_NAME[$i] = $TM_PATH_SEL[$i]) =~ s/^.*\///;
        # TM_NAME: TM's name
        ($TM_LOCATION[$i] = $TM_PATH_SEL[$i]) =~ s/\/[^\/]+$//;
        # TM_LOCATION: path to TM file (not including TM's name)

        $first_item[$i] = 1;
        for (my $j=0; $j<$i ;$j++) {
            if ($TM_LOCATION[$j] eq $TM_LOCATION[$i]) {
                $first_item[$i] = 0;
            }
        } # check if the TM is the first item in TM_LOCATION
    }
}

#   Description   : read "Address_map_info.txt" to get P_PASS break-point
# load P_PASS from Address_map_info.txt
sub load_p_pass {
#    open(MAP_FILE, "$ADDRESS_MAP_DIR/Address_map_info.txt") || die ("Unable to open Address_map_info.txt.\n");
#    my @LINE = <MAP_FILE>;
#    close(MAP_FILE);
#    for (my $i=0; $i<=$#LINE ;$i++) {
#        if ($LINE[$i] =~ /^P_PASS/) {
#            $LINE[$i] =~ s/\t/ /g;
#            my @LINE_SPLIT = split(/ +/,$LINE[$i]);
#            $P_PASS = $LINE_SPLIT[$#LINE_SPLIT];
#        }
#    }
    $P_PASS = "PASS";
}

#   Description   : check PC break-point in log files
sub check_log {

    for (my $i=0; $i<=$#TM_PATH_SEL ;$i++) {

        $MISS_LOG[$i] = 0;
        my $no_script = 0;
        open(LOG_FILE, "$LOG_DIR/$TM_PATH_SEL[$i].log") || ($MISS_LOG[$i] = 1);
        open(SCRIPT,"$PAT_DIR/$TM_PATH_SEL[$i]_check_log.pl") || ($no_script = 1);
        # if log file of TM found
        if ($MISS_LOG[$i] == 0) {
            my @LOG_LINE = <LOG_FILE>;
            close(LOG_FILE);
            if ($no_script == 1) {
                $LOG_RESULT[$i] = 0;
                my $P_PASS_FOUND = grep(/$P_PASS/,@LOG_LINE);
                $LOG_RESULT[$i] = $P_PASS_FOUND;

                if ($LOG_RESULT[$i] == 1) {
                    if ($VERSION ne "") {
                        if (open (EXPECTED_LOG, "<$SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$VERSION\_$TIME_MODE/$TM_PATH_SEL[$i].log")) { # Added '$PAT\_' by nhuvo 20180615
                            if (open (CHECKED_LOG, "<$LOG_DIR/$TM_PATH_SEL[$i].log")) {
                                my @expected_result = <EXPECTED_LOG>;
                                my @checked_result = <CHECKED_LOG>;
    
                                for(my $my_index = 0; $my_index < $#expected_result-3; $my_index++) {
                                    if ($expected_result[$my_index] =~ /V C S   S i m u l a t i o n   R e p o r t/) {
                                        $my_index = $#expected_result;
                                    } elsif (!($expected_result[$my_index] =~ /####/) && !($expected_result[$my_index] =~ /sc_time.cpp:/) 
                                            && !($expected_result[$my_index] =~ /SystemC 2.3.0/)
                                            && !($expected_result[$my_index] =~ /Received transaction from unknown tlm_bw_transport_if/)
                                            && $expected_result[$my_index] ne $checked_result[$my_index]) {
                                        $LOG_RESULT[$i] = 0;
                                        $my_index = $#expected_result;
                                    }
                                }
                            } else {
                                print "Cannot open $LOG_DIR/$TM_PATH_SEL[$i].log\n";
                                $LOG_RESULT[$i] = 0;
                            }
                            close EXPECTED_LOG;
                        } else {
                            print "Cannot open $SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$VERSION\_$TIME_MODE/$TM_PATH_SEL[$i].log\n"; # Added '$PAT\_' by nhuvo 20180615
                            $LOG_RESULT[$i] = 0;
                        }
                        # if ($TM_PATH_SEL[$i] =~ /tp03_06/ || $TM_PATH_SEL[$i] =~ /tp03_03_04/ || $TM_PATH_SEL[$i] =~ /tp03_03_07/ || $TM_PATH_SEL[$i] =~ /tp03_03_10/ || $TM_PATH_SEL[$i] =~ /tp04_01_04/){ ### Added by NganTran - Aug 15th, 2015 to check trace log
                            # if (open (EXPECTED_TRACE_LOG, "<$SCRIPT_DIR/check_result/expected_log_osci_$MODEL\_$VERSION\_$TIME_MODE/$TM_PATH_SEL[$i]_trace.log")) {
                                # if (open (TRACE_LOG_FILE, "$LOG_DIR/$TM_PATH_SEL[$i]_trace.log")) {
                                    # my @expected_result = <EXPECTED_TRACE_LOG>;
                                    # my @checked_result = <TRACE_LOG_FILE>;
                                    # for(my $my_index = 0; $my_index < $#expected_result; $my_index++) {
                                        # if ($expected_result[$my_index] ne $checked_result[$my_index]) {
                                            # $LOG_RESULT[$i] = 0;
                                            # $my_index = $#expected_result;
                                        # }
                                    # }
                                    # close TRACE_LOG_FILE;
                                # } else {
                                    # print "Cannot open $LOG_DIR/$TM_PATH_SEL[$i]_trace.log\n";
                                    # $LOG_RESULT[$i] = 0;
                                # }
                                # close EXPECTED_TRACE_LOG;
                            # } else {
                                # print "Cannot open $SCRIPT_DIR/check_result/expected_log_osci_$MODEL\_$VERSION\_$TIME_MODE/$TM_PATH_SEL[$i]_trace.log\n";
                                # $LOG_RESULT[$i] = 0;
                            # }
                        # }
                    } else {
                        if (open (EXPECTED_LOG, "<$SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$TIME_MODE/$TM_PATH_SEL[$i].log")) { # Added '$PAT\_' by nhuvo 20180615
                            if (open (CHECKED_LOG, "<$LOG_DIR/$TM_PATH_SEL[$i].log")) {
                                my @expected_result = <EXPECTED_LOG>;
                                my @checked_result = <CHECKED_LOG>;
    
                                for(my $my_index = 0; $my_index < $#expected_result-3; $my_index++) {
                                    if ($expected_result[$my_index] =~ /V C S   S i m u l a t i o n   R e p o r t/) {
                                        $my_index = $#expected_result;
                                    } elsif (!($expected_result[$my_index] =~ /####/) && !($expected_result[$my_index] =~ /sc_time.cpp:/) 
                                            && !($expected_result[$my_index] =~ /SystemC 2.3.0/)
                                            && !($expected_result[$my_index] =~ /Received transaction from unknown tlm_bw_transport_if/)
                                            && $expected_result[$my_index] ne $checked_result[$my_index]) {
                                        $LOG_RESULT[$i] = 0;
                                        $my_index = $#expected_result;
                                    }
                                }
                            } else {
                                print "Cannot open $LOG_DIR/$TM_PATH_SEL[$i].log\n";
                                $LOG_RESULT[$i] = 0;
                            }
                            close EXPECTED_LOG;
                        } else {
                            print "Cannot open $SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$TIME_MODE/$TM_PATH_SEL[$i].log\n"; # Added '$PAT\_' by nhuvo 20180615
                            $LOG_RESULT[$i] = 0;
                        }
                        # if ($TM_PATH_SEL[$i] =~ /tp03_06/ || $TM_PATH_SEL[$i] =~ /tp03_03_04/ || $TM_PATH_SEL[$i] =~ /tp03_03_07/ || $TM_PATH_SEL[$i] =~ /tp03_03_10/ || $TM_PATH_SEL[$i] =~ /tp04_01_04/){ ### Added by NganTran - Aug 15th, 2015 to check trace log
                            # if (open (EXPECTED_TRACE_LOG, "<$SCRIPT_DIR/check_result/expected_log_osci_$MODEL\_$TIME_MODE/$TM_PATH_SEL[$i]_trace.log")) {
                                # if (open (TRACE_LOG_FILE, "$LOG_DIR/$TM_PATH_SEL[$i]_trace.log")) {
                                    # my @expected_result = <EXPECTED_TRACE_LOG>;
                                    # my @checked_result = <TRACE_LOG_FILE>;
                                    # for(my $my_index = 0; $my_index < $#expected_result; $my_index++) {
                                        # if ($expected_result[$my_index] ne $checked_result[$my_index]) {
                                            # $LOG_RESULT[$i] = 0;
                                            # $my_index = $#expected_result;
                                        # }
                                    # }
                                    # close TRACE_LOG_FILE;
                                # } else {
                                    # print "Cannot open $LOG_DIR/$TM_PATH_SEL[$i]_trace.log\n";
                                    # $LOG_RESULT[$i] = 0;
                                # }
                                # close EXPECTED_TRACE_LOG;
                            # } else {
                                # print "Cannot open $SCRIPT_DIR/check_result/expected_log_osci_$MODEL\_$TIME_MODE/$TM_PATH_SEL[$i]_trace.log\n";
                                # $LOG_RESULT[$i] = 0;
                            # }
                        # }
                    }
                } else {
                    # When PASS/FAIL is not output -> TM is passed when checked_log same as expected_log
#                    $LOG_RESULT[$i] = 1;
#                    my $no_script = 0;
#                    if ($VERSION ne "") { 
#                        open (EXPECTED_LOG, "<$SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$MODULE\_$VERSION\_$TIME_MODE/$TM_PATH_SEL[$i].log"); # Added '$PAT\_' by nhuvo 20180615
#                    } else {
#                        open (EXPECTED_LOG, "<$SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$MODULE\_$TIME_MODE/$TM_PATH_SEL[$i].log"); # Added '$PAT\_' by nhuvo 20180615
#                    }
#                    open (CHECKED_LOG, "<$LOG_DIR/$TM_PATH_SEL[$i].log");
#                    my @expected_result = <EXPECTED_LOG>;
#                    my @checked_result = <CHECKED_LOG>;
#                    for(my $my_index = 0; $my_index < $#expected_result-2; $my_index++) {
#                        if ($expected_result[$my_index] ne $checked_result[$my_index]) {
                            $LOG_RESULT[$i] = 0;
#                            $my_index = $#expected_result;
#                        }
#                    }
                }
            } else {
                open (USER_SCRIPT,"perl $PAT_DIR/$TM_PATH_SEL[$i]_check_log.pl $LOG_DIR/$TM_PATH_SEL[$i].log |"); # Using user script to check the log file 
                my $log_result = <USER_SCRIPT>;      
                if ($log_result =~ "PASS") {             # Pass if user script print "PASS"
                   $LOG_RESULT[$i] = 1;
                }else {                                  # Fail if user script doesn't print "PASS"
                   $LOG_RESULT[$i] = 0;
                }
            }
             
            # get simulation information from log file: DATE, CPU TIME, MEMORY

            # DATE
            $TM_DATE[$i] = $LOG_LINE[$#LOG_LINE];
            if ($TM_DATE[$i] =~ /\n$/) {
                chop($TM_DATE[$i]);
            }
	    #$TM_DATE[$i] = $LOG_LINE[0];
	    #$TM_DATE[$i] =~ s/#### Created date was //;
	    #$TM_DATE[$i] =~ s/ .*//;
            #chop($TM_DATE[$i]);

            # CPU TIME
            my $TIME_LINE = $LOG_LINE[$#LOG_LINE-1];
            my @TIME_LINE_SPLIT = split(/ /,$TIME_LINE);
            for (my $j=0; $j<=$#TIME_LINE_SPLIT ;$j++) {
                if ($TIME_LINE_SPLIT[$j] =~ /s$/) {
                    ($TM_CPUTIME[$i] = $TIME_LINE_SPLIT[$j]) =~ s/s//;
                }
            }
        }
        # if log file not found
        else {
            $miss_log = 1;
            if ($first_item[$i] == 1) {
                # transfer the 'first item' label to next TM in category
                for (my $j=$i+1; $j<=$#TM_PATH_SEL ;$j++) {
                    if ($TM_LOCATION[$j] eq $TM_LOCATION[$i]) {
                        $first_item[$j] = 1;
                        last;
                    }
                }
            }
        }
    }
}

#   Description   : find user's dump-file judgement script and call this script if exist, or call default 'check_dump' subroutine
sub check_judgement_script {

    for ($i=0; $i<=$#TM_PATH_SEL ;$i++) {

        if ($MISS_LOG[$i] == 0) {
            my $no_script = 0;
            # check if user have original script for result judgement.
            open(SCRIPT,"$PAT_DIR/$TM_PATH_SEL[$i]_check_dump.pl") || ($no_script = 1);

            #if found, call this script
            if ($no_script == 0) {

                print "<INFO> User's script found and applied for TM: $TM_PATH_SEL[$i]\n";

                open (USER_SCRIPT,"perl $PAT_DIR/$TM_PATH_SEL[$i]_check_dump.pl $SIM_DIR/$TM_LOCATION[$i] |");
                my $dump_result = <USER_SCRIPT>;       
                if ($dump_result =~ "PASS") {             # Pass if check dump user script print "PASS"
                   $DUMP_RESULT[$i] = 1;
                }elsif ($dump_result =~ "FAIL") {         # Fail if check dump user script print "FAIL"
                   $DUMP_RESULT[$i] = 0;
                } else {                                  # Dump file does not exist 
                   $DUMP_RESULT[$i] = 2;
                }

            }
            else { # if not found, call default script (check_dump)
                &check_dump ($i);
            }
        }
    }
}

#   Description   : compare detected data and expected data of specified TM
sub check_dump {

    my $i = $_[0];

    $DUMP_RESULT[$i]  = 1; # no dump files for this TM

    # search for all detected dump file
    opendir(DIR,"$RESULT_DIR/$TM_LOCATION[$i]");

    open(LOG_FILE, "$LOG_DIR/$TM_PATH_SEL[$i].log") || ("Can't open $LOG_DIR/$TM_PATH_SEL[$i].log");

    my @DUMP_FILE_LIST = grep(/$TM_NAME[$i]_Det[0-9]*\.dmp/,readdir(DIR));
    closedir(DIR);

    my @LOG_LINE = <LOG_FILE>;

    my $P_PASS_FOUND = grep(/$P_PASS/,@LOG_LINE);

    if ($P_PASS_FOUND == 1) {#XXX check it
        my $DET = "$LOG_DIR$TM_PATH_SEL[$i].log";
        my $EXP = "";
        if ($VERSION ne "") {
            $EXP = "$SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$VERSION\_$TIME_MODE/$TM_PATH_SEL[$i].log"; # Added '$PAT\_' by nhuvo 20180615
        } else {
            $EXP = "$SCRIPT_DIR/check_result/expected_log_osci_$PAT\_$MODEL\_$TIME_MODE/$TM_PATH_SEL[$i].log"; # Added '$PAT\_' by nhuvo 20180615
        }

        open (SDIFF,"diff $DET $EXP |");

        my $ret_val = <SDIFF>;
        my $curr_result = 1;
        if ($ret_val eq "") {
            $curr_result = 1;
        }

        if ($DUMP_RESULT[$i] == 2) {
            $DUMP_RESULT[$i] = $curr_result;
        }
        else { # DUMP_RESULT = 0 or 1
            $DUMP_RESULT[$i] = $DUMP_RESULT[$i] * $curr_result;
        }
    }

    # dump file found

    if ($#DUMP_FILE_LIST >= 0) {

        print "<INFO> Default script applied for TM: $TM_PATH_SEL[$i].\n";

        # for each TM, compare all dump files (detected <-> expected)
        for (my $j=0; $j<=$#DUMP_FILE_LIST ;$j++) {

            my $k;
            # if more than 1 dump files for 1 TM, the index after 'Det' or 'Exp' will be used.
            if ($#DUMP_FILE_LIST >= 1 ) {
                $k = $j + 1;
            }
            else {
                $k ="";
            }
     
            my $DET = "$RESULT_DIR/$TM_PATH_SEL[$i]_Det$k.dmp";
            my $EXP = "$RESULT_DIR/$TM_PATH_SEL[$i]_Exp$k.dmp";

            open (SDIFF,"diff $DET $EXP |");
            my $ret_val = <SDIFF>;
            my $curr_result = 0;
            if ($ret_val eq "") {
                $curr_result = 1;
            }

            if ($DUMP_RESULT[$i] == 2) {
                $DUMP_RESULT[$i] = $curr_result;
            }
            else { # DUMP_RESULT = 0 or 1
                $DUMP_RESULT[$i] = $DUMP_RESULT[$i] * $curr_result;
            }
        }
    }
}

#   Description   : check if 'result.log' exist, if not -> creat new
sub create_log_file {

    for (my $i=0; $i<=$#TM_PATH_SEL ;$i++) {

        if ($MISS_LOG[$i] == 0) {
            my $error = 0;

            open(RESULT_LOG,"$RESULT_DIR/$TM_LOCATION[$i]/result.txt") || ($error = 1);

            if ($error == 1) { # result.log does not exist
                my @TM_LOCATION_SPLIT = split(/\//,$TM_LOCATION[$i]);
                my $TM_SET = $TM_LOCATION_SPLIT[$#TM_LOCATION_SPLIT];
                open(NEW_LOG,">$RESULT_DIR/$TM_LOCATION[$i]/result.txt") || die ("Cannot create result.txt.\n");
                print NEW_LOG ("====================================================================\n");
                print NEW_LOG ("=                              RESULT                              =\n");
                print NEW_LOG ("====================================================================\n");
                print NEW_LOG ("  TM_SET :  $TM_SET \n");
                close NEW_LOG;
            }
            else {
                close RESULT_LOG;
            }
        }
    }
}

#   Description   : print result to 'result.txt' file
sub print_result {

    for ($i=0; $i<=$#TM_PATH_SEL ;$i++) {

        if ($MISS_LOG[$i] == 0) {
            # PC break-point string
            if ($LOG_RESULT[$i] == 0) {
                $pc_break_str = "No";
            }
            else {
                $pc_break_str = "Yes";
            }

            # dump result string
            if ($DUMP_RESULT[$i] == 2) {
                $DUMP_RESULT[$i] = 1;
                $dump_str = "none";
            }
            elsif ($DUMP_RESULT[$i] == 0) {
                $dump_str = "Not match";
            }
            else {
                $dump_str = "Match";
            }


            $FINAL_RESULT[$i] = $LOG_RESULT[$i] * $DUMP_RESULT[$i];
            $pass_fail_str = &pass_fail($FINAL_RESULT[$i]);

            open (RESULT_LOG,">>$RESULT_DIR/$TM_LOCATION[$i]/result.txt");

            # if the TM is the first item in TM_LOCATION, print field titles
            if ($first_item[$i] == 1) {
                print RESULT_LOG ("--------------------------------------------------------------------\n");
                print RESULT_LOG (" TM NAME                          DATE         RESULT   CPU_TIME(s)      ( PC break at <$P_PASS>          Dump files )\n");
            }

            # write TM information to 'result.txt'
            select(RESULT_LOG);
            $~ = "TM_RESULT_FORMAT";
            write;
            close RESULT_LOG;
            $dump_str = "";
        }
    }
}

#   Description   : return the string "Pass" or "Fail"
sub pass_fail {
    my $pass_fail_code = $_[0];
    my $str = "";
    if ($pass_fail_code == 1) {
        $str = "Pass";
    }
    else {
        $str = "Fail";
    }
    return ($str);
}

###################################################################
#                   File output print format                      #
###################################################################

format TM_RESULT_FORMAT =
 @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   @<<<<<<<<<   @<<<     @<<<<            (           @<<                  @<<<<<< )
$TM_NAME[$i], $TM_DATE[$i], $pass_fail_str, $TM_CPUTIME[$i], $pc_break_str, $dump_str;
.
