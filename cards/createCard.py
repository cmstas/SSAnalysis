import ROOT
import sys
import os
import subprocess
import math

#example: python createCard.py dir t1tttt_1500 
#example: python createCard.py dir t1tttt_1500 hihi sr card-hihi.txt
#example: for dir in v4.04; do for sig in t1tttt_1200 t1tttt_1500 t6ttww_650 t6ttww_600 t5tttt_deg t5qqqq_1200; do python createCard.py ${dir} ${sig}; done; done

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
        self.TTW = "-"
        self.ttw_pdf = "-"
        self.ttw_extr_hth = "-"
        self.ttw_extr_htl = "-"
        self.ttw_extr_ll  = "-"
        self.TTZH = "-"
        self.ttzh_pdf = "-"
        self.ttzh_extr_hth = "-"
        self.ttzh_extr_htl = "-"
        self.ttzh_extr_ll  = "-"
        self.WZ = "-"
        #self.wz_extr = "-"
        self.WW = "-"
        self.XG = "-"
        self.rares = "-"
        self.fakes = "-"
        self.fakes_EWK = "-"
        self.flips = "-"
    def rate(self): 
        f = ROOT.TFile(dir+"/"+self.rootf)
        if f.Get(self.plot): return f.Get(self.plot).Integral()
        else: 
            print self.plot+" not found in "+self.rootf
            return 0

def writeStatForProcess(dir, card, kine, process, processes):
    f = ROOT.TFile(dir+"/"+process.rootf,"UPDATE")
    hup = f.Get(("%s_statUp" % (process.name)))
    hdn = f.Get(("%s_statDown" % (process.name)))
    if hup:
        for bin in range(1,hup.GetNbinsX()+1):
            card.write ("%-40s %-5s " % (("%s%s%s%s" % (process.name,"_stat_",kine,bin)), "shape") )
            for myprocess in processes: 
                if myprocess.count == process.count: 
                    card.write("%-15s " % "1")
                    hupnew = hup.Clone("%s%s%s%sUp" % (process.name,"_stat_",kine,bin))
                    hupnew.Reset()
                    hupnew.SetBinContent(bin,hup.GetBinContent(bin))
                    hdnnew = hdn.Clone("%s%s%s%sDown" % (process.name,"_stat_",kine,bin))
                    hdnnew.Reset()
                    hdnnew.SetBinContent(bin,hdn.GetBinContent(bin))
                    hdnnew.Write()
                else:  card.write("%-15s " % ("-"))
            card.write("\n")
    else: 
        print self.plot+" not found in "+self.rootf
        return 0
    f.Close()
    
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
    card.write("shapes data_obs * "+dir+"/"+data.rootf+" "+plot+" "+plot+"\n")
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

    #nuisance XG
    card.write("%-40s %-5s " % ("XG","lnN"))
    for process in processes: card.write("%-15s " % (process.XG))
    card.write("\n")

    #nuisance RARES
    card.write("%-40s %-5s " % ("rares","lnN"))
    for process in processes: card.write("%-15s " % (process.rares))
    card.write("\n")

    #nuisance fakes
    card.write("%-40s %-5s " % ("fakes","lnN"))
    for process in processes: card.write("%-15s " % (process.fakes))
    card.write("\n")
    #nuisance fakes EWK
    card.write("%-40s %-5s " % ("fakes_EWK","shape"))
    for process in processes: card.write("%-15s " % (process.fakes_EWK))
    card.write("\n")

    #nuisance flips
    card.write("%-40s %-5s " % ("flips","lnN"))
    for process in processes: card.write("%-15s " % (process.flips))
    card.write("\n")

    for process in processes: writeStatForProcess(dir,card,kine,process,processes)

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
    XG  = Process(5,"xg","xg_histos_"+kine+"_"+lumi+"ifb.root",plot)
    rares = Process(6,"rares","rares_histos_"+kine+"_"+lumi+"ifb.root",plot)
    fakes = Process(7,"fakes","fakes_histos_"+kine+"_"+lumi+"ifb.root",plot)
    flips = Process(8,"flips","flips_histos_"+kine+"_"+lumi+"ifb.root",plot)
    #overwrite nuisances
    signal.lumi  = "1.09"
    signal.jes  = "1"
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
    WZ.WZ = "1.30"
    #WZ.wz_extr = "1"
    WZ.jes  = "1"
    WZ.btag = "1"
    WZ.trigeff = "1"
    WW.WW = "1.30"
    WW.lumi = "1.09"
    WW.jes  = "1"
    WW.lepeff  = "1.04"
    WW.lephlt  = "1.02"
    WW.hthlt  = "1"
    WW.btag = "1"
    WW.trigeff = "1.04"
    XG.XG = "1.50"
    XG.lumi = "1.09"
    XG.jes  = "1"
    XG.lepeff  = "1.04"
    XG.lephlt  = "1.02"
    XG.hthlt  = "1"
    XG.btag = "1"
    XG.trigeff = "1.04"
    rares.RARES = "1.30"
    rares.lumi = "1.09"
    rares.jes  = "1"
    rares.lepeff  = "1.04"
    rares.lephlt  = "1.02"
    rares.hthlt  = "1"
    rares.btag = "1"
    rares.trigeff = "1.04"
    fakes.fakes = "1.30"
    fakes.fakes_EWK = "1"
    flips.flips = "1.30"

    #fill list of processes    
    processes = []
    processes.append(signal)
    processes.append(TTW)
    processes.append(TTZH)
    processes.append(WZ)
    processes.append(WW)
    processes.append(XG)
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
