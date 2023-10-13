#! /usr/bin/perl

#----------------------------------------------------------------------
# Status    :
#    $Source: /shsv/sld/ipp/project/2012/rel/201210_prj_7228_UpdatedModel/Work/ducduong/DATA/SCI/Output/sim/scripts_linux/gen_sim/gen_sim.pl,v $
#    $Revision: 1.1.1.1 $
#    $Date  : $
#    $Author: ducduong $
#    $Locker:  $
#    $State : $
#    $Id: gen_sim.pl,v 1.1.1.1 2013/01/18 02:50:17 ducduong Exp $
#----------------------------------------------------------------------
#  (C) Copyright 2009   RVC (Renesas Design Vietnam Co., Ltd.)
#  All rights reserved. RVC Confidential Proprietary
#  (C) Copyright 2009   RENESAS Technology Corp.  All rights reserved.
#----------------------------------------------------------------------

use strict;
use Switch;
# relative path from 'gen_sim.pl' script to directories
my $TO_SIM_DIR = "../../sim";
my $TO_LOG_DIR = "../../log";
#my $TO_VCD_DIR = "../../sim/vcd";
my $TO_MEMCHECK_DIR = "../../reports/memcheck";
my $TO_RESULT_DIR = "../../results";
my $TO_PAT_DIR = "../../pat";

# relative path from 'gen_sim.pl' script to 'TM_path.txt'
my $TM_PATH_TXT = "../../pat/TM_path.txt";
my $REV_INFO_TXT = "rev_info.txt";           # Environment Info file
my $ENV_INFO_TXT = "env_info.txt";           # Environment Info file
my $ENV_SCI_FILE = "handleCommand";
my $CREATE_BAT = "create_batch.pl";
my $CREATE_BAT_PATH = "../scripts_linux/gen_sim/create_batch.pl";

# relative path from 'sim/' to other directories (pat, log, results, ...). These paths used in batch files
my $CHECK_RESULT_DIR = "../scripts_linux/check_result";
my $PAT_DIR = "../pat";
my $LOG_DIR = "../log";
my $RESULT_DIR = "../results";
my $SSC_DIR = $PAT_DIR;
my $SSC_EXT = ".ssc";
#my $VCD_DIR = "../sim/vcd";
my $SIM_DIR = "../sim";

## path of python if and config skeleteon
#my $PYTHONIF_SKL_PATH = "./";
my $CONFIG_SKL_PATH = "./";

# Global variables declaration
my $MODEL = "";              #Added DungNguyen
my $VERSION = "";            #Added DungNguyen
my $TIME_MODE = "";          #Added DungNguyen
my $PAT = "";                #Added nhuvo 20180615
my $FOREST_PATH = "";
my $CCI_PATH    = "";
my @PATH_LIST   = ();
my @TM_PATH     = ();
my @TM_PATH_FULL     = ();
my @TM_NAME     = ();
my @TM_LOCATION = ();
my @LAST_TM_LOCATION_SPLIT = ();
my $DIR = "";
my @DIR_LIST = ();
my $dir_count = -1;
my $relative_path = 0;
my $stim_env = 0;               # flag indicates that environment is stimulus or CPU environment 
my $vcs_env = 0;                # flag indicates that environment is vcs 
my $memcheck = 0;               # flag indicates that checking memory leak check or not           #Added by SonTran Nov-16
my $MODEL = "";                 # model name    #Added by SonTran Nov-18
my $source_file = "";
my $revision = "";
my $author = "";

my $tm_indx = 0;

my $need_to_link = 0;           #flag indicates that .cfg and.py file should be generated, added by DuyMai

###################################################################
#                           Main process                          #
###################################################################

&load_argument ();
&read_TM_path  ();
mkdir("$TO_MEMCHECK_DIR");            # create memory leak check directory   # Added by SonTran Nov-16
&check_directory ($TO_SIM_DIR,1);
#mkdir("$TO_VCD_DIR");
#&check_directory ($TO_VCD_DIR,0);
&check_directory ($TO_LOG_DIR,0);
&check_directory ($TO_RESULT_DIR,0);
&create_run_all();
&create_env_info_file($TO_SIM_DIR);

###################################################################
#                    Subroutine declaration                       #
###################################################################

sub print_help {
     print "\nUsage : perl gen_sim.pl -m <model> -t <timing_mode> -p <pat> -exe <sim_exe_path_fn> -cci <cci_path_fn> [-r]\n";
     print "Script is used to generate running simulation batch file for verification.\n";
     print "\n";
     print "<model>            : (required) model name\n";
     print "<timing_mode>      : (required) timing mode lt/at\n";
     print "<pat>              : (required) pat_1/ pat_2\n";
     print "<sim_exe_path_fn>  : (required) path to simulator execution file\n";
     print "<cci_path_fn>      :  path to cci file\n";
     print "-r                 : specify that above paths are relative path.";
     print "                     if this option is not provided, all paths are absolute paths.";
     print "-help              : print help information\n\n";
     exit;
}

#   Description   : load argument from command line
sub load_argument {

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
         # get the number of header column
         case "-exe"                      
         {
            $FOREST_PATH = $cur_value;
         } 
         # get pattern folder path  
#         case "-cci"                      
#         {
#            $CCI_PATH = $cur_value;
#         } 
         # checking memory leak check  # Added by SonTran Nov-18
         case "-memcheck"
         {
             $memcheck = 1;
             $MODEL = $cur_value;
         }
         case "-m"
         {
             $MODEL = $cur_value;
         }
         case "-v"
         {
             $VERSION = $cur_value;
         }
         case "-t"
         {
             $TIME_MODE = $cur_value;
         }
		 
		 # Added by nhuvo 20180615
		 case "-p"
         {
             $PAT = $cur_value;
         }
		 ###
      }
      $CCI_PATH = $FOREST_PATH;  
      switch ($cur_value) 
      {
         case "-help" 
         {
            &print_help();
         } 
         case "-stim"
         {
            $stim_env = 1;
         } 
         case "-vcs"
         {
            $vcs_env = 1;
         } 
         $arg = $cur_value;
      }
    }
    if ($stim_env == 1) {
       if ($FOREST_PATH =~ "../") {
          $relative_path = 1;
       }
    } else {
       if (($FOREST_PATH =~ "../") && ($CCI_PATH =~ "../")) {
           $relative_path = 1;
       } elsif (($FOREST_PATH !~ "../") && ($CCI_PATH !~ "../")) {
           $relative_path = 0;
       } else {
           print "[gen_sim.pl] Error: input paths must be both relative or absolute path.\n";
       }
    }
    if (($FOREST_PATH eq "") || (($stim_env == 0)&&($CCI_PATH eq ""))){
        print "[gen_sim.pl] Error: input path missed.\nUse option '-help' for syntax info.\n";
        exit;
    }
}

#   Description   : read "TM_path.txt" to get the TM list
sub read_TM_path {

    open(TM_PATH_FILE, "$TM_PATH_TXT") || die ("[gen_sim.pl] Error: Cannot open TM_path.txt.\n");
    my @INPUT_PATH = <TM_PATH_FILE>;
    my $tm_indx = 0;
    
    foreach my $tm_path (@INPUT_PATH) {
        $tm_path =~ s/ +//;
        $tm_path =~ s/\t+//;
        $tm_path =~ s/#.*$//;
        
       if (length($tm_path) != 1) {
          ($TM_PATH_FULL[$tm_indx] = $tm_path) =~ s/\*//;
          $TM_PATH[$tm_indx] = $tm_path;
          $TM_PATH[$tm_indx] =~ s/\.[^.]+$//;
          ($TM_NAME[$tm_indx] = $TM_PATH[$tm_indx]) =~ s/^.*\///;
          ($TM_LOCATION[$tm_indx] = $TM_PATH[$tm_indx]) =~ s/\/[^\/]+$/\//;
          $tm_indx++;
       }
    }
}

#   Description   : check if the input directory exist.
#                   If directory exist, check the content of
#                   that directory and ask the user whether
#                   to delete them and creat new sub-directory
#                   structure.
#                   -> applied for 'sim', 'vcd', 'log' dir
#                   ($_[1] is to pass to "make_directory")
sub check_directory {

    my ($directory_to_check,$creat_batch_files) = @_;

    my $dir_exist = 1;
    opendir(SDIR, $directory_to_check) || ($dir_exist = 0);

    # directory not exist
    if ($dir_exist == 0) {
        print "\n[gen_sim.pl] Error: $directory_to_check directory not exist.\n\n";
        exit;
    }
    # directory exist
    else
    {
        my @files = readdir(SDIR);

        # directory is not empty
        if ($#files ne 1) {
            my $delete = "";
            # ask user whether to delete
            while ( ($delete ne "Y") && ($delete ne "N") ) {
                print "\n[gen_sim.pl] Warning: $directory_to_check directory already contains some files or sub-directories.\nRunning this script will remove all of them. Do you want to continue? (Y/N) ";
                $delete = <STDIN>;
                chop($delete);
            }

            # user choose "Y"
            if ($delete eq "Y") {
                print "Clean $directory_to_check dir.\n";
                system "rm -rf $directory_to_check/*";
                &make_directory($directory_to_check, $creat_batch_files);
            }
            # user choose "N"
            else {
                print "\n[gen_sim.pl] Info: $directory_to_check directory not changed!\n";
                if ($creat_batch_files == 1) { # if user choose not to remove old batch files -> exit
                    exit;
                }
            }

        }
        # directory is empty
        else {
            &make_directory($directory_to_check, $creat_batch_files);
        }
    }
}

#   Description   : make sub-directories based on category subdivision and create batch files if requested
sub make_directory {

    my ($parent_dir, $creat_batch_files) = @_;
    # Make list all test pattern   
    open (ALL_LIST_FILE, ">>$parent_dir/all_list.txt") || die ("[gen_sim.pl] Error: Unable to create list of all test patterns.\n"); # Create list file
    for (my $i=0; $i<=$#TM_PATH_FULL; $i++) {
        printf ALL_LIST_FILE "$TM_PATH_FULL[$i]";
    }

    @LAST_TM_LOCATION_SPLIT = ();
    for (my $i=0; $i<=$#TM_PATH ;$i++) {

        # if location of current TM differs from last TM's -> make new directory
        my $last_location = "";
        if ($i != 0) {
            $last_location = $TM_LOCATION[$i-1];
        }
        if (($TM_LOCATION[$i] ne $last_location)||($#TM_LOCATION == 0)) {

            my @TM_LOCATION_SPLIT = split(/\//,$TM_LOCATION[$i]);
            $DIR = $parent_dir; # parent directory where sub-directories put in
            my $REF_DIR = "";   # reference directory 

            # find the already exist directory by comparing location of current TM and last TM
            # make sub-directories from this directory

            my $j = 0;
            while ( ($j<=$#TM_LOCATION_SPLIT) && ($TM_LOCATION_SPLIT[$j] eq $LAST_TM_LOCATION_SPLIT[$j]) ) {
                $DIR = $DIR."/".$TM_LOCATION_SPLIT[$j];
                $REF_DIR = $REF_DIR."/".$TM_LOCATION_SPLIT[$j];
                $j++;
            }

            @LAST_TM_LOCATION_SPLIT = @TM_LOCATION_SPLIT;

            # make sub-directories and category run-sim batch file
            for (my $k=$j; $k<=$#TM_LOCATION_SPLIT ;$k++) {
                # Son add Nov-6-09------#
                my @folder_path = split(/$TM_LOCATION_SPLIT[$k]/,$TM_LOCATION[$i]);  
                my $check_in_path = $TM_LOCATION[$i];
                $check_in_path =~ s/\w+\/$//;
                $check_in_path =~ s/\w+\/$//;
                $check_in_path = $folder_path[0].$TM_LOCATION_SPLIT[$k];
                my @check_in_path_split = split(/\//,$check_in_path);
                # ----------------------#
                $DIR = $DIR."/".$TM_LOCATION_SPLIT[$k];
                $REF_DIR = $REF_DIR."/".$TM_LOCATION_SPLIT[$k];
                print "make dir $DIR\n";
                (my $MEMCHECK_DIR = $DIR) =~ s/\/sim\//\/reports\/memcheck\//;
                mkdir($DIR);
                mkdir($MEMCHECK_DIR);
                if ($creat_batch_files == 1) {
                    my $tm_prefix = "";
                    if ($k == 0) {
                        $tm_prefix = "../";
                    }
                    else {
                        for (my $prefix_indx=0; $prefix_indx<=$#check_in_path_split ;$prefix_indx++) {   # modified by Son Nov-9
                            $tm_prefix = $tm_prefix."../";
                        }
                    }
                    &create_subcat_batch_file ($TM_LOCATION_SPLIT[$k]);
                    my $category_name = lc ($TM_LOCATION_SPLIT[$k]);
                    &write_batch_file ($TM_LOCATION_SPLIT[$k],$tm_prefix);
                    open (LIST_FILE, ">>$DIR/$category_name\_list.txt") || die ("[gen_sim.pl] Error: Unable to create list of test patterns.\n");         # Create list file
                    open (MOT      , ">>$DIR/compile_tm_$category_name.csh") || die ("Unable to open compile_tm_$category_name.csh"); #Chau Nguyen 120512
                    
                    printf MOT ("foreach tp (`cat $category_name\_list.txt | grep -ve \"^#\" | sed \"s/\.cpp//\"`)\n");
                    printf MOT ("    if (-e $tm_prefix../pat/$check_in_path/\$tp.elf) then \n");
                    printf MOT ("        rm -rf $tm_prefix../pat/$check_in_path/\$tp.elf  \n");
                    printf MOT ("    endif\n");
                    printf MOT ("    pushd $tm_prefix../scripts_linux/gen_mot/\n");
                    printf MOT ("    setenv COMMON_DIR $tm_prefix"."0_common_files\n");
                    printf MOT ("    perl gen_mot.pl -f ../../pat/$check_in_path/\$tp.cpp -i ../../pat/0_common_files/ \n");
                    printf MOT ("    popd\n");
                    printf MOT ("    if (-e $tm_prefix../pat/$check_in_path/\$tp.elf) then \n");
                    printf MOT ("    else\n");
                    printf MOT ("        exit \n");
                    printf MOT ("    endif\n");
                    printf MOT ("end\n");
                    
                    printf MOT ("\n");
                    
                    for (my $list_indx=0; $list_indx<=$#TM_PATH ;$list_indx++) {
                    #printf"$TM_PATH[$list_indx] ### $TM_LOCATION[$i]\n";
                        if ($TM_PATH[$list_indx] =~ "$check_in_path") {          # modified by Son Nov-9
                            my $tm_name = "";
                            $REF_DIR =~ s/^\///;
                            ($tm_name = $TM_PATH_FULL[$list_indx]) =~ s/$REF_DIR//;
                            $tm_name =~ s/^\///;
                            printf LIST_FILE ("$tm_name");  
                        }
                    }
                    close (LIST_FILE);
                    close (MOT);
                }
            }
        }
    }
}

#   Description   : create new batch files, only called by 'make_directory' subroutine.
sub create_subcat_batch_file {
    my $category_name = lc($_[0]);
    open (CAT,">$DIR/run_$category_name.csh");
#    open (CAT_VCD,">$DIR/run_$category_name\_vcd.csh");
#    open (CAT_VCD_DEBUG,">$DIR/run_$category_name\_vcd_debug.csh");

    # Chau Nguyen 12052012
    open (MOT,">$DIR/compile_tm_$category_name.csh");

    print CAT ("#!/bin/csh -f\n\n");
#    print CAT_VCD ("#!/bin/csh -f\n\n");
#    print CAT_VCD_DEBUG ("#!/bin/csh -f\n\n");

    # Chau Nguyen 120512
    print MOT ("#!/bin/csh -f\n\n");

    close(CAT);
#    close(CAT_VCD);
#    close(CAT_VCD_DEBUG);

    # Chau Nguyen 120512
    close(MOT);

    system "chmod +x $DIR/run_$category_name.csh";
#    system "chmod +x $DIR/run_$category_name\_vcd.csh";
#    system "chmod +x $DIR/run_$category_name\_vcd_debug.csh";

    # Chau Nguyen 120512
    system "chmod +x $DIR/compile_tm_$category_name.csh";

    # make link to Forest_SCI.txt and CCI file
    $need_to_link = 1;
    &make_link($DIR);

    $dir_count++;
    ($DIR_LIST[$dir_count] = $DIR) =~ s/$TO_SIM_DIR\/// ;
}

#   Description   : generate 3 kind of batch files:
#                   - run_...           : standard simulation (with log files generated).
#                   - run_..._vcd       : standard simulation + waveform files.
#                   - run_..._vcd_debug : standard simulation + waveform + debug mode.
sub write_batch_file {
    my $category_name = lc(@_[0]);
    open (CAT, ">>$DIR/run_$category_name.csh") || die ("Unable to create category batch file.\n");
#    open (CAT_VCD, ">>$DIR/run_$category_name\_vcd.csh") || die ("Unable to create category batch file.\n");
#    open (CAT_VCD_DEBUG, ">>$DIR/run_$category_name\_vcd_debug.csh") || die ("Unable to create category batch file.\n");
    my @COMMAND_LINES = &print_command_line (@_[0],@_[1]);
    # print command lines for current TM to current category's batch files
    print CAT ("$COMMAND_LINES[0]");
#    print CAT_VCD ("$COMMAND_LINES[1]");
#    print CAT_VCD_DEBUG ("$COMMAND_LINES[2]");
    close(CAT);
#    close(CAT_VCD);
#    close(CAT_VCD_DEBUG);
}

#   Description   : write the command lines for each TM (3 kind of command lines for 3 kind of batch files)
sub print_command_line {
    my @COMMAND_LINE = ();
    my $category_name = lc(@_[0]);
    (my $category_dir = $DIR) =~ s/^.*sim\///;
    $COMMAND_LINE[0] = "echo \"@_[0] category\"\nrm -rf run_exec.csh\n";
    if (@_[0] eq "all") {
        $COMMAND_LINE[0] = $COMMAND_LINE[0]."perl create_batch.pl -l $SIM_DIR/$category_name\_list.txt";
#        $COMMAND_LINE[1] = $COMMAND_LINE[0]." -vcd"." $VCD_DIR";
    }else {
        $COMMAND_LINE[0] = $COMMAND_LINE[0]."perl create_batch.pl -l @_[1]$SIM_DIR/$category_dir/$category_name\_list.txt";
#        $COMMAND_LINE[1] = $COMMAND_LINE[0]." -vcd"." @_[1]"."$VCD_DIR/$category_dir";
    }
    $COMMAND_LINE[2] = $COMMAND_LINE[1]." -debug";
    my $FOREST_EXEC = $FOREST_PATH;
    if (($relative_path == 1)&&(@_[0] ne "all")) {
       $FOREST_EXEC =  "@_[1]"."$FOREST_PATH";
    }
    (my $CCI_FILE =  $CCI_PATH) =~ s/^.+[\/]+//;
    
    my $RUN_COMMAND = " -exe $FOREST_EXEC";
    if ($vcs_env == 1) {
       $RUN_COMMAND = $RUN_COMMAND . " -vcs";
    }
    if ($stim_env == 0) {
       $RUN_COMMAND = $RUN_COMMAND." -src $CCI_FILE";          # for CPU Environment 
    }else {
       $RUN_COMMAND = $RUN_COMMAND." -stim";                   # for Stimulus Environment 
    }

    # checking memory leak check
    if ($memcheck == 1) {            # Added by SonTran Nov-16
        $RUN_COMMAND = $RUN_COMMAND." -memcheck $MODEL";
    }

    if ($relative_path == 1) {
       $RUN_COMMAND = $RUN_COMMAND." -r\n./run_exec.csh\n\n";                                      
    }
    else {
       $RUN_COMMAND = $RUN_COMMAND."\n./run_exec.csh\n\n";                                      
    }
    $COMMAND_LINE[0] = $COMMAND_LINE[0].$RUN_COMMAND;
    $COMMAND_LINE[1] = $COMMAND_LINE[1].$RUN_COMMAND;
    $COMMAND_LINE[2] = $COMMAND_LINE[2].$RUN_COMMAND;
    my $CHECK_RESULT = "";
    if ($category_name eq "all") {
        $CHECK_RESULT = "pushd $CHECK_RESULT_DIR\n";
        if ($VERSION ne "") {
			$CHECK_RESULT = $CHECK_RESULT."perl check_result_osci.pl -m $MODEL -v $VERSION -t $TIME_MODE -p $PAT\n"; #added '-p $PAT' by nhuvo 20180615
        } else {
            $CHECK_RESULT = $CHECK_RESULT."perl check_result_osci.pl -m $MODEL -t $TIME_MODE -p $PAT\n"; #added '-p $PAT' by nhuvo 20180615
        }
    }
    else {
        $CHECK_RESULT = "pushd @_[1]$CHECK_RESULT_DIR\n";
        if ($VERSION ne "") {
            $CHECK_RESULT = $CHECK_RESULT."perl check_result_osci.pl -c $category_dir -m $MODEL -v $VERSION -t $TIME_MODE -p $PAT\n"; #added '-p $PAT' by nhuvo 20180615
        } else {
            $CHECK_RESULT = $CHECK_RESULT."perl check_result_osci.pl -c $category_dir -m $MODEL -t $TIME_MODE -p $PAT\n"; #added '-p $PAT' by nhuvo 20180615
        }
    }
    $CHECK_RESULT = $CHECK_RESULT."popd\n"; 
    $COMMAND_LINE[0] = $COMMAND_LINE[0].$CHECK_RESULT;
    $COMMAND_LINE[1] = $COMMAND_LINE[1].$CHECK_RESULT;
    $COMMAND_LINE[2] = $COMMAND_LINE[2].$CHECK_RESULT;
 
    return (@COMMAND_LINE);
}


#   Description   : make link to Forest_SCI and cci file in each batch files' sub-directories
sub make_link {

    my $dest = $_[0];

    (my $short_dest = $dest) =~ s/$TO_SIM_DIR//;
    
    my $env_path;
    my $exe_path;
    my $cci_path;
    my $create_bat_path;

    my $path_prefix = "";
    my @dest_split = split(/\//,$short_dest);
    for (my $i=1; $i<=$#dest_split ;$i++) {
        $path_prefix = $path_prefix."../";
    }

#    ($exe_path = $FOREST_PATH) =~ s/\/[^\/]+$/\/$ENV_SCI_FILE/;
    ($env_path = $FOREST_PATH) =~ s/\/[^\/]+$/\/$ENV_INFO_TXT/;
    $cci_path = $CCI_PATH;
    $create_bat_path = $CREATE_BAT_PATH;
    if ($relative_path == 1) {
        $env_path = $path_prefix.$env_path;
        $exe_path = $path_prefix.$exe_path;
        $cci_path = $path_prefix.$cci_path;
        $create_bat_path = $path_prefix.$create_bat_path;
    }
    if ($stim_env == 0) {                            # for CPU Environment
       system "ln -fs $cci_path $dest";
       system "ln -fs $path_prefix../scripts_linux/gen_sim/env/MEV.map $dest";
       system "ln -fs $path_prefix../scripts_linux/gen_sim/env/BUS.map $dest";
       system "ln -fs $path_prefix../scripts_linux/gen_sim/env/FASTISS.map $dest";
       system "ln -fs $path_prefix../scripts_linux/gen_sim/env/CAISS.map $dest";
       system "ln -fs $path_prefix../scripts_linux/gen_sim/run_multi.skl $dest";
    }
    if (!(-e "$dest/$CREATE_BAT")) {
       symlink "$create_bat_path","$dest/$CREATE_BAT";
    }

    (my $source = $dest) =~ s/\.\.\/\.\.\/sim\///;

    # search for all files in corresponding category in 'pat'
    opendir(DIR,"$TO_PAT_DIR/$source/");
    my @files = readdir(DIR);
    for (my $i=0; $i<=$#files ;$i++) {
        if (!(-d "$TO_PAT_DIR/$source/$files[$i]")) {
            system "cp -f $path_prefix./pat/$source/$files[$i] $dest";
            if($files[$i] =~ "tp0"){
                my @tm_name = split(/\./,$files[$i]);
                $tm_name[0] =~ s/_lt//;
                $tm_name[0] =~ s/_at//;
                if (-e "$path_prefix./pat/$source/$tm_name[0].cfg") {
                    system "cp -f $path_prefix./pat/$source/$tm_name[0].cfg $dest/$tm_name[0].cfg";
                } elsif (-e "$path_prefix./pat/$source/$tm_name[0]_$TIME_MODE.cfg" && !(-e "$path_prefix./pat/$source/$tm_name[0].cfg")) {
                    system "cp -f $path_prefix./pat/$source/$tm_name[0]_$TIME_MODE.cfg $dest/$tm_name[0].cfg";
                } else {
                    &create_config_file ($dest, $tm_name[0]);
                }
                system "cp -f $path_prefix./scripts_linux/gen_sim/simmode_$TIME_MODE.py $dest/simmode.py"; # revised by chanle 20170716 for ICUS221
                system "cp -f $path_prefix./pat/0_common_files/*.py $dest/"; # added by chanle 20170716 for ICUS221
                if (-e "$path_prefix./pat/$source/$tm_name[0].py") { 
                    system "cp -f $path_prefix./pat/$source/$tm_name[0].py $dest/$tm_name[0].py";
                } elsif(-e "$path_prefix./pat/$source/$tm_name[0]_$TIME_MODE.py" && !(-e "$path_prefix./pat/$source/$tm_name[0].py")) {
                    system "cp -f $path_prefix./pat/$source/$tm_name[0]_$TIME_MODE.py $dest/$tm_name[0].py";
                } else {
                    system "cp -f $path_prefix./scripts_linux/gen_sim/default_pythonfile.py $dest/$tm_name[0].py";
                }
				
				#Add by nhuvo 20180531
                if (!(-e "$path_prefix./pat/$source/$tm_name[0].hex"))
                {
                    system "cp -f $path_prefix./scripts_linux/gen_sim/default_hexfile.hex $dest/$tm_name[0].hex";
                }
				###

                if($TIME_MODE =~ "at") {
                    &create_run_multi($dest, $tm_name[0]);
                }
            }
        }
    }
    
}

#   Description   : create run all batch file                            
sub create_run_all {

    open(RUN, ">>$TO_SIM_DIR/run_all.csh") || die ("[gen_sim.pl] Error: Unable to open output batch file.\n");
#    open(RUN_VCD, ">>$TO_SIM_DIR/run_all_vcd.csh") || die ("[gen_sim.pl] Error: Unable to open output batch file.\n");
#    open(RUN_VCD_DEBUG, ">>$TO_SIM_DIR/run_all_vcd_debug.csh") || die ("[gen_sim.pl] Error: Unable to open output batch file.\n");
    print RUN ("#!/bin/csh -f\n\n");
#    print RUN_VCD ("#!/bin/csh -f\n\n");
#    print RUN_VCD_DEBUG ("#!/bin/csh -f\n\n");
    $DIR = ""; 
    my @COMMAND_LINES = &print_command_line ("all","../");
    # print command lines for current TM to current category's batch files
    print RUN ("$COMMAND_LINES[0]");
#    print RUN_VCD ("$COMMAND_LINES[1]");
#    print RUN_VCD_DEBUG ("$COMMAND_LINES[2]");
    close(RUN);
#    close(RUN_VCD);
#    close(RUN_VCD_DEBUG);
    system "chmod +x $TO_SIM_DIR/run_all.csh";
#    system "chmod +x $TO_SIM_DIR/run_all_vcd.csh";
#    system "chmod +x $TO_SIM_DIR/run_all_vcd_debug.csh";
    
    # make link to Forest_SCI.txt and CCI file
    &make_link($TO_SIM_DIR);
}

sub create_env_info_file 
{
#{{{
    my $break_flag = 1;     # break flag used to get name of source file
    my $path_prefix = "";
    my $source_path = "";
    my @source_name = ();
    my @source_info = ();  # each source file revision information
    my @rev_info    = ();  # all source files revision information
    (my $short_dest = $_[0]) =~ s/$SIM_DIR//;
    my @dest_split = split(/\//,$short_dest);
    for (my $i=1; $i<=@dest_split ;$i++) {
        $path_prefix = $path_prefix."../";
    }
    # Process to get revision information of source files in Makefile
    (my $MAKEFILE = $FOREST_PATH) =~ s/\/[^\/]+$/\/Makefile/;
    if ($relative_path == 1) {
        $MAKEFILE = $path_prefix.$MAKEFILE;
    }
    open (MAKEFILE, "$MAKEFILE") || die ("Cannot open $MAKEFILE.\n");
    while (<MAKEFILE>) {
        ($source_path = $MAKEFILE) =~ s/Makefile//;
        (my $line = $_) =~ s/ +//;
        $line =~ s/\t+//;
        $line =~ s/#.*$//;
        if (length($line) != 1) {
            if (($line =~ /SRCS= /) || ($line =~ /HEDS= /) || ($break_flag == 0)) {
                $break_flag = 0;
                $line =~ s/SRCS= //;
                $line =~ s/HEDS= //;
                @source_name = split(/\ /,$line);
                foreach my $name (@source_name) {
                    if ((length($name) != 0) && !($name =~ /\\/) && ($name =~ /\$/)) {
                            (my $common_path = $name) =~ s/^\$\(//;
                            $common_path =~ s/\).*//;
                            $source_path = $ENV{$common_path};
                            $source_path = $source_path."\/";
                            $name =~ s/^.*\)\///;
                            $name =~ s/\s*\\\n//;
                            @source_info = &get_source_id($source_path, $name);
                            if ($#source_info >= 5) {
                                @rev_info = (@rev_info,"$source_path$name $source_info[2] $source_info[5]\n");
                            }
                    }
                }
                if (!($source_name[$#source_name] =~ /\\/)) {
                    $break_flag = 1;
                }
            }
        }
    }
    # Process to get Environment information
    (my $rev_info_file = $path_prefix.$FOREST_PATH) =~ s/\/[^\/]+$/\/$REV_INFO_TXT/;
    if (-e $rev_info_file) {
        open (REV_INFO_TXT, "<$rev_info_file") || die ("Cannot open $rev_info_file.\n");
    }
    open (REV_INFO_TMP, ">>rev_info.tmp") || die ("Cannot open rev_info.tmp.\n");
    my @chk_dup_name = ();
    while (<REV_INFO_TXT>) {
        my $print_flag = 1;
        if (length($_) != 1) {
            (my $old_source_name = $_) =~ s/[\ ]+.*$//;
            $old_source_name =~ s/\n//;
            foreach my $name (@chk_dup_name){
                if ($name =~ $old_source_name) {
                    $print_flag = 0;
                    last;
                }
            }
            if ($print_flag == 1) {
                @chk_dup_name = (@chk_dup_name, $old_source_name);
                printf REV_INFO_TMP "$_";
            }
        }
    }
    foreach my $rev_info (@rev_info) {
        ($source_file, $revision, $author) = split (/\ /, $rev_info);
        @chk_dup_name = (@chk_dup_name,$source_file);
        select(REV_INFO_TMP);
        $~ = "REV_INFO_FORMAT";
        write;
    }
    close (REV_INFO_TXT);   
    close (REV_INFO_TMP);   
    system("mv rev_info.tmp $rev_info_file");
    close (MAKEFILE);   
#}}}
}

sub get_source_id 
{
#{{{
    my @source_info = ();
    open (SOURCE_FILE, $_[0].$_[1]) || die ("Cannot open $_[0].$_[1].\n");
    while (<SOURCE_FILE>) {
        (my $source_line = $_) =~ s/ +//;
        $source_line =~ s/\t+//;
        $source_line =~ s/^[^\/\/]*//;
        if (($source_line =~ /\/\/.*/) && (($source_line =~ /\/\/\$Id/) || ($source_line =~ /\/\/Id/))){
            $source_line =~ s/Id\ */Id/;
            $source_line =~ s/Id:\ */Id:\ /;
            @source_info = split (/\ /, $source_line);
        }
    }
    return @source_info;
#}}}
}

#==================================================#
# function: create config file                     #
# input   : path to test pattern directory         #
# ouput   : cfg files of test patterns             #
# date    : 01/22/13                               #
# content : create config file from configfile.skl #
#==================================================#
sub create_config_file{
    #{{{
    my $path = $_[0];
    my $tm_name = $_[1];
    my $cf_file_skl = $CONFIG_SKL_PATH."configfile_$TIME_MODE.skl";
    my $cf_file = shift;
    unless(open(CF_SKL,$cf_file_skl)){
        die("<error> can not open $cf_file_skl file !\n");
        exit;
    }
    unless(open(CF,">$path\/$tm_name.cfg")){
        die("Can not create new config file !\n");
        exit;
    }
    
    my @content = <CF_SKL>;
    for(my $i=0;$i<=$#content;$i++){
        if($content[$i] =~ /\%\%TM_NAME/){       #destination path replace
        $content[$i] =~ s/\%\%TM_NAME/$tm_name/g;
        }
        if($content[$i] =~ /\/\//){ 
            $content[$i] =~ s/\/\//\//g;
        }
        print CF $content[$i];
    }
    close(CF);
    close(CF_SKL);
    #}}}
}

#==================================================#
# function: create MULTI execution filei           #
# input   : path to test pattern directory         #
# ouput   : run_multi.csh files of test patterns  #
# date    : 07/08/13                               #
# content : create config file from run_multi.skl #
#==================================================#
sub create_run_multi{
    #{{{
    my $path = $_[0];
    my $tm_name = $_[1];
    my $cf_file_skl = $CONFIG_SKL_PATH."run_multi.skl";
    my $cf_file = shift;
    unless(open(CF_SKL,$cf_file_skl)){
        die("<error> can not open $cf_file_skl file !\n");
        exit;
    }
#    unless(open(CF,">$cf_file")){
    unless(open(CF,">$path\/run_multi.csh")){
        die("Can not create new config file !\n");
        exit;
    }
    
    my @content = <CF_SKL>;
    for(my $i=0;$i<=$#content;$i++){
        if($content[$i] =~ /\%\%TM_NAME/){       #destination path replace
        $content[$i] =~ s/\%\%TM_NAME/$tm_name/g;
        
        }
        if($content[$i] =~ /\/\//){ 
            $content[$i] =~ s/\/\//\//g;
        }
        printf CF $content[$i];
    }
    close(CF);
    system "chmod +x $path\/run_multi.csh";
    close(CF_SKL);
    #}}}
}

##==================================================#
## function: create python interface file           #
## input   : path to test pattern directory         #
## ouput   : py files of test patterns              #
## date    : 01/22/13                               #
## content : create python if file from             #
##           pythonfile.skl                         #
##==================================================#
#sub create_pythonif_file{
#    #{{{
#    my $py_file_skl = $PYTHONIF_SKL_PATH."\/pythonfile.skl";
#    my $py_file = shift;
#    unless(open(PY_SKL,$py_file_skl)){
#        die("<error> can not open $py_file_skl file !\n");
#        exit;
#    }
#    unless(open(PY,">$py_file")){
#        die("Can not create new python interface file !\n");
#        exit;
#    }
#    
#    my @content = <PY_SKL>;
#    for(my $i=0;$i<$#content;$i++){
#        if($content[$i] =~ /\%\%TM_NAME/){       #destination path replace
#        $content[$i] =~ s/\%\%TM_NAME/$TM_NAME/g;
#        }
#        if($content[$i] =~ /\/\//){ 
#            $content[$i] =~ s/\/\//\//g;
#        }
#        print PY $content[$i];
#    }
#    close(PY);
#    close(PY_SKL);
#    #}}}
#}

format REV_INFO_FORMAT = 
      @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< @<<<<<<<<<<<<<<<<<<< @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
$source_file $revision $author
.
