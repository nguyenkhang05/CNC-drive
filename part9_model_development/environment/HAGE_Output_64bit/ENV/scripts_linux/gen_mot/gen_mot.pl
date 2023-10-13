#! /usr/bin/perl
# ----------------------------------------------------------------------
#     $Id: gen_mot.pl,v 1.1.1.1 2012/09/27 10:12:25 chaunguyen Exp $
# ----------------------------------------------------------------------
#   (C) Copyright 2009   RVC (Renesas Design Viet Nam Co., Ltd.)
#       All rights reserved. RVC Confidential Proprietary
#   (C) Copyright 2009   RENESAS Technology Corp.  All rights reserved.
# ----------------------------------------------------------------------*/


#==================================================#
#                variables declaration             #
#==================================================#
# variables for replacing
$PRG_SECTION  = "";
$DEST         = "./";
$PATH_INC     = "./";
$SRC          = "./";
$SRC_PATH     = "./";
$TM_NAME      = "./";
$MODE         = 0;
$TIMEMODE     = ""; #Added by NganTran (Dec 09, 2014)
# variables for openning
$SCRIPT_PATH        = "../";
$MAKEFILE_SKL_PATH  = "./";

#==================================================#
#                 main declaration                 #
#==================================================#

#{{{
if ( $#ARGV < 0 || $ARGV[0] =~ "-help" || $ARGV[0] =~ "-h") {
    &print_help();
}
elsif ( $#ARGV < 1 ){
    printf ("<error> lacking of arguments, please use -help for usage.\n\n");
    exit;
}
#}}}

#==================================================#
#                 sub routine calling              #
#==================================================#

# get input parameter 
&get_input_argument();

# judge calling build file or directory
if($MODE == 1){     # input directory
   &compile_directory($SRC_PATH);
}
elsif($MODE == 2){  # input file
   &compile_TM();
}

#==================================================#
#             sub routine declaration              #
#==================================================#

#==================================================#
# function: print_help                             #
# input   : none                                   #
# ouput   : none                                   #
# date    : 03/10/09                               #
# content : display helping                        #
#==================================================#
sub print_help{
    #{{{
    print ("=====================================================\n");
    print ("=               GEN_MOT.PL script                   =\n");
    print ("=====================================================\n");
    print ("+ This script used to convert cpp file to mot file \n\n");
    print ("+ Usage: gen_mot.pl <input_option> <input> [option]\n");
    print ("\t* <input_option> : -f -> <input> is file, -r -> <input> is directory\n");
    print ("\t* <input>        : input path to cpp file or directory\n\n");
    print ("\t* [option]\n");
    print ("\t  + -o <output>      : option for output path to mot file - defaut value = <input> directory\n");
    print ("\t  + -i <include>     : path to included file - default value = <input> directory\n\n");
    exit;
    #}}}
}

#==================================================#
# function: get_input_argument                     #
# input   : none                                   #
# ouput   : none                                   #
# date    : 03/10/09                               #
# content : argument parsing                       #
#==================================================#
sub get_input_argument{
   #{{{
   if($ARGV[0] =~ "-r"){      #input is directory
       $MODE = 1;
   }
   elsif($ARGV[0] =~ "-f"){   #input is file
       $MODE = 2;
   } 
   else{
       $MODE = 0;
   }
   if( $MODE > 0 ){
      if($#ARGV > 0 && $#ARGV % 2 != 0){
         for($i=0;$i<$#ARGV;$i++){
            if($ARGV[$i] =~ "-r"){
               $SRC  = $ARGV[++$i];
               $DEST = $SRC;               
               $SRC_PATH = $SRC;
               $PATH_INC = $SRC_PATH;
            }
            elsif($ARGV[$i] =~ "-f"){
               $SRC          = $ARGV[++$i];
               my $name      = "";
               my @path_list;
               my @name_list = split(/\//,$SRC);
               $name         = $name_list[$#name_list];
               @path_list    = split(/$name/,$SRC);
               @name_list    = split(/\./,$name);
               $TM_NAME      = $name_list[0];
               $TM_NAME      =~ s/ *//g;
               $DEST         = $path_list[0];
               $SRC_PATH     = $DEST;
               $PATH_INC     = $SRC_PATH;
               print "TM NAME $TM_NAME\n";
            }
            elsif($ARGV[$i] =~ "-o"){
               $DEST = $ARGV[++$i];  
            }
            elsif(@ARGV[$i] =~ "-i"){
               $PATH_INC = @ARGV[++$i];
            }
            elsif(@ARGV[$i] =~ "-t"){ #Added by NganTran (Dec 09, 2014)
               $TIMEMODE = @ARGV[++$i];
            }
         }
      }
      else{
         print "<error> lacking argument for option !\n"; 
         exit;
      }
   }
   else{ # $MODE == 0
       print "<error> option is not legal !\n";
       exit;
   }
   #}}}
}

#==================================================#
# function: create_makefile                        #
# input   : path to generated Makefile file        #
# output  : Makefile content                       #
# date    : 03/10/09                               #
# content : create make_file from makefile.skl     #
#==================================================#
sub create_makefile{
   #{{{
   my $interrupt_enable = 0;   
   my $mk_file_skl     = "";
   $mk_file = shift;
   # Interrupt detecting ...
   unless(open(COMMON,"$PATH_INC/share.h")){
      die("<error> can not open share.h file !\n");
      exit;
   }
   my @common_content = <COMMON>;
   $mk_file_skl = $MAKEFILE_SKL_PATH."\/imakefile.skl";
   unless(open(MK_SKL,$mk_file_skl)){
      die("<error> can not open $mk_file_skl file !\n");
      exit;
   }
   unless(open(MK,">$mk_file")){
      die("Can not create new makefile file !\n");
      exit;
   }

   my @content = <MK_SKL>;
   for($i=0;$i<$#content;$i++){
      if($content[$i] =~ /\%\%TM_NAME/){       #destination path replace
         $content[$i] =~ s/\%\%TM_NAME/$TM_NAME/g;
      }
      #Added by NganTran Dec 09th, 2014
      if($content[$i] =~ /\%\%TIMEMODE/){       #destination path replace
         my $change_str = "";
         if($TIMEMODE eq "at"){
            $change_str = "-D_AT_TIME_MODE_";
         } elsif($TIMEMODE eq "lt"){
            $change_str = "-D_LT_TIME_MODE_";
         }
         $content[$i] =~ s/\%\%TIMEMODE/$change_str/g;
      }
      ##################################
      if($content[$i] =~ /\/\//){ 
         $content[$i] =~ s/\/\//\//g;
      }
      print MK $content[$i];
   }
   close(COMMON);
   close(MK);
   close(MK_SKL);
   #}}}
}

#==================================================#
# function: compile_directory                      #
# input   : path to test pattern directory         #
# ouput   : mot files of test patterns             #
# date    : 03/11/09                               #
# content : compile test pattern                   #
#==================================================#
sub compile_directory{
   #{{{
   my $dir = shift;
   my $file_name;
   my @file_list;
   my @name_list;
   if(substr(($dir,length($dir)-1,length($dir)-1)) =~ "/"){
      $dir    = substr($dir,0,length($dir)-1);
   }
   if(opendir(MYDIR,$dir) || die ("<error> can not open $dir directory !\n")){
      @file_list = readdir(MYDIR);
      closedir(MYDIR);
      foreach $file_name (sort @file_list){
         next if($file_name eq "." || $file_name eq "..");
         if(-d $dir."\/".$file_name){   #sub directory
             &compile_directory($dir."\/".$file_name."\/");
         }
         else{                 #file
             if((($file_name =~ /\.cpp$/)&&($file_name ne "intv1m.c")) || (($file_name =~ /\.c$/)&&($file_name ne "intv1m.c"))){
                 $SRC       = $dir."/".$file_name;
                 $SRC_PATH  = $dir."\/";
                 $DEST      = $dir;
                 if($file_name =~ /\.cpp/){
                     @name_list = split(/.cpp/,$file_name);
                 }
                 else{
                     @name_list = split(/.c/,$file_name);
                 }
                 $TM_NAME   = $name_list[0];
                 &compile_TM();
                  
             }
         }
      }
   }
   return 1;
   #}}}
}

#==================================================#
# function: compile_TM                             #
# input   : path to test pattern                   #
# output  : mot file of test pattern               #
# date    : 03/11/09                               #
# content : compile test pattern                   #
#==================================================#
sub compile_TM{
   #{{{
   if(!(-e $SRC )){
       die("<error> can not open $SRC file !\n");
   }
   # generate Makefile file path 
   my $mk_file = $SRC_PATH."Makefile";
   # generate Makefile content 
   &create_makefile($mk_file);
   # build test pattern by calling gmake
   # system("cp -f $PATH_INC/*.h $SRC_PATH");
   system("gmake clean -C $SRC_PATH");
   ######system("rm -rf $SRC_PATH/*.obj $SRC_PATH/tpldir");
   system("gmake -C $SRC_PATH");
   # remove tmp data
   #####system("rm -rf $SRC_PATH/Makefile $SRC_PATH/*.obj $SRC_PATH/tpldir $SRC_PATH/*.lis");
   #####system("rm -rf $SRC_PATH/*.h");
   #}}} 
}
# vim600: set foldmethod=marker :
