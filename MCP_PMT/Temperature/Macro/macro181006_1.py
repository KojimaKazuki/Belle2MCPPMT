import numpy as np
from ROOT import TFile


dtype_data1 = [('data1_Num','i8'),('data1_t','S32'),('data1_ms','d8'),('data1_Ch1','d8'),('data1_Ch2','d8'),('data1_Alarm1','S16'),('data1_Alarm2','S16'),('data1_AlarmOut','S8')]
data1 = np.loadtxt("/home/bqt/kojima/MCP_PMT/Data/1810/", delimiter=",", unpack = True, dtype = dtype_data1)


Print("Please write the root file name for input.")
inFile = input(">>> ")


Print("Is the file in /chai/sgt1/bfactory/mcpppmt_data/kihon/03/ ? (y/n)")
Answer1 = input(">>> ")

if (Answer1 == y or Answer1 == Y)
/home/bqt/kojima/MCP_PMT/Data/1810/

data2 = TFile(inFile2)
mcp = data2.Get('mcp')


tdc = np.array(16, dtype = 'float64')
adc = np.array(16, dtype = 'float64')
ipos = np.array(1, dtype = 'float64')
HV = np.array(1, dtype = 'float64')

Nmcp = mcp.Get()




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


hist1 = TH1F("hist1","Temperature Histogram Ch1",50,35,40)
hist2 = TH1F("hist2","Temperature Histogram Ch2",50,35,40)


