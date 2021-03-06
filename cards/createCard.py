import ROOT
import sys
import os
import subprocess
import math

#example: python createCard.py dir t1tttt_1500 
#example: python createCard.py dir t1tttt_1500 hihi sr card-hihi.txt
#example: for dir in v5.06; do for sig in t1tttt_1200 t1tttt_1500 t6ttww_650 t6ttww_600 t5tttt_deg t5qqqqww_1200 t5qqqqww_deg; do python createCard.py ${dir} ${sig}; done; done

#then get expected limits with: combine -M Asymptotic dir/card.txt --run expected --noFitAsimov
#for sig in t1tttt_1200 t1tttt_1500 t6ttww_650 t6ttww_600 t5tttt_deg t5qqqqww_1200 t5qqqqww_deg; do echo ${sig}; combine -M Asymptotic v5.06/card_${sig}_2.1ifb-all.txt | grep ": r <" ; done

#to add more nuisances edit Process, writeOneCardFromProcesses and then set values in writeOneCard

# lumi = "4.0"
# lumi = "12.9"
# lumi = "17.3"
# lumi = "36.5"
# lumi = "36.8"
lumi = "35.9"
# lumi = "6.3"
# lumi = "2.3"

if "LUMI" in os.environ:
    lumi = os.environ["LUMI"]

pseudoData = 0

doComb = False
doMetUnc = False

class Process:
    def __init__(self, mycount, myname, myrootf, myplot):
        self.count = mycount
        self.name = myname
        self.rootf = myrootf
        self.plot = myplot
        self.lumi  = "-"
        self.jes  = "-"
        self.isr  = "-"
        self.met  = "-"
        self.scale  = "-"
        #self.norm  = "-"
        self.fs_lep  = "-"
        self.fs_hlt  = "-"
        self.lepeff  = "-"
        self.hlt  = "-"
        self.lephlt  = "-"
        self.hthlt  = "-"
        self.btag = "-"
        self.pu = "-"
        self.TTW = "-"
        # self.ttw_pdf = "-"
        # self.ttw_extr_scale = "-"
        # self.ttw_extr_htl = "-"
        # self.ttw_extr_njl = "-"
        # self.ttw_extr_njh = "-"
        # self.ttw_extr_ll  = "-"
        self.TTZH = "-"
        self.TTZSF = "-"
        # self.ttzh_pdf = "-"
        # self.ttzh_extr_scale = "-"
        # self.ttzh_extr_htl = "-"
        # self.ttzh_extr_njl = "-"
        # self.ttzh_extr_njh = "-"
        # self.ttzh_extr_ll  = "-"
        self.WZSF = "-"
        # self.wz_extr_scale = "-"
        # self.ww_extr_scale = "-"
        self.WW = "-"
        # self.ww_pdf = "-"
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

def writeStatForProcessCorrelated(card,process,processes):
    card.write ("%-40s %-5s " % (("%s%s" % (process.name,"_stat")), "shape") )
    for myprocess in processes: 
        if myprocess.count == process.count: 
            card.write("%-15s " % "1")
        else:  card.write("%-15s " % ("-"))
    card.write("\n")

def writeStatForProcess(dir, card, kine, process, processes, statshape=None):
    if process.name=="sig": return #fake signal for MI limits does not need stat unc.
    ra5 = ""
    if doComb: ra5 = "ra5_"
    # open file first to read. if one of the hists exists, then we've already called this function
    # and no need to open the file again and write
    f = ROOT.TFile(dir+"/"+process.rootf)

    name = "_stat_"
    if statshape is not None:
        name = "_statshape_"

    h_test = "%s%s%s%s%sUp" % (ra5,process.name,name,kine,1)
    bypass = False
    if not f.Get(h_test) or bypass:
        f = ROOT.TFile(dir+"/"+process.rootf,"UPDATE")

    h = f.Get("sr")
    hup = f.Get(("%s_statUp" % (process.name)))
    hdn = f.Get(("%s_statDown" % (process.name)))

    if hup:
        for bin in range(1,hup.GetNbinsX()+1):
            card.write ("%-40s %-5s " % (("%s%s%s%s%s" % (ra5,process.name,name,kine,bin)), "shape") )
            for myprocess in processes: 
                if myprocess.count == process.count: 
                    card.write("%-15s " % "1")
                    hupnewtest = f.Get("%s%s%s%s%sUp" % (ra5,process.name,name,kine,bin))
                    if not hupnewtest or bypass:
                        hupnew = h.Clone("%s%s%s%s%sUp" % (ra5,process.name,name,kine,bin))
                        if statshape is None:
                            hupnew.SetBinContent(bin,hup.GetBinContent(bin))
                        else:
                            cent = h.GetBinContent(bin)
                            up_stat = hup.GetBinContent(bin)
                            dup_stat = up_stat-cent
                            dup_scale = statshape*cent # statshape must be 0.3 for 30% up or something
                            up = cent + (dup_stat**2.0+dup_scale**2.0)**0.5
                            hupnew.SetBinContent(bin,up)
                        hupnew.Write()
                    hdnnewtest = f.Get("%s%s%s%s%sDown" % (ra5,process.name,name,kine,bin))
                    if not hdnnewtest or bypass:
                        hdnnew = h.Clone("%s%s%s%s%sDown" % (ra5,process.name,name,kine,bin))
                        if statshape is None:
                            if hdn.GetBinContent(bin) > 0: hdnnew.SetBinContent(bin,hdn.GetBinContent(bin))
                            else: hdnnew.SetBinContent(bin,0)
                        else:
                            cent = h.GetBinContent(bin)
                            down_stat = hdn.GetBinContent(bin)
                            ddown_stat = cent-down_stat
                            ddown_scale = statshape*cent # statshape must be 0.3 for 30% down or something
                            down = cent - (ddown_stat**2.0+ddown_scale**2.0)**0.5
                            hdnnew.SetBinContent(bin,max(down,1.e-6))
                        hdnnew.Write()
                else:  card.write("%-15s " % ("-"))
            card.write("\n")
    else: 
        print hup.GetName()+" not found in "+process.rootf
        return 0
    if not h_test:
        f.Close()

def writeDummyPdfForSignal(dir, card, kine, process, processes):
    if process.name.find("fs_") == -1: return 0
    f = ROOT.TFile(dir+"/"+process.rootf,"UPDATE")
    h = f.Get("sr")
    hup = f.Get("pdf_flatUp")
    hdn = f.Get("pdf_flatDown")
    if hup:
        for bin in range(1,hup.GetNbinsX()+1):
            card.write ("%-40s %-5s " % (("%s%s%s%s" % (process.name,"_pdf_",kine,bin)), "shape") )
            for myprocess in processes: 
                if myprocess.count == process.count: 
                    card.write("%-15s " % "1")
                    hupnewtest = f.Get("%s%s%s%sUp" % (process.name,"_pdf_",kine,bin))
                    if not hupnewtest:
                        hupnew = h.Clone("%s%s%s%sUp" % (process.name,"_pdf_",kine,bin))
                        hupnew.SetBinContent(bin,hup.GetBinContent(bin))
                        hupnew.Write()
                    hdnnewtest = f.Get("%s%s%s%sDown" % (process.name,"_pdf_",kine,bin))
                    if not hdnnewtest:
                        hdnnew = h.Clone("%s%s%s%sDown" % (process.name,"_pdf_",kine,bin))
                        if hdn.GetBinContent(bin) > 0: hdnnew.SetBinContent(bin,hdn.GetBinContent(bin))
                        else: hdnnew.SetBinContent(bin,0)
                        hdnnew.Write()
                else:  card.write("%-15s " % ("-"))
            card.write("\n")
    else: 
        print hup.GetName()+" not found in "+process.rootf
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
        card.write("shapes "+process.name+" * "+dir+"/"+process.rootf+" "+plot+" $SYSTEMATIC\n")
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
    #nuisance isr
    card.write("%-40s %-5s " % ("isr","shape"))
    for process in processes: card.write("%-15s " % (process.isr))
    card.write("\n")
    if doMetUnc or "DOMETUNC" in os.environ:
        #nuisance met
        card.write("%-40s %-5s " % ("metUnc","shape"))
        for process in processes: card.write("%-15s " % (process.met))
        card.write("\n")
    #nuisance scale
    if kine != "agg":
        if "NOSCALEUNC" not in os.environ:
            card.write("%-40s %-5s " % ("scale","shape"))
            for process in processes: card.write("%-15s " % (process.scale))
            card.write("\n")
    #nuisance pdf_flat
    #for process in processes: writeDummyPdfForSignal(dir,card,kine,process,processes)
    #nuisance norm
    #card.write("%-40s %-5s " % ("norm","lnN"))
    #for process in processes: card.write("%-15s " % (process.norm))
    #card.write("\n")
    #nuisance fs_lep
    card.write("%-40s %-5s " % ("fs_lep","lnN"))
    for process in processes: card.write("%-15s " % (process.fs_lep))
    card.write("\n")
    #nuisance fs_hlt
    card.write("%-40s %-5s " % ("fs_hlt","lnN"))
    for process in processes: card.write("%-15s " % (process.fs_hlt))
    card.write("\n")
    #nuisance lepeff
    card.write("%-40s %-5s " % ("lepeff","lnN"))
    for process in processes: card.write("%-15s " % (process.lepeff))
    card.write("\n")
    if doComb == True:
        #nuisance hlt
        card.write("%-40s %-5s " % ("hlt","lnN"))
        for process in processes: card.write("%-15s " % (process.hlt))
        card.write("\n")
    else:
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

    #nuisance pu
    card.write("%-40s %-5s " % ("pu","shape"))
    for process in processes: card.write("%-15s " % (process.pu))
    card.write("\n")

    #nuisance TTW
    card.write("%-40s %-5s " % ("TTW","lnN"))
    for process in processes: card.write("%-15s " % (process.TTW))
    card.write("\n")
    # #nuisance TTW pdf
    # card.write("%-40s %-5s " % ("ttw_pdf","lnN"))
    # for process in processes: card.write("%-15s " % (process.ttw_pdf))
    # card.write("\n")


    if kine != "br":
        pass
        # #nuisance TTW extr_hth
        # card.write("%-40s %-5s " % ("ttw_extr_scale","shape"))
        # for process in processes: card.write("%-15s " % (process.ttw_extr_scale))
        # card.write("\n")
        # #nuisance TTW extr_htl
        # card.write("%-40s %-5s " % ("ttw_extr_htl","shape"))
        # for process in processes: card.write("%-15s " % (process.ttw_extr_htl))
        # card.write("\n")
        #nuisance TTW extr_njl
        # card.write("%-40s %-5s " % ("ttw_extr_njl","shape"))
        # for process in processes: card.write("%-15s " % (process.ttw_extr_njl))
        # card.write("\n")
        # #nuisance TTW extr_njh
        # card.write("%-40s %-5s " % ("ttw_extr_njh","shape"))
        # for process in processes: card.write("%-15s " % (process.ttw_extr_njh))
        # card.write("\n")
        # #nuisance TTW extr_ll
        # card.write("%-40s %-5s " % ("ttw_extr_ll","shape"))
        # for process in processes: card.write("%-15s " % (process.ttw_extr_ll))
        # card.write("\n")

    #nuisance TTZH
    card.write("%-40s %-5s " % ("TTZH","lnN"))
    for process in processes: card.write("%-15s " % (process.TTZH))
    card.write("\n")
    #nuisance TTZH
    card.write("%-40s %-5s " % ("TTZSF","lnN"))
    for process in processes: card.write("%-15s " % (process.TTZSF))
    card.write("\n")
    # #nuisance TTZH pdf
    # card.write("%-40s %-5s " % ("ttzh_pdf","lnN"))
    # for process in processes: card.write("%-15s " % (process.ttzh_pdf))
    # card.write("\n")


    if kine != "br":
        pass
        # #nuisance TTZH extr_hth
        # card.write("%-40s %-5s " % ("ttzh_extr_scale","shape"))
        # for process in processes: card.write("%-15s " % (process.ttzh_extr_scale))
        # card.write("\n")
        # #nuisance TTZH extr_htl
        # card.write("%-40s %-5s " % ("ttzh_extr_htl","shape"))
        # for process in processes: card.write("%-15s " % (process.ttzh_extr_htl))
        # card.write("\n")
        #nuisance TTZH extr_njl
        # card.write("%-40s %-5s " % ("ttzh_extr_njl","shape"))
        # for process in processes: card.write("%-15s " % (process.ttzh_extr_njl))
        # card.write("\n")
        # #nuisance TTZH extr_njh
        # card.write("%-40s %-5s " % ("ttzh_extr_njh","shape"))
        # for process in processes: card.write("%-15s " % (process.ttzh_extr_njh))
        # card.write("\n")
        # #nuisance TTZH extr_ll
        # card.write("%-40s %-5s " % ("ttzh_extr_ll","shape"))
        # for process in processes: card.write("%-15s " % (process.ttzh_extr_ll))
        # card.write("\n")

    #nuisance WZ
    if doComb == True:
        card.write("%-40s %-5s " % ("ra5_WZ","lnN"))
    else:
        card.write("%-40s %-5s " % ("WZSF","lnN"))
    for process in processes: card.write("%-15s " % (process.WZSF))
    card.write("\n")

    if kine != "br":
        pass
        # #nuisance WZ extr_hth
        # card.write("%-40s %-5s " % ("wz_extr_scale","shape"))
        # for process in processes: card.write("%-15s " % (process.wz_extr_scale))
        # card.write("\n")

    #nuisance WW
    card.write("%-40s %-5s " % ("WW","lnN"))
    for process in processes: card.write("%-15s " % (process.WW))
    card.write("\n")

    if kine != "br":
        pass
        # #nuisance WW extr_hth
        # card.write("%-40s %-5s " % ("ww_extr_scale","shape"))
        # for process in processes: card.write("%-15s " % (process.ww_extr_scale))
        # card.write("\n")

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

    for process in processes: 
        if process.name in ["ttw","ttzh","wz","ww"]:
            statshape = 0.15 # ttw, ttzh, wz
            if process.name in ["ww"]: statshape = 0.20
            writeStatForProcess(dir,card,kine,process,processes, statshape=statshape)
        else:
            writeStatForProcess(dir,card,kine,process,processes)
    # print "warning: using correlated statistical uncertainties"
    # for process in processes: writeStatForProcessCorrelated(card,process,processes)

    return

def writeOneCard(dir, signal, kine, plot, output):
    #define processes (signal first)
    if pseudoData:
        print "Using pseudo data!"
        data = Process(-1,"data","pdata_histos_"+kine+"_"+lumi+"ifb.root",plot)
    else:
        # print "Using real data!"
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
    lumiunc = "1.026"
    signal.lumi  = lumiunc
    signal.lephlt  = "1.02"
    signal.hlt  = "1.03"
    if signal.name!="sig":  #fake signal for MI limits does not need these
        signal.jes  = "1"
        if signal.name.find("fs_") != -1:
            #these are only for fast sim
            signal.scale  = "1"
            signal.fs_hlt = "1.05"
            if kine is "hihi": signal.fs_lep  = "1.08"
            if kine is "agg": signal.fs_lep  = "1.08"
            if kine is "hilow": signal.fs_lep  = "1.15"
            if kine is "lowlow": signal.fs_lep  = "1.20"
            signal.isr  = "1" #fixme: this one we should have also for fullsim (need to update the yieldMaker)
            signal.met  = "1"
        if kine is "hihi": signal.lepeff  = "1.04"
        if kine is "agg": signal.lepeff  = "1.04"
        if kine is "hilow": signal.lepeff  = "1.08"
        if kine is "lowlow": signal.lepeff  = "1.10"
        signal.hthlt  = "1"
        signal.btag = "1"
        signal.pu = "1"
    else:
        print "making fake signal for MI limits!"
    ttz_sf = "1.30" # TTZ SF ERROR
    wz_sf = "1.12" # WZ SF ERROR
    TTW.TTW          = "1.13"
    TTW.lumi         = lumiunc
    # TTW.ttw_pdf      = "1.04"
    # TTW.ttw_extr_scale = "1"
    # TTW.ttw_extr_htl = "1"
    # TTW.ttw_extr_njl = "1"
    # TTW.ttw_extr_njh = "1"
    # TTW.ttw_extr_ll  = "1"
    TTW.jes  = "1"
    if kine is "hihi": TTW.lepeff  = "1.04"
    if kine is "agg": TTW.lepeff  = "1.04"
    if kine is "hilow": TTW.lepeff  = "1.08"
    if kine is "lowlow": TTW.lepeff  = "1.10"
    TTW.lephlt  = "1.02"
    TTW.hlt  = "1.03"
    TTW.hthlt  = "1"
    TTW.btag = "1"
    TTW.pu = "1"
    TTZH.TTZSF          = ttz_sf
    TTZH.TTZH          = "1.0"
    TTZH.lumi          = lumiunc
    # TTZH.ttzh_pdf      = "1.04"
    # TTZH.ttzh_extr_scale = "1"
    # TTZH.ttzh_extr_njh = "1"
    # TTZH.ttzh_extr_njl = "1"
    # TTZH.ttzh_extr_htl = "1"
    # TTZH.ttzh_extr_ll  = "1"
    TTZH.jes  = "1"
    if kine is "hihi": TTZH.lepeff  = "1.04"
    if kine is "agg": TTZH.lepeff  = "1.04"
    if kine is "hilow": TTZH.lepeff  = "1.08"
    if kine is "lowlow": TTZH.lepeff  = "1.10"
    TTZH.lephlt  = "1.02"
    TTZH.hlt  = "1.03"
    TTZH.hthlt  = "1"
    TTZH.btag = "1"
    TTZH.pu = "1"
    WZ.WZSF = wz_sf
    # WZ.wz_extr_scale = "1"
    WZ.jes  = "1"
    WZ.btag = "1"
    WZ.pu = "1"
    WW.WW = "1.20"
    # WW.ww_extr_scale = "1"
    # WW.ww_pdf = "1.04"
    WW.lumi = lumiunc
    WW.jes  = "1"
    if kine is "hihi": WW.lepeff  = "1.04"
    if kine is "agg": WW.lepeff  = "1.04"
    if kine is "hilow": WW.lepeff  = "1.08"
    if kine is "lowlow": WW.lepeff  = "1.10"
    WW.lephlt  = "1.02"
    WW.hlt  = "1.03"
    WW.hthlt  = "1"
    WW.btag = "1"
    WW.pu = "1"
    XG.XG = "1.50"
    XG.lumi = lumiunc
    XG.jes  = "1"
    if kine is "hihi": XG.lepeff  = "1.04"
    if kine is "agg": XG.lepeff  = "1.04"
    if kine is "hilow": XG.lepeff  = "1.08"
    if kine is "lowlow": XG.lepeff  = "1.10"
    XG.lephlt  = "1.02"
    XG.hlt  = "1.03"
    XG.hthlt  = "1"
    XG.btag = "1"
    XG.pu = "1"
    rares.rares = "1.50"
    rares.lumi = lumiunc
    rares.jes  = "1"
    if kine is "hihi": rares.lepeff  = "1.04"
    if kine is "agg": rares.lepeff  = "1.04"
    if kine is "hilow": rares.lepeff  = "1.08"
    if kine is "lowlow": rares.lepeff  = "1.10"
    rares.lephlt  = "1.02"
    rares.hlt  = "1.03"
    rares.hthlt  = "1"
    rares.btag = "1"
    rares.pu = "1"
    fakes.fakes = "1.30"
    fakes.fakes_EWK = "1"
    flips.flips = "1.20"

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

def writeAllCardsForOneKine(dir, signal, kine, suffix=""):
    print "Writing all cards for one kinematic region"
    writeOneCard(dir, signal, kine,   "sr"+suffix, "card_"+signal+suffix+"_"+lumi+"ifb-"+kine+".txt" )
    olddir = os.getcwd()
    os.chdir(dir)
    f = open('card_'+signal+suffix+'_'+lumi+'ifb-all.txt', 'wb')
    subprocess.call(["combineCards.py","card_"+signal+suffix+"_"+lumi+"ifb-"+kine+".txt"],stdout=f)
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
elif len(sys.argv)==5:
    dir = sys.argv[1]
    signal = sys.argv[2]
    suffix = sys.argv[3]
    kine = sys.argv[4]
    suffix = "" # override suffix. we only need it to make the number of arguments greater than the previous elif
    writeAllCardsForOneKine( dir, signal, kine, suffix )
elif len(sys.argv)==6:
    dir = sys.argv[1]
    signal = sys.argv[2]
    kine = sys.argv[3]
    plot = sys.argv[4]
    output = sys.argv[5]
    writeOneCard( dir, signal, kine, plot, output )
else: print "number of arguments not supported"
