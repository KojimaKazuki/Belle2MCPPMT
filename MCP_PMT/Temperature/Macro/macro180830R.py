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

from ROOT import TCanvas,TGraph,gStyle,gROOT,TAxis,TStyle,TLegend,TH2F
from ROOT import kRed,kBlue

gStyle.SetOptStat()
gStyle.SetOptTitle(0)
gStyle.SetStatX(0.88)
gStyle.SetStatY(0.88)
#gROOT.SetStyle("Plain")


graph1 = TGraph(N[0])
graph2 = TGraph(N[0])



hist1 = TH2F('hist1','hist1',25,23.,28.,25,23.,28.)


unixtime = []


for k in range(N[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data1[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k, unixtime[k], data1[k][3])
    graph2.SetPoint(k, unixtime[k], data1[k][4])
    hist1.Fill(data1[k][3],data1[k][4])


graph1.GetXaxis().SetTimeDisplay(1)
graph1.GetXaxis().SetNdivisions(503)
graph1.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph1.GetXaxis().SetTimeOffset(0,"JST")
graph1.SetTitle(";Date&Time;Temperature [degree Celsius]")
graph1.GetXaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleSize(0.04)
graph1.GetYaxis().SetTitleOffset(1.2)
#graph1.GetXaxis().SetRangeUser(unixtime[0],unixtime[500])
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
#graph2.GetXaxis().SetRangeUser(unixtime[0],unixtime[500])
graph2.GetYaxis().SetRangeUser(23.,30.)
graph2.SetLineColor(kBlue)
graph2.SetLineWidth(2)


hist1.SetTitle(";Ch1 Temperature [degree Celsius];Ch2 Temperature [degree Celsius]")
hist1.GetXaxis().SetTitleSize(0.04)
hist1.GetYaxis().SetTitleSize(0.04)
hist1.GetYaxis().SetTitleOffset(1.2)


c1 = TCanvas("c1","c1")
c2 = TCanvas("c2","c2")

c1.cd()
graph1.Draw()
graph2.Draw("same")


legend = TLegend(0.3,0.73,0.88,0.88)
legend.AddEntry(graph1,"Ch1 on the side of PMT","l")
legend.AddEntry(graph2,"Ch2 (Room Temperature)","l")
legend.AddEntry(0, "MCP-PMT ID: KT0835;   HV = 2,110 V", "")
legend.Draw("same")


c2.cd()
hist1.Draw("colz")



# Graph for Rate of Change
graph3 = []
graph4 = []
graph5 = []


c3 = TCanvas ("c3","c3")
Nsum = 2
NdivC = Nsum//2 + Nsum%2
#NdivC = 3
print NdivC
c3.Divide(2,NdivC)


c4 = TCanvas("c4","c4")
c4.Divide(2,NdivC)


average1 = []
average2 = []
change1 = [0]
change2 = [0]

mstart = 2700
mend = 2800


for n in range(Nsum):
    graph3.append(TGraph())
    graph4.append(TGraph())
    graph5.append(TGraph())

    for m in range(mstart,mend):
        sum1_ml = 0
        average1_m = 0
        change1_m = 0

        sum2_ml = 0
        average2_ml = 0
        change2_m = 0

        for l in range(n+1):
            sum1_ml += data1[m+l][3]
            sum2_ml += data1[m+l][4]

        average1_m = sum1_ml/(n+1)
        average1.append(average1_m)

        average2_m = sum2_ml/(n+1)
        average2.append(average2_m)
        

        if m > mstart - 1:
            change1_m = average1[m-mstart] - average1[m-mstart-1]
            change1.append(change1_m)
            graph3[n].SetPoint( m-mstart, unixtime[m], change1[m-mstart])

            change2_m = average2[m-mstart] -average2[m-mstart-1]
            change2.append(change2_m)
            graph4[n].SetPoint( m-mstart, unixtime[m], change2[m-mstart])


        if change1_m == change2_m:
            ratio_m = 0
            graph5[n].SetPoint( m-mstart, unixtime[m], ratio_m)
        elif change1_m < change2_m:
            ratio_m = 1
            graph5[n].SetPoint( m-mstart, unixtime[m], ratio_m)
        elif change1_m > change2_m:
            ratio_m = -1
            graph5[n].SetPoint( m-mstart, unixtime[m], ratio_m)
            

    graph3[n].GetXaxis().SetTimeDisplay(1)
    graph3[n].GetXaxis().SetNdivisions(503)
    graph3[n].GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
    graph3[n].GetXaxis().SetTimeOffset(0,"JST")
    graph3[n].SetTitle(";Date&Time;Temperature [degree Celsius]")
    graph3[n].GetXaxis().SetTitleSize(0.04)
    graph3[n].GetYaxis().SetTitleSize(0.04)
    graph3[n].GetYaxis().SetTitleOffset(1.2)   
    #graph3[n].GetYaxis().SetRangeUser(23.,30.)
    graph3[n].SetLineWidth(2)

    #graph4[n].GetXaxis().SetTimeDisplay(1)
    #graph4[n].GetXaxis().SetNdivisions(503)
    #graph4[n].GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
    #graph4[n].GetXaxis().SetTimeOffset(0,"JST")
    #graph4[n].SetTitle(";Date&Time;Temperature[degree Celsius]")
    #graph4[n].GetXaxis().SetTitleSize(0.04)
    #graph4[n].GetYaxis().SetTitleSize(0.04)
    #graph4[n].GetYaxis().SetTitleOffset(1.2)
    #graph4[n].GetXaxis().SetRangeUser(unixtime[0],unixtime[500])
    #graph4[n].GetYaxis().SetRangeUser(23.,30.)
    graph4[n].SetLineColor(kRed)
    graph4[n].SetLineWidth(1)

    
    graph5[n].GetXaxis().SetTimeDisplay(1)
    graph5[n].GetXaxis().SetNdivisions(503)
    graph5[n].GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
    graph5[n].GetXaxis().SetTimeOffset(0,"JST")
    graph5[n].SetTitle(";Date&Time;Temperature [degree Celsius]")
    graph5[n].GetXaxis().SetTitleSize(0.04)
    graph5[n].GetYaxis().SetTitleSize(0.04)
    graph5[n].GetYaxis().SetTitleOffset(1.2)   
    #graph5[n].GetYaxis().SetRangeUser(23.,30.)
    graph5[n].SetLineWidth(2)

for j in range(Nsum):
    jnew = j
    c3.cd(j+1)
    graph3[jnew].Draw()
    graph4[jnew].Draw("same")
    c4.cd(j+1)
    c4.SetLogy()
    graph5[jnew].Draw()


wait = raw_input(">> Press Enter to Continue. \n")

