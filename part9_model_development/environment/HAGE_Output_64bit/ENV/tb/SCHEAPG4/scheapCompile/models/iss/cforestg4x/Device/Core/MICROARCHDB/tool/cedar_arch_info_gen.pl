#!/usr/bin/env perl

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

open(INPUT, "<$ARGV[0]") || die "Can't open $ARGV[0]: $!\n";
open(OUTPUT_C, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "$ARGV[1].cpp") or die "$ARGV[1]: $!";
open(OUTPUT_FUNC_TABLE, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "inst_func_table.cpp");
open(OUTPUT_INST_ID_LIST, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "inst_id_list.h");
open(OUTPUT_INST_DECL, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "inst_declaration.h");
open(OUTPUT_INST_ARGTYPE, ($opt_crlf) ? ">:utf8:crlf" : ">:utf8", "inst_argtype.h");

# micro_arch_db.hのSRC_REG_NUM/DST_REG_NUMと一致させること
$SRC_REG_NUM  = 32;
$DST_REG_NUM  = 32;
$END_ENTRY    = "END_ENTRY";

&data_input;
&gen_code_table;

&output_cbody;
&output_func_table;
&output_inst_id_list;
&output_inst_decl;
&output_inst_argtype;

IO::Handle::flush(OUTPUT_C);
eval { IO::Handle::sync(OUTPUT_C); };
close (OUTPUT_C);
IO::Handle::flush(OUTPUT_FUNC_TABLE);
eval { IO::Handle::sync(OUTPUT_FUNC_TABLE); };
close (OUTPUT_FUNC_TABLE);
IO::Handle::flush(OUTPUT_INST_ID_LIST);
eval { IO::Handle::sync(OUTPUT_INST_ID_LIST); };
close (OUTPUT_INST_ID_LIST);
IO::Handle::flush(OUTPUT_INST_DECL);
eval { IO::Handle::sync(OUTPUT_INST_DECL); };
close (OUTPUT_INST_DECL);
IO::Handle::flush(OUTPUT_INST_ARGTYPE);
eval { IO::Handle::sync(OUTPUT_INST_ARGTYPE); };
close (OUTPUT_INST_ARGTYPE);

sub data_input {
    # タイトル行からハッシュテーブルを生成
    $title = <INPUT>;
    chomp ($title);
    @line = split(/[\t]/, $title);
    $c = 0;
    while ($cname = shift (@line)) {
        $columnname{$cname} = $c;
        $c++;
    }

    # 命令No は 1 から開始
    # 命令No 0 (各リストの先頭) にはダミーを登録
    push (@idlist, 0);             # 命令NO
    push (@coprotypelist, 0);      # コプロ種別
    push (@codelist, 0);           # 命令コード
    push (@argtypelist, "ERROR");  # 引数タイプ
    push (@keylist, 0);            # デコードキー
    push (@funclist, 0);           # ライブラリ関数名
    push (@instnamelist, 0);       # 命令名
    push (@operandformatlist, 0);  # オペランドフォーマット
    push (@instsizelist, 0);       # 命令サイズ
    push (@srcreglist, 0);         # Source Register
    push (@rrstagelist, 0);        # Register Read Stage
    push (@rrstageg3mhlist, 0);    # Register Read Stage
    push (@dstreglist, 0);         # Destination Register
    push (@rastagelist, 0);        # Register Available Stage
    push (@rastageg3mhlist, 0);    # Register Available Stage
	push (@rastageg3khlist, 0);    # Register Available Stage
    push (@wbstagelist, 0);        # Write Back Stage
    push (@parallelgrouplist, "");     # 並列発行グループ
    push (@parallelgroupchangedlist, ""); # 並列発行グループ(特殊条件用)
	push (@parallelgroupg3khlist, ""); # 並列発行グループ(特殊条件用)
	push (@fusiontypelist, ""); # Fusion Instruction Type
	push (@iddividedlist, ""); # ID divided instructions
	push (@rndividedlist, ""); # RN divided instructions
	push (@dpdividedlist, ""); # DP divided instructions
	push (@wbdividedlist, ""); # WB divided instructions
	push (@exissuelist, ""); # EX issue limitations
	push (@singlecmlist, ""); # single commit instructions
    push (@Instructiongrouplist, ""); # 命令グループ
    push (@cyclecalclist, "");     # サイクル計算条件
    push (@issueratelist, 0);      # 発行レート
    push (@issuerateg3mhlist, 0);  # 発行レート
	push (@issuerateg3khlist, 0);  # 発行レート
    push (@privilegelist, 0);      # 特権命令
    push (@meiinsttypelist, 0);    # MEI命令種別コード
    push (@mdpexpcodebitlist, 0);  # MDP例外要因コードビット番号
    push (@branchlist, 0);         # 分岐命令
    push (@branchg3mhlist, 0);     # 分岐命令
	push (@branchg3khlist, 0);     # 分岐命令
    push (@loadtypelist, "UNDEF");       # Load結果の格納方法
    push (@rsresource1typelist, "UNDEF");   # RS resource1
    push (@rsresource2typelist, "UNDEF");   # RS resource2
    push (@exresource1typelist, "UNDEF");   # Exec resource1
    push (@exresource2typelist, "UNDEF");   # Exec resource2
    push (@pswdependlist, "NONE");      # PSW.CC依存
    $idlist_from_index{0} = 0;

    $i = 1;
    while (<INPUT>) {
        chomp ($_);
        @line = split(/\t/, $_);

        # 非公開命令はスキップ
        if ($line[$columnname{'Publicity'}] =~ m/H/) { next; }

        push (@idlist,            $i);                     # 命令NO
        push (@coprotypelist,     $line[$columnname{'Arcitectural Category'}]);  # コプロ種別
        push (@codelist,          $line[$columnname{'Instruction Code[31:0]'}]);       # 命令コード
        push (@argtypelist,       $line[$columnname{'Argument Type'}]);          # 引数タイプ
        push (@keylist,           $line[$columnname{'Decode Type'}]);            # デコードキー
        push (@funclist,          $line[$columnname{'Library Function Name'}]);  # ライブラリ関数名
        push (@instnamelist,      $line[$columnname{'Instruction Name'}]);       # 命令名
        push (@operandformatlist, $line[$columnname{'Operand Format'}]);         # オペランドフォーマット
        push (@instsizelist,      $line[$columnname{'Instruction Size'}]);       # 命令サイズ
        push (@srcreglist,        $line[$columnname{'Source Register'}]);        # Source Register
        push (@rrstagelist,       $line[$columnname{'Register Read Stage'}]);    # Register Read Stage
        if ($line[$columnname{'Register Read Stage G3MH'}] eq "") {
            push (@rrstageg3mhlist,   $line[$columnname{'Register Read Stage'}]);    # Register Read Stage
        } else {
            push (@rrstageg3mhlist,   $line[$columnname{'Register Read Stage G3MH'}]);    # Register Read Stage
        }
        push (@dstreglist,        $line[$columnname{'Destination Register'}]);   # Destination Register
        push (@rastagelist,       $line[$columnname{'Register Available Stage'}]);   # Register Available Stage
        if ($line[$columnname{'Register Available Stage G3MH'}] eq "") {
            push (@rastageg3mhlist,   $line[$columnname{'Register Available Stage'}]);   # Register Available Stage
        } else {
            push (@rastageg3mhlist,   $line[$columnname{'Register Available Stage G3MH'}]);   # Register Available Stage
        }
		if ($line[$columnname{'Register Available Stage G3KH'}] eq "") {
            push (@rastageg3khlist,   $line[$columnname{'Register Available Stage'}]);   # Register Available Stage
        } else {
            push (@rastageg3khlist,   $line[$columnname{'Register Available Stage G3KH'}]);   # Register Available Stage
        }
        push (@wbstagelist,       $line[$columnname{'Write Back Stage'}]);       # Write Back Stage
        push (@parallelgrouplist,     $line[$columnname{'Parallel Group'}]);      # 並列発行グループ
        push (@parallelgroupchangedlist, $line[$columnname{'Parallel Group Changed'}]); # 並列発行グループ(特殊条件時)
		push (@parallelgroupg3khlist, $line[$columnname{'Parallel Group G3KH'}]); # 並列発行グループ(特殊条件時)
		push (@fusiontypelist, $line[$columnname{'Fusion Instruction Type'}]); # Fusion Instruction Type
		push (@iddividedlist, $line[$columnname{'ID divided'}]); # ID divided instructions
		push (@rndividedlist, $line[$columnname{'RN divided'}]); # RN divided instructions
		push (@dpdividedlist, $line[$columnname{'DP divided'}]); # DP divided instructions
		push (@wbdividedlist, $line[$columnname{'WB divided'}]); # WB divided instructions
		push (@exissuelist, $line[$columnname{'EX issue limit'}]); # EX issue limitations
		push (@singlecmlist, $line[$columnname{'CM limit'}]); # single commit instructions
        push (@Instructiongrouplist, $line[$columnname{'Instruction Group V850'}]); # 命令グループ
        push (@cyclecalclist, $line[$columnname{'Cycle Calculation Type'}]);     # サイクル計算条件
        push (@issueratelist,     $line[$columnname{'Issue Rate'}]);             # 発行レート
        if ($line[$columnname{'Issue Rate G3MH'}] eq "") {
            push (@issuerateg3mhlist, $line[$columnname{'Issue Rate'}]);             # 発行レート
        } else {
            push (@issuerateg3mhlist, $line[$columnname{'Issue Rate G3MH'}]);        # 発行レート
        }
		if ($line[$columnname{'Issue Rate G3KH'}] eq "") {
            push (@issuerateg3khlist, $line[$columnname{'Issue Rate'}]);             # 発行レート
        } else {
            push (@issuerateg3khlist, $line[$columnname{'Issue Rate G3KH'}]);        # 発行レート
        }
        push (@privilegelist,     $line[$columnname{'Privilege Type'}]);         # 特権命令
        push (@meiinsttypelist,   $line[$columnname{'Mei instruction type'}]);   # MEI命令種別コード
        push (@mdpexpcodebitlist, $line[$columnname{'Mdp exception code bit'}]); # MDP例外要因コードビット番号
        push (@branchlist,        $line[$columnname{'Branch Type'}]);            # 分岐命令
        if ($line[$columnname{'Branch Type G3MH'}] eq "") {
            push (@branchg3mhlist,    $line[$columnname{'Branch Type'}]);            # 分岐命令
        } else {
            push (@branchg3mhlist,    $line[$columnname{'Branch Type G3MH'}]);       # 分岐命令
        }
		if ($line[$columnname{'Branch Type G3KH'}] eq "") {
            push (@branchg3khlist,    $line[$columnname{'Branch Type'}]);            # 分岐命令
        } else {
            push (@branchg3khlist,    $line[$columnname{'Branch Type G3KH'}]);       # 分岐命令
        }
        push (@loadtypelist,      $line[$columnname{'Load Type'}]);              # Load結果の格納方法
        push (@rsresource1typelist,  $line[$columnname{'RS resource1'}]);            # RS resource1
        push (@rsresource2typelist,  $line[$columnname{'RS resource2'}]);            # RS resource2
        push (@exresource1typelist,  $line[$columnname{'EX resource1'}]);            # Exec resource1
        push (@exresource2typelist,  $line[$columnname{'EX resource2'}]);            # Exec resource2
        push (@pswdependlist,     $line[$columnname{'PSW.CC Dependency'}]);      # PSW.CC依存
        push (@numlist, $i);        # 管理用番号

        $idlist_from_index{$i} = $i;
        $i++;
    }
}


sub gen_code_table {
    $table_pos = 64;    # 次のテーブルの先頭位置
                        # V850 最初のキーは DECKEY_B: 6bit

    # 全命令分くりかえす
   foreach $n (@numlist) {
        &gen_code_table_body ($idlist[$n], $codelist[$n], $keylist[$n], 0, 0);
    }
}


# sub gen_code_table_body ($$$$$)
#   $instid : 命令ID
#   $code   : 命令コード
#   $deckey : デコードキー
#   $decpos : デコードキーの現在位置
#   $block_offset : テーブル先頭からのブロックの位置
#
sub gen_code_table_body ($$$$$) {
    my ($instid, $code, $deckey, $decpos, $block_offset) = @_;

    %shiftkey  = ('A' => 16, 'B' => 21, 'C' => 27, 'D' =>  0, 'E' =>  5, 'F' => 11,
                  'a' => 16, 'b' => 21, 'c' => 27, 'd' =>  0, 'e' =>  5, 'f' => 11,
                  'J' =>  5, 'K' => 10, 'S' => 16, 'T' => 16, 'U' => 10, 'V' => 10, 'W' => 10);
    %widthkey  = ('A' =>  5, 'B' =>  6, 'C' =>  5, 'D' =>  5, 'E' =>  6, 'F' =>  5,
                  'a' =>  5, 'b' =>  6, 'c' =>  5, 'd' =>  5, 'e' =>  6, 'f' =>  5,
                  'J' =>  5, 'K' =>  6, 'S' =>  5, 'T' =>  5, 'U' =>  6, 'V' =>  6, 'W' =>  6);
    %idsizekey = ('A' => 32, 'B' => 64, 'C' => 32, 'D' => 32, 'E' => 64, 'F' => 32,
                  'a' =>  2, 'b' =>  2, 'c' =>  2, 'd' =>  4, 'e' =>  2, 'f' =>  2,
                  'J' => 32, 'K' => 64, 'S' => 32, 'T' => 32, 'U' => 64, 'V' => 64, 'W' => 64);

    $t_deckey = substr($deckey, $decpos, 1);
    $codestr = substr($code, $shiftkey{$t_deckey}, $widthkey{$t_deckey});

    my @indexes = ();
    if ($t_deckey =~ m/[A-FJKS-W]/) {
        # don't careを bit展開するケース
        my @codestr_list = ();
        my @tmp_codestr_list = ();
        if ($codestr =~ m/[^01]/) {
            # don't care を bit展開
            @codestr_list = bit_extend ($codestr);
        } else {
            @codestr_list = ($codestr);
        }

        # ブロック内のインデックスを取得 (R0は別命令にマッピングetc)
        foreach $s (@codestr_list) {
            if ($t_deckey =~ m/[A-FJK]/) {
                # 通常ケース
                push @tmp_codestr_list, $s;
            } elsif ($t_deckey =~ m/[T]/) {
                # vvvv=0000 は別命令なので登録しない
                push @tmp_codestr_list, $s if $s !~ /^.0000$/;
            } elsif ($t_deckey =~ m/[S]/) {
                # rrrr=0000 は別命令なので登録しない
                push @tmp_codestr_list, $s if $s !~ /^0000.$/;
            } elsif ($t_deckey =~ m/[U]/) {
                # iiii=0000 は別命令なので登録しない
                push @tmp_codestr_list, $s if $s !~ /^0000..$/;
            } elsif ($t_deckey =~ m/[V]/) {
                # iiii=0000,1111 は別命令なので登録しない
                push @tmp_codestr_list, $s if (($s !~ /^0000..$/) && ($s !~ /^1111..$/));
            } elsif ($t_deckey =~ m/[W]/) {
                # cccc=1101 は別命令なので登録しない
                push @tmp_codestr_list, $s if $s !~ /^.1101.$/;
            } else {
                printf("Error : unknown decode key in ID %d: %c\n", $instid, $t_deckey);
            }
        }
        @codestr_list = @tmp_codestr_list;

        foreach $codestr (@codestr_list) {
            # 2進数文字列を数値に変換
            $index = unpack("C", pack("B8", $codestr)) >> (8 - $widthkey{$t_deckey});
            push @indexes, $index;
        }
    } elsif ($t_deckey =~ m/[a-f]/) {
        # All 0 か否かを判定
        if ($codestr =~ m/^0+$/) {
            $index = 0;
        } else {
            $index = 1;
            if ($t_deckey =~ m/[d]/) {
                # DECKEY_M : check bit[0]==0 or not
                if ($codestr =~ m/^....0$/) {
                    # wwww0
                    # record to index 1
                } else {
                    # wwwww
                    # record to index 1 and 3
                    push @indexes, 3;
                }
            }
        }
        push @indexes, $index;
    } else {
        printf("Error : unknown decode key in ID %d: %c\n", $instid, $t_deckey);
    }

    foreach $index (@indexes) {
        # テーブル先頭からのインデックス値に変換
        $index_from_head = $block_offset + $index;

        $next_deckey = substr($deckey, $decpos+1, 1);
        if ($next_deckey eq "-") {        # 情報取得完了
            # 先頭からのインデックス
            $indextable[$index_from_head] = $index_from_head;
            # 命令コード
            $codetable[$index_from_head] = $instid;
            # 完了フラグ
            $nexttable[$index_from_head] = -1;

        } else {
            if (!defined($indextable[$index_from_head])) {
                # 新ブロック作成
                # 先頭からのインデックス
                $indextable[$index_from_head] = $index_from_head;
                # 次のデコードキー
                $codetable[$index_from_head] = $next_deckey;
                # 次のブロックインデックス
                $nexttable[$index_from_head] = $table_pos;
                $table_pos += $idsizekey{$next_deckey};
            } else {
                # 既にブロックが存在している
                # 矛盾していたら MicroArchDBが間違っているので念のためチェック
                if ($indextable[$index_from_head] != $index_from_head) {
                    printf("Error : index calculation mismatch in inst_id %d.\n", $instid);
                }
                # 次のデコードキー
                if ($shiftkey{$codetable[$index_from_head]} != $shiftkey{$next_deckey}) {
                    printf("Warning : doubtful decode key in inst_id %d.\n", $instid);
                }
            }

            # 再帰的にテーブル生成
            &gen_code_table_body ($instid, $code, $deckey, $decpos + 1, $nexttable[$index_from_head]);
        }
    }

    return;
}


sub output_inst_id_list {
    select (OUTPUT_INST_ID_LIST);

    printf ${bom} if $opt_with_bom;
    printf("/*\n");
    printf(" * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
    printf(" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf(" * This program must be used solely for the purpose for which\n");
    printf(" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf(" * program may be reproduced or disclosed to others, in any\n");
    printf(" * form, without the prior written permission of Renesas Electronics\n");
    printf(" * Corporation.\n");
    printf(" */\n");
    printf("\n");

    printf("#pragma once\n");
    printf("\n");

    for ($i = 0; $i <= $#numlist+1; $i++) {
        if ($i == 0) {
            printf("#define  INST_ID_V_%-20s%d\n", "0", 0);
        } else {
            printf("#define  INST_ID_V_%-20s%d\n", $funclist[$i], $idlist[$i]);
        }
    }

    printf("\n");
    printf("#define TOTAL_INST_NUM %d\n", $#numlist+1);
    printf("\n");
    printf("\n");
}

sub output_inst_decl {
    select (OUTPUT_INST_DECL);

    printf ${bom} if $opt_with_bom;
    printf("/*\n");
    printf(" * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
    printf(" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf(" * This program must be used solely for the purpose for which\n");
    printf(" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf(" * program may be reproduced or disclosed to others, in any\n");
    printf(" * form, without the prior written permission of Renesas Electronics\n");
    printf(" * Corporation.\n");
    printf(" */\n");
    printf("\n");

    printf("#pragma once\n");
    printf("\n");

    printf("    void Inst_0 (uint64_t opcode);\n");
    for ($i = 1; $i <= $#numlist+1; $i++) {
        printf("    void Inst_%s (uint64_t opcode);\n", $funclist[$i]);
    }
}


sub output_cbody {
    select (OUTPUT_C);

    printf ${bom} if $opt_with_bom;
    printf("/*\n");
    printf(" * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
    printf(" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf(" * This program must be used solely for the purpose for which\n");
    printf(" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf(" * program may be reproduced or disclosed to others, in any\n");
    printf(" * form, without the prior written permission of Renesas Electronics\n");
    printf(" * Corporation.\n");
    printf(" */\n");
    printf("\n");
    printf("#include \"./config.h\"\n");
    printf("#include \"./forest_common.h\"\n");
    printf("#include \"./micro_arch_db.h\"\n");
    printf("#include \"./inst_id_list.h\"\n");
    printf("\n");

    &output_code_table;
    &output_info_table;
}


sub output_code_table {
    %keystring = ('A' => "DECKEY_A",  'B' => "DECKEY_B",
                  'C' => "DECKEY_C",  'D' => "DECKEY_D",
                  'E' => "DECKEY_E",  'F' => "DECKEY_F",
                  'a' => "DECKEY_A0", 'b' => "DECKEY_B0",
                  'c' => "DECKEY_C0", 'd' => "DECKEY_D0",
                  'e' => "DECKEY_E0", 'f' => "DECKEY_F0",
                  'J' => "DECKEY_J",  'K' => "DECKEY_K",
                  'S' => "DECKEY_S",  'T' => "DECKEY_T",
                  'U' => "DECKEY_U",  'V' => "DECKEY_V",
                  'W' => "DECKEY_W");

    # プロローグ
    printf("const MicroArchDB::CodeTable MicroArchDB::m_code_table[%d] = {\n", $table_pos + 1);

    # 本体の出力
    for ($i = 0; $i < $table_pos; $i++) {
        # 行末のコンマ
        if ($i != $table_pos-1) {
            $comma = ",";
        } else {
            $comma = "";
        }

        if (!defined($codetable[$i])){
            $codetable[$i] = "";
        }
        if (!defined($nexttable[$i])){
            $nexttable[$i] = "";
        }
        if ($codetable[$i] =~ /[A-Fa-fJKS-W]/) {
            # DecodeKey型
            printf("    { %-9s , 0                     , %2s }%s\n",
                    $keystring{$codetable[$i]}, $nexttable[$i], $comma);
        } else {
            if ($codetable[$i] eq "") {
                # 予約命令
                printf("    { DEC_END   , INST_ID_V_0           , -1 }%s    /* Reserved */\n", $comma);
            } else {
                # 本体の出力
                printf("    { DEC_END   , INST_ID_V_%-12s, %2s }%s    /* %s */\n",
                        $funclist[$codetable[$i]], $nexttable[$i], $comma,
                        $codelist[$idlist_from_index{$codetable[$i]}]);
            }
        }
    }

    # エピローグ
    printf("};\n");
}


sub output_info_table {

    # プロローグ
    printf("\n");
    printf("const MicroArchDB::InstInfo MicroArchDB::m_inst_info[TOTAL_INST_NUM + 1] = {\n");
    $id = 1;

    # 本体の出力
    for ($i = 0; $i <= $#numlist+1; $i++) {
        # 行末のコンマ
        if ($i != $#numlist+1) {
            $comma = ",";
        } else {
            $comma = "";
        }

        # 関数テーブルをセット
        if ($i == 0) {
            $funclist[0] = "NULL";
            $argtypelist[0] = "ARG_ERROR";
            $instsizelist[0] = (32 >> 3);
        } else {
            $id ++;
            $argtypelist[$i] = "ARG_" . $argtypelist[$i];
        }

        printf("    { {%s, ", ($branchlist[$i] =~ /BP/) ? "BR_PRED"
                             : ($branchlist[$i] =~ /B/) ? "BR_NORM" : "BR_NOT"); # 分岐命令
        printf("%s, ",  ($branchg3mhlist[$i] =~ /BP/) ? "BR_PRED"
                        : ($branchg3mhlist[$i] =~ /B/) ? "BR_NORM" : "BR_NOT"); # 分岐命令
		printf("%s}, ",  ($branchg3khlist[$i] =~ /BP/) ? "BR_PRED"
                        : ($branchg3khlist[$i] =~ /B/) ? "BR_NORM" : "BR_NOT"); # 分岐命令

        printf("%s, ", ($coprotypelist[$i] =~ /FPU/ ) ? "COPRO_FPU"
                      : ($coprotypelist[$i] =~ /SIMD/) ? "COPRO_SIMD" : "COPRO_NO");   # コプロ種別

        printf("%s, ", $argtypelist[$i]);     # 引数フォーマットテーブル

        # 設定されていなければ未定義値（PGRP_UNDEF)として扱う
        if ($parallelgrouplist[$i] eq "") {
            $parallelgrouplist[$i] = "UNDEF";
        }
        printf("PGRP_%s, ",  $parallelgrouplist[$i]); # 並列発行グループ

        # 設定されていなければ未定義値（PGRP_UNDEF)として扱う
        if ($parallelgroupchangedlist[$i] eq "") {
            $parallelgroupchangedlist[$i] = "UNDEF";
        }
        printf("PGRP_%s, ",  $parallelgroupchangedlist[$i]); # 並列発行グループ(特殊条件時)

		if ($parallelgroupg3khlist[$i] eq "") {
            $parallelgroupg3khlist[$i] = "UNDEF";
        }
        printf("PGRP_%s, ",  $parallelgroupg3khlist[$i]); # 並列発行グループ(特殊条件時)

        # If entry isn't existed, treated as "UNDEF"
        if ($fusiontypelist[$i] eq "") {
            $fusiontypelist[$i] = "UNDEF";
        }
        printf("FUSION_%s, ",  $fusiontypelist[$i]); # Fusion Instruction Type

        # If entry isn't existed, treated as "UNDEF"
        if ($iddividedlist[$i] eq "") {
            $iddividedlist[$i] = "UNDEF";
        }
        printf("IDD_%s, ",  $iddividedlist[$i]); # ID devided instructions

        # If entry isn't existed, treated as "UNDEF"
        if ($rndividedlist[$i] eq "") {
            $rndividedlist[$i] = "UNDEF";
        }
        printf("RND_%s, ",  $rndividedlist[$i]); # RN devided instructions

        # If entry isn't existed, treated as "UNDEF"
        if ($dpdividedlist[$i] eq "") {
            $dpdividedlist[$i] = "UNDEF";
        }
        printf("DPD_%s, ",  $dpdividedlist[$i]); # DP devided instructions

        # If entry isn't existed, treated as "UNDEF"
        if ($wbdividedlist[$i] eq "") {
            $wbdividedlist[$i] = "UNDEF";
        }
        printf("WBD_%s, ",  $wbdividedlist[$i]); # WB devided instructions

        # If entry isn't existed, treated as "UNDEF"
        if ($exissuelist[$i] eq "") {
            $exissuelist[$i] = "UNDEF";
        }
        printf("EX_LIMIT_%s, ",  $exissuelist[$i]); # EX issue limitations

        # If entry isn't existed, treated as "UNDEF"
        if ($singlecmlist[$i] eq "") {
            $singlecmlist[$i] = "UNDEF";
        }
        printf("SINGLE_CM_%s, ",  $singlecmlist[$i]); # Single commit instructions

        # 設定されていなければ未定義値（IGRP_UNDEF)として扱う
        if ($Instructiongrouplist[$i] eq "") {
            $Instructiongrouplist[$i] = "UNDEF";
        }
        printf("IGRP_%s, ",  $Instructiongrouplist[$i]); # 命令グループ

        # 設定されていなければ条件なし（CCT_NORM)として扱う
        if ($cyclecalclist[$i] eq "") {
            $cyclecalclist[$i] = "NORM";
        }
        printf("CCT_%s, ",  $cyclecalclist[$i]); # サイクル計算条件

        # 設定されていなければ未定義値（IGRP_UNDEF)として扱う
        if ($loadtypelist[$i] eq "") {
            $loadtypelist[$i] = "UNDEF";
        }
        $loadtypelist[$i] = "LDTYPE_" . $loadtypelist[$i];
        printf("%s, ",  $loadtypelist[$i]); # Load結果の格納方法

        # If entry isn't existed, treated as "UNDEF"
        if ($rsresource1typelist[$i] eq "") {
            $rsresource1typelist[$i] = "UNDEF";
        }
        printf("RS_RES_%s, ",  $rsresource1typelist[$i]); # RS resource1

        # If entry isn't existed, treated as "UNDEF"
        if ($rsresource2typelist[$i] eq "") {
            $rsresource2typelist[$i] = "UNDEF";
        }
        printf("RS_RES_%s, ",  $rsresource2typelist[$i]); # RS resource2

        # If entry isn't existed, treated as "UNDEF"
        if ($exresource1typelist[$i] eq "") {
            $exresource1typelist[$i] = "UNDEF";
        }
        printf("EX_RES_%s, ",  $exresource1typelist[$i]); # EX resource1

        # If entry isn't existed, treated as "UNDEF"
        if ($exresource2typelist[$i] eq "") {
            $exresource2typelist[$i] = "UNDEF";
        }
        printf("EX_RES_%s, ",  $exresource2typelist[$i]); # EX resource2

        # pswdependlist
        # 設定されていなければ条件なし（PSW_DEP_NONE)として扱う
        if ($pswdependlist[$i] eq "") {
            $pswdependlist[$i] = "NONE";
        }
        printf("PSW_DEP_%s, ",  $pswdependlist[$i]); # PSW依存

        printf("%d, ", ($instsizelist[$i] >> 3));                                        # 命令サイズ
        printf("{%d, ", (($issueratelist[$i] =~ m/[0-9]/) ? $issueratelist[$i] : 0));     # 発行レート
        printf("%d, ", (($issuerateg3mhlist[$i] =~ m/[0-9]/) ? $issuerateg3mhlist[$i] : 0));     # 発行レート
		printf("%d}, ", (($issuerateg3khlist[$i] =~ m/[0-9]/) ? $issuerateg3khlist[$i] : 0));     # 発行レート
        printf("%s, ", ($privilegelist[$i] =~ /SV/) ? "PRIV_PERMIT_SV"
                      : ($privilegelist[$i] =~ /HV/) ? "PRIV_PERMIT_HV"
                      : ($privilegelist[$i] =~ /DB/) ? "PRIV_PERMIT_DBUM" : "PRIV_PERMIT_UM"); # 特権命令
        printf("%d, ", ($meiinsttypelist[$i] =~ m/[0-9]/) ? $meiinsttypelist[$i] : 0);   # MEI命令種別コード
        printf("%d,\n", (($mdpexpcodebitlist[$i] =~ m/[0-9]/) ? $mdpexpcodebitlist[$i] : 0));      # MDP例外要因コードビット番号

        printf("      {{");
        @srlist = split (/\//, $srcreglist[$i]);
        @rrlist = split (/\//, $rrstagelist[$i]);
        if ($#srlist != $#rrlist){
            # SRCレジスタとSRCレジスタのRRステージ記述の数が合わない
            print("Error: The number of src register and rr stage is unmatched!");
        }
        if ($#srlist > $SRC_REG_NUM){
            # SRCレジスタの数が最大個数をオーバー
            print("Error: The number of src register is exceeded the SRC_REG_NUM!");
        }
        for ($j = 0, $k = 0; $j < $SRC_REG_NUM; $j++, $k++) {           # レジスタ読み込みステージテーブル
            $comma2 = ($j != $SRC_REG_NUM-1) ? "," : "";
            if ($k <= $#rrlist && $rrlist[$k] ne "-") {
                if ($srlist[$k] =~ m/REGS/) {
                    # REGSなら残りのレジスタ読み込みステージを指定値で埋める
                    for (; $j < $SRC_REG_NUM; $j++) {
                        $comma2 = ($j != $SRC_REG_NUM-1) ? "," : "";
                        printf("%d%s ", $rrlist[$k], $comma2);
                    }
                } else {
                    printf("%d%s ", $rrlist[$k], $comma2);
                }
            } else {
                printf("%s%s ", $END_ENTRY, $comma2);
            }
        }
        printf("},\n      {");

        @srlist = split (/\//, $srcreglist[$i]);
        @rrlist = split (/\//, $rrstageg3mhlist[$i]);
        if ($#srlist != $#rrlist){
            # SRCレジスタとSRCレジスタのRRステージ記述の数が合わない
            print("Error: The number of src register and rr stage is unmatched!");
        }
        if ($#srlist > $SRC_REG_NUM){
            # SRCレジスタの数が最大個数をオーバー
            print("Error: The number of src register is exceeded the SRC_REG_NUM!");
        }
        for ($j = 0, $k = 0; $j < $SRC_REG_NUM; $j++, $k++) {           # レジスタ読み込みステージテーブル
            $comma2 = ($j != $SRC_REG_NUM-1) ? "," : "";
            if ($k <= $#rrlist && $rrlist[$k] ne "-") {
                if ($srlist[$k] =~ m/REGS/) {
                    # REGSなら残りのレジスタ読み込みステージを指定値で埋める
                    for (; $j < $SRC_REG_NUM; $j++) {
                        $comma2 = ($j != $SRC_REG_NUM-1) ? "," : "";
                        printf("%d%s ", $rrlist[$k], $comma2);
                    }
                } else {
                    printf("%d%s ", $rrlist[$k], $comma2);
                }
            } else {
                printf("%s%s ", $END_ENTRY, $comma2);
            }
        }
        printf("}},\n      {{");

        @drlist = split (/\//, $dstreglist[$i]);
        @ralist = split (/\//, $rastagelist[$i]);
        if ($#drlist != $#ralist){
            # DSTレジスタとDSTレジスタのRAステージ記述の数が合わない
            print("Error: The number of dst register and ra stage is unmatched!");
        }
        if ($#drlist > $DST_REG_NUM){
            # DSTレジスタの数が最大個数をオーバー
            print("Error: The number of dst register is exceeded the DST_REG_NUM!");
        }
        for ($j = 0, $k = 0; $j < $DST_REG_NUM; $j++, $k++) {           # レジスタ書き込みステージテーブル
            $comma2 = ($j != $DST_REG_NUM-1) ? "," : "";
            if ($k <= $#ralist && $ralist[$k] ne "-") {
                if ($drlist[$k] =~ m/DR|XD/) {
                    printf("%d, %d%s ", $ralist[$k], $ralist[$k], $comma2);
                    $j++;
                } elsif ($drlist[$k] =~ m/FV/) {
                    printf("%d, %d, %d, %d%s ", $ralist[$k], $ralist[$k],
                                                 $ralist[$k], $ralist[$k], $comma2);
                    $j += 4;
                } else {
                    printf("%d%s ", $ralist[$k], $comma2);
                }
            } else {
                printf("%s%s ", $END_ENTRY, $comma2);
            }
        }
        printf("},\n      {");

        @drlist = split (/\//, $dstreglist[$i]);
        @ralist = split (/\//, $rastageg3mhlist[$i]);
        if ($#drlist != $#ralist){
            # DSTレジスタとDSTレジスタのRAステージ記述の数が合わない
            print("Error: The number of dst register and ra stage is unmatched!");
        }
        if ($#drlist > $DST_REG_NUM){
            # DSTレジスタの数が最大個数をオーバー
            print("Error: The number of dst register is exceeded the DST_REG_NUM!");
        }
        for ($j = 0, $k = 0; $j < $DST_REG_NUM; $j++, $k++) {           # レジスタ書き込みステージテーブル
            $comma2 = ($j != $DST_REG_NUM-1) ? "," : "";
            if ($k <= $#ralist && $ralist[$k] ne "-") {
                if ($drlist[$k] =~ m/DR|XD/) {
                    printf("%d, %d%s ", $ralist[$k], $ralist[$k], $comma2);
                    $j++;
                } elsif ($drlist[$k] =~ m/FV/) {
                    printf("%d, %d, %d, %d%s ", $ralist[$k], $ralist[$k],
                                                 $ralist[$k], $ralist[$k], $comma2);
                    $j += 4;
                } else {
                    printf("%d%s ", $ralist[$k], $comma2);
                }
            } else {
                printf("%s%s ", $END_ENTRY, $comma2);
            }
        }
		printf("},\n {");

		@drlist = split (/\//, $dstreglist[$i]);
        @ralist = split (/\//, $rastageg3khlist[$i]);
        if ($#drlist != $#ralist){
            # DSTレジスタとDSTレジスタのRAステージ記述の数が合わない
            print("Error: The number of dst register and ra stage is unmatched!");
        }
        if ($#drlist > $DST_REG_NUM){
            # DSTレジスタの数が最大個数をオーバー
            print("Error: The number of dst register is exceeded the DST_REG_NUM!");
        }
        for ($j = 0, $k = 0; $j < $DST_REG_NUM; $j++, $k++) {           # レジスタ書き込みステージテーブル
            $comma2 = ($j != $DST_REG_NUM-1) ? "," : "";
            if ($k <= $#ralist && $ralist[$k] ne "-") {
                if ($drlist[$k] =~ m/DR|XD/) {
                    printf("%d, %d%s ", $ralist[$k], $ralist[$k], $comma2);
                    $j++;
                } elsif ($drlist[$k] =~ m/FV/) {
                    printf("%d, %d, %d, %d%s ", $ralist[$k], $ralist[$k],
                                                 $ralist[$k], $ralist[$k], $comma2);
                    $j += 4;
                } else {
                    printf("%d%s ", $ralist[$k], $comma2);
                }
            } else {
                printf("%s%s ", $END_ENTRY, $comma2);
            }
        }
        printf("}},\n      {");

        @wblist = split (/\//, $wbstagelist[$i]);
        for ($j = 0, $k = 0; $j < $DST_REG_NUM; $j++, $k++) {           # レジスタライトバックステージテーブル
            $comma2 = ($j != $DST_REG_NUM-1) ? "," : "";
            if ($k <= $#wblist && $wblist[$k] ne "-") {
                if ($drlist[$k] =~ m/DR|XD/) {
                    printf("%d, %d%s ", $wblist[$k], $wblist[$k], $comma2);
                    $j++;
                } elsif ($drlist[$k] =~ m/FV/) {
                    printf("%d, %d, %d, %d%s ", $wblist[$k], $wblist[$k],
                                                 $wblist[$k], $wblist[$k], $comma2);
                    $j += 4;
                } else {
                    printf("%d%s ", $wblist[$k], $comma2);
                }
            } else {
                printf("%s%s ", $END_ENTRY, $comma2);
            }
        }
        printf("},\n");

        # 不用な " を削除
        $operandformatlist[$i] =~ s/["]//g;
        $instnamelist[$i] =~ s/["]//g;

        printf("    \"%s\",\n", $instnamelist[$i]);      # 命令名テーブル
        printf("    \"%s\",\n", $operandformatlist[$i]); # オペランドフォーマット
        printf("    }%s\n", $comma);
    }

    # エピローグ
    printf("};\n");

}

sub output_func_table {
    select (OUTPUT_FUNC_TABLE);

    printf ${bom} if $opt_with_bom;
    printf("/*\n");
    printf(" * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
    printf(" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf(" * This program must be used solely for the purpose for which\n");
    printf(" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf(" * program may be reproduced or disclosed to others, in any\n");
    printf(" * form, without the prior written permission of Renesas Electronics\n");
    printf(" * Corporation.\n");
    printf(" */\n");
    printf("\n");
    printf("#include \"./config.h\"\n");
    printf("#include \"./forest_common.h\"\n");
    printf("#include \"./inst_v850.h\"\n");
    printf("\n");

    printf("void InstV850::SetFuncTable (void)\n");
    printf("{\n");

    printf("    m_instfunc[0] = (INSTP)(&InstV850::Inst_0);\n");
    for ($i = 1; $i <= $#numlist+1; $i++) {
        printf("    m_instfunc[%d] = (INSTP)(&InstV850::%s);\n",
                $idlist[$i], ("Inst_" . $funclist[$i]));
    }
    printf("}\n");
}


sub output_inst_argtype {
    select (OUTPUT_INST_ARGTYPE);

    printf ${bom} if $opt_with_bom;
    printf("/*\n");
    printf(" * (c) 2011,2012,2013 Renesas Electronics Corporation\n");
    printf(" * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY\n");
    printf(" * This program must be used solely for the purpose for which\n");
    printf(" * it was furnished by Renesas Electronics Corporation. No part of this\n");
    printf(" * program may be reproduced or disclosed to others, in any\n");
    printf(" * form, without the prior written permission of Renesas Electronics\n");
    printf(" * Corporation.\n");
    printf(" */\n");
    printf("\n");
    printf("#pragma once\n");
    printf("\n");

    # ArgFormatのenum定義を生成
    printf("enum ArgFormat {\n");

    # エントリの重複削除と名前順ソート
    my %argtype_hash = ();
    foreach (@argtypelist) {
        $argtype_hash{$_} = 1;
    }
    delete $argtype_hash{ARG_ERROR};
    my @unique_argtype = keys %argtype_hash;
    my @sorted_argtype = sort @unique_argtype;

    foreach my $argtype (@sorted_argtype) {
        printf("    $argtype,\n");
    }
    printf("    ARG_ERROR\n");
    printf("};\n");

    printf("\n\n");

    # Load Typeの定義を生成
    printf("enum LoadType {\n");

    # エントリの重複削除と名前順ソート
    my %loadtype_hash = ();
    foreach (@loadtypelist) {
        $loadtype_hash{$_} = 1;
    }
    delete $loadtype_hash{LDTYPE_UNDEF};
    my @unique_loadtype = keys %loadtype_hash;
    my @sorted_loadtype = sort @unique_loadtype;

    foreach my $loadtype (@sorted_loadtype) {
        printf("    $loadtype,\n");
    }
    printf("    LDTYPE_UNDEF\n");
    printf("};\n");
}

# ドントケアを含む文字列を 01 のみの文字列リストに展開
sub bit_extend ($) {
    @str = @_;

    while (grep (/[^01]/, @str)) {
        @str0 = @str;
        @str1 = @str;
        foreach $s0 (@str0)  {
            $s0 =~ s/[^01]/0/;
        }
        foreach $s1 (@str1)  {
            $s1 =~ s/[^01]/1/;
        }
        @str = (@str0, @str1);
    }

    @str;
}
