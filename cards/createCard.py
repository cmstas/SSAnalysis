import ROOT
import sys
import os
import subprocess
import math

#example: python createCard.py dir t1tttt_1500 
#example: python createCard.py dir t1tttt_1500 hihi sr card-hihi.txt
#example: for dir in dir1 dir2; do for sig in t1tttt_1200 t1tttt_1500; do python createCard.py ${dir} ${sig}; done; done

#for dir in gc.v1.24; do for sig in t1tttt_1500 t1tttt_1200 t1ttbbww_1300 t5tttt_1000 t5qqww_1500 t5qqww_1200 t5qqww_deg t6ttww_650 t6ttww_600; do python createCard.py ${dir} ${sig}; done; done

#then get expected limits with: combine -M Asymptotic dir/card.txt --run expected --noFitAsimov

#to add more nuisances edit Process, writeOneCardFromProcesses and then set values in writeOneCard

lumi = "10.0" #"3.0"

class Process:
    def __init__(self, mycount, myname, myrootf, myplot):
        self.count = mycount
        self.name = myname
        self.rootf = myrootf
        self.plot = myplot
        self.jes  = "-"
        self.signal_stat = "-"
        self.TTW = "-"
        self.ttw_extr = "-"
        self.ttw_stat = "-"
        self.TTZ = "-"
        self.ttz_extr = "-"
        self.ttz_stat = "-"
        self.WZ = "-"
        self.wz_extr = "-"
        self.fakes = "-"
        self.sfakes_stat = "-"
        self.flips = "-"
    def rate(self): 
        f = ROOT.TFile(dir+"/"+self.rootf)
        if f.Get(self.plot): return f.Get(self.plot).Integral()
        else: 
            print self.plot+" not found in "+self.rootf
            return 0

#write card regardless of number of processes (but make sure signal is first in list)
def writeOneCardFromProcesses(dir, kine, plot, output, data, processes):
    line = "---------------------------------------------------------------"
    bin = "SS"
    card = open(str(dir)+'/'+str(output), 'w')
    card.write("imax 1  number of channels \n")
    card.write("jmax *  number of backgrounds \n")
    card.write("kmax *  number of nuisance parameters \n")
    card.write(line+"\n")
    for process in processes:
        card.write("shapes "+process.name+" * "+dir+"/"+process.rootf+" "+plot+" "+plot+"\n")
    card.write("shapes data_obs * "+dir+"/"+data.rootf+" "+plot+" "+plot+"\n")#pseudo data for now
    card.write(line+"\n")
    card.write("bin "+str(bin)+"\n")
    #pseudo data for now
    card.write("observation %-15.3f \n" % (data.rate()))
    card.write(line+"\n")
    #bin
    card.write("%-40s %-5s " % ("bin",""))
    for process in processes: card.write("%-15s " % (bin))
    card.write("\n")
    #process count
    card.write("%-40s %-5s " % ("process",""))
    for process in processes: card.write("%-15s " % (process.count))
    card.write("\n")
    #process name
    card.write("%-40s %-5s " % ("process",""))
    for process in processes: card.write("%-15s " % (process.name))
    card.write("\n")
    #process rate
    card.write("%-40s %-5s " % ("rate",""))
    for process in processes: card.write("%-15.3f " % (process.rate()))
    card.write("\n")
    #separate
    card.write(line+"\n")

    #nuisance SIGNAL stat
    card.write("%-40s %-5s " % (signal+"_stat","shape"))
    for process in processes: card.write("%-15s " % (process.signal_stat))
    card.write("\n")

    #nuisance TTW
    card.write("%-40s %-5s " % ("TTW","lnN"))
    for process in processes: card.write("%-15s " % (process.TTW))
    card.write("\n")
    #nuisance TTW extr
    card.write("%-40s %-5s " % ("ttw_extr","shape"))
    for process in processes: card.write("%-15s " % (process.ttw_extr))
    card.write("\n")
    #nuisance TTW stat
    card.write("%-40s %-5s " % ("ttw_stat","shape"))
    for process in processes: card.write("%-15s " % (process.ttw_stat))
    card.write("\n")

    #nuisance TTZ
    card.write("%-40s %-5s " % ("TTZ","lnN"))
    for process in processes: card.write("%-15s " % (process.TTZ))
    card.write("\n")
    #nuisance TTZ extr
    card.write("%-40s %-5s " % ("ttz_extr","shape"))
    for process in processes: card.write("%-15s " % (process.ttz_extr))
    card.write("\n")
    #nuisance TTZ stat
    card.write("%-40s %-5s " % ("ttz_stat","shape"))
    for process in processes: card.write("%-15s " % (process.ttz_stat))
    card.write("\n")

    #nuisance WZ
    card.write("%-40s %-5s " % ("WZ","lnN"))
    for process in processes: card.write("%-15s " % (process.WZ))
    card.write("\n")
    #nuisance WZ extr
    card.write("%-40s %-5s " % ("wz_extr","shape"))
    for process in processes: card.write("%-15s " % (process.wz_extr))
    card.write("\n")

    #nuisance fakes
    card.write("%-40s %-5s " % ("fakes","lnN"))
    for process in processes: card.write("%-15s " % (process.fakes))
    card.write("\n")
    #nuisance sfakes stat
    card.write("%-40s %-5s " % ("sfakes_dd_stat","shape"))
    for process in processes: card.write("%-15s " % (process.sfakes_stat))
    card.write("\n")

    #nuisance flips
    card.write("%-40s %-5s " % ("flips","lnN"))
    for process in processes: card.write("%-15s " % (process.flips))
    card.write("\n")

    #nuisance jes
    card.write("%-40s %-5s " % ("jes","shape"))
    for process in processes: card.write("%-15s " % (process.jes))
    card.write("\n")

    return

def writeOneCard(dir, signal, kine, plot, output):
    #define processes (signal first)
    data = Process(-1,"data","pdata_histos_"+kine+"_"+lumi+"ifb.root",plot)
    signal = Process(0,signal,signal+"_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TTW = Process(1,"ttw","ttw_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TTZ = Process(2,"ttz","ttz_histos_"+kine+"_"+lumi+"ifb.root",plot)
    WZ  = Process(3,"wz","wz_histos_"+kine+"_"+lumi+"ifb.root",plot)
    sfakes = Process(4,"sfakes","sfakes_dd_histos_"+kine+"_"+lumi+"ifb.root",plot)
    dfakes = Process(5,"dfakes","dfakes_mc_histos_"+kine+"_"+lumi+"ifb.root",plot)
    flips = Process(6,"flips","flips_mc_histos_"+kine+"_"+lumi+"ifb.root",plot)
    #overwrite nuisances
    signal.jes  = "1"
    signal.signal_stat = "1"
    TTW.TTW = "1.1"
    TTW.ttw_extr = "1"
    TTW.jes  = "1"
    TTW.ttw_stat = "1"
    TTZ.TTZ = "1.1"
    TTZ.ttz_extr = "1"
    TTZ.jes  = "1"
    TTZ.ttz_stat = "1"
    WZ.WZ = ("%4.2f" % (1. + 0.08*math.sqrt(10.0)/math.sqrt(float(lumi))) ) #lumi dependent
    WZ.wz_extr = "1"
    sfakes.fakes = "1.4"
    sfakes.sfakes_stat = "1"
    dfakes.fakes = "1.4"
    flips.flips = "1.5"
    #fill list of processes    
    processes = []
    processes.append(signal)
    processes.append(TTW)
    processes.append(TTZ)
    processes.append(WZ)
    processes.append(sfakes)
    processes.append(dfakes)
    processes.append(flips)
    #create it
    writeOneCardFromProcesses(dir, kine, plot, output, data, processes )
    return

def writeAllCards(dir, signal, suffix=""):
    writeOneCard(dir, signal, "hihi",   "sr"+suffix, "card_"+signal+suffix+"_"+lumi+"ifb-hihi.txt" )
    writeOneCard(dir, signal, "hilow",  "sr"+suffix, "card_"+signal+suffix+"_"+lumi+"ifb-hilow.txt" )
    writeOneCard(dir, signal, "lowlow", "sr"+suffix, "card_"+signal+suffix+"_"+lumi+"ifb-lowlow.txt" )
    olddir = os.getcwd()
    os.chdir(dir)
    f = open('card_'+signal+suffix+'_'+lumi+'ifb-all.txt', 'wb')
    subprocess.call(["combineCards.py","card_"+signal+suffix+"_"+lumi+"ifb-hihi.txt","card_"+signal+suffix+"_"+lumi+"ifb-hilow.txt","card_"+signal+suffix+"_"+lumi+"ifb-lowlow.txt"],stdout=f)
    os.chdir(olddir)

#main body
if len(sys.argv)==3:
    dir = sys.argv[1]
    signal = sys.argv[2]
    writeAllCards( dir, signal )
elif len(sys.argv)==4:
    dir = sys.argv[1]
    signal = sys.argv[2]
    suffix = sys.argv[3]
    writeAllCards( dir, signal, suffix )
elif len(sys.argv)==6:
    dir = sys.argv[1]
    signal = sys.argv[2]
    kine = sys.argv[3]
    plot = sys.argv[4]
    output = sys.argv[5]
    writeOneCard( dir, signal, kine, plot, output )
else: print "number of arguments not supported"
