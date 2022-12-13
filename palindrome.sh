#! /bin/bash
echo "hello world"

Palindrome(){
	num=$1
	sum=0
	rem=0
	rev=""
	temp=$num
	
	while [ $num -gt 0 ]
	do
		rem=$(( $num % 10 ))
		num=$((num / 10))
		rev=$(echo ${rev}${rem})
	done
	
	if [ $temp -eq $rev ]
	then
		echo "Palindrome"
	else
		echo "Not palindrome"
	fi	
}

echo "Enter numbers :"
read -a num

for i in ${num[@]}
do
	Palindrome $i
done

