sed 's/,//g' $1 | awk '/DEC_END/{a[$3]++; if (s<$3) s=$3;} END{for (i=0;i<=s;i++) print i, " ", a[i]}'
