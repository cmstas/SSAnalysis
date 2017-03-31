#!/bin/bash

# path="/hadoop/cms/store/user/namin/condor/ss_babies_Nov25/"
# finaldir="/nfs-7/userdata/ss2015/ssBabies/v8.07/"

path="/hadoop/cms/store/user/namin/AutoTwopler_babies/SS_v9.06/"
finaldir="/nfs-7/userdata/namin/tupler_babies/merged/SS/v9.06/output/"

# DataDoubleEGRerecoB.root
# DataDoubleEGRerecoG.root
# DataDoubleEGHv3.root

# /hadoop/cms/store/user/namin/AutoTwopler_babies/SS_v9.06/DoubleEG_Run2016H-03Feb2017_ver2-v1/

samples = [

        # ["DataDoubleEGRerecoB", "DoubleEG_Run2016B-03Feb2017_ver2-v2/output/output*.root"],
        # ["DataDoubleEGRerecoG", "DoubleEG_Run2016G-03Feb2017-v1/output/output*.root"],
        # ["DataDoubleEGH", "DoubleEG_Run2016H-03Feb2017_ver2-v1/output/output*.root"],
        # ["DataDoubleEGHv3", "DoubleEG_Run2016H-03Feb2017_ver3-v1/output/output*.root"],

        # ["DataDoubleMuonRerecoB", "DoubleMuon_Run2016B-03Feb2017_ver2-v2/output/output*.root"],
        # ["DataDoubleMuonH", "DoubleMuon_Run2016H-03Feb2017_ver2-v1/output/output*.root"],

        ["DataMuonEGRerecoG", "MuonEG_Run2016G-03Feb2017-v1/output/output*.root"],

# ["prompt_DataMuonEG","datamuoneg*.root"],
# ["prompt_DataDoubleEG","datadoubleeg*.root"],
# ["prompt_DataDoubleMuon","datadoublemuon*.root"],

# ["DataMuonEG",["datamuonegh*.root","datamuonegrereco*.root"]],
# ["DataDoubleEG",["datadoubleegh*.root","datadoubleegrereco*.root"]],
# ["DataDoubleMuon",["datadoublemuonh*.root","datadoublemuonrereco*.root"]],
# ["DataJetHT",["datajethth*.root"]],

# ["WZLO", "wzlo_*.root"],

# ["T1TTTT_main", "t1tttt_main_*.root"],
# ["T6TTWW_main", "t6ttww_main_*.root"],
# ["T5TTTT_dm175", "t5tttt_dm175_*.root"],
# ["T5TTCC_main", "t5ttcc_main_*.root"],
# ["T5QQQQVV_main", "t5qqqqvv_main_*.root"],
# ["T5QQQQVV_dm20", "t5qqqqvv_dm20_*.root"],
# ["T1TTBB_main", "t1ttbb_main_*.root"],

# ["TTH_scan", "tth_scan_*.root"],
# ["THW_scan", "thw_scan_*.root"],
# ["THQ_scan", "thq_scan_*.root"],

# ["TTZLOW","ttzlow_*.root"],
# ["WJets","wjets_*.root"],
# ["TWZ","twz_*.root"],
# ["WJets_HTbinned","wjets*to*_*.root"],
# ["TTHtoNonBB","tthtononbb_*.root"],
# ["GGHtoZZto4L","gghtozzto4l_*.root"],
# ["DY_high","dy_high_[0-9]*.root"],
# ["DY_low","dy_low_*.root"],
# ["WWZ","wwz_*.root"],
# ["WWW","www_*.root"],
# # ["TTZ","ttz_*.root"],
# # ["TTZlofix","ttzlofix_*.root"],
# # ["TTW","ttw_*.root"],
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

 ]

import os,glob

def getChunks(v,n=50): return [ v[i:i+n] for i in range(0, len(v), n) ]

path = path.replace("${USER}",os.getenv("USER"))

do_skip_bad = True

if do_skip_bad:
    print "[!] Hey! You've told me to skip bad files. Doing so begrudgingly."

for final,loc in samples:

    files = []
    if type(loc) == list:
        for patt in loc:
            print "[%s] globbing for %s/%s" % (final, path,patt)
            files.extend(glob.glob("%s/%s" % (path,patt)))

    else:
        print "[%s] globbing for %s/%s" % (final, path,loc)
        files = glob.glob("%s/%s" % (path,loc))

    if "prompt" in final:
        n_files_tot = len(files)
        files = [f for f in files if "rereco" not in f]
        n_files_prompt = len(files)
        print "[%s] removing %i rereco files from prompt filename list" % (final,n_files_tot-n_files_prompt)

    print "[%s] Found %i files" % (final,len(files))

    if(len(files) < 1): continue


    if not os.path.isfile(final+".root"):
        # print "%s.root already in here, skipping" % final
        # continue

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

    if not os.path.exists(finaldir): os.system("mkdir -p %s" % finaldir)
    os.system("mv %s.root %s/" % (final,finaldir))

