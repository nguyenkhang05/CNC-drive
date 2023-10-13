#! /usr/bin/perl

#----------------------------------------------------------------------
# Status    :
#    $Source: /shsv/sld/ipp/project/2012/rel/201210_prj_7228_UpdatedModel/Work/ducduong/DATA/SCI/Output/sim/scripts_linux/gen_sim/create_batch.pl,v $
#    $Revision: 1.1.1.1 $
#    $Date  : $
#    $Author: ducduong $
#    $Locker:  $
#    $State : $
#    $Id: create_batch.pl,v 1.1.1.1 2013/01/18 02:50:17 ducduong Exp $
#----------------------------------------------------------------------
#  (C) Copyright 2009   RVC (Renesas Design Vietnam Co., Ltd.)
#  All rights reserved. RVC Confidential Proprietary
#  (C) Copyright 2009   RENESAS Technology Corp.  All rights reserved.
#----------------------------------------------------------------------

use strict;
use Switch;

my $LIST_FILE    = "";           # name of list test pattern
my $EXE_PATH     = "";           # path to executing file
my $CCI_FILE     = "";           # CCi file name
my $VCD_OPTION   = 0 ;           # -vcd option
my $VCD_FILE     = "";           # vcd file 
my $LOG_FILE     = "";           # log file 
my $DEBUG_OPTION = 0 ;           # -debug option
my $RELATIVE_PATH= 0 ;           # indicates that input path is relative path
my @TEST_PATTERN = "";           # test pattern 
my @TEST_PATTERN_FULL = "";           # test pattern full path and name
my @TM_NAME      = "";           # test pattern name
my $TM_PATH      = "";           # path to test pattern
my @TM_LOCATION  = "";           # test pattern location
my $BATCH_FILE   = "run_exec.csh";           # batch file
my $RUN_WINDOW   = "run.bat";           # file to run TMs on window
my $CHECK_WINDOW = "check_results.bat";           # file to check on window
my $SSC_EXT      = ".ssc";        # ssc extension
my $SCI_EXT      = ".txt";        # ssc extension

# relative path from 'sim/' to other directories (pat, log, results, ...). These paths used in batch files
my $PAT_DIR = "../pat";
my $LOG_DIR = "../log";
my $RESULT_DIR = "../results";
my $REPORT_DIR = "../reports";
my $SCRIPT_DIR = "../scripts_linux";
my $SSC_DIR = $PAT_DIR;
my $VCD_DIR = "../sim/vcd";
my $path_prefix = "";
my $stim_env = 0;                # indicate that verification environment is stimulus environment
my $MEMCHECK_OPTION = 0;                # flag indicates that checking memory leak check or not           #Added by SonTran Nov-16
my $MODEL = "";                  # model name    #Added by SonTran Nov-18
my $MODEL_memcheck = "";                  # model name    #Added by SonTran Nov-18
my $VCS_OPTION = "";                  # VCS option
my $exe_file = "";            # run command - added by DM

&get_command;
&read_list_tm;
&create_batch_file;

# Function : read the command then assign input value to variable for operation   
sub get_command
{
#{{{
   # ------------- processing the command --------------------#
   my $cur_value = ""; 
   my $arg = "";
   if ($#ARGV == -1) 
   {
       push (@ARGV,"help");  
   }
   foreach $cur_value (@ARGV)
   {
      switch ($arg) 
      {
         # --------- option processing section ---------- #
         # get option -vcd to generate batch file
         case "-vcd"
         {
             $VCD_OPTION = 1;
             $VCD_FILE = $cur_value;
         } 
         # get cci file name 
         case '-src'                      
         {  
              $CCI_FILE = $cur_value;
         } 
         # get the path of Forest execute file
         case '-exe'                      
         {
              $EXE_PATH = $cur_value;
             ($path_prefix = $EXE_PATH) =~ s/\.\.\/tb\/.*$//;
              my @PATH_SPLIT = split(/\//, $EXE_PATH);
              $exe_file = $PATH_SPLIT[$#PATH_SPLIT];
         }
         # --------- Get Input file ----------#
         case '-l'
         {
             my $ext = '\.txt'."\$";
             if ($cur_value =~ $ext){              
                 $LIST_FILE = $cur_value;
                 ($TM_PATH = $LIST_FILE) =~ s/^.*\/sim\///;
                 $TM_PATH =~ s/[\w]*\.[^.]*$//;
                 if (!(-e $LIST_FILE)){
                    print "\n[create_batch.pl] Error:  Can't open list file : $LIST_FILE\n";
                    exit;
                }            
             }
         }
         # checking memory leak check  # Added by SonTran Nov-16
         case "-memcheck"
         {
             $MEMCHECK_OPTION = 1;
             $MODEL = $cur_value;
             $MODEL_memcheck = uc $cur_value;
         }
      }
      switch ($cur_value) 
      {
         case "-help" 
         {
             print " This script is used to generate batch file from list of test pattern file \n\n";
             print "     -exe <EXE PATH> : indicates path to executing file \n"; 
             print "     -cci <CCI FILE> : indicates cci file name\n"; 
             print "     -vcd            : vcd option to generate batch file \n"; 
             print "     -debug          : debug option to generate batch file \n"; 
             exit;       
         } 
         # get option -debug to generate batch file
         case "-debug"
         {
             $DEBUG_OPTION = 1;
         } 
         case "-r"
         {
             $RELATIVE_PATH = 1;
         } 
         case "-stim"
         {
             $stim_env = 1;
         } 
         #VCS run
         case "-vcs"
         {
             $VCS_OPTION = "-systemcrun";
         }
         $arg = $cur_value;
      }
   }
   if ($EXE_PATH eq "") {
      print "\n[create_batch.pl] Error: Lack of information of execute file\n";
      exit;
   }
   elsif (($stim_env == 0)&&($CCI_FILE eq "")) {
      print "\n[create_batch.pl] Error: Lack of information of cci file\n";
      exit;
   }
  #}}}
}

#   Description   : read "TM_path.txt" to get the TM list
sub read_list_tm {
#{{{
    open(LIST_FILE, "<"."$LIST_FILE") || die ("Cannot open $LIST_FILE.\n");
    my @INPUT_FILE = <LIST_FILE>;
    close (LIST_FILE);
    my $tm_indx = 0;
    my $previous_tm = "";
    foreach my $test_pattern (@INPUT_FILE) {
        $test_pattern =~ s/ +//;
        $test_pattern =~ s/\t+//;
        $test_pattern =~ s/#.*$//;
        
       if ((length($test_pattern) != 1)&&("$test_pattern" ne "$previous_tm")&&(length($test_pattern) != 0)) {
          ($TEST_PATTERN_FULL[$tm_indx] = $test_pattern) =~ s/\*//;
          $TEST_PATTERN[$tm_indx] = $test_pattern;
          $TEST_PATTERN[$tm_indx] =~ s/\.[^.]+$//;
          ($TM_NAME[$tm_indx] = $TEST_PATTERN[$tm_indx]) =~ s/^.*\///;
          if ($TEST_PATTERN_FULL[$tm_indx] =~ "/") {
             ($TM_LOCATION[$tm_indx] = $TEST_PATTERN[$tm_indx]) =~ s/\/[^\/]+$/\//;
          } 
          else {
             $TM_LOCATION[$tm_indx] = "";
          }
          $tm_indx++;
       }
       $previous_tm = $test_pattern;
    }
#}}}
}

# Function : generate batch file basing on list of test pattern                   
sub create_batch_file
{
#{{{
   my $COMMAND_LINE = "";
   my $COMMAND_LINE_W = "";
   my $COMMAND_LINE_WIN_CHECK = "";
   open (BATCH_FILE, ">$BATCH_FILE") || die ("Cannot open $BATCH_FILE.\n");
   open (RUN_WINDOW, ">$RUN_WINDOW") || die ("Cannot open $RUN_WINDOW.\n");
   open (CHECK_WINDOW, ">$CHECK_WINDOW") || die ("Cannot open $CHECK_WINDOW.\n");
   print BATCH_FILE ("#!/bin/csh -f\n\n");
   print BATCH_FILE ("set SIML_EXE=\"./$exe_file\"\n");
   print BATCH_FILE ("set CYCL_NUM=\"250\"\n\n\n\n");

   for (my $i=0; $i<=$#TEST_PATTERN; $i++) {
       my @TM_LOCATION_SPLIT = split(/\//,$TEST_PATTERN[$i]);
       my $tm_prefix = $path_prefix;
       my $src_prefix = "";
       for (my $j=0; $j<$#TM_LOCATION_SPLIT ;$j++) {
          $tm_prefix = $tm_prefix."../";
          if ($RELATIVE_PATH == 1) {
              $src_prefix = $src_prefix."../";
          }
       }

       # print "echo" command line
       $COMMAND_LINE = "set RSLT_LOG=\"../../../log/$TM_PATH$TM_LOCATION[$i]$TM_NAME[$i].log\"\n";
       $COMMAND_LINE = $COMMAND_LINE."set SCRIPT=\"$TM_NAME[$i].py\"\n";
       $COMMAND_LINE = $COMMAND_LINE."set HEAP_CFG=\"$TM_NAME[$i].cfg\"\n";
       print "$TM_LOCATION[$i]$TM_NAME[$i]\n";
   
       # locate run category
       if ($TEST_PATTERN_FULL[$i] =~ "/") {
          $COMMAND_LINE = $COMMAND_LINE."pushd $TM_LOCATION[$i]\n"; 
       }
      
       $COMMAND_LINE = $COMMAND_LINE."(echo -n \"#### Created date was \"; date +%%x) >& \${RSLT_LOG} \n";
       $COMMAND_LINE = $COMMAND_LINE."(echo -n \"#### Current directory was \"; pwd) >>& \${RSLT_LOG} \n";
       $COMMAND_LINE = $COMMAND_LINE."(echo \"#### Check programs are as follows\") >>& \${RSLT_LOG} \n";
       $COMMAND_LINE = $COMMAND_LINE."(grep \"G3MCPU_PROGRAM\" \${HEAP_CFG}) >>& \${RSLT_LOG} \n";
       # print option to check memory leak         # Added by SonTran Nov-16
       if ($MEMCHECK_OPTION == 1) {
           $COMMAND_LINE = $COMMAND_LINE."(time valgrind --suppressions=../../../scripts_linux/gen_sim/valgrind-python.supp --num-callers=4 --log-file=";# Added by SyNguyen (python issue)
           $COMMAND_LINE = $COMMAND_LINE."\"$tm_prefix$REPORT_DIR/memcheck/$TM_PATH$TEST_PATTERN[$i]_valgrind.log\" --leak-check=full ";
            # Added by NganTran Aug-30-2013: add time resolution
           if ($TM_NAME[$i] =~ /tp02_01_001/){
                $COMMAND_LINE = $COMMAND_LINE."\${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_FS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_02_001/) {
                $COMMAND_LINE = $COMMAND_LINE."\${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_PS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_03_001/) {
                $COMMAND_LINE = $COMMAND_LINE."\${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_NS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_04_001/) {
                $COMMAND_LINE = $COMMAND_LINE."\${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_US) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_05_001/) {
                $COMMAND_LINE = $COMMAND_LINE."\${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_MS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_06_001/) {
                $COMMAND_LINE = $COMMAND_LINE."\${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_SEC) | tee \${RSLT_LOG}\n";
           }
           else{
                $COMMAND_LINE = $COMMAND_LINE."\${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT}) | tee \${RSLT_LOG}\n";
           }
       } else {
           if ($TM_NAME[$i] =~ /tp02_01_001/){
                $COMMAND_LINE = $COMMAND_LINE."(time \${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_FS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_02_001/) {
                $COMMAND_LINE = $COMMAND_LINE."(time \${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_PS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_03_001/) {
                $COMMAND_LINE = $COMMAND_LINE."(time \${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_NS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_04_001/) {
                $COMMAND_LINE = $COMMAND_LINE."(time \${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_US) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_05_001/) {
                $COMMAND_LINE = $COMMAND_LINE."(time \${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_MS) | tee \${RSLT_LOG}\n";
           }elsif ($TM_NAME[$i] =~ /tp02_06_001/) {
                $COMMAND_LINE = $COMMAND_LINE."(time \${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT} -sctime SC_SEC) | tee \${RSLT_LOG}\n";
           }
           else{
                $COMMAND_LINE = $COMMAND_LINE."(time \${SIML_EXE} -config \${HEAP_CFG} -py_scr \${SCRIPT}) | tee \${RSLT_LOG}\n";
           }
           ###########################################################
       }
       # if ($TM_NAME[$i] =~ /tp03_06/ || $TM_NAME[$i] =~ /tp03_03_04/ || $TM_NAME[$i] =~ /tp03_03_07/ || $TM_NAME[$i] =~ /tp03_03_10/ || $TM_NAME[$i] =~ /tp04_01_04/){
          # $COMMAND_LINE = $COMMAND_LINE."grep -R -A 0 -B 0 \"PSEQ\" trace.log > ../../../log/$TM_PATH$TM_LOCATION[$i]$TM_NAME[$i]_trace.log\n";
          # $COMMAND_LINE = $COMMAND_LINE."cp -f trace.log ../../../log/$TM_PATH$TM_LOCATION[$i]$TM_NAME[$i]_full_trace.log\n";
       # }

       # print option to print output to log files
       #my $MORE_COMMAND = " | tee $tm_prefix$LOG_DIR/$TM_PATH$TEST_PATTERN[$i].log\n";
       my $MORE_COMMAND = "";

       # print option to check memory leak         # Added by SonTran Nov-16
       if ($MEMCHECK_OPTION == 1) {
           $MORE_COMMAND = $MORE_COMMAND."\nchmod 770 $tm_prefix$REPORT_DIR/memcheck/$TM_PATH$TEST_PATTERN[$i]_valgrind.log\n";
           $MORE_COMMAND = $MORE_COMMAND."perl $tm_prefix$SCRIPT_DIR/gen_report/read_rpt.pl ";
           $MORE_COMMAND = $MORE_COMMAND."$tm_prefix$REPORT_DIR/memcheck/$TM_PATH$TEST_PATTERN[$i]_valgrind.log ";
           $MORE_COMMAND = $MORE_COMMAND."$tm_prefix$REPORT_DIR/memcheck/$TM_PATH$TEST_PATTERN[$i]_valgrind_$MODEL.log\n";
       }
       $COMMAND_LINE = $COMMAND_LINE.$MORE_COMMAND;
       $COMMAND_LINE = $COMMAND_LINE."date +%%x>>& \${RSLT_LOG}\n";

       # return to executing category
       if ($TEST_PATTERN_FULL[$i] =~ "/") {
          $COMMAND_LINE = $COMMAND_LINE."popd\n\n"; 
       }
       else {
          $COMMAND_LINE = $COMMAND_LINE."\n\n"; 
       }
       printf BATCH_FILE "$COMMAND_LINE";
   }
   
   close (BATCH_FILE);   
   system "chmod +x $BATCH_FILE";
 #}}}
}
# vim600: set foldmethod=marker :
