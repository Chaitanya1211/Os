#! /bin/bash
#prime
arr=(12 43 76 89)
Prime(){
	num=$1
	flag=0
	for((i=2;i<$num;i++))
	do
		rem=`expr $num % $i`
		if [ $rem -eq 0 ]
		then
			flag=1
		fi
	done
	
	if [ $flag -eq 1 ]
	then

		echo "not a prime number"
	else
		echo "prime number"
	fi
	flag=0
}

for i in ${arr[@]}
do
Prime $i	
done

