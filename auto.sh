
lsize=300
rm -f results.txt
touch results.txt
while [ $lsize -lt 6001 ]
do
	echo 'LOOPSIZE' $lsize | tee -a results.txt
	bin/timed_test 1 $lsize &
	bin/timed_test 0 $lsize >> results.txt
	bin/timed_test 1 $lsize &
	bin/timed_test 0 $lsize >> results.txt
	bin/timed_test 1 $lsize &
	bin/timed_test 0 $lsize >> results.txt
	echo '--------------------------------------------------------------------------------' >> results.txt
	let "lsize = $lsize + 300"
done

echo 'DONE'
