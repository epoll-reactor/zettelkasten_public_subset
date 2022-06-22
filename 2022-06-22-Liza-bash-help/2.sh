[[ "$#" -ne 2 ]] && echo "Ожидается 2 аргумента" && exit

L=$1
K=$2

[[ $L -lt 0 || $L -ge $K ]] && echo "Неправильные аргументы" && exit

dir="KZAL1-18"
mkdir $dir
touch $dir/wynik1.txt

counter=0
total_size=0

for filename in `find /etc -type f -size +"$L"c -size -"$K"c`; do
        size=`du -b $filename | cut -f -1`
        ((total_size=total_size+size))
        echo "$counter--$filename--`stat -c '%y' $filename`--`wc -l $filename`--$size" >> $dir/wynik1.txt
        ((counter=counter+1))
done

echo "-----------" >> $dir/wynik1.txt
echo $total_size >> $dir/wynik1.txt
