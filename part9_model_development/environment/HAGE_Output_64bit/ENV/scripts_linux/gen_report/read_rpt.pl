#!/usr/bin/perl

if ($#ARGV + 1 < 3) {
    print ("Number of arguments do not match!\n\n");
    print ("Instruction: perl read_rpt.pl <valgrind report> <output file> <file source 1> < file source 2> ...\n\n");
}
else {
    open(HIN, "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";
    open(HOUT, ">>$ARGV[1]") || die "Can't open $ARGV[1]: $!\n";

    my @list_source = ();
    my $i = 0;
    for ($i = 2; $i <= $#ARGV; $i++) {
        push (@list_source, $ARGV[$i]);
    }

    my $temp_string = "";
    while (<HIN>) {
        $line = $_;

        $i = 0;
        if ($line =~ /==\d+==\s$/) {
            for ($i = 0; $i <= $#list_source; $i++) {
                if ($temp_string =~ /$list_source[$i]/) {
                    print HOUT "$temp_string\n\n";
                    last;
                }
            }

            $temp_string = "";
        }
        else {
            $temp_string = $temp_string . $line;
        }
    }
}
