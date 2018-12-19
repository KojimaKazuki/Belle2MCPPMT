#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np
NCH = 16
NHV = 7
HV_norm = 3260


###############################################################
# *Gain Analysis Data List*
dtype_dataGainList = "S32"
dataGainListName = "/home/bqt/kojima/MCP_PMT/Data/1810/AnalysisData/YH0205DataListGain.txt"
dataGainList = np.loadtxt( dataGainListName, delimiter=",",unpack = True, dtype = dtype_dataGainList)
dataGainListNew = np.reshape( dataGainList,( -1, 1))

NGainList = dataGainList.shape
NGainListNew = dataGainListNew.shape
print dataGainList[0]


# *Temperature Analysis Data*
dataList0 = "/home/bqt/kojima/MCP_PMT/Data/1810/AnalysisData/"
dtype_dataGain = []
dataGain = []
dataGainNew = []

for Ndata in range(NGainList[0]):

    dataGainNameIn = "dataGain%d" % (Ndata)

    dataGain_idIn = "dataGain%d_id" % (Ndata)
    dataGain_chIn = "dataGain%d_ch" % (Ndata)
    dataGain_dateIn = "dataGain%d_date" % (Ndata)
    dataGain_constIn = "dataGain%d_const" % (Ndata)
    dataGain_slopeIn = "dataGain%d_slope" % (Ndata)

    dtype_dataGainIn = [(dataGain_idIn,'S8'),(dataGain_chIn,'i8'),(dataGain_dateIn,'i8'),(dataGain_constIn,'d8'),(dataGain_slopeIn,'d8')]
    dtype_dataGain.append(dtype_dataGainIn)


    dataGainListIn = dataList0 + dataGainList[Ndata]
    dataGainIn = np.loadtxt( dataGainListIn, delimiter=",", unpack = True, dtype = dtype_dataGain[Ndata])
    dataGainInNew = np.reshape( dataGainIn,( -1, 1))

    NGainIn = dataGainIn.shape
    NGainInNew = dataGainInNew.shape

    dataGain.append(dataGainIn)
    dataGainNew.append(dataGainInNew)

print dataGain[2][5][4]



###############################################################
# *Temperature Analysis Data List*
dtype_dataTTSList = "S32"
dataTTSListName = "/home/bqt/kojima/MCP_PMT/Data/1810/AnalysisData/YH0205DataListTTS.txt"
dataTTSList = np.loadtxt( dataTTSListName, delimiter=",",unpack = True, dtype = dtype_dataTTSList)
dataTTSListNew = np.reshape( dataTTSList,( -1, 1))

NTTSList = dataTTSList.shape
NTTSListNew = dataTTSListNew.shape
print dataTTSList[0]


# *Temperature Analysis Data*
dtype_dataTTS = []
dataTTS = []
dataTTSNew = []

for Ndata in range(NTTSList[0]):

    dataTTSNameIn = "dataTTS%d" % (Ndata)

    dataTTS_idIn = "dataTTS%d_id" % (Ndata)
    dataTTS_chIn = "dataTTS%d_ch" % (Ndata)
    dataTTS_dateIn = "dataTTS%d_date" % (Ndata)
    dataTTS_sigmaIn = "dataTTS%d_sigma" % (Ndata)
    dataTTS_errorSigmaIn = "dataTTS%d_errorSigma" % (Ndata)

    dtype_dataTTSIn = [(dataTTS_idIn,'S8'),(dataTTS_chIn,'i8'),(dataTTS_dateIn,'i8'),(dataTTS_sigmaIn,'d8'),(dataTTS_errorSigmaIn,'d8')]
    dtype_dataTTS.append(dtype_dataTTSIn)


    dataTTSListIn = dataList0 + dataTTSList[Ndata]
    dataTTSIn = np.loadtxt( dataTTSListIn, delimiter=",", unpack = True, dtype = dtype_dataTTS[Ndata])
    dataTTSInNew = np.reshape( dataTTSIn,( -1, 1))

    NTTSIn = dataTTSIn.shape
    NTTSInNew = dataTTSInNew.shape

    dataTTS.append(dataTTSIn)
    dataTTSNew.append(dataTTSInNew)

print dataTTS[2][5][4]



###############################################################
# *Temperature Analysis Data List*
dtype_dataTempList = "S32"
dataTempListName = "/home/bqt/kojima/MCP_PMT/Data/1810/AnalysisData/YH0205DataListTemp.txt"
dataTempList = np.loadtxt( dataTempListName, delimiter=",",unpack = True, dtype = dtype_dataTempList)
dataTempListNew = np.reshape( dataTempList,( -1, 1))

NTempList = dataTempList.shape
NTempListNew = dataTempListNew.shape
print dataTempList[0]



# *Temperature Analysis Data*
dtype_dataTemp = []
dataTemp = []
dataTempNew = []

for Ndata in range(NTempList[0]):

    dataTempNameIn = "dataTemp%d" % (Ndata)

    dataTemp_idIn = "dataTemp%d_id" % (Ndata)
    dataTemp_TsetIn = "dataTemp%d_Tset" % (Ndata)
    dataTemp_HVsetIn = "dataTemp%d_HVset" % (Ndata)
    dataTemp_Ch1TempIn = "dataTemp%d_Ch1Temp" % (Ndata)
    dataTemp_errorCh1TempIn = "dataTemp%d_errorCh1Temp" % (Ndata)
    dataTemp_Ch2TempIn = "dataTemp%d_Ch2Temp" % (Ndata)
    dataTemp_errorCh2TempIn = "dataTemp%d_errorCh2Temp" % (Ndata)
    dataTemp_TempAveIn = "dataTemp%d_TempAve" % (Ndata)
    dataTemp_errorTempAveIn = "dataTemp%d_errorTempAve" % (Ndata)

    dtype_dataTempIn = [(dataTemp_idIn,'S8'),(dataTemp_TsetIn,'i8'),(dataTemp_HVsetIn,'i8'),(dataTemp_Ch1TempIn,'d8'),(dataTemp_errorCh1TempIn,'d8'),(dataTemp_Ch2TempIn,'d8'),(dataTemp_errorCh2TempIn,'d8'),(dataTemp_TempAveIn,'d8'),(dataTemp_errorTempAveIn,'d8')]
    dtype_dataTemp.append(dtype_dataTempIn)

    #dtype_data = [(data_id,'S8'),(data_Tset,'i8'),(data_HVset,'i8'),(data_Ch1Temp,'d8'),(data_errorCh1Temp,'d8'),(data_Ch2Temp,'d8'),(data_errorCh2Temp,'d8'),(data_TempAve,'d8'),(data_errorTempAve,'d8')]


    dataTempListIn = dataList0 + dataTempList[Ndata]
    dataTempIn = np.loadtxt( dataTempListIn, delimiter=",", unpack = True, dtype = dtype_dataTemp[Ndata])
    dataTempInNew = np.reshape( dataTempIn,( -1, 1))

    NTempIn = dataTempIn.shape
    NTempInNew = dataTempInNew.shape

    dataTemp.append(dataTempIn)
    dataTempNew.append(dataTempInNew)

print dataTemp[2][5][4]



# *Graph: Temporal Resolution vs. Temperature; Gain vs. Temperature*
from datetime import datetime
import time

import math

from ROOT import TCanvas,TGraph,TGraphErrors,gStyle,gROOT,TAxis,TStyle,TLegend,TText,TF1
from ROOT import kRed,kBlue,kBlack,kMagenta

gStyle.SetOptStat()
gStyle.SetOptTitle(0)
gStyle.SetLabelSize(0.05,"X")
gStyle.SetLabelSize(0.05,"Y")
#gROOT.SetStyle("Plain")

#c1 = TCanvas("c1","c1")
#c2 = TCanvas("c2","c2")

graph1 = TGraphErrors()
graph2 = TGraphErrors()


unixtime = []
Gain = []
Nstart = 0


for k in range(NTempList[0]):
    #print data1[k][1], data1[k][2], data1[k][3]

    #tks = datetime.strptime(data1[k][1], '%Y/%m/%d %H:%M:%S')
    #unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k]

    # *Graph: Temporal Resolution vs. Temperatrue*
    graph1.SetPoint(k, dataTemp[k][0][7], dataTTS[k][0][3])
    graph1.SetPointError(k, dataTemp[k][0][8], dataTTS[k][0][4])

    # *Garph: Gain vs. Temperatrue*
    GainExpIn = dataGain[k][0][3] + dataGain[k][0][4]*HV_norm 
    GainIn = math.exp(GainExpIn)
    Gain.append(GainIn/1000)

    print GainExpIn,GainIn

    graph2.SetPoint(k, dataTemp[k][0][7], Gain[k])
    graph2.SetPointError(k, dataTemp[k][0][8], 0.)


graph1.SetTitle(";Temperature / degree Celsius; Temporal Resolution / ps")
graph1.GetXaxis().SetTitleSize(0.047)
graph1.GetYaxis().SetTitleSize(0.047)
graph1.GetYaxis().SetTitleOffset(1.)
graph1.GetXaxis().SetRangeUser(25.,45.)
graph1.GetYaxis().SetRangeUser(20.,50.)
graph1.SetMarkerStyle(8)
graph1.SetMarkerSize(1.5)


graph2.SetTitle(";Temperature / degree Celsius; Gain / #times 10^{9}")
graph2.GetXaxis().SetTitleSize(0.047)
graph2.GetYaxis().SetTitleSize(0.047)
graph2.GetYaxis().SetTitleOffset(1.08)
graph2.GetXaxis().SetRangeUser(25.,45.)
graph2.GetYaxis().SetRangeUser(3.,4.)
graph2.SetMarkerColor(kBlack)
graph2.SetMarkerStyle(8)
graph2.SetMarkerSize(1.5)


"""
func1Fit = TF1("func1Fit","pol2",25.,45.)
func1Fit.SetParameter(1, 5)
func1Fit.SetParameter(2, -0.05)
graph1.Fit("func1Fit","","",25.,45.)
func1 = TF1("func1", "[0]+[1]*x+[2]*x*x", 25., 45.)
func1.SetParameters(func1Fit.GetParameter(0),func1Fit.GetParameter(1),func1Fit.GetParameter(2))
"""

func2Fit = TF1("func2Fit","pol1")
func2Fit.SetParameter(0, 4000)
func2Fit.SetParameter(1, -1)
graph2.Fit("func2Fit","0","",28.,42.)
func2 = TF1("func2", "[0]+[1]*x", 25., 45.)
func2.SetParameters(func2Fit.GetParameter(0),func2Fit.GetParameter(1))
#func2.SetParameters(graph2.GetFunction("pol1").GetParameter(0),graph2.GetFunction("pol1").GetParameter(1))
func2.SetLineStyle(2)
func2.SetLineWidth(1)
func2.SetLineColor(kRed-7)


func3Fit = TF1("func3Fit","pol0")
func3Fit.SetParameter(1, -0.01)
graph2.Fit("func3Fit","0","",28.,42.)
func3 = TF1("func3", "[0]+[1]*x", 25., 45.)
func3.SetParameters(func3Fit.GetParameter(0),func3Fit.GetParameter(1))
func3.SetLineStyle(2)
func3.SetLineWidth(1)
func3.SetLineColor(kMagenta)


legend = TLegend(0.3,0.73,0.88,0.88)
legend.AddEntry(graph1,"Ch1 on the side of PMT","p")
legend.AddEntry(graph2,"Ch2 near the thermocouple terminal of the heater","p")
legend.AddEntry(None, "MCP-PMT ID: KT0835 ;   HV = 2,110 V", "")

text1 = TText(0.5, 0.5, "")
text1_2 = TText(0.5, 0.5, "")
text2 = TText(0.5, 0.5, "")
text2_2 = TText(0.5, 0.5, "")


c1 = TCanvas("c1","c1",1100,500)
c1.Divide(2,1)

c1.cd(1)
c1.DrawFrame( 25., 25., 45., 50.)
graph1.Draw("AP")
text1.DrawTextNDC(0.3, 0.84, "YH0205 (Life-extended ALD)")
text1_2.DrawTextNDC(0.4, 0.785, "  Normal HV = 2,300 V")

c1.cd(2)
c1.DrawFrame( 25., 3000., 45., 4000.)
graph2.Draw("AP")
#func2.Draw("same")
func3.Draw("same")
c1.Update()

#legend.Draw("same")
text2.DrawTextNDC(0.3, 0.84, "YH0205 (Life-extended ALD)")
text2_2.DrawTextNDC(0.4, 0.785, "  Normal HV = 2,300 V")
c1.Update()

wait = raw_input(">> Press Enter to Continue. \n")

