#!/bin/sh

#created at 4th Aug, 2015
#by Maeda Yosuke

cd /home/takt/lifetime/daq/mcp/bin
RunDir=`cat /home/takt/lifetime/daq/directory.txt`
RunNum=`cat /home/takt/lifetime/dataset/"${RunDir}"/data/laser/runnumber`
OutputDir="/home/takt/lifetime/dataset/${RunDir}/data/current"
OutputFile="${OutputDir}/current_${RunNum}.dat"

if [ ! -e "${OutputDir}" ]
    then
    mkdir "${OutputDir}"
    echo "New directory \"${OutputDir}\" was created"
    echo "(Vmon for ch0 of crate01) (Imon for 0-01) (Vmon for 1-01) (Imon for 1-01) (Vmon for 2-01)... (Vmon for 0-10) (Imon for 0-10) (unix time)" > "${OutputDir}/format"
    echo "readout ch : crate01-ch0,ch1,ch2,ch3, crate02-ch0,ch1,ch2,ch3, crate10-ch0" >> "${OutputDir}/format"
fi

echo "Voltage/Current monitoring : ${OutputFile}"
./GetHVAll "${OutputFile}"
