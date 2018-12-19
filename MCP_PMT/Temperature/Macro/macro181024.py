#!/usr/bin/env python
# -*- coding: utf-8 -*-


# *Get the Set Temperature*
print(">> What is the temperature set on the measurement ? (Please answer it by ONLY figures.)")
temp_set = int(raw_input(">> "))
temp_min = temp_set - 2
temp_max = temp_set + 2

# *Get the Name of Temperature Data*
print(">> Please Input the File Name of Temperature Data. (CSV Format: Temperature~.csv)")
fileNameInTemp = raw_input(">> ")

# *Get the Name of Basic Measurement Data*
print(">> Please Input the ROOT File Name of Basic Measurement. (ROOT Format: JT0100~.root)")
fileNameIn = raw_input(">> ") 

 
fileNameInGain = fileNameIn.replace('.root','Gain.csv')
fileNameInTTS = fileNameIn.replace('.root','TTS.csv')
fileNameInTime = fileNameIn.replace('.root','Time.csv')

fileName0Temp = "/home/bqt/kojima/MCP_PMT/Data/1811/"
fileName0 = "/home/bqt/kojima/MCP_PMT/Data/1811/AnalysisData/"

fileNameTemp = fileName0Temp + fileNameInTemp
fileNameGain = fileName0 + fileNameInGain
fileNameTTS = fileName0 + fileNameInTTS
fileNameTime = fileName0 + fileNameInTime


print ('Temperature Data Input: %s' % fileNameTemp)
#print ('Gain Data Input: %s' % fileNameGain)
#print ('TTS Data Input: %s' % fileNameTTS)
print ('Time Data Input: %s' % fileNameTime)


import numpy as np

# *Temperature Data*
dtype_dataTemp = [('dataTemp_Num','i8'),('dataTemp_t','S32'),('dataTemp_ms','d8'),('dataTemp_Ch1','d8'),('dataTemp_Ch2','d8'),('dataTemp_Alarm1','S16'),('dataTemp_Alarm2','S16'),('dataTemp_AlarmOut','S8')]
dataTemp = np.loadtxt( fileNameTemp, delimiter=",", unpack = True, dtype = dtype_dataTemp)
dataTempNew = np.reshape( dataTemp,( -1, 1))

NTemp = dataTemp.shape
NTempNew = dataTempNew.shape
print NTemp, NTempNew

"""
# *Gain Data*
dtype_dataGain = [('dataGain_id','S16'),('dataGain_ch','i8'),('dataGain_date','i8'),('dataGain_p1','d16'),('dataGain_p0','d16')]
dataGain = np.loadtxt( fileNameGain, delimiter=",", unpack = True, dtype = dtype_dataGain)
dataGainNew = np.reshape( dataGain,( -1, 1))


# *TTS Data*
dtype_dataTTS = [('dataTTS_id','S16'),('dataTTS_ch','i8'),('dataTTS_date','i8'),('dataTTS_sigma','d16'),('dataTTS_err_sigma','d16')]
dataTTS = np.loadtxt( fileNameTTS, delimiter=",", unpack = True, dtype = dtype_dataTTS)
dataTTSNew = np.reshape( dataTTS,( -1, 1))
"""

# *Time Data*
dtype_dataTime = [('dataTime_id','S16'),('dataTime_ch','i8'),('dataTime_date','i8'),('dataTime_entry','i8'),('dataTime_HVset','i8'),('dataTime_HVstart','i8'),('dataTime_HVend','i8'),('dataTime_timestart','i8'),('dataTime_timeend','i8')]
dataTime = np.loadtxt( fileNameTime, delimiter=",", unpack = True, dtype = dtype_dataTime)
dataTimeNew = np.reshape( dataTime,( -1, 1))


# *Output File Name -> PDF Format*
fileOutPDFName = fileNameIn.replace('.root','Temp.pdf')
fileOutPDF2Name = fileNameIn.replace('.root','TempHV.pdf')
# *Output File Name -> CSV Format*
fileOutCSVName = fileNameIn.replace('.root','Temp.csv')

import csv
fileOutCSV = open(fileOutCSVName,'w') #'a')
writerCSV = csv.writer(fileOutCSV,lineterminator='\n')

csvListTitleIn = []
csvList = []
csvListTitleIn.append("#MCP-PMT ID")
csvListTitleIn.append("Set Temperature")
csvListTitleIn.append("Set HV")
csvListTitleIn.append("Ch1 Temperature")
csvListTitleIn.append("Ch1 Temperature Sigma")
csvListTitleIn.append("Ch2 Temperature")
csvListTitleIn.append("Ch2 Temperature Sigma")
csvListTitleIn.append("Average Temperature")
csvListTitleIn.append("Error of Average Temperature")
csvList.append(csvListTitleIn)





#############################################################
from datetime import datetime
import time

from ROOT import TCanvas,TGraph,gStyle,gROOT,TAxis,TStyle,TLegend,TH1F,TF1,TPaveStats
from ROOT import kRed,kBlue,kBlack,kOrange,kCyan

gStyle.SetOptStat(000000011)
gStyle.SetOptFit(1)
gStyle.SetOptTitle(0)
#gROOT.SetStyle("Plain")




#############################################################
# *All Data during the Measurement*
channel = []

hist1_0 = TH1F("hist1_0","Temperature Histogram Ch1",100,temp_set-5,temp_set+5)
hist2_0 = TH1F("hist2_0","Temperature Histogram Ch2",100,temp_set-5,temp_set+5)

func1_0 = TF1("func1_0","gaus")
func2_0 = TF1("func2_0","gaus")
func3_0 = TF1("func3_0","gaus")

func1_0.SetParameter(1,temp_set+1)
func2_0.SetParameter(1,temp_set+1)


unixtime = []
Nstart = 0


for k0 in range(Nstart,NTemp[0]):
    hist1_0.Fill(dataTemp[k0][3])
    hist2_0.Fill(dataTemp[k0][4])

    tks = datetime.strptime(dataTemp[k0][1], '%Y/%m/%d %H:%M:%S')
    unixtime.append(int(time.mktime(tks.timetuple())))
    #print unixtime[k0]


hist1_0.SetTitle(";Temperature [degree Celsius];# of Events")
hist2_0.SetTitle(";Temperature [degree Celsius];# of Events")


hist1_0.SetLineColor(kBlack)
hist1_0.SetLineWidth(2)
hist1_0.SetFillColor(kCyan)
hist1_0.SetFillStyle(3344)
hist1_0.GetYaxis().SetRangeUser(0,4000)

hist2_0.SetLineColor(kBlack)
hist2_0.SetLineWidth(2)
hist2_0.SetFillColor(kOrange)
hist2_0.SetFillStyle(3344)
hist2_0.GetYaxis().SetRangeUser(0,4000)

func1_0.SetLineColor(kBlue)
func1_0.SetLineWidth(2)
func1_0.SetLineStyle(9)
func2_0.SetLineColor(kRed)
func2_0.SetLineWidth(2)
func2_0.SetLineStyle(9)
func3_0.SetLineColor(kBlue)
func3_0.SetLineWidth(3)
func3_0.SetLineStyle(9)

#hist1.Fit("func3","","",36.5,37)
hist1_0.Fit("func1_0","","",temp_min,temp_max)
hist2_0.Fit("func2_0","","",temp_min,temp_max)


legend = TLegend(0.3,0.73,0.9,0.9)
legend.AddEntry(hist1_0,"Ch1: Left side viewing from the light source","lf")
legend.AddEntry(hist2_0,"Ch2: Right side viewing from the light source","lf")
legend.AddEntry(None, "MCP-PMT ID: JT0695; Normal HV = 3,260 V", "")


c0 = TCanvas("c0","c0",794,567)


c0.cd()
hist2_0.Draw()
hist2_0.GetYaxis().SetTitleOffset(1.3)
hist1_0.Draw("sames")
func3_0.Draw("same")
c0.cd().Update()
legend.Draw("same")


st1_0 = hist1_0.FindObject("stats")
st1_0.SetX1NDC(0.63)
st1_0.SetX2NDC(0.89)
st1_0.SetY1NDC(0.51)
st1_0.SetY2NDC(0.68)
st1_0.SetTextColor(kBlue)
c0.cd().Update()

st2_0 = hist2_0.FindObject("stats")
st2_0.SetX1NDC(0.63)
st2_0.SetX2NDC(0.89)
st2_0.SetY1NDC(0.34)
st2_0.SetY2NDC(0.51)
st2_0.SetTextColor(kRed)
c0.cd().Modified()
c0.cd().Update()


c0.Print(fileOutPDFName,"Temperature")


Constant1_0 = func1_0.GetParameter(0)
Mean1_0 = func1_0.GetParameter(1)
Sigma1_0 = func1_0.GetParameter(2)

Constant2_0 = func2_0.GetParameter(0)
Mean2_0 = func2_0.GetParameter(1)
Sigma2_0 = func2_0.GetParameter(2)

MeanAve_0 = (Mean1_0 + Mean2_0) / 2.
SigmaAve_0 = np.sqrt(Sigma1_0**2 + Sigma2_0**2)

csvListIn0 = []
csvListIn0.append("JT0695")
csvListIn0.append(temp_set)
csvListIn0.append("0")
csvListIn0.append(Mean1_0)
csvListIn0.append(Sigma1_0)
csvListIn0.append(Mean2_0)
csvListIn0.append(Sigma2_0)
csvListIn0.append(MeanAve_0)
csvListIn0.append(SigmaAve_0)
csvList.append(csvListIn0)





#############################################################
# *Temperature Distribution of Each Channel*
NCH = 16
NHV = 7

hist1 = []
hist2 = []

func1 = []
func2 = []
func3 = []

canvas = []
st1 = []
st2 = []

timeStart = []
timeEnd = []


for ch in range(NCH):
    hist1Ch = []
    hist2Ch = []

    func1Ch = []
    func2Ch = []
    func3Ch = []

    st1Ch = []
    st2Ch = []

    timeStartCh = []
    timeEndCh = []


    for iHV in range(NHV):
        ndata1 = ch*(NHV+1) + iHV + 2

        hist1ChName = "hist1_Ch%dHV%d" % (ch+1,dataTime[ndata1][4]) 
        hist1ChTitle = "Temperature Histogram1 Ch%d HV%d" % (ch+1,dataTime[ndata1][4]) 
        hist1ChIn = TH1F(hist1ChName,hist1ChTitle,100,temp_set-5,temp_set+5) 
        hist1Ch.append(hist1ChIn)

        hist2ChName = "hist2_Ch%dHV%d" % (ch+1,dataTime[ndata1][4])
        hist2ChTitle = "Temperature Histogram2 Ch%d HV%d" % (ch+1,dataTime[ndata1][4])
        hist2ChIn = TH1F(hist2ChName,hist2ChTitle,100,temp_set-5,temp_set+5)
        hist2Ch.append(hist2ChIn)


        func1ChInName = "func1_Ch%dHV%d" % (ch+1,dataTime[ndata1][4])
        func2ChInName = "func2_Ch%dHV%d" % (ch+1,dataTime[ndata1][4])
        func3ChInName = "func3_Ch%dHV%d" % (ch+1,dataTime[ndata1][4])

        func1ChIn = TF1(func1ChInName,"gaus")
        func2ChIn = TF1(func2ChInName,"gaus")
        func3ChIn = TF1(func3ChInName,"gaus")

        func1Ch.append(func1ChIn)
        func2Ch.append(func2ChIn)
        func3Ch.append(func3ChIn)

        timeStartCh.append(int(dataTime[ndata1][7]))
        timeEndCh.append(int(dataTime[ndata1][8]))

        
    hist1.append(hist1Ch)
    hist2.append(hist2Ch)

    func1.append(func1Ch)
    func2.append(func2Ch)
    func3.append(func3Ch)

    timeStart.append(timeStartCh)
    timeEnd.append(timeEndCh)


"""
for ch in range(NCH):
    for iHV in range(NHV):
        st1ChIn = hist1[ch][iHV].FindObject("stats")
        st2ChIn = hist2[ch][iHV].FindObject("stats")

        st1Ch.append(st1ChIn)
        st2Ch.append(st2ChIn)

    st1.append(st1Ch)
    st2.append(st2Ch)
"""


print timeStart[0][0], timeEnd[NCH-1][NHV-1]
print len(timeStart)

timeStartTemp = []
timeEndTemp = []
nStartTemp = []
nEndTemp = []


for ch in range(NCH):

    timeStartTempCh = []
    timeEndTempCh = []
    nStartTempCh = []
    nEndTempCh = []

    for iHV in range(NHV): 

        k1 = 0
        while ( unixtime[k1] < timeStart[ch][iHV] ):            
            k1 += 1

        timeStartTempCh.append(unixtime[k1])
        nStartTempCh.append(k1) 
        timeEndTempCh.append(unixtime[k1+1])
        nEndTempCh.append(k1)

        while ( unixtime[k1] < timeEnd[ch][iHV] ):
            timeEndTempCh[iHV] = unixtime[k1]
            nEndTempCh[iHV] = k1
            k1 += 1

    
    timeStartTemp.append(timeStartTempCh)
    nStartTemp.append(nStartTempCh) 
    timeEndTemp.append(timeEndTempCh)
    nEndTemp.append(nEndTempCh)


ch = 0

canvasName = "c%d" % (ch+1)
canvasIn = TCanvas(canvasName,canvasName)
canvas.append(canvasIn)

c1 = TCanvas("c1","c1",794,1123)
c1.Divide(2,NHV//2+NHV%2)


Constant1 = []
Mean1 = []
Sigma1 = []
Constant2 = []
Mean2 = []
Sigma2 = []
MeanAve = []
SigmaAve = []


for iHV in range(NHV):
    k2StartTemp = nStartTemp[0][iHV]
    k2EndTemp = nEndTemp[NCH-1][iHV] + 1
    print k2StartTemp, k2EndTemp

    ndata2 = ch*(NHV+1) + iHV + 2

    c1.cd(iHV+1)

    for k2 in range(k2StartTemp, k2EndTemp):
        hist = hist1[ch][iHV]
        hist.Fill(dataTemp[k2][3])
        hist2[ch][iHV].Fill(dataTemp[k2][4])


    hist1[ch][iHV].SetTitle(";Temperature [degree Celsius];# of Events")
    hist2[ch][iHV].SetTitle(";Temperature [degree Celsius];# of Events")

    hist1[ch][iHV].SetLineColor(kBlack)
    hist1[ch][iHV].SetLineWidth(2)
    hist1[ch][iHV].SetFillColor(kCyan)
    hist1[ch][iHV].SetFillStyle(3344)
    hist1[ch][iHV].GetYaxis().SetRangeUser(0,1000)

    hist2[ch][iHV].SetLineColor(kBlack)
    hist2[ch][iHV].SetLineWidth(2)
    hist2[ch][iHV].SetFillColor(kOrange)
    hist2[ch][iHV].SetFillStyle(3344)
    hist2[ch][iHV].GetYaxis().SetRangeUser(0,1000)

    func1[ch][iHV].SetLineColor(kBlue)
    func1[ch][iHV].SetLineWidth(2)
    func1[ch][iHV].SetLineStyle(9)
    func2[ch][iHV].SetLineColor(kRed)
    func2[ch][iHV].SetLineWidth(2)
    func2[ch][iHV].SetLineStyle(9)
    func3[ch][iHV].SetLineColor(kBlue)
    func3[ch][iHV].SetLineWidth(3)
    func3[ch][iHV].SetLineStyle(9)

    #hist1.Fit("func3","","",36.5,37)
    func1[ch][iHV].SetParameter(1,temp_set + 1)
    func2[ch][iHV].SetParameter(1,temp_set - 1)
    hist1[ch][iHV].Fit(func1[ch][iHV],"","",temp_min,temp_max)
    hist2[ch][iHV].Fit(func2[ch][iHV],"","",temp_min,temp_max)

    hist2[ch][iHV].Draw()
    hist2[ch][iHV].GetYaxis().SetTitleOffset(1.3)
    hist1[ch][iHV].Draw("sames")
    func3[ch][iHV].Draw("same")
    c1.cd(iHV+1).Update()
    #legend.Draw("same")


    
    stat1 = hist1[ch][iHV].FindObject("stats")
    stat1.SetX1NDC(0.63)
    stat1.SetX2NDC(0.89)
    stat1.SetY1NDC(0.71)
    stat1.SetY2NDC(0.88)
    stat1.SetTextColor(kBlue)
    c1.cd(iHV+1).Update()

    stat2 = hist2[ch][iHV].FindObject("stats")
    stat2.SetX1NDC(0.63)
    stat2.SetX2NDC(0.89)
    stat2.SetY1NDC(0.53)
    stat2.SetY2NDC(0.70)
    stat2.SetTextColor(kRed)
    c1.cd(iHV+1).Modified()
    c1.cd(iHV+1).Update()


    hist1Mean = hist1[ch][iHV].GetMean()
    hist1MeanError = hist1[ch][iHV].GetMeanError()
    hist2Mean = hist2[ch][iHV].GetMean()
    hist2MeanError = hist2[ch][iHV].GetMeanError()



    Constant1.append(func1[ch][iHV].GetParameter(0))
    Mean1.append(func1[ch][iHV].GetParameter(1))
    Sigma1.append(func1[ch][iHV].GetParameter(2))
    if ( Mean1[iHV] > temp_set + 2):
        Mean1[iHV] = hist1Mean
        Sigma1[iHV] = hist1MeanError

    Constant2.append(func2[ch][iHV].GetParameter(0))
    Mean2.append(func2[ch][iHV].GetParameter(1))
    Sigma2.append(func2[ch][iHV].GetParameter(2))
    if ( Mean2[iHV] > temp_set + 2):
        Mean2[iHV] = hist2Mean
        Sigma2[iHV] = hist2MeanError


    MeanAveIn = (Mean1[iHV] + Mean2[iHV]) / 2.
    MeanAve.append(MeanAveIn)
    SigmaAveIn = np.sqrt(Sigma1[iHV]**2 + Sigma2[iHV]**2)
    SigmaAve.append(SigmaAveIn)


    csvListIn = []
    csvListIn.append("JT0695")
    csvListIn.append(temp_set)
    csvListIn.append(dataTime[ndata2][4])
    csvListIn.append(Mean1[iHV])
    csvListIn.append(Sigma1[iHV])
    csvListIn.append(Mean2[iHV])
    csvListIn.append(Sigma2[iHV])
    csvListIn.append(MeanAve[iHV])
    csvListIn.append(SigmaAve[iHV])
    csvList.append(csvListIn)


writerCSV.writerows(csvList)
fileOutCSV.close()

c1.Print(fileOutPDF2Name,"Temperature at Each HV")


wait = raw_input(">> Press Enter to Continue. \n")

