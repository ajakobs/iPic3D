#!/bin/bash

summe(){
	FILE=$1
	echo "ftotal fcomput fcommun fmsgpro fsndrcv fallred fcb ptotal pcomput pcommun pmsgpro psndrcv pallred pcb mtotal mcomput mcommun mmsgpro msndrcv mallred mcb ttotal tcomput tcommun tmsgpro tsndrcv tallred tcb"
	numphases=28
	i=2
	while [ $i -le $[$numphases+1] ];
	do
		awk -v col=$i '{ total += $col ; count++ } END { printf "%f ", total }' $FILE
		i=$[$i+1]
	done
}

INFILES=$@

for file in $INFILES;
do
        if [ -d $file ]
        then
                FILES="$FILES `find $file -name "*_parsed"`"
        else
                FILES="$FILES $file"
        fi
done

for file in $FILES
do
        summe $file > ${file}_sum
        echo Calculated sums from $file to ${file}_sum
done


