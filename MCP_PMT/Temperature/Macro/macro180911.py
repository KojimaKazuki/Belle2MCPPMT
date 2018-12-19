#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np

dtype_data1 = [('data1_Num','i8'),('data1_t','S32'),('data1_Ch1','d8'),('data1_Ch2','d8'),('data1_Dark1','d16')]
data1 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1809/DarkRate180907_1H.csv", delimiter=",", unpack = True, dtype = dtype_data1)


data1new = np.reshape( data1,( -1, 1))

N = data1.shape
Nnew = data1new.shape
print N, Nnew



from datetime import datetime
import time

from ROOT import TCanvas,TGraph,gStyle,gROOT,TAxis,TStyle,TLegend
from ROOT import kRed,kBlue

gStyle.SetOptStat()
gStyle.SetOptTitle(0)
#gROOT.SetStyle("Plain")

#c1 = TCanvas("c1","c1")
#c2 = TCanvas("c2","c2")

graph1 = TGraph()
graph2 = TGraph()


unixtime = []
Nstart = 0


for k in range(Nstart,N[0]):
    #print data1[k][1], data1[k][2], data1[k][3]

    #tks = datetime.strptime(data1[k][1], '%Y/%m/%d %H:%M:%S')
    #unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]


    graph1.SetPoint(k-Nstart, data1[k][2], data1[k][4])
    graph2.SetPoint(k-Nstart, data1[k][3], data1[k][4])


graph1.SetTitle(";Temperature / degree Celsius; Dark Rate / kHz")
graph1.GetXaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleOffset(1.2)
graph1.GetXaxis().SetRangeUser(30.,40.)
graph1.SetMarkerStyle(8)
graph1.SetMarkerSize(1.5)


#graph2.SetTitle(";Temperature / degree Celsius; Dark Rate / kHz")
#graph2.GetXaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleOffset(1.2)
graph2.GetXaxis().SetRangeUser(30.,40.)
graph2.SetMarkerColor(kBlue)
graph2.SetMarkerStyle(8)
graph2.SetMarkerSize(1.5)


legend = TLegend(0.3,0.73,0.88,0.88)
legend.AddEntry(graph1,"Ch1 on the side of PMT","p")
legend.AddEntry(graph2,"Ch2 near the thermocouple terminal of the heater","p")
legend.AddEntry(None, "MCP-PMT ID: KT0835 ;   HV = 2,110 V", "")


c1 = TCanvas("c1","c1")


c1.cd()
c1.DrawFrame(36.,0.7,38.5,2.)
graph1.Draw("P")
graph2.Draw("P")
legend.Draw("same")


wait = raw_input(">> Press Enter to Continue. \n")

