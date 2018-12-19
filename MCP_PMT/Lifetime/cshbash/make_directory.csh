#!/bin/csh

if($#argv != 1)then
    echo "Error"
    exit
endif

rm -f directory.txt
echo $argv[1] > directory.txt

mkdir ../dataset/${argv[1]}/
cd ../dataset/${argv[1]}/
mkdir data/
cd data
mkdir test
mkdir pedestal
mkdir laser
mkdir led
echo "0" > test/runnumber
echo "0" > pedestal/runnumber
echo "0" > laser/runnumber
echo "0" > led/runnumber
echo "0" > led/filenumber
cd ../
mkdir dst
cd dst
mkdir laser
mkdir led
cd ../
mkdir converted
cd converted
mkdir root
mkdir ascii
mkdir psfile
cd psfile
mkdir kihon
cd ../../
mkdir summary

\rm mcp/oc_monitor/temp.dat
\rm mcp/oc_monitor/runnumber.dat
