#!/bin/sh

cat directory.txt | while read life

do
#echo ${life}
cat /home/takt/lifetime/dataset/${life}/data/laser/runnumber | while read line

do
i=${line}
#j=`expr $i - 1`
l=1

while(($l > 0)) #added at 2nd Nov, 2017 by muroyama
do
    j=`expr $i - $l`
    echo $j

    ./Analysis.csh $j
    l=$(( l-1 ))
done
done
done