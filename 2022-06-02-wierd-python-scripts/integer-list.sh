import sys

print(
    ",".join(
        map(
            lambda digit: str(int(digit) + 40),
            sys.argv[1].split(",")
        )
    )
)

# Берёт список чисел, разделённый запятыми и увеличивает каждое число на 40
for coords in `egrep -o "([0-9]{2,3},)+[0-9]{2,3}" index.html`;do  
    sed -i "s/$coords/`python3 ~/40.py $coords`/g" index.html
done

