#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np

dtype_data1 = [('data1_Num','i8'),('data1_t','S32'),('data1_ms','d8'),('data1_Ch1','d8'),('data1_Ch2','d8'),('data1_Alarm1','S16'),('data1_Alarm2','S16'),('data1_AlarmOut','S8')]
data1 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181022_1HVon.csv", delimiter=",", unpack = True, dtype = dtype_data1)
data1New = np.reshape( data1,( -1, 1))

dtype_data2 = [('data2_Num','i8'),('data2_t','S32'),('data2_ms','d8'),('data2_Ch1','d8'),('data2_Ch2','d8'),('data2_Alarm1','S16'),('data2_Alarm2','S16'),('data2_AlarmOut','S8')]
data2 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181022_2LED1k.csv", delimiter=",", unpack = True, dtype = dtype_data2)
data2New = np.reshape( data2,( -1, 1))

dtype_data3 = [('data3_Num','i8'),('data3_t','S32'),('data3_ms','d8'),('data3_Ch1','d8'),('data3_Ch2','d8'),('data3_Alarm1','S16'),('data3_Alarm2','S16'),('data3_AlarmOut','S8')]
data3 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181023_1LED10k.csv", delimiter=",", unpack = True, dtype = dtype_data3)
data3New = np.reshape( data3,( -1, 1))

dtype_data4 = [('data4_Num','i8'),('data4_t','S32'),('data4_ms','d8'),('data4_Ch1','d8'),('data4_Ch2','d8'),('data4_Alarm1','S16'),('data4_Alarm2','S16'),('data4_AlarmOut','S8')]
data4 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181023_2Laser3.csv", delimiter=",", unpack = True, dtype = dtype_data4)
data4New = np.reshape( data4,( -1, 1))

dtype_data5 = [('data5_Num','i8'),('data5_t','S32'),('data5_ms','d8'),('data5_Ch1','d8'),('data5_Ch2','d8'),('data5_Alarm1','S16'),('data5_Alarm2','S16'),('data5_AlarmOut','S8')]
data5 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181023_3LED100k.csv", delimiter=",", unpack = True, dtype = dtype_data5)
data5New = np.reshape( data5,( -1, 1))

dtype_data6 = [('data6_Num','i8'),('data6_t','S32'),('data6_ms','d8'),('data6_Ch1','d8'),('data6_Ch2','d8'),('data6_Alarm1','S16'),('data6_Alarm2','S16'),('data6_AlarmOut','S8')]
data6 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181024_1LEDoff.csv", delimiter=",", unpack = True, dtype = dtype_data6)
data6New = np.reshape( data6,( -1, 1))

dtype_data7 = [('data7_Num','i8'),('data7_t','S32'),('data7_ms','d8'),('data7_Ch1','d8'),('data7_Ch2','d8'),('data7_Alarm1','S16'),('data7_Alarm2','S16'),('data7_AlarmOut','S8')]
data7 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181025_1LED100k.csv", delimiter=",", unpack = True, dtype = dtype_data7)
data7New = np.reshape( data7,( -1, 1))

dtype_data8 = [('data8_Num','i8'),('data8_t','S32'),('data8_ms','d8'),('data8_Ch1','d8'),('data8_Ch2','d8'),('data8_Alarm1','S16'),('data8_Alarm2','S16'),('data8_AlarmOut','S8')]
data8 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181026_1HVoff.csv", delimiter=",", unpack = True, dtype = dtype_data8)
data8New = np.reshape( data8,( -1, 1))

dtype_data9 = [('data9_Num','i8'),('data9_t','S32'),('data9_ms','d8'),('data9_Ch1','d8'),('data9_Ch2','d8'),('data9_Alarm1','S16'),('data9_Alarm2','S16'),('data9_AlarmOut','S8')]
data9 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181026_2LED100k.csv", delimiter=",", unpack = True, dtype = dtype_data9)
data9New = np.reshape( data9,( -1, 1))


N1 = data1.shape
N2 = data2.shape
N3 = data3.shape
N4 = data4.shape
N5 = data5.shape
N6 = data6.shape
N7 = data7.shape
N8 = data8.shape
N9 = data9.shape

NNew = []
NNew.append(data1New.shape)
NNew.append(data2New.shape)
NNew.append(data3New.shape)
NNew.append(data4New.shape)
NNew.append(data5New.shape)
NNew.append(data6New.shape)
NNew.append(data7New.shape)
NNew.append(data8New.shape)
NNew.append(data9New.shape)

print N1, NNew


from datetime import datetime
import time

from ROOT import TCanvas,TGraph,gStyle,gROOT,TAxis,TStyle,TLegend,TLine
from ROOT import kRed,kBlue

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
Ntotal = 0

for k in range(N1[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data1[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k, unixtime[k], data1[k][3])
    graph2.SetPoint(k, unixtime[k], data1[k][4])
    graph3.SetPoint(k, unixtime[k], data1[k][4]/data1[k][3])

graph4.SetPoint(0, unixtime[Ntotal], 1.)
graph5.SetPoint(0, unixtime[Ntotal], 1.)
Ntotal += N1[0]
print unixtime[Ntotal-1]

for k in range(N2[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data2[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data2[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data2[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data2[k][4]/data2[k][3])

graph4.SetPoint(1, unixtime[Ntotal], 1.015)
graph5.SetPoint(1, unixtime[Ntotal], 1.227)
Ntotal += N2[0]
print unixtime[Ntotal-1]

for k in range(N3[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data3[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data3[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data3[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data3[k][4]/data3[k][3])

graph4.SetPoint(2, unixtime[Ntotal], 1.023)
graph5.SetPoint(2, unixtime[Ntotal], 1.431)
Ntotal += N3[0]
print unixtime[Ntotal-1]

for k in range(N4[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data4[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data4[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data4[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data4[k][4]/data4[k][3])

#graph4.SetPoint(3, unixtime[Ntotal], 1.003)
#graph5.SetPoint(3, unixtime[Ntotal], 1.499)
Ntotal += N4[0]
print unixtime[Ntotal-1]

for k in range(N5[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data5[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data5[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data5[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data5[k][4]/data5[k][3])

graph4.SetPoint(3, unixtime[Ntotal], 1.003)
graph5.SetPoint(3, unixtime[Ntotal], 1.499)
Ntotal += N5[0]
print unixtime[Ntotal-1]

for k in range(N6[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data6[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data6[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data6[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data6[k][4]/data6[k][3])

graph4.SetPoint(4, unixtime[Ntotal], 1.006)
graph5.SetPoint(4, unixtime[Ntotal], 1.449)
Ntotal += N6[0]
print unixtime[Ntotal-1]

for k in range(N7[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data7[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data7[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data7[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data7[k][4]/data7[k][3])

graph4.SetPoint(5, unixtime[Ntotal], 1.008)
graph5.SetPoint(5, unixtime[Ntotal], 1.549)
Ntotal += N7[0]
print unixtime[Ntotal-1]

for k in range(N8[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data8[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data8[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data8[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data8[k][4]/data8[k][3])

graph4.SetPoint(6, unixtime[Ntotal], 0.980)
graph5.SetPoint(6, unixtime[Ntotal], 1.408)
Ntotal += N8[0]
print unixtime[Ntotal-1]

for k in range(N9[0]):
    #print data1[k][1], data1[k][3], data1[k][4]

    tks = datetime.strptime(data9[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    graph1.SetPoint(k+Ntotal, unixtime[k+Ntotal], data9[k][3])
    graph2.SetPoint(k+Ntotal, unixtime[k+Ntotal], data9[k][4])
    graph3.SetPoint(k+Ntotal, unixtime[k+Ntotal], data9[k][4]/data9[k][3])

graph4.SetPoint(7, unixtime[Ntotal], 0.998)
graph5.SetPoint(7, unixtime[Ntotal], 1.557)
Ntotal += N9[0]
print unixtime[Ntotal-1]

graph4.SetPoint(8, unixtime[Ntotal-1], 1.)
graph5.SetPoint(8, unixtime[Ntotal-1], 1.)



graph1.GetXaxis().SetTimeDisplay(1)
graph1.GetXaxis().SetNdivisions(1006)
graph1.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph1.GetXaxis().SetTimeOffset(0,"JST")
graph1.SetTitle(";Date&Time;Temperature [degree Celsius]")
graph1.GetXaxis().SetTitleSize(0.047)
graph1.GetYaxis().SetTitleSize(0.047)
graph1.GetYaxis().SetTitleOffset(.7)
graph1.GetYaxis().SetRangeUser(18.,32.)
graph1.SetLineWidth(2)


#graph2.GetXaxis().SetTimeDisplay(1)
#graph2.GetXaxis().SetNdivisions(503)
#graph2.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
#graph2.GetXaxis().SetTimeOffset(0,"JST")
#graph2.SetTitle(";Date&Time;Temperature [degree Celsius]")
#graph2.GetXaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleSize(0.04)
#graph2.GetYaxis().SetTitleOffset(1.2)
graph2.GetYaxis().SetRangeUser(18.,32.)
graph2.SetLineColor(kBlue)
graph2.SetLineWidth(2)
graph2.SetMarkerColor(kBlue)


graph3.GetXaxis().SetTimeDisplay(1)
graph3.GetXaxis().SetNdivisions(503)
graph3.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph3.GetXaxis().SetTimeOffset(0,"JST")
graph3.SetTitle(";Date&Time;Ratio of Temperature")
graph3.GetXaxis().SetTitleSize(0.047)
graph3.GetYaxis().SetTitleSize(0.047)
graph3.GetYaxis().SetTitleOffset(1.1)
graph3.GetYaxis().SetRangeUser(0.5,1.1)
graph3.SetLineWidth(2)


graph4.GetXaxis().SetTimeDisplay(1)
graph4.GetXaxis().SetNdivisions(1006)
graph4.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph4.GetXaxis().SetTimeOffset(0,"JST")
graph4.SetTitle(";Date&Time;Relative Gain")
graph4.GetXaxis().SetTitleSize(0.047)
graph4.GetYaxis().SetTitleSize(0.047)
graph4.GetYaxis().SetTitleOffset(.7)
graph4.GetYaxis().SetRangeUser(0.,2.)
#graph4.SetLineWidth(2)
graph4.SetMarkerStyle(8)
graph4.SetMarkerSize(1.5)


graph5.GetXaxis().SetTimeDisplay(1)
graph5.GetXaxis().SetNdivisions(1006)
graph5.GetXaxis().SetTimeFormat("%Y-%m-%d %H:%M")
graph5.GetXaxis().SetTimeOffset(0,"JST")
graph5.SetTitle(";Date&Time;Relative Gain")
graph5.GetXaxis().SetTitleSize(0.047)
graph5.GetYaxis().SetTitleSize(0.047)
graph5.GetYaxis().SetTitleOffset(.7)
graph5.GetYaxis().SetRangeUser(0.,2.)
#graph5.SetLineWidth(2)
graph5.SetMarkerStyle(8)
graph5.SetMarkerSize(1.5)
graph5.SetMarkerColor(kBlue)
graph5.SetLineColor(kBlue)
 

legend = TLegend(0.3,0.2,0.88,0.38)
#legend = TLegend(0.3,0.73,0.88,0.88)
legend.AddEntry(graph1,"Thermocouple Ch1 on the left side of JT0695","l")
legend.AddEntry(graph2,"Thermocouple Ch2 on the left side of YH0205","l")
#legend.AddEntry(graph3,"The Ratio of Temperature Ch2/Ch1","l")
#legend.AddEntry(graph5,"Ratio = 1.0","l")
legend.AddEntry(None, "Viewing from the light source.", "")
#legend.AddEntry(None, "MCP-PMT ID: JT0695 ;   HV = 0 V", "")


c1 = TCanvas("c1","c1",1500,600)
c2 = TCanvas("c2","c2",1500,600)
#c2 = TCanvas("c2","c2")

c1.cd()
graph1.Draw()
#graph2.Draw()
graph2.Draw("same")
#graph4.Draw("same")
#legend.Draw("same")

c2.cd()
graph4.Draw("ALP")
graph5.Draw("LP")
#legend.Draw("same")



wait = raw_input(">> Press Enter to Continue. \n")

