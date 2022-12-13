#! /bin/bash
echo "Enter array"
read -a num
length=${#num[@]}
#bubble sort
for((i=0;i<length;i++))
do
	j=`expr $i + 1`
	for((j;j<length;j++))
	do
		if [ ${num[$i]} -gt ${num[$j]} ]
		then 
			#swap
			temp=${num[$i]}
			num[$i]=${num[$j]}
			num[$j]=$temp 
			#echo "swap"
		fi
	done
done

echo ${num[@]}