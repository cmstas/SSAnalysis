#!/bin/bash

# path="/hadoop/cms/store/user/${USER}/condor/ss_13_babies/"
# path="/hadoop/cms/store/user/namin/condor/ss_babies_July8_6p26ifb/"
# path="/hadoop/cms/store/user/namin/condor/ss_babies_July27_isr/"
# path="/hadoop/cms/store/user/namin/condor/ss_babies_Aug25/"
# path="/hadoop/cms/store/user/namin/condor/ss_babies_June22_3p99ifb/" # for ttw_nlo, ttz_nlo
# path="/hadoop/cms/store/user/namin/condor/ss_babies_Sept22/"
# path="/hadoop/cms/store/user/namin/condor/ss_babies_Oct4/"
# path="/hadoop/cms/store/user/namin/condor/ss_babies_Oct22/"
path="/hadoop/cms/store/user/namin/condor/ss_babies_Nov5/"

samples = [

# ["DataMuonEG","datamuoneg*.root"],
# ["DataDoubleEG","datadoubleeg*.root"],
# ["DataDoubleMuon","datadoublemuon*.root"],

# ["T6TTWW_main", "t6ttww_main_*.root"],
# ["T5TTTT_dm175", "t5tttt_dm175_*.root"],
# ["T5TTCC_main", "t5ttcc_main_*.root"],
# ["T5QQQQVV_main", "t5qqqqvv_main_*.root"],
# ["T5QQQQVV_dm20", "t5qqqqvv_dm20_*.root"],
["T1TTTT_main", "t1tttt_main_*.root"],
# ["T1TTBB_main", "t1ttbb_main_*.root"],

# ["TTH_scan", "tth_scan_*.root"],
# ["THW_scan", "thw_scan_*.root"],
# ["THQ_scan", "thq_scan_*.root"],

# ["TTZLOW","ttzlow_*.root"],
# ["WJets","wjets_*.root"],
# ["TTHtoNonBB","tthtononbb_*.root"],
# ["GGHtoZZto4L","gghtozzto4l_*.root"],
# ["DY_high","dy_high_[0-9]*.root"],
# ["DY_low","dy_low_*.root"],
# ["WWZ","wwz_*.root"],
# ["WWW","www_*.root"],
# ["TTZ","ttz_*.root"],
# ["TTW","ttw_*.root"],
# ["WZ","wz_*.root"],
# ["WGToLNuG","wgtolnug_*.root"],
# ["TTG","ttg_*.root"],
# ["TZQ","tzq_*.root"],
# ["TTTT","tttt_*.root"],
# ["VHtoNonBB","vhtononbb_*.root"],
# ["WZZ","wzz_*.root"],
# ["WWDPS","wwdps_*.root"],
# ["QQWW","qqww_*.root"],
# ["TG","tg_*.root"],
# ["ZG","zg_*.root"],
# ["ZZ","zz_*.root"],
# ["TTBAR_PH","ttbar_ph_*.root"],
# ["TTZnlo", "ttznlo_*.root"],
# ["TTWnlo", "ttwnlo_*.root"],

# ["DY_high_LO","dy_high_LO_*.root"],
# ["TTBAR","ttbar_[0-9]*.root"],
# ["WWZ","wwz_*.root"],
# ["ZZ","zz_*.root"],
# ["TTZ","ttz_*.root"],
# ["TTW","ttw_*.root"],
# ["TTWQQ","ttwqq_*.root"],
# ["WZ","wz_*.root"],
# ["WGToLNuG","wgtolnug_*.root"],
# ["TTG","ttg_*.root"],
# ["TTHtoNonBB","tthtononbb_*.root"],
# ["TZQ","tzq_*.root"],
# ["TTTT","tttt_*.root"],

# ["VHtoNonBB","vhtononbb_*.root"],
# ["WZZ","wzz_*.root"],
# ["TTZQ","ttzq_*.root"],

# ["TTBAR_PH","ttbar_ph_*.root"],
# ["WWDPS","wwdps_*.root"],
# ["SINGLETOP1","singletop1_*.root"],
# ["SINGLETOP2","singletop2_*.root"],
# ["SINGLETOP4","singletop4_*.root"],
# ["SINGLETOP5","singletop5_*.root"],
# ["QQWW","qqww_*.root"],
# ["TG","tg_*.root"],
# ["ZG","zg_*.root"],
# ["GGHtoZZto4L","gghtozzto4l_*.root"],
# ["WGMG","wgmg_*.root"],


# ["T6TTWW_600_425_50","t6ttww_600_425_50_*.root"],
# ["T6TTWW_650_150_50","t6ttww_650_150_50_*.root"],
# ["T1TTTT_1200","t1tttt_1200_*.root"],
# ["T1TTTT_1500","t1tttt_1500_*.root"],
# ["T5qqqqWW_1200_1000_800","t5qqqqww_1200_1000_800_[1-9]*.root"],
# ["T5ttttDeg_1000_300_285_280","t5ttttdeg_1000_300_285_280_[1-9]*.root"],
# ["T5qqqqWWDeg_1000_315_300","t5qqqqwwdeg_1000_315_300_[1-9]*.root"],
# ["TTZLOW","ttzlow_*.root"],
# ["GGHtoZZto4L","gghtozzto4l_*.root"],
# ["WZMG","wzmg_*.root"],
# ["WGMG","wgmg_*.root"],
# ["T1TTTT_1950_700to950","t1tttt_1950_700to950_*.root"],
# ["T1TTTT_1100_1to775","t1tttt_1100_1to775_*.root"],
# ["T1TTTT_1175_950","t1tttt_1175_950_*.root"],
# ["T1TTTT_1275_900to975","t1tttt_1275_900to975_*.root"],
# ["T1TTTT_1300_1to1075","t1tttt_1300_1to1075_*.root"],
# ["T1TTTT_1300to1325_700to1100","t1tttt_1300to1325_700to1100_*.root"],
# ["T1TTTT_1350to1375_50to1025","t1tttt_1350to1375_50to1025_*.root"],
# ["T1TTTT_1500to1525_50to1125","t1tttt_1500to1525_50to1125_*.root"],
# ["T1TTTT_1800to1850_1to1450","t1tttt_1800to1850_1to1450_*.root"],
# ["T1TTTT_625_275to375","t1tttt_625_275to375_*.root"],
# ["T1TTTT_625to650_200to400","t1tttt_625to650_200to400_*.root"],
# ["T1TTTT_650to675_250to425","t1tttt_650to675_250to425_*.root"],
# ["T1TTTT_1100to1125_700to900","t1tttt_1100to1125_700to900_*.root"],
# ["T1TTTT_700to750_200to500","t1tttt_700to750_200to500_*.root"],
# ["T1TTTT_1150_1to800","t1tttt_1150_1to800_*.root"],
# ["T1TTTT_1150to1175_750to925","t1tttt_1150to1175_750to925_*.root"],
# ["T1TTTT_1200_1to825","t1tttt_1200_1to825_*.root"],
# ["T1TTTT_1200to1225_800to1000","t1tttt_1200to1225_800to1000*.root"],
# ["T1TTTT_1250to1275_700to1050","t1tttt_1250to1275_700to1050_*.root"],
# ["T1TTTT_1325to1350_1to1125","t1tttt_1325to1350_1to1125_*.root"],
# ["T1TTTT_1400_1to1175","t1tttt_1400_1to1175_*.root"],
# ["T1TTTT_1400to1425_50to1100","t1tttt_1400to1425_50to1100_*.root"],
# ["T1TTTT_1425to1450_1to1200","t1tttt_1425to1450_1to1200_*.root"],
# ["T1TTTT_1450to1475_50to1075","t1tttt_1450to1475_50to1075_*.root"],
# ["T1TTTT_1475to1500_1to1250","t1tttt_1475to1500_1to1250_*.root"],
# ["T1TTTT_1525to1550_1to1300","t1tttt_1525to1550_1to1300_*.root"],
# ["T1TTTT_1600to1650_1to1350","t1tttt_1600to1650_1to1350_*.root"],
# ["T1TTTT_1650to1700_1to1400","t1tttt_1650to1700_1to1400_*.root"],
# ["T1TTTT_1700to1750_1to1450","t1tttt_1700to1750_1to1450_*.root"],
# ["T1TTTT_1750_50to1450","t1tttt_1750_50to1450_*.root"],
# ["T1TTTT_1850to1900_1to1450","t1tttt_1850to1900_1to1450_*.root"],
# ["T1TTTT_1900to1950_0to1450","t1tttt_1900to1950_0to1450_*.root"],
# ["T1TTTT_600_250to325","t1tttt_600_250to325_*.root"],
# ["T1TTTT_675_325to450","t1tttt_675_325to450_*.root"],
# ["T1TTTT_700_1to450","t1tttt_700_1to450_*.root"],
# ["T1TTTT_750to775_350to525","t1tttt_750to775_350to525_*.root"],
# ["T1TTTT_775_475to550","t1tttt_775_475to550_*.root"],
# ["T1TTTT_800to825_1to575","t1tttt_800to825_1to575_*.root"],
# ["T1TTTT_825to850_200to600","t1tttt_825to850_200to600_*.root"],
# ["T1TTTT_850to875_450to625","t1tttt_850to875_450to625_*.root"],
# ["T1TTTT_875to900_1to650","t1tttt_875to900_1to650_*.root"],
# ["T1TTTT_950to975_350to725","t1tttt_950to975_350to725_*.root"],
# ["T1TTTT_975_600to750","t1tttt_975_600to750_*.root"],
# ["T1TTTT_1375_950to1150","t1tttt_1375_950to1150_*.root"],
# ["T1TTTT_1400_1to1175","t1tttt_1400_1to1175_*.root"],
# ["T1TTTT_1000to1050_1to800","t1tttt_1000to1050_1to800_*.root"],
# ["T1TTTT_1000_1to700","t1tttt_1000_1to700_*.root"],
# ["T1TTTT_1050_50to775","t1tttt_1050_50to775_*.root"],
# ["T1TTTT_1050to1075_650to850","t1tttt_1050to1075_650to850_*.root"],
# ["T1TTTT_1225to1250_1to1025","t1tttt_1225to1250_1to1025_*.root"],
# ["T1TTTT_1550to1575_500to1175","t1tttt_1550to1575_500to1175_*.root"],
# ["T1TTTT_600_1to225","t1tttt_600_1to225_*.root"],
# ["T1TTTT_600to625_250to375","t1tttt_600to625_250to375_*.root"],
# ["T1TTTT_900to950_200to700","t1tttt_900to950_200to700_*.root"],
# ["T1TTTT_975_600to750","t1tttt_975_600to750_*.root"],
# ["T5QQQQVV_1000to1075_1to950","t5qqqqvv_1000to1075_1to950_*.root"],
# ["T5QQQQVV_1100to1175_1to1050","t5qqqqvv_1100to1175_1to1050_*.root"],
# ["T5QQQQVV_1200to1275_1to1150","t5qqqqvv_1200to1275_1to1150_*.root"],
# ["T5QQQQVV_1300to1375_1to1250","t5qqqqvv_1300to1375_1to1250_*.root"],
# ["T5QQQQVV_1400to1550_1to1275","t5qqqqvv_1400to1550_1to1275_*.root"],
# ["T5QQQQVV_1600to1750_1to950","t5qqqqvv_1600to1750_1to950_*.root"],
# ["T5QQQQVV_600to675_1to550","t5qqqqvv_600to675_1to550_*.root"],
# ["T5QQQQVV_700to775_1to650","t5qqqqvv_700to775_1to650_*.root"],
# ["T5QQQQVV_800to975_1to850","t5qqqqvv_800to975_1to850_*.root"],
# ["T6TTWW_50_300to600_225to575","t6ttww_50_300to600_225to575_*.root"],
# ["T6TTWW_50_300to600_75to125","t6ttww_50_300to600_75to125_*.root"],
# ["T6TTWW_50_350to600_150to400","t6ttww_50_350to600_150to400_*.root"],
# ["T6TTWW_50_625to950_500to850","t6ttww_50_625to950_500to850_*.root"],
# ["T6TTWW_50_625to950_550to875","t6ttww_50_625to950_550to875_*.root"],
# ["T6TTWW_50_625to950_75to125","t6ttww_50_625to950_75to125_*.root"],
# ["T6TTWW_50_650to950_150to750","t6ttww_50_650to950_150to750_*.root"],
# ["T6TTWW_50_300to600_150to500","t6ttww_50_300to600_150to500_*.root"],
#  ["T5QQQQWW_1025to1200_0to1175",  "t5qqqqww_1025to1200_0to1175_*.root"], 
#  ["T5QQQQWW_1225to1400_0to1175",  "t5qqqqww_1225to1400_0to1175_*.root"], 
#  ["T5QQQQWW_1425to1600_0to1175",  "t5qqqqww_1425to1600_0to1175_*.root"], 
#  ["T5QQQQWW_1650to1700_0to1150",  "t5qqqqww_1650to1700_0to1150_*.root"], 
#  ["T5QQQQWW_600to800_0to725",     "t5qqqqww_600to800_0to725_*.root"],    
#  ["T5QQQQWW_825to1000_0to925",    "t5qqqqww_825to1000_0to925_*.root"],   
#  ["T1TTBB_1225to1400_0to1050",    "t1ttbb_1225to1400_0to1050_*.root"],   
#  ["T1TTBB_1425to1600_0to1250",    "t1ttbb_1425to1600_0to1250_*.root"],   
#  ["T1TTBB_1425to1600_1100to1350", "t1ttbb_1425to1600_1100to1350_*.root"],
#  ["T1TTBB_600to800_275to575",     "t1ttbb_600to800_275to575_*.root"],
#  ["T1TTBB_1225to1400_900to1175",  "t1ttbb_1225to1400_900to1175_*.root"],
#  ["T1TTBB_1650to2000_0to1450",    "t1ttbb_1650to2000_0to1450_*.root"],       
#  ["T1TTBB_825to1000_500to775",    "t1ttbb_825to1000_500to775_*.root"],       
#  ["T5ttttDM175_825to1000_500to725",   "t5ttttdm175_825to1000_500to725_*.root"],    
#  ["T5ttttDM175_1025to1200_700to925",  "t5ttttdm175_1025to1200_700to925_*.root"],   
#  ["T5ttttDM175_1225to1400_0to1050",   "t5ttttdm175_1225to1400_0to1050_*.root"],    
#  ["T5ttttDM175_1225to1400_900to1125", "t5ttttdm175_1225to1400_900to1125_*.root"],  
#  ["T5ttttDM175_1425to1625_1100to1300","t5ttttdm175_1425to1625_1100to1300_*.root"], 
#  ["T5ttttDM175_600to800_275to525",    "t5ttttdm175_600to800_275to525_*.root"],     
#  ["T5ttttDM175_825to1000_0to650",     "t5ttttdm175_825to1000_0to650_*.root"], 
#  ["T5tttt_degen_1225to1400_1075to1275", "t5tttt_degen_1225to1400_1075to1275_*.root"],
#  ["T5tttt_degen_1425to1600_1275to1375", "t5tttt_degen_1425to1600_1275to1375_*.root"],
#  ["T5tttt_degen_825to1000_0to825",      "t5tttt_degen_825to1000_0to825_*.root"],
#  ["T5tttt_degen_825to1000_675to875",    "t5tttt_degen_825to1000_675to875_*.root"],
#  ["T1TTBB_1650to1750_1350to1450"   , "t1ttbb_1650to1750_1350to1450_*.root"   ],
#  ["T1TTBB_825to1000_0to625"        , "t1ttbb_825to1000_0to625_*.root"        ],
#  ["T1TTBB_600to800_0to450"         , "t1ttbb_600to800_0to450_*.root"         ],
#  ["T5tttt_degen_1225to1400_0to1225", "t5tttt_degen_1225to1400_0to1225_*.root"], 
#  ["T5tttt_degen_600to800_450to675", "t5tttt_degen_600to800_450to675_*.root" ],    
#  ["T5tttt_degen_1425to1600_0to1350", "t5tttt_degen_1425to1600_0to1350_*.root"],    
#  ["T5tttt_degen_1650to1700_0to1350", "t5tttt_degen_1650to1700_0to1350_*.root"],    
#  ["T5tttt_degen_1025to1200_0to1025", "t5tttt_degen_1025to1200_0to1025_*.root"],    
#   ["T5ttcc_1025to1200_0to1025"      , "t5ttcc_1025to1200_0to1025_*.root"],
#   ["T5ttcc_825to1000_675to875"      , "t5ttcc_825to1000_675to875_*.root"],     ,
#   ["T5ttcc_1025to1200_875to1075"    , "t5ttcc_1025to1200_875to1075_*.root"]    ,
#   ["T5ttcc_825to1000_0to825"        , "t5ttcc_825to1000_0to825_*.root"]        ,
#   ["T5ttcc_600to800_450to675"       , "t5ttcc_600to800_450to675_*.root"]       ,
#   ["T5ttcc_1225to1400_1075to1225"   , "t5ttcc_1225to1400_1075to1225_*.root"]   ,
#   ["T5ttcc_1225to1400_0to1225"      , "t5ttcc_1225to1400_0to1225_*.root"]      ,
#   ["T5ttcc_1650to1700_0to1350"      , "t5ttcc_1650to1700_0to1350_*.root"]      ,
#  ["T5ttcc_1425to1525_1275to1375"   , "t5ttcc_1425to1525_1275to1375_*.root"]   ,
#   ["T5ttttDM175_1425to1600_0to1250" , "t5ttttdm175_1425to1600_0to1250_*.root"] ,
#   ["T5ttttDM175_1025to1200_0to850"  , "t5ttttdm175_1025to1200_0to850_*.root"]  ,
#   ["T5ttttDM175_1625to1700_0to1300" , "t5ttttdm175_1625to1700_0to1300_*.root"] ,
#   ["T5ttttDM175_600to800_0to450"    , "t5ttttdm175_600to800_0to450_*.root"   ] ,
#   ["T1TTBB_1025to1200_700to975"     , "t1ttbb_1025to1200_700to975_*.root" ]    ,
    # ["T1TTBB_1025to1200_0to850"           , "t1ttbb_1025to1200_0to850_*.root" ] ,         
    # ["T5tttt_degen_600to800_0to625"       , "t5tttt_degen_600to800_0to625_*.root"],      
    # ["T5tttt_degen_1025to1200_875to1075"  , "t5tttt_degen_1025to1200_875to1075_*.root"],
    # ["T5ttcc_600to800_0to625","t5ttcc_600to800_0to625_*.root"],
    # ["T5ttcc_1425to1600_0to1350","t5ttcc_1425to1600_0to1350"],
 ]

import os,glob

def getChunks(v,n=50): return [ v[i:i+n] for i in range(0, len(v), n) ]

path = path.replace("${USER}",os.getenv("USER"))

do_skip_bad = True

if do_skip_bad:
    print "[!] Hey! You've told me to skip bad files. Doing so begrudgingly."

for final,loc in samples:

    files = glob.glob("%s/%s" % (path,loc))

    if(len(files) < 1): continue

    print "[%s] Found %i files" % (final,len(files))

    if os.path.isfile(final+".root"):
        print "%s.root already in here, skipping" % final
        continue

    print "[%s] %s" % (final,path+"/"+loc)

    chunks = getChunks(files, 50)

    if len(chunks) > 1:
        mergedChunks = []
        for ichunk,chunk in enumerate(chunks):
            args = " ".join(chunk)
            outfile = "%s_chunk%i.root" % (final, ichunk)
            mergedChunks.append(outfile)
            print "[%s] Making chunk %i of %i: %s" % (final, ichunk, len(chunks)-1, outfile)
            if do_skip_bad:
                os.system("hadd -k %s %s > haddlog.txt" % (outfile, args))
            else:
                os.system("hadd  %s %s > haddlog.txt" % (outfile, args))
        print "[%s] Making final file: %s.root" % (final,final)
        if do_skip_bad:
            os.system("hadd -k %s.root %s >> haddlog.txt" % (final, " ".join(mergedChunks)))
        else:
            os.system("hadd %s.root %s >> haddlog.txt" % (final, " ".join(mergedChunks)))
        os.system("rm %s_chunk*.root" % (final))
    else:
        print "[%s] Making final file: %s.root" % (final,final)
        if do_skip_bad:
            os.system("hadd -k %s.root %s >> haddlog.txt" % (final, " ".join(chunks[0])))
        else:
            os.system("hadd  %s.root %s >> haddlog.txt" % (final, " ".join(chunks[0])))

