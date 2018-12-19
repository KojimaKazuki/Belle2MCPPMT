#!/bin/sh
  echo "Please write shift reports"
  echo "Shifter name:"
  read name
  #echo "$name"
  echo "Laser Run number:"
  read run_num
  echo "Write down something else (if you have no notice, please write fine.):"
  read reports

echo "From: takt@hepl.phys.nagoya-u.ac.jp" > mail.txt
#echo "To: muroyama@hepl.phys.nagoya-u.ac.jp" >> mail.txt
echo "To: muroyama@hepl.phys.nagoya-u.ac.jp,kojima@hepl.phys.nagoya-u.ac.jp" >> mail.txt
echo "Subject: Lifetime shift reports" >> mail.txt
echo >> mail.txt
echo "Shifter name : $name" >> mail.txt
echo "Notice : $reports" >> mail.txt
echo "Laser run $run_num is finished!" >> mail.txt
cat mail.txt | sendmail -i -t
#rm -f mail.txt
