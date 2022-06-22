[[ "$#" -ne 3 ]] && echo "Ожидается 3 аргумента: out_file, ext, search_dir" && exit

out_file=$1
extension=$2
search_dir=$3

counter=0
total_size=0

for filename in `ls $search_dir`; do
	# Тут число типа 644, где
	# 1 цифра: права пользователя
	# 2 цифра: права группы
	# 3 цифра: права other (всех остальных)
	permissions=`stat -c "%a %n" $search_dir/$filename | cut -d " " -f1`
	# Нам нужна первая цифра, и она должна быть >= 4, т.к.
	# права в линуксе формируются так:
	#
	# r (read)    = 4
	# w (write)   = 2
	# x (execute) = 1
	#
	# Значит, нам нужно число больше 4 (типа 4+0+0, 4+0+1 и т.д)
	read_field=`echo ${permissions:0:1}`
	if [[ $read_field -ge 4 ]]; then
		bytes=`du -b $search_dir/$filename | cut -f -1`
		echo "$counter------$bytes-----$filename-----" >> $out_file
		# $((total_size=total_size+bytes))
		((counter=counter+1))
		((total_size=total_size+bytes))
	fi
done

echo "==========" >> $out_file
echo $total_size >> $out_file
