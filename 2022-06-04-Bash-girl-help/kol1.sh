#!/bin/bash

[[ "$#" -ne 2 ]] && echo "Wrong arguments" && exit

k=$1
l=$2

[[ $k -le 0 || $k -gt $l ]] && echo "Wrong arguments" && exit

dir="KZAL1-40"
rm -rf $dir
mkdir $dir
touch $dir/wynik1.txt

counter=0
total_size=0

for filename in `find /var/log -type f -size +"$k"c -size -"$l"c`; do
	size=`du -b $filename | cut -f -1`
	((total_size=total_size+size))
	echo "$counter---$filename---`wc -l $filename`---$size" >> $dir/wynik1.txt
	((counter=counter+1))
done

echo "----------" >> $dir/wynik1.txt
echo $total_size >> $dir/wynik1.txt
