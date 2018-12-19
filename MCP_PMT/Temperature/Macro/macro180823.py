#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np

dtype_data1 = [('data1_Num','i8'),('data1_t','S32'),('data1_ms','d8'),('data1_Ch1','d8'),('data1_Ch2','d8'),('data1_Alarm1','S16'),('data1_Alarm2','S16'),('data1_AlarmOut','S8')]
data1 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1808/Temperature180828_1.csv", delimiter=",", unpack = True, dtype = dtype_data1)


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

graph1 = TGraph(N[0])
graph2 = TGraph(N[0])


unixtime = []


for k in range(N[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data1[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]


    graph1.SetPoint(k, unixtime[k], data1[k][3])
    graph2.SetPoint(k, unixtime[k], data1[k][4])



graph1.GetXaxis().SetTimeDisplay(1)
graph1.GetXaxis().SetNdivisions(503)
graph1.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph1.GetXaxis().SetTimeOffset(0,"JST")
graph1.SetTitle(";Date&Time;Temperature [degree Celsius]")
graph1.GetXaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleOffset(1.2)
graph1.GetYaxis().SetRangeUser(23.,30.)
graph1.SetLineWidth(2)


#graph2.GetXaxis().SetTimeDisplay(1)
#graph2.GetXaxis().SetNdivisions(503)
#graph2.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
#graph2.GetXaxis().SetTimeOffset(0,"JST")
#graph2.SetTitle(";Date&Time;Temperature[degree Celsius]")
#graph2.GetXaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleOffset(1.2)
graph2.GetYaxis().SetRangeUser(23.,30.)
graph2.SetLineColor(kBlue)
graph2.SetLineWidth(2)


c1 = TCanvas("c1","c1")
#c2 = TCanvas("c2","c2")

c1.cd()
graph1.Draw()
#c2.cd()
graph2.Draw("same")


legend = TLegend(0.3,0.73,0.88,0.88)
legend.AddEntry(graph1,"Ch1 on the side of PMT","l")
legend.AddEntry(graph2,"Ch2 (Room Temperature)","l")
legend.AddEntry(0, "MCP-PMT ID: KT0835;   HV = 2,110 V", "")
legend.Draw("same")


wait = raw_input(">> Press Enter to Continue. \n")

