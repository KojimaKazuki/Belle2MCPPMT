#!/bin/csh

#rm -f OC_monitor/OC_data.dat
#rm OC_monitor/OC_data.dat

#echo ""
#echo "****** Check RefPMT turned OFF ******   "
#echo ""
#echo " If OK, turn LED ON and push ENTER to start LED run"

#$<

cd mcp/bin/

./SetHV -c off
if ($?) then
  echo "HV Error. Call expert."
  exit
endif

./SetLED on

while(1)
    
    ../../CurrentMonitor.sh #added at 4th Aug, 2015 by Maeda Yosuke

    ./lifetime led
    date
    
    cd ../oc_monitor
    root -l -q monitor.C

    if ($?) then
      cd ../bin
      ./SetLED -i off
      if ($? == "0") then
        RunDir = 'cat /home/takt/lifetime/daq/directory.txt'
        cp -u /home/takt/lifetime/mcp/oc_monitor/${RunDir}LEDRunData.dat /home/takt/lifetime/dataset/"${RunDir}"/dst/led/
        exit
      endif
    else
      cd ../bin
    endif
    #sleep 120 # temporary
end
