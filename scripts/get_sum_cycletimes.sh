#!/bin/bash

summe(){
	echo "ftotal fcomput fcommun fmsgpro fsndrcv fallred fcb ptotal pcomput pcommun pmsgpro psndrcv pallred pcb mtotal mcomput mcommun mmsgpro msndrcv mallred mcb ttotal tcomput tcommun tmsgpro tsndrcv tallred tcb"
	numphases=28
	i=2
	while [ $i -le $[$numphases+1] ];
	do
		awk -v col=$i '{ total += $col ; count++ } END { printf "%f ", total }' $FILE
		i=$[$i+1]
	done
}

FILE=$1
summe $FILE > ${FILE}_sum
