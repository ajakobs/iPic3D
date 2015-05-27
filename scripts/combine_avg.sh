#!/bin/bash

# puts together the average times of all phases (columns) for a given number of processes 
# f*: fields
# p*: particles
# m*: moments
# t*: total

if [ $# -lt 2 ]
then
	echo "Call script with number of processes and either "cluster" or "booster"!"
	exit
fi

PROCS=$1
syspart=$2
SET=${PROCS}P
OUT=${SET}_${syspart}.combined

echo $SET "ftotal fcomput fcommun fmsgpro fsndrcv fallred fcb ptotal pcomput pcommun pmsgpro psndrcv pallred pcb mtotal mcomput mcommun mmsgpro msndrcv mallred mcb ttotal tcomput tcommun tmsgpro tsndrcv tallred tcb" > $OUT
THREADS=`ls ${SET}*${syspart}* | egrep -o [0-9]+T | sed -e s/[^0-9]//g | sort -n`
for t in $THREADS 
do
	FILE=`find . -name "${SET}${t}T*${syspart}*"`
	printf "%d " $t >> $OUT
	echo `sed -n '2p' $FILE` >> $OUT 	
done


