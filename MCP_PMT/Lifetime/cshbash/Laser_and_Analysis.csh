#!/bin/csh

rm -f timer.txt
date > timer.txt

set n=15
#set n=1
set l=1 #add at 2nd Nov, 2017 by muroyama
cd mcp/bin

echo "***************************************"
echo "   Check below that LED rate is 0 Hz   "
echo "***************************************"
./SetLED -c off
if ($?) then
  echo "LED Error. Call expert."
  exit
endif

#echo ""
#echo "****** Be sure to check LED turned OFF & refPMT turned ON. ******   "
#echo ""
#echo "If OK, push ENTER"

#if ($<)then
#endif

../../CurrentMonitor.sh #added at 4th Aug, 2015 by Maeda Yosuke

./SetHV on
if ($?) then
  exit
endif

#set t=""
#while ($t != "y" && $t != "Y" && $t != "yes" && $t != "Yes" && $t != "YES")
    ./lifetime test
    echo ""
    echo "Please check count rates. Is it OK? [y/n]:"
    set t=$<
    if ($t != "y") then
      ./SetHV -f off
      exit
    endif
#end

set dir=`cat ../../directory.txt`
set runnum=`cat ../../../dataset/$dir/data/laser/runnumber`
echo ""
echo "****** Start Laser Run $runnum ******"
echo ""

while ($n > 0)
    ../../CurrentMonitor.sh #added at 4th Aug, 2015 by Maeda Yosuke
    echo "Wait for ${n} min."
    @ n = $n - 1
    sleep 60
end
../../CurrentMonitor.sh #added at 4th Aug, 2015 by Maeda Yosuke

while ($l > 0)
    ./lifetime laser
    echo "start laser and after-pulse run${l}"
    @ l = $l - 1
end #add at 2nd Nov, 2017 by muroyama

../../CurrentMonitor.sh #added at 4th Aug, 2015 by Maeda Yosuke

./SetHV -f off

cd ../

date >> timer.txt

echo ""
echo "DAQ finished."
date
echo ""
cd ../

./Ana.sh

./CurrentMonitor.sh #added at 4th Aug, 2015 by Maeda Yosuke

./mail_text.sh #added at 22th June, 2018 by Genta muroyama
#./afterpulse 2000
#./Analysis.csh $argv[1]
