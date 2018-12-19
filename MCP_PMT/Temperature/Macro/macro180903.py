#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np

dtype_data1 = [('data1_Num','i8'),('data1_t','S32'),('data1_ms','d8'),('data1_Ch1','d8'),('data1_Ch2','d8'),('data1_Alarm1','S16'),('data1_Alarm2','S16'),('data1_AlarmOut','S8')]
data1 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181025_1LED100k.csv", delimiter=",", unpack = True, dtype = dtype_data1)
data1New = np.reshape( data1,( -1, 1))

N1 = data1.shape
N1New = data1New.shape
print N1, N1New



from datetime import datetime
import time

from ROOT import TCanvas,TGraph,gStyle,gROOT,TAxis,TStyle,TLegend,TF1
from ROOT import kRed,kBlue,kOrange

gStyle.SetOptStat()
gStyle.SetOptTitle(0)
#gROOT.SetStyle("Plain")

#c1 = TCanvas("c1","c1")
#c2 = TCanvas("c2","c2")

graph1 = TGraph()
graph2 = TGraph()
graph3 = TGraph()
graph4 = TGraph()
graph5 = TGraph()


unixtime = []
Tset0 = 40.
Tset1 = 40.
Tset2 = 40.
Tset3 = 40.
Tset4 = 40.
Nstart = 0
Nend = 720

for k in range(Nstart,Nend):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data1[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]


    graph1.SetPoint(k-Nstart, unixtime[k-Nstart], data1[k][3])
    graph2.SetPoint(k-Nstart, unixtime[k-Nstart], data1[k][4])
    graph3.SetPoint(k-Nstart, unixtime[k-Nstart], data1[k][4]/data1[k][3])
    if k < 61:
        graph4.SetPoint(k-Nstart, unixtime[k-Nstart], Tset0)
    elif k > 60 and k < 361 :
        graph4.SetPoint(k-Nstart, unixtime[k-Nstart], Tset1)
    elif k > 360 and k < 661 :
        graph4.SetPoint(k-Nstart, unixtime[k-Nstart], Tset2)
    elif k > 660 and k < 901 :
        graph4.SetPoint(k-Nstart, unixtime[k-Nstart], Tset3)
    elif k > 900 and k < N1[0] :
        graph4.SetPoint(k-Nstart, unixtime[k-Nstart], Tset4)
    graph5.SetPoint(k-Nstart, unixtime[k-Nstart], 1.)

print unixtime[0],unixtime[120],unixtime[Nend-1]



graph1.GetXaxis().SetTimeDisplay(1)
graph1.GetXaxis().SetNdivisions(503)
graph1.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph1.GetXaxis().SetTimeOffset(0,"JST")
graph1.SetTitle(";Date&Time;Temperature [degree Celsius]")
graph1.GetXaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleOffset(1.2)
graph1.GetYaxis().SetRangeUser(20.,35.)
graph1.SetLineWidth(2)


#graph2.GetXaxis().SetTimeDisplay(1)
#graph2.GetXaxis().SetNdivisions(503)
#graph2.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
#graph2.GetXaxis().SetTimeOffset(0,"JST")
#graph2.SetTitle(";Date&Time;Temperature [degree Celsius]")
#graph2.GetXaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleOffset(1.2)
graph2.GetYaxis().SetRangeUser(20.,35.)
graph2.SetLineColor(kBlue)
graph2.SetLineWidth(2)
graph2.SetMarkerColor(kBlue)


graph3.GetXaxis().SetTimeDisplay(1)
graph3.GetXaxis().SetNdivisions(503)
graph3.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph3.GetXaxis().SetTimeOffset(0,"JST")
graph3.SetTitle(";Date&Time;Ratio of Temperature")
graph3.GetXaxis().SetTitleSize(0.04)
graph3.GetYaxis().SetTitleSize(0.04)
graph3.GetYaxis().SetTitleOffset(1.2)
graph3.GetYaxis().SetRangeUser(0.9,1.1)
graph3.SetLineWidth(2)


#graph4.GetXaxis().SetTimeDisplay(1)
#graph4.GetXaxis().SetNdivisions(503)
#graph4.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
#graph4.GetXaxis().SetTimeOffset(0,"JST")
#graph4.SetTitle(";Date&Time;Ratio of Temperature")
#graph4.GetXaxis().SetTitleSize(0.04)
#graph4.GetYaxis().SetTitleSize(0.04)
#graph4.GetYaxis().SetTitleOffset(1.2)
#graph4.GetYaxis().SetRangeUser(0.9,1.1)
graph4.SetLineColor(kRed)
graph4.SetLineWidth(2)

graph5.SetLineColor(kRed)
graph5.SetLineWidth(2)


#legend = TLegend(0.3,0.14,0.88,0.29)
legend = TLegend(0.3,0.73,0.88,0.88)
legend.AddEntry(graph1,"Thermocouple Ch1 (JT0695) on the left side","l")
legend.AddEntry(graph2,"Thermocouple Ch2 (YH0205) on the right side","l")
#legend.AddEntry(graph4,"Set Temperature","l")
#legend.AddEntry(graph3,"The Ratio of Temperature Ch2/Ch1","l")
#legend.AddEntry(graph5,"Ratio = 1.0","l")
legend.AddEntry(None, "Viewing from the light source", "")
#legend.AddEntry(None, "MCP-PMT ID: JT0695 ;   HV = 0 V", "")


c1 = TCanvas("c1","c1")
c2 = TCanvas("c2","c2")

c1.cd()
graph1.Draw()
#graph2.Draw()
graph2.Draw("same")
func1.Draw("same")
func2.Draw("same")
func3.Draw("same")
func4.Draw("same")
#graph4.Draw("same")
legend.Draw("same")

c2.cd()
graph3.Draw()
graph5.Draw("same")
#legend.Draw("same")



wait = raw_input(">> Press Enter to Continue. \n")

