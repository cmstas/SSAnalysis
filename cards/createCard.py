import ROOT
import sys
import os
import subprocess
import math

#example: python createCard.py dir t1tttt_1500 
#example: python createCard.py dir t1tttt_1500 hihi sr card-hihi.txt
#example: for dir in v4.04; do for sig in t1tttt_1200 t1tttt_1500 t6ttww_650 t6ttww_600 t5tttt_deg; do python createCard.py ${dir} ${sig}; done; done
#example: for dir in dir1 dir2; do for sig in t1tttt_1200 t1tttt_1500; do python createCard.py ${dir} ${sig}; done; done

#for dir in gc.v1.24; do for sig in t1tttt_1500 t1tttt_1200 t1ttbbww_1300 t5tttt_1000 t5qqww_1500 t5qqww_1200 t5qqww_deg t6ttww_650 t6ttww_600; do python createCard.py ${dir} ${sig}; done; done

#then get expected limits with: combine -M Asymptotic dir/card.txt --run expected --noFitAsimov

#to add more nuisances edit Process, writeOneCardFromProcesses and then set values in writeOneCard

#lumi = "0.1"
lumi = "1.3"
#lumi = "3.0"

pseudoData = 0

class Process:
    def __init__(self, mycount, myname, myrootf, myplot):
        self.count = mycount
        self.name = myname
        self.rootf = myrootf
        self.plot = myplot
        self.lumi  = "-"
        self.jes  = "-"
        self.lepeff  = "-"
        self.lephlt  = "-"
        self.hthlt  = "-"
        self.btag = "-"
        self.trigeff = "-"
        self.signal_stat = "-"
        self.TTW = "-"
        self.ttw_pdf = "-"
        self.ttw_extr_hth = "-"
        self.ttw_extr_htl = "-"
        self.ttw_extr_ll  = "-"
        self.ttw_stat = "-"
        self.TTZH = "-"
        self.ttzh_pdf = "-"
        self.ttzh_extr_hth = "-"
        self.ttzh_extr_htl = "-"
        self.ttzh_extr_ll  = "-"
        self.ttzh_stat = "-"
        self.WZ = "-"
        self.WZ_stat = "-"
        #self.wz_extr = "-"
        self.WW = "-"
        self.WW_stat = "-"
        self.TG = "-"
        self.tg_stat = "-"
        self.VG = "-"
        self.vg_stat = "-"
        self.rares = "-"
        self.rares_stat = "-"
        self.fakes = "-"
        self.fakes_stat = "-"
        self.fakes_EWK = "-"
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

    #nuisance lumi
    card.write("%-40s %-5s " % ("lumi","lnN"))
    for process in processes: card.write("%-15s " % (process.lumi))
    card.write("\n")
    #nuisance jes
    card.write("%-40s %-5s " % ("jes","shape"))
    for process in processes: card.write("%-15s " % (process.jes))
    card.write("\n")
    #nuisance lepeff
    card.write("%-40s %-5s " % ("lepeff","lnN"))
    for process in processes: card.write("%-15s " % (process.lepeff))
    card.write("\n")
    #nuisance lephlt
    card.write("%-40s %-5s " % ("lephlt","lnN"))
    for process in processes: card.write("%-15s " % (process.lephlt))
    card.write("\n")
    #nuisance hthlt
    card.write("%-40s %-5s " % ("hthlt","shape"))
    for process in processes: card.write("%-15s " % (process.hthlt))
    card.write("\n")
    #nuisance btag
    card.write("%-40s %-5s " % ("btag","shape"))
    for process in processes: card.write("%-15s " % (process.btag))
    card.write("\n")
    #nuisance trigeff
    card.write("%-40s %-5s " % ("trigeff","lnN"))
    for process in processes: card.write("%-15s " % (process.trigeff))
    card.write("\n")

    #nuisance TTW
    card.write("%-40s %-5s " % ("TTW","lnN"))
    for process in processes: card.write("%-15s " % (process.TTW))
    card.write("\n")
    #nuisance TTW extr_hth
    card.write("%-40s %-5s " % ("ttw_extr_hth","shape"))
    for process in processes: card.write("%-15s " % (process.ttw_extr_hth))
    card.write("\n")
    #nuisance TTW extr_htl
    card.write("%-40s %-5s " % ("ttw_extr_htl","shape"))
    for process in processes: card.write("%-15s " % (process.ttw_extr_htl))
    card.write("\n")
    #nuisance TTW extr_ll
    card.write("%-40s %-5s " % ("ttw_extr_ll","shape"))
    for process in processes: card.write("%-15s " % (process.ttw_extr_ll))
    card.write("\n")
    #nuisance TTW stat
    card.write("%-40s %-5s " % ("ttw_stat","shape"))
    for process in processes: card.write("%-15s " % (process.ttw_stat))
    card.write("\n")

    #nuisance TTZH
    card.write("%-40s %-5s " % ("TTZH","lnN"))
    for process in processes: card.write("%-15s " % (process.TTZH))
    card.write("\n")
    #nuisance TTZH extr_hth
    card.write("%-40s %-5s " % ("ttzh_extr_hth","shape"))
    for process in processes: card.write("%-15s " % (process.ttzh_extr_hth))
    card.write("\n")
    #nuisance TTZH extr_htl
    card.write("%-40s %-5s " % ("ttzh_extr_htl","shape"))
    for process in processes: card.write("%-15s " % (process.ttzh_extr_htl))
    card.write("\n")
    #nuisance TTZH extr_ll
    card.write("%-40s %-5s " % ("ttzh_extr_ll","shape"))
    for process in processes: card.write("%-15s " % (process.ttzh_extr_ll))
    card.write("\n")
    #nuisance TTZH stat
    card.write("%-40s %-5s " % ("ttzh_stat","shape"))
    for process in processes: card.write("%-15s " % (process.ttzh_stat))
    card.write("\n")

    #nuisance WZ
    card.write("%-40s %-5s " % ("WZ","lnN"))
    for process in processes: card.write("%-15s " % (process.WZ))
    card.write("\n")
    #nuisance WZ extr
    #card.write("%-40s %-5s " % ("wz_extr","shape"))
    #for process in processes: card.write("%-15s " % (process.wz_extr))
    #card.write("\n")

    #nuisance WW
    card.write("%-40s %-5s " % ("WW","lnN"))
    for process in processes: card.write("%-15s " % (process.WW))
    card.write("\n")

    #nuisance TG
    card.write("%-40s %-5s " % ("TG","lnN"))
    for process in processes: card.write("%-15s " % (process.TG))
    card.write("\n")

    #nuisance VG
    card.write("%-40s %-5s " % ("VG","lnN"))
    for process in processes: card.write("%-15s " % (process.VG))
    card.write("\n")

    #nuisance RARES
    card.write("%-40s %-5s " % ("rares","lnN"))
    for process in processes: card.write("%-15s " % (process.rares))
    card.write("\n")

    #nuisance fakes
    card.write("%-40s %-5s " % ("fakes","lnN"))
    for process in processes: card.write("%-15s " % (process.fakes))
    card.write("\n")
    #nuisance fakes stat
    card.write("%-40s %-5s " % ("fakes_stat","shape"))
    for process in processes: card.write("%-15s " % (process.fakes_stat))
    card.write("\n")
    #nuisance fakes EWK
    card.write("%-40s %-5s " % ("fakes_EWK","shape"))
    for process in processes: card.write("%-15s " % (process.fakes_EWK))
    card.write("\n")

    #nuisance flips
    card.write("%-40s %-5s " % ("flips","lnN"))
    for process in processes: card.write("%-15s " % (process.flips))
    card.write("\n")

    return

def writeOneCard(dir, signal, kine, plot, output):
    #define processes (signal first)
    if pseudoData:
        print "Using pseudo data!"
        data = Process(-1,"data","pdata_histos_"+kine+"_"+lumi+"ifb.root",plot)
    else:
        print "Using real data!"
        data = Process(-1,"data","data_histos_"+kine+"_"+lumi+"ifb.root",plot)
    signal = Process(0,signal,signal+"_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TTW = Process(1,"ttw","ttw_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TTZH = Process(2,"ttzh","ttzh_histos_"+kine+"_"+lumi+"ifb.root",plot)
    WZ  = Process(3,"wz","wz_histos_"+kine+"_"+lumi+"ifb.root",plot)
    WW  = Process(4,"ww","ww_histos_"+kine+"_"+lumi+"ifb.root",plot)
    TG  = Process(5,"tg","tg_histos_"+kine+"_"+lumi+"ifb.root",plot)
    VG  = Process(6,"vg","vg_histos_"+kine+"_"+lumi+"ifb.root",plot)
    rares = Process(7,"rares","rares_histos_"+kine+"_"+lumi+"ifb.root",plot)
    fakes = Process(8,"fakes","fakes_histos_"+kine+"_"+lumi+"ifb.root",plot)
    flips = Process(9,"flips","flips_histos_"+kine+"_"+lumi+"ifb.root",plot)
    #overwrite nuisances
    signal.lumi  = "1.09"
    signal.jes  = "1"
    signal.signal_stat = "1"
    TTW.TTW          = "1.13"
    TTW.lumi         = "1.09"
    TTW.ttw_pdf      = "1.04"
    TTW.ttw_extr_hth = "1"
    TTW.ttw_extr_htl = "1"
    TTW.ttw_extr_ll  = "1"
    TTW.jes  = "1"
    TTW.lepeff  = "1.04"
    TTW.lephlt  = "1.02"
    TTW.hthlt  = "1"
    TTW.btag = "1"
    TTW.trigeff = "1.04"
    TTW.ttw_stat = "1"
    TTZH.TTZH          = "1.11"
    TTZH.lumi          = "1.09"
    TTZH.ttzh_pdf      = "1.04"
    TTZH.ttzh_extr_hth = "1"
    TTZH.ttzh_extr_htl = "1"
    TTZH.ttzh_extr_ll  = "1"
    TTZH.jes  = "1"
    TTZH.lepeff  = "1.04"
    TTZH.lephlt  = "1.02"
    TTZH.hthlt  = "1"
    TTZH.btag = "1"
    TTZH.trigeff = "1.04"
    TTZH.ttzh_stat = "1"
    WZ.WZ = "1.30"
    #WZ.wz_extr = "1"
    WZ.jes  = "1"
    WZ.btag = "1"
    WZ.trigeff = "1"
    WZ.wz_stat = "1"
    WW.WW = "1.30"
    WW.lumi = "1.09"
    WW.jes  = "1"
    WW.lepeff  = "1.04"
    WW.lephlt  = "1.02"
    WW.hthlt  = "1"
    WW.btag = "1"
    WW.trigeff = "1.04"
    WW.ww_stat = "1"
    TG.TG = "1.50"
    TG.lumi = "1.09"
    TG.jes  = "1"
    TG.lepeff  = "1.04"
    TG.lephlt  = "1.02"
    TG.hthlt  = "1"
    TG.btag = "1"
    TG.trigeff = "1.04"
    TG.tg_stat = "1"
    VG.VG = "1.50"
    VG.lumi = "1.09"
    VG.jes  = "1"
    VG.lepeff  = "1.04"
    VG.lephlt  = "1.02"
    VG.hthlt  = "1"
    VG.btag = "1"
    VG.trigeff = "1.04"
    VG.vg_stat = "1"
    rares.RARES = "1.30"
    rares.lumi = "1.09"
    rares.jes  = "1"
    rares.lepeff  = "1.04"
    rares.lephlt  = "1.02"
    rares.hthlt  = "1"
    rares.btag = "1"
    rares.trigeff = "1.04"
    rares.rares_stat = "1"
    fakes.fakes = "1.30"
    fakes.fakes_stat = "1"
    fakes.fakes_EWK = "1"
    flips.flips = "1.30"

    #fill list of processes    
    processes = []
    processes.append(signal)
    processes.append(TTW)
    processes.append(TTZH)
    processes.append(WZ)
    processes.append(WW)
    processes.append(TG)
    processes.append(VG)
    processes.append(rares)
    processes.append(fakes)
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
