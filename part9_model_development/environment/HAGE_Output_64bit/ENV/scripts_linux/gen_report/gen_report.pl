#! /usr/bin/perl -I./Bin

#----------------------------------------------------------------------
# Status    :
#    $Source: /shsv/sld/ipp/project/2012/rel/201207_prj_m40_SC_HEAP/Work/tungbui/11_Release/02_Final_Version/092712_M40_SYS_CVSROOT/Output/sim/scripts_linux/gen_report/gen_report.pl,v $
#    $Revision: 1.1.1.1 $
#    $Date  : $
#    $Author: chaunguyen $
#    $Locker:  $
#    $State : $
#    $Id: gen_report.pl,v 1.1.1.1 2012/09/27 10:12:25 chaunguyen Exp $
#----------------------------------------------------------------------
#  (C) Copyright 2009   RVC (Renesas Design Vietnam Co., Ltd.)
#  All rights reserved. RVC Confidential Proprietary
#  (C) Copyright 2009   RENESAS Technology Corp.  All rights reserved.
#----------------------------------------------------------------------

use strict;
#use Spreadsheet::ParseExcel;
#use Spreadsheet::ParseExcel::SaveParser;
use Switch;

my $REPORTS_DIR = "../../reports";
my $RESULTS_DIR = "../../results";
my $TM_PATH_DIR = "../../pat";

my $CAT     = ();
my @CAT_SEL = ();
my $cat_sel_count = 0;
my $checklist_update = 0;
my $last_pos;
my $count;
my $date;

my @TM_PATH    = ();
my @TM_FILENAME=();
my @TM_NAME    = ();
my @TM_DATE    = ();
my @TM_RESULT  = ();
my @TM_CPUTIME = ();
#my @TM_MEMORY  = ();

my @DATE_LIST  = ();
my $date_count = -1;

my $excel;
my $template;
my $check_list;

my $cur_pos;
my $cur_tm;
my $cur_date;

my $start_date;
my $finish_date;
my $pass_count;
my $fail_count;
my $cpu_time;
#my $memory;
###################################################################
#                           Main process                          #
###################################################################

&load_argument ();
&load_result_list ();
&write_summary_report ();
&write_detail_report ();
&sort_date_list ();
print STDOUT ("<INFO> All reports are stored in $REPORTS_DIR.\n");

if ($checklist_update == 1) {

    if (-e $check_list) {
        # Open the template with SaveParser 
        $excel     = new Spreadsheet::ParseExcel::SaveParser;
        $template  = $excel->Parse ($check_list);

        # Write new data into detail execution sheet
        &update_general_sheet ();
        &update_execution_sheet ();

        # Save file with new value
        $excel->SaveAs ($template,$check_list);
        print STDOUT ("<INFO> Reports are updated to the checklist: $check_list\n");
    }
    else {
        print STDOUT ("<ERROR> The checklist: $check_list does not exist.\n");
    }
}

###################################################################
#                    Subroutine declaration                       #
###################################################################

sub print_help {
     print "\nUsage : perl gen_report.pl [-c <category>] [-l <checklist>]\n";
     print "Script is used to generate simulation reports.\n";
     print "\n";
     print "-c <category>   : check result for specified TM category.\n\n";
     print "-l <checklist>  : add reports sheet to specified checklist.\n\n";
     print "-help           : print help information.\n";
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
         case "-c"       
         {
            $CAT = $cur_value;
            # omit "/" at the beginning and the end of input category
            $CAT =~ s/^[\/]*//;
            $CAT =~ s/[\/]*$//;
         } 
         case "-l"        
         {
            $checklist_update = 1;
            $check_list = $cur_value;
         } 
      }
  
      switch ($cur_value) 
      {
         case "-help" 
         {
            &print_help();
         } 
         $arg = $cur_value;
      }
    }

}

#   Description   : read "TM_path.txt" to get list of directory containing "result.txt"
sub load_result_list {

    open(TM_PATH_FILE, "$TM_PATH_DIR/TM_path.txt") || die ("Unable to open TM_path.txt.\n");
    @TM_PATH = <TM_PATH_FILE>;
    close(TM_PATH_FILE);

    @CAT_SEL = ();
    # if input category contains 'result.txt', it is stored in @CAT_SEL
    # if no input category, @CAT_SEL stores all category which contains 'result.txt' (no duplication)

    for (my $i=0; $i<=$#TM_PATH ;$i++) {

        # @TM_PATH lists sub-directories containing "result.txt" (may contains duplication)

        # remove "*" mark at the end of TM_PATH
        $TM_PATH[$i] =~ s/\*//;

        ($TM_FILENAME[$i] = $TM_PATH[$i]) =~ s/^.*\///;

        # remove TM's filename at the end of TM_PATH
        $TM_PATH[$i] =~ s/\/$TM_FILENAME[$i]//;

        if ($CAT eq "") {
            &load_result ($TM_PATH[$i]);
        }
        else {
            # check if input category argument match with current category
            my $TM_PATH_REFINE = "/".$TM_PATH[$i]."/";
            my $CAT_REFINE     = "/".$CAT."/";
            if ($TM_PATH_REFINE =~ /$CAT_REFINE/) {
                &load_result ($TM_PATH[$i]);
            }
        }

    }

    if ($#CAT_SEL < 0) {
        print "<ERROR> Specified category does not exist. No report generated.\n";
        exit;
    } # input category does not exist

    print STDOUT "<INFO> Categories to be checked for reports:\n";
    for (my $i=0; $i<=$#CAT_SEL ;$i++) {
        print STDOUT "-- $CAT_SEL[$i]\n";
    }
}

#   Description   : check & add current category to @CAT_SEL
sub load_result {

    my $current_category = $_[0];

    # check if current category already in the CAT_SEL
    my $exist = 0;
    for (my $j=0; $j<$cat_sel_count ;$j++) {
        if ($CAT_SEL[$j] eq $current_category) {
            $exist = 1;
        }
    }

    # if not yet exist -> add to CAT_SEL
    if ($exist == 0) {
        $CAT_SEL[$cat_sel_count] = $current_category;
        $cat_sel_count++;
    }
}

#   Description   : generate summary report
sub write_summary_report {

    if ($CAT eq "") {
        open(SUMMARY_REPORT,">$REPORTS_DIR/summary_report.txt");
    }else {
        (my $prefix_name = $CAT) =~ s/\//\_\_/;
        $prefix_name =~ s/\W/\_/;
        open(SUMMARY_REPORT,">$REPORTS_DIR/$prefix_name\_summary_report.txt");
    }
    print SUMMARY_REPORT ("=====================================================================================================\n");
    print SUMMARY_REPORT ("=                                             SUMMARY REPORT                                        =\n");
    print SUMMARY_REPORT ("=====================================================================================================\n");
    open (SDATE,"date|");
    $date = <SDATE>;
    print SUMMARY_REPORT (" $date");
    print SUMMARY_REPORT ("--------------------------------------------------------------------------------------------\n\n");
    print SUMMARY_REPORT (" CATEGORY                                             FROM          TO           PASS   FAIL   CPU_TIME(s)\n\n");

    my $total_pass = 0;
    my $total_fail = 0;
    my $total_cputime = 0;
    my $total_memory  = 0;
    for ($cur_pos=0; $cur_pos<=$#CAT_SEL ;$cur_pos++) {

        &read_detail_result ($CAT_SEL[$cur_pos]);

        ($start_date, $finish_date, $pass_count, $fail_count, $cpu_time) = &category_summary_calculate ($last_pos, $count);

        select(SUMMARY_REPORT);
        $~ = "SUMMARY_REPORT_FORMAT";
        write;

        $total_pass += $pass_count;
        $total_fail += $fail_count;
        $total_cputime += $cpu_time;
#        $total_memory  += $memory;
    }
    my $total_TM = $total_pass + $total_fail;

    printf SUMMARY_REPORT ("\n\n\nSummary: Total TM : %4d   TMs \n",$total_TM);
    printf SUMMARY_REPORT ("         PASS     : %4d   TMs \n",$total_pass);
    printf SUMMARY_REPORT ("         FAIL     : %4d   TMs \n",$total_fail);
    printf SUMMARY_REPORT ("         CPU_TIME : %3.3f  s \n",$total_cputime);
#    printf SUMMARY_REPORT ("         MEMORY   : %3.3f  MB \n",$total_memory);
    print STDOUT ("<INFO> Summary report generated.\n");
    close(SUMMARY_REPORT);
}

#   Description   : read 'result.txt' files to extract simulation information by day, record to 5 arrays (name, date, result, cputime)
sub read_detail_result () {

    my $category = $_[0];
    my $str;
    if ($_[1] eq "") {
        $str = "\/";
    }
    else {
        $str = $_[1];
    }

#    print STDOUT "$RESULTS_DIR/$category/result.txt\n";
    if (-e "$RESULTS_DIR/$category/result.txt") {
        open (RESULT_LOG, "$RESULTS_DIR/$category/result.txt") || die ("Unable to open 'result.txt' file.\n");
        my @RESULT_LINE = <RESULT_LOG>;

        $count = 0; # 'count': total number of TMs' result lines in 'result.txt'
        my $jump_flag  = 0;
        for (my $i=4; $i<=$#RESULT_LINE ;$i++) {

            if ($RESULT_LINE[$i] =~ /^--/ ) {
                $i = $i + 2;
                $jump_flag = 1;
            }

            chop($RESULT_LINE[$i]);
            $RESULT_LINE[$i] =~ s/^ //; # remove the blank space at the beginning of line
            my @RESULT_LINE_SPLIT = split(/ +/,$RESULT_LINE[$i]);

            if ($RESULT_LINE_SPLIT[1] =~ /$str/) {
                $TM_NAME[$count]   = $RESULT_LINE_SPLIT[0];
                $TM_DATE[$count]   = $RESULT_LINE_SPLIT[1];
                $TM_RESULT[$count] = $RESULT_LINE_SPLIT[2];
                $TM_CPUTIME[$count]= $RESULT_LINE_SPLIT[3];
#                $TM_MEMORY[$count] = $RESULT_LINE_SPLIT[4];

                if ($jump_flag == 1) {
                    $last_pos = $count; # position of the first TM of the lastest result
                    $jump_flag = 0;
                }

                &add_new_day($TM_DATE[$count]);

                $count++;
            }
        }
    }else {
        print "[gen_report.pl] Warning: $category/result.txt does not exist \n";
    }
    $count--;

    close RESULT_LOG;
}

#   Description   : summarize simulation information for 1 category (for the 1 time running simulation)
sub category_summary_calculate {

    my ($start_pos, $end_pos) = @_;
    my $pass_count = 0; # number of passed TM
    my $fail_count = 0; # number of failed TM
    my $cpu_time   = 0; # total CPU time
#    my $memory     = 0; # total memory

    # Get above information from the latest result
    for (my $i=$start_pos; $i<=$end_pos ;$i++) {

        if ($TM_RESULT[$i] eq "Pass") {
            $pass_count++;
        }
        elsif ($TM_RESULT[$i] eq "Fail") {
            $fail_count++;
        }

        $cpu_time += $TM_CPUTIME[$i];
#        $memory   += $TM_MEMORY[$i];

    }


    my $start_date  = "99/99/9999"; # the day start run the category
    my $finish_date = "00/00/0000"; # the day finish run the category

    # Get day information from whole simulation result
    for (my $i=0; $i<=$count ;$i++) {

        my $early_start = &day_compare($start_date, $TM_DATE[$i]);
        my $late_finish = &day_compare($TM_DATE[$i], $finish_date);
        if ($early_start > 0) {
            $start_date = $TM_DATE[$i];
        }
        if ($late_finish > 0) {
            $finish_date = $TM_DATE[$i];
        }

    }

    my @ret_val = ($start_date, $finish_date, $pass_count, $fail_count, $cpu_time);
    return(@ret_val);
}

#   Description   : generate detail report
sub write_detail_report {

    if ($CAT eq "") {
        open (DETAIL_REPORT,">$REPORTS_DIR/detail_report.txt");
    }else {
        (my $prefix_name = $CAT) =~ s/\//\_\_/;
        $prefix_name =~ s/\W/\_/;
        open(DETAIL_REPORT,">$REPORTS_DIR/$prefix_name\_detail_report.txt");
    }
    print DETAIL_REPORT ("====================================================================\n");
    print DETAIL_REPORT ("=                           DETAIL REPORT                          =\n");
    print DETAIL_REPORT ("====================================================================\n");
    print DETAIL_REPORT (" $date");
    print DETAIL_REPORT ("--------------------------------------------------------------------\n");
    
    for ($cur_pos=0; $cur_pos<=$#CAT_SEL ;$cur_pos++) {

        # read result.txt for all days
        &read_detail_result ($CAT_SEL[$cur_pos]);

        # category summary for latest result (from $last_pos)
        ($start_date, $finish_date, $pass_count, $fail_count, $cpu_time) = &category_summary_calculate ($last_pos, $count);

        print DETAIL_REPORT ("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        print DETAIL_REPORT ("   CATEGORY :  $CAT_SEL[$cur_pos]\n    PASS    :  $pass_count\n    FAIL    :  $fail_count\n");
        print DETAIL_REPORT ("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        print DETAIL_REPORT (" TM NAME                          DATE         RESULT   CPU_TIME(s)\n\n");

        for ($cur_tm=0; $cur_tm<=$count ;$cur_tm++) {
            select(DETAIL_REPORT);
            $~ = "DETAIL_REPORT_FORMAT";
            write;
        }
    }
    print STDOUT ("<INFO> Detail report generated.\n");
    close(DETAIL_REPORT);
}

#   Description   : add new day to run-simulation day list (if not yet in the list)
sub add_new_day {

    my $new_day = $_[0];
    my $flag = 0;
    # check if input date already in the list of days
    for (my $j=0; $j<=$date_count ;$j++) {
        if ($DATE_LIST[$j] eq $new_day) {
            $flag = 1;
        }
    }

    if ($flag == 0) {
       $date_count++;
       $DATE_LIST[$date_count] = $new_day;
    }
}

#   Description   : compare 2 days
sub day_compare {
    my ($day1, $day2) = @_;
    my @day1_s = split(/\//,$day1);
    my $day1_val   = $day1_s[2]*10000 + $day1_s[0]*100 + $day1_s[1];
    my @day2_s = split(/\//,$day2);
    my $day2_val   = $day2_s[2]*10000 + $day2_s[0]*100 + $day2_s[1];
    my $ret_val= $day1_val - $day2_val;
    return($ret_val);
}

#   Description   : sort days in DATE_LIST in ascending order
sub sort_date_list {

    for (my $i=0; $i<= $date_count; $i++) {
        for (my $j=$i+1; $j<=$date_count ;$j++) {
            if (&day_compare($DATE_LIST[$i], $DATE_LIST[$j]) > 0) {
                my $temp = $DATE_LIST[$i];
                $DATE_LIST[$i] = $DATE_LIST[$j];
                $DATE_LIST[$j] = $temp;
            }
        }
    }
}

#   Description   : update 'Daily Execution' sheet in checklist
sub update_execution_sheet {

    # Updated begining location
    my $updated_sheet       =  2;   # Detail report
    my $updated_row         =  2;   # Begining row
    my $updated_col         =  1;   # Begining column

    # Defination format
    my $format_sheet       =  2;    # Sheet contains format
    my $format_row         =  2;    # Row contains format
    my $format_col         =  1;    # Column contains format

    # Get current cell format
    my $format = $template->{Worksheet}[$format_sheet]->{Cells}[$format_row][$format_col]->{FormatNo};

    for (my $cur_date=0; $cur_date<=$date_count ;$cur_date++) {

        for ($cur_pos=0; $cur_pos<=$#CAT_SEL ;$cur_pos++) {

            # read result.txt to extract simulation result items on specified day
            &read_detail_result ($CAT_SEL[$cur_pos], $DATE_LIST[$cur_date]);

            if ($count >= 0) {

                my $number_of_TM = $count - $last_pos + 1;

                for (my $start_pos=0; $start_pos<=$last_pos ; $start_pos = $start_pos + $number_of_TM) {

                    # category summary for each time running simulation
                    ($start_date, $finish_date, $pass_count, $fail_count, $cpu_time) = &category_summary_calculate ($start_pos, $start_pos + $number_of_TM - 1);

                    # update information for detail execution 
                    my $category  = $CAT_SEL[$cur_pos];
                    my $date      = $start_date;
                    my $pass      = $pass_count;
                    my $fail      = $fail_count;
                    my $cpu       = $cpu_time;
#                    my $memory    = $memory; 

                    # Category information
                    $template->AddCell ($updated_sheet,$updated_row,$updated_col  ,$category,$format);
                    # Date information
                    $template->AddCell ($updated_sheet,$updated_row,$updated_col+1,$date    ,$format);
                    # Pass information
                    $template->AddCell ($updated_sheet,$updated_row,$updated_col+2,$pass    ,$format);
                    # Fail information
                    $template->AddCell ($updated_sheet,$updated_row,$updated_col+3,$fail    ,$format);
                    # CPU hour information
                    $template->AddCell ($updated_sheet,$updated_row,$updated_col+4,$cpu     ,$format);
                    # Memory information
#                    $template->AddCell ($updated_sheet,$updated_row,$updated_col+5,$memory  ,$format);

                    $updated_row++;
                }
            }
        }
    }
}

#   Description   : update 'General' sheet in checklist
sub update_general_sheet {

    # Updated begining location
    my $updated_sheet       =  0 ;   # General report
    my $updated_row         =  20;   # Begining row
    my $updated_col         =  1 ;   # Begining column

    # Defination format
    my $format_sheet       =  0 ;    # Sheet contains format
    my $format_row         =  20;    # Row contains format
    my $format_col         =  1 ;    # Column contains format

    # Get current cell format
    my $format = $template->{Worksheet}[$format_sheet]->{Cells}[$format_row][$format_col]->{FormatNo};

    for ($cur_pos=0; $cur_pos<=$#CAT_SEL ;$cur_pos++) {

        # read result.txt for all days
        &read_detail_result ($CAT_SEL[$cur_pos]);

        # category summary for latest result (from $last_pos)
        ($start_date, $finish_date, $pass_count, $fail_count, $cpu_time) = &category_summary_calculate ($last_pos, $count);

	# update information for general 
	my $category           = $CAT_SEL[$cur_pos];
	my $schedule_start     = "";
	my $schedule_finish    = "";
	my $actual_start       = $start_date;
	my $actual_finish      = $finish_date;
	my $pass               = $pass_count; 
	my $fail               = $fail_count; 
	my $cpu                = $cpu_time;
#	my $memory             = $memory; 

	# Category information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col  ,$category          ,$format);
	# Schedule Start information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col+1,$schedule_start    ,$format);
	# Schedule Finish information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col+2,$schedule_finish   ,$format);
	# Actual start information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col+3,$actual_start      ,$format);
	# Actual finish information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col+4,$actual_finish     ,$format);
	# Pass information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col+5,$pass              ,$format);
	# Fail information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col+6,$fail              ,$format);
	# CPU information
	$template->AddCell ($updated_sheet,$updated_row,$updated_col+7,$cpu               ,$format);
	# Memory information
#	$template->AddCell ($updated_sheet,$updated_row,$updated_col+8,$memory            ,$format);

        $updated_row++;
    }
}

###################################################################
#                   File output print format                      #
###################################################################

format SUMMARY_REPORT_FORMAT = 
 @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   @<<<<<<<<<    @<<<<<<<<<    @<<<     @<<<    @<<<<<< 
$CAT_SEL[$cur_pos],$start_date,$finish_date,$pass_count,$fail_count,$cpu_time
.

format DETAIL_REPORT_FORMAT = 
 @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   @<<<<<<<<<    @<<<    @<<<<<<
$TM_NAME[$cur_tm], $TM_DATE[$cur_tm], $TM_RESULT[$cur_tm], $TM_CPUTIME[$cur_tm]
.

format DAILY_REPORT_FORMAT = 
 @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<    @<<<     @<<<    @<<<<<<
$CAT_SEL[$cur_pos],$pass_count,$fail_count,$cpu_time
.
