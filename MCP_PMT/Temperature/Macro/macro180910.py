#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np

dtype_data1 = [('data1_Num','i8'),('data1_t','S32'),('data1_ms','d8'),('data1_Ch1','d8'),('data1_Ch2','d8'),('data1_Alarm1','S16'),('data1_Alarm2','S16'),('data1_AlarmOut','S8')]
data1 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/Temperature181026_2LED100k.csv", delimiter=",", unpack = True, dtype = dtype_data1)


data1new = np.reshape( data1,( -1, 1))

N = data1.shape
Nnew = data1new.shape
print N, Nnew



from datetime import datetime
import time

from ROOT import TCanvas,TGraph,gStyle,gROOT,TAxis,TStyle,TLegend,TH1F,TF1,TPaveStats
from ROOT import kRed,kBlue,kBlack,kOrange,kCyan

gStyle.SetOptStat(000000011)
gStyle.SetOptFit(1)
gStyle.SetOptTitle(0)
#gROOT.SetStyle("Plain")


hist1 = TH1F("hist1","Temperature Histogram Ch1",100,18,38)
hist2 = TH1F("hist2","Temperature Histogram Ch2",100,18,38)

func1 = TF1("func1","gaus")
func1.SetParameter(1,30.)
func2 = TF1("func2","gaus")
#func2.SetParameter(1,20.)
func3 = TF1("func3","gaus")


unixtime = []
Nstart = 0


for k in range(Nstart,N[0]):
    hist1.Fill(data1[k][3])
    hist2.Fill(data1[k][4])

    tks = datetime.strptime(data1[k][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]


hist1.SetTitle(";Temperature[degree Celsius];# of Events")
hist2.SetTitle(";Temperature[degree Celsius];# of Events")


hist1.SetLineColor(kBlack)
hist1.SetLineWidth(2)
hist1.SetFillColor(kCyan)
hist1.SetFillStyle(3344)
hist1.GetYaxis().SetRangeUser(0,2000)

hist2.SetLineColor(kBlack)
hist2.SetLineWidth(2)
hist2.SetFillColor(kOrange)
hist2.SetFillStyle(3344)
hist2.GetYaxis().SetRangeUser(0,2000)

func1.SetLineColor(kBlue)
func1.SetLineWidth(2)
func1.SetLineStyle(9)
func2.SetLineColor(kRed)
func2.SetLineWidth(2)
func2.SetLineStyle(9)
func3.SetLineColor(kBlue)
func3.SetLineWidth(3)
func3.SetLineStyle(9)

#hist1.Fit("func3","","",36.5,37)
hist1.Fit(func1)
hist2.Fit(func2)


legend = TLegend(0.3,0.73,0.9,0.9)
legend.AddEntry(hist1,"Thermocouple Ch1 on the right side of JT0695","lf")
legend.AddEntry(hist2,"Thermocouple Ch2 on the left side of YH0205","lf")
legend.AddEntry(None, "Viewing from the light source", "")
#legend.AddEntry(None, "MCP-PMT ID: KT0835;HV = 2,110 V", "")


c1 = TCanvas("c1","c1")


c1.cd()
hist2.Draw()
hist2.GetYaxis().SetTitleOffset(1.3)
hist1.Draw("sames")
func3.Draw("same")
c1.cd().Update()
legend.Draw("same")


st1 = hist1.FindObject("stats")
st1.SetX1NDC(0.63)
st1.SetX2NDC(0.89)
st1.SetY1NDC(0.51)
st1.SetY2NDC(0.68)
st1.SetTextColor(kBlue)
c1.cd().Update()

st2 = hist2.FindObject("stats")
st2.SetX1NDC(0.63)
st2.SetX2NDC(0.89)
st2.SetY1NDC(0.34)
st2.SetY2NDC(0.51)
st2.SetTextColor(kRed)
c1.cd().Modified()
c1.cd().Update()


wait = raw_input(">> Press Enter to Continue. \n")

