[[ "$#" -ne 2 ]] && echo "Ожидается 2 аргумента" && exit

L=$1 
K=$2 

# Тут была ошибка в синтаксисе условия
[[ 0 -gt $L || $L -ge $K ]] && break;
 
touch wynik1.txt 
counter=0 
linii=0 

# В конце диапазона в for нужна ;
for filename in `find /etc -type f -size +"$K"c -size -"$L"c | tail -n 7`;
do 
        size=`du -b $filename | cut -f -1` 
        ((linii=linii+size)) 
        #                                    v Тут забыла пробел)
        #                                          v И имя файла
        echo "$counter---$filename---`stat -c '%y' $filename`---$size">>wynik1.txt 
        ((counter=counter+1)) 
done 
 
echo "___________">>wynik1.txt 
echo "$linii">>wynik1.txt
