#!/bin/csh

if ($#argv != 1)then
    echo "Usage: ./Analsys.csh [laser run no]"
    exit
endif

set n=0
@ n = $argv[1] - 1

cd mcp/bin
./MakePlot $argv[1]
./LEDSummary $n
./LaserSummary $argv[1]

