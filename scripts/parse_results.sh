#!/bin/bash

# parses output files of iPiC3D in "parsed_filename"
# creates data table which includes the times of all phases (columns) for each cycle (rows)
# f*: fields
# p*: particles
# m*: moments
# t*: total

parse(){
	FILE=$1
	echo "cycle ftotal fcomput fcommun fmsgpro fsndrcv fallred fcb ptotal pcomput pcommun pmsgpro psndrcv pallred pcb mtotal mcomput mcommun mmsgpro msndrcv mallred mcb ttotal tcomput tcommun tmsgpro tsndrcv tallred tcb"
	numcycles=`grep -c "times for cycle" $FILE`
	#grep -m1 "times for cycle" $FILE | awk '{print $5}'
	cycle=0
	while [ $cycle -lt $numcycles ];
	do
		OUT=`grep -A5 "times for cycle $cycle " $FILE`
		OUT=`echo $OUT | sed 's/[^0-9\.^0-9]/ /g'` 
		echo $OUT
		cycle=$[$cycle+1]
	done
			
}

INFILES=$@

for file in $INFILES;
do
	if [ -d $file ]
	then
		FILES="$FILES `find $file -name "*.out"`"
	else
		FILES="$FILES $file"
	fi
done

for file in $FILES
do
	parse $file > ${file}_parsed
	if [ `wc -l < ${file}_parsed` -le 1 ] # only the header line, $file is not a regular output file, therefor delete it
	then
		rm ${file}_parsed
	else
		echo Parsed $file to ${file}_parsed
	fi
done

